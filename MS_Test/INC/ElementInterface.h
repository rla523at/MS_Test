#pragma once

#include "Flux.h"

template <typename C>
class ConservationLawCellIntegral
{
private:
	const PhysicalFlux* physical_flux_function_;

	RowMajorMatrix basis_QP_;

	RowMajorMatrix quadrature_gradient_basis_;

	size_t dimension_;

public:
	void initialize(const C& cell, const PhysicalFlux& physical_flux_function){
		this->physical_flux_function_ = &physical_flux_function;
		this->dimension_ = cell.dimension();

		const auto solution_order = cell.solution_order();
		const auto integrand_order = 2 * solution_order;

		const auto quadrature_rule = cell.calculate_Quadrature_Rule(integrand_order);

		this->basis_QP_ = cell.calculate_Basis_Point(quadrature_rule.quadrature_point_set);
		this->quadrature_gradient_basis_ = cell.calculate_Quadrature_Gradient_Basis(quadrature_rule);
	};

	RowMajorMatrix calculate(const C& cell) const {
		const auto solution_QP = cell.calculate_Solution_Point(this->basis_QP_);

		const auto [num_equation, num_QP] = solution_QP.size();
		const auto num_variable = this->dimension_;

		RowMajorMatrix physical_flux_QP(MatrixType::Full, num_equation, num_variable * num_QP);
		for (size_t q = 0; q < num_QP; ++q) {
			const auto physical_flux = (*this->physical_flux_function_)(solution_QP.column(q));

			for (size_t e = 0; e < num_equation; ++e)
				for (size_t v = 0; v < num_variable; ++v)
					physical_flux_QP.at(e, q * num_variable + v) = physical_flux.at(e, v);
		}

		return physical_flux_QP * this->quadrature_gradient_basis_;
	};
};


template <typename C>
class ICellTimeStep
{
protected:
	double time_step_constant_ = 0.0;

public:
	virtual double calculate(const C& cell) const = 0;
};


template <typename C>
class CFLCellTimeStep : public ICellTimeStep<C>
{
protected:
	double P0_basis_value_;
	
	double volume_;

	std::vector<double> projection_volume_set_;


public:
	CFLCellTimeStep(const C& cell, const double time_step_constant) {
		this->P0_basis_value_ = cell.P0_mode_basis_function()({ 0 });
		this->volume_ = cell.calculate_Volume();
		this->projection_volume_set_ = cell.calculate_Projection_Volume_Set();
		this->time_step_constant_ = time_step_constant / static_cast<double>(2 * cell.solution_order() + 1);
	};
};


template <typename C>
class CFL2DCellTimeStep : public CFLCellTimeStep<C>
{
public:
	CFL2DCellTimeStep(const C& cell, const double time_step_constant)
		: CFLCellTimeStep<C>(cell, time_step_constant) {};

	double calculate(const C& cell) const override {
		const auto P0_coefficient = cell.P0_mode_coefficient();
		const auto P0_solution = P0_coefficient * this->P0_basis_value_;

		const auto velocity = EOS::calculate_Velocity_Vector(P0_solution);
		const auto u = velocity[0];
		const auto v = velocity[1];

		const auto a = EOS::calculate_Acoustic_Speed(P0_solution, velocity);
		if (!std::isnormal(a))
			FATAL_ERROR(Editor::to_String(cell.index()) + " cell has negative pressure");

		const auto projection_volume_on_x0_axis = this->projection_volume_set_[0];
		const auto projection_volume_on_x1_axis = this->projection_volume_set_[1];

		const auto local_dt = this->volume_ / ((std::abs(u) + a) * projection_volume_on_x1_axis + (std::abs(v) + a) * projection_volume_on_x0_axis);
		return this->time_step_constant_ * local_dt;
	};
};


template <typename C>
class ConstantCellTimeStep : public ICellTimeStep<C>
{
public:
	ConstantCellTimeStep(const double time_step_constant){
		this->time_step_constant_ = time_step_constant;
	};

	double calculate(const C& cell) const override {
		return this->time_step_constant_;
	}
};

template <typename C>
class CellPostPointSolution
{
private:
	RowMajorMatrix basis_post_point_;

public:
	void initialize(const C& cell, const std::vector<MathVector>& post_point_set) {
		this->basis_post_point_ = cell.calculate_Basis_Point(post_point_set);
	}

	RowMajorMatrix calculate(const C& cell) const {
		return cell.calculate_Solution_Point(this->basis_post_point_);
	}
};


template <typename F>
class ConservationLawBoudnaryFaceIntegral
{
protected:
	std::vector<MathVector> QP_normal_vector_set_;

	RowMajorMatrix owner_basis_QP_;					// B X QP

	RowMajorMatrix owner_quadrature_basis_;			// QP X B

	const BoundaryFlux* boundary_flux_function_;

public:
	void initialize(const F& boundary_face, const NumericalFlux& numerical_flux_function) {
		const auto boundary_type_name = boundary_face.type_name();
		this->boundary_flux_function_ = BOUNDARY_FLUX_FACTORY.order(boundary_type_name, numerical_flux_function);

		const auto& solution_order = boundary_face.owner_cell_solution_order();
		const auto integrand_order = 2 * solution_order + 1;

		const auto quadrature_rule = boundary_face.calculate_Quadrature_Rule(integrand_order);

		this->QP_normal_vector_set_ = boundary_face.calculate_Normal_Vector(quadrature_rule.quadrature_point_set);
		this->owner_basis_QP_ = boundary_face.calculate_Owner_Basis_Point(quadrature_rule.quadrature_point_set);
		this->owner_quadrature_basis_ = -1.0 * boundary_face.calculate_Owner_Quadrature_Basis(quadrature_rule);		// numerical flux comes out from owner cell 
	}

	void calculate(const F& boundary_face, std::unordered_map<size_t,RowMajorMatrix>& cell_index_to_RHS) const {
		const auto owner_solution_QP = boundary_face.calculate_Owner_Solution_Point(this->owner_basis_QP_);

		const auto [num_equation, num_QP] = owner_solution_QP.size();
		RowMajorMatrix boundary_flux_QP(MatrixType::Full, num_equation, num_QP);
		for (size_t q = 0; q < num_QP; ++q) {
			const auto boundary_flux_vector = (*this->boundary_flux_function_)(owner_solution_QP.column(q), this->QP_normal_vector_set_[q]);

			boundary_flux_QP.change_Column(q, boundary_flux_vector);
		}

		const auto owner_face_integral_value = boundary_flux_QP * this->owner_quadrature_basis_;
		cell_index_to_RHS.at(boundary_face.owner_cell_index()) += owner_face_integral_value;
	};
};


template <typename F>
class ConservationLawInterCellFaceIntegral
{
protected:
	std::vector<MathVector> QP_normal_vector_set_;

	RowMajorMatrix owner_basis_QP_;						// B X QP

	RowMajorMatrix owner_quadrature_basis_;				// QP X B

	RowMajorMatrix neighbor_basis_QP_;					// B X QP

	RowMajorMatrix neighbor_quadrature_basis_;			// QP X B

	const NumericalFlux* numerical_flux_function_;

public:
	void calculate(const F& inter_cell_face, std::unordered_map<size_t, RowMajorMatrix>& cell_index_to_RHS) const {
		const auto owner_cell_solution_QP = inter_cell_face.calculate_Owner_Solution_Point(this->owner_basis_QP_);
		const auto neighbor_cell_solution_QP = inter_cell_face.calculate_Neighbor_Solution_Point(this->neighbor_basis_QP_);

		const auto [num_equation, num_QP] = owner_cell_solution_QP.size();
		RowMajorMatrix numerical_flux_QP(MatrixType::Full, num_equation, num_QP);

		for (size_t q = 0; q < num_QP; ++q) {
			const auto numerical_flux_vector = (*this->numerical_flux_function_)(owner_cell_solution_QP.column(q), neighbor_cell_solution_QP.column(q), this->QP_normal_vector_set_[q]);

			numerical_flux_QP.change_Column(q, numerical_flux_vector);
		}

		const auto owner_face_integral_value = numerical_flux_QP * this->owner_quadrature_basis_;
		const auto neighbor_face_integral_value = numerical_flux_QP * this->neighbor_quadrature_basis_;

		cell_index_to_RHS.at(inter_cell_face.owner_cell_index()) += owner_face_integral_value;
		cell_index_to_RHS.at(inter_cell_face.neighbor_cell_index()) += neighbor_face_integral_value;
	};
};


template <typename F>
class ConservationLawInnerFaceIntegral : public ConservationLawInterCellFaceIntegral<F>
{
public:
	void initialize(const F& inner_face, const NumericalFlux& numerical_flux_function){
		this->numerical_flux_function_ = &numerical_flux_function;
		const auto max_solution_order = std::max(inner_face.owner_cell_solution_order(), inner_face.neighbor_cell_solution_order());
		const auto integrand_order = 2 * max_solution_order + 1;

		const auto quadrature_rule = inner_face.calculate_Quadrature_Rule(integrand_order);

		this->QP_normal_vector_set_ = inner_face.calculate_Normal_Vector(quadrature_rule.quadrature_point_set);
		this->owner_basis_QP_ = inner_face.calculate_Owner_Basis_Point(quadrature_rule.quadrature_point_set);
		this->owner_quadrature_basis_ = -1.0 * inner_face.calculate_Owner_Quadrature_Basis(quadrature_rule);		// numerical flux comes out from owner cell 

		this->neighbor_basis_QP_ = inner_face.calculate_Neighbor_Basis_Point(quadrature_rule.quadrature_point_set);
		this->neighbor_quadrature_basis_ = inner_face.calculate_Neighbor_Quadrature_Basis(quadrature_rule);
	}
};


template <typename F>
class ConservationLawPeriodicFaceIntegral : public ConservationLawInterCellFaceIntegral<F>
{
public:
	void initialize(const F& periodic_face, const NumericalFlux& numerical_flux_function) {
		this->numerical_flux_function_ = &numerical_flux_function;
		const auto max_solution_order = std::max(periodic_face.owner_cell_solution_order(), periodic_face.neighbor_cell_solution_order());
		const auto integrand_order = 2 * max_solution_order + 1;

		const auto owner_cell_side_quadrature_rule = periodic_face.calculate_Owner_Quadrature_Rule(integrand_order);
		const auto neighbor_cell_side_quadrature_rule = periodic_face.calculate_Neighbor_Quadrature_Rule(integrand_order);

		this->QP_normal_vector_set_ = periodic_face.calculate_Normal_Vector(owner_cell_side_quadrature_rule.quadrature_point_set);

		this->owner_basis_QP_ = periodic_face.calculate_Owner_Basis_Point(owner_cell_side_quadrature_rule.quadrature_point_set);
		this->owner_quadrature_basis_ = -1.0 * periodic_face.calculate_Owner_Quadrature_Basis(owner_cell_side_quadrature_rule);		// numerical flux comes out from owner cell 

		this->neighbor_basis_QP_ = periodic_face.calculate_Neighbor_Basis_Point(neighbor_cell_side_quadrature_rule.quadrature_point_set);
		this->neighbor_quadrature_basis_ = periodic_face.calculate_Neighbor_Quadrature_Basis(neighbor_cell_side_quadrature_rule);
	}
};
















