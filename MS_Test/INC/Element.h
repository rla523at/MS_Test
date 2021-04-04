#pragma once

#include "ElementBase.h"
#include "ElementInterFace.h"


class Cell : public CellBase
{
protected:
	CellPostPointSolution<CellBase> post_point_solution_calculator_;
	
	ConservationLawCellIntegral<CellBase> integral_calculator_;

	std::unique_ptr<ICellTimeStep<CellBase>> time_step_calculator_;

public:
	explicit Cell(CellData&& cell_data)
		:CellBase(std::move(cell_data)) {};


	void initialize_Post_Point_Solution_Calculator(const std::vector<MathVector>& post_point_set) {
		this->post_point_solution_calculator_.initialize(*this, post_point_set);
	}

	void initialize_Integral_Caclulator(const PhysicalFlux& physical_flux) {
		this->integral_calculator_.initialize(*this, physical_flux);
	}

	void initialize_Time_Step_Calculator(const std::string& time_step_type_name, const double time_step_constant);

	RowMajorMatrix calculate_Post_Point_Solution(void) const {
		return this->post_point_solution_calculator_.calculate(*this);
	}

	RowMajorMatrix calculate_Conservation_Law_Integral(void) const {
		return this->integral_calculator_.calculate(*this);
	}

	double calculate_Time_Step(void) const {
		return this->time_step_calculator_->calculate(*this);
	}
};


class BoundaryFace : public FaceBase
{
protected:
	ElementType type_;
	
	ConservationLawBoudnaryFaceIntegral<BoundaryFace> integral_calculator_;

public:
	explicit BoundaryFace(FaceData&& face_data, const ElementType type)
		:FaceBase(std::move(face_data)), type_(type) {};


	std::set<size_t> node_index_set(void) const {
		return std::set(this->node_index_set_.begin(),this->node_index_set_.end());
	}

	std::string type_name(void) const {
		return Editor::to_String(this->type_);
	}

	void initialize_Integral_Caclulator(const NumericalFlux& numerical_flux) {
		this->integral_calculator_.initialize(*this, numerical_flux);
	}

	void calculate_Conservation_Law_Integral(std::unordered_map<size_t, RowMajorMatrix>& cell_index_to_RHS) const {
		this->integral_calculator_.calculate(*this, cell_index_to_RHS);
	}		
};


class InnerFace : public InterCellFaceBase
{
private:
	ConservationLawInnerFaceIntegral<InnerFace> integral_calculator_;

public:
	explicit InnerFace(FaceData&& face_data, const size_t neighbor_cell_index, const CellBase& neighbor_cell_base)
		: InterCellFaceBase(std::move(face_data), neighbor_cell_index, neighbor_cell_base) {};


	void initialize_Integral_Caclulator(const NumericalFlux& numerical_flux_function) {
		this->integral_calculator_.initialize(*this, numerical_flux_function);
	}

	void calculate_Conservation_Law_Integral(std::unordered_map<size_t, RowMajorMatrix>& cell_index_to_rhs) const {
		return this->integral_calculator_.calculate(*this, cell_index_to_rhs);
	}
};


class PeriodicFace : public InterCellFaceBase
{
private:
	ElementType type_;

	IndexedFigure neighbor_cell_side_figure_;

	ConservationLawPeriodicFaceIntegral<PeriodicFace> integral_calculator_;

public:
	explicit PeriodicFace(FaceData&& face_data, IndexedFigureData&& neighbor_cell_side_figure_data,  const size_t neighbor_cell_index, const CellBase& neighbor_cell_base, const ElementType type)
		: InterCellFaceBase(std::move(face_data), neighbor_cell_index, neighbor_cell_base), neighbor_cell_side_figure_(std::move(neighbor_cell_side_figure_data)), type_(type) {};


	QuadratureRule calculate_Owner_Quadrature_Rule(const size_t integrand_order) const {
		return this->calculate_Quadrature_Rule(integrand_order);
	}

	QuadratureRule calculate_Neighbor_Quadrature_Rule(const size_t integrand_order) const {
		return this->neighbor_cell_side_figure_.calculate_Quadrature_Rule(integrand_order);
	}

	void initialize_Integral_Caclulator(const NumericalFlux& numerical_flux_function) {
		this->integral_calculator_.initialize(*this, numerical_flux_function);
	}

	void calculate_Conservation_Law_Integral(std::unordered_map<size_t, RowMajorMatrix>& cell_index_to_rhs) const {
		return this->integral_calculator_.calculate(*this, cell_index_to_rhs);
	}
};


struct ElementSet
{
	std::vector<Cell> cell_set;
	std::vector<BoundaryFace> boundary_face_set;
	std::vector<InnerFace> inner_face_set;
	std::vector<PeriodicFace> periodic_face_set;
};


class ElementBuilder
{
private:
	std::unordered_map<size_t, MathVector> index_to_node_;

public:
	ElementSet build_Element_Set(GridData& grid_data, const size_t solution_order, const InitialCondition& initial_condition_function);

private:
	std::vector<size_t> find_Cell_Index_Has_This_Face(const std::unordered_map<size_t, std::set<size_t>>& vertex_node_index_to_cell_index, const std::vector<size_t>& face_node_index_set) const;

	std::unordered_map<size_t, size_t> match_Periodic_Face_Grid_Data_Index(const std::vector<ElementGridData>& periodic_face_grid_data_set, const std::unordered_map<size_t, std::vector<const MathVector*>>& data_index_to_node_set) const;

	std::unordered_map<size_t, size_t> match_Periodic_Face_Data_Index_by_Type(const std::unordered_map<size_t, std::vector<const MathVector*>>& data_set_index_to_periodic_face_consisting_node, const ElementType element_type) const;

	bool is_Matched_Periodic_Face(const std::vector<const MathVector*>& i_periodic_face_consisting_node_set, const std::vector<const MathVector*>& j_periodic_face_consisting_node_set, const ElementType element_type) const;

	bool is_There_Mathced_Periodic_Node(const std::vector<const MathVector*>& node_set, const MathVector& node, const ElementType element_type) const;

	bool is_Axis_Translation(const MathVector& point_vector1, const MathVector& point_vector2, const size_t aixs_tag) const;
};