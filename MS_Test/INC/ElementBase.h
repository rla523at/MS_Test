#pragma once

#include "Figure.h"
#include "GridReader.h"
#include "InitialCondition.h"

#include <set>
#include <unordered_set>


struct CellData
{
	size_t index;
	IndexedFigure figure;
	size_t solution_order;
	const InitialCondition& initial_condition_function;
};


class CellBase : public IndexedFigure
{
protected:
	size_t index_;

	RowMajorMatrix solution_coefficient_;				// E X B

	std::multimap<size_t, Polynomial> order_to_basis_function_;

	std::map<size_t, size_t> order_to_order_end_index_;

public:
	explicit CellBase(CellData&& cell_data);


	RowMajorMatrix calculate_Basis_Point(const std::vector<MathVector>& point_set) const;				// basis value at some points : [ B * P ] matrix

	RowMajorMatrix calculate_Quadrature_Basis(const QuadratureRule& quadrature_rule) const;				// basis value * QW at QP : [ QP * B ] matrix

	RowMajorMatrix calculate_Quadrature_Gradient_Basis(const QuadratureRule& quadrature_rule) const;	// gradient basis value * QW at QP : [ (QP * V) * B ] matrix

	RowMajorMatrix calculate_Solution_Point(const RowMajorMatrix& basis_point) const {
		return this->solution_coefficient_ * basis_point;
	};

	RowMajorMatrix calculate_Solution_Point(const std::vector<MathVector>& point_set) const {
		const auto basis_point = this->calculate_Basis_Point(point_set);
		return this->solution_coefficient_ * basis_point;
	}

	size_t index(void) const {
		return this->index_;
	}

	size_t solution_order(void) const {
		return this->order_to_basis_function_.rbegin()->first;
	};

	size_t num_equation(void) const {
		return this->solution_coefficient_.size().first;
	}

	const Polynomial& P0_mode_basis_function(void)  const {
		return this->order_to_basis_function_.begin()->second;
	}

	std::vector<const Polynomial*> Pn_mode_basis_function_set(const size_t Pn_mode)  const;

	MathVector P0_mode_coefficient(void) const {
		return this->solution_coefficient_.column(0);
	}

	RowMajorMatrix Pn_mode_coefficient(const size_t Pn_mode) const;

	const RowMajorMatrix& coefficient(void) const {
		return this->solution_coefficient_;
	}

	void update(RowMajorMatrix&& new_coefficient) {
		this->solution_coefficient_ = std::move(new_coefficient);
	};
};


struct FaceData
{
	IndexedFigure figure_data;
	const size_t owner_cell_index;
	const CellBase& owner_cell;
};


class FaceBase : public IndexedFigure
{
protected:
	const size_t owner_cell_index_;

	const CellBase& owner_cell_base_;

	int align_normal_coefficient_ = 1;	// if normal point outside of element 1 else -1

public:
	FaceBase(FaceData&& face_data);

	
	MathVector calculate_Normal_Vector(const MathVector& position_vector) const;

	std::vector<MathVector> calculate_Normal_Vector(const std::vector<MathVector>& QP_set) const;

	RowMajorMatrix calculate_Owner_Basis_Point(const std::vector<MathVector>& point_set) const {
		return this->owner_cell_base_.calculate_Basis_Point(point_set);
	}

	RowMajorMatrix calculate_Owner_Quadrature_Basis(const QuadratureRule& quadrature_rule) const {
		return this->owner_cell_base_.calculate_Quadrature_Basis(quadrature_rule);
	}

	RowMajorMatrix calculate_Owner_Solution_Point(const RowMajorMatrix& basis_point) const {
		return this->owner_cell_base_.calculate_Solution_Point(basis_point);
	}

	const std::vector<size_t>& owner_cell_side_node_index_set(void) const {
		return this->node_index_set_;
	}

	size_t owner_cell_solution_order(void) const {
		return this->owner_cell_base_.solution_order();
	}

	size_t owner_cell_index(void) const {
		return this->owner_cell_index_;
	}
};


class InterCellFaceBase : public FaceBase
{
private:
	const size_t neighbor_cell_index_;

	const CellBase& neighbor_cell_base_;

public:
	InterCellFaceBase(FaceData&& face_data, const size_t neighbor_cell_index, const CellBase& neighbor_cell_base)
		: FaceBase(std::move(face_data)), neighbor_cell_index_(neighbor_cell_index), neighbor_cell_base_(neighbor_cell_base) {};


	RowMajorMatrix calculate_Neighbor_Basis_Point(const std::vector<MathVector>& point_set) const {
		return this->neighbor_cell_base_.calculate_Basis_Point(point_set);
	}

	RowMajorMatrix calculate_Neighbor_Quadrature_Basis(const QuadratureRule& quadrature_rule) const {
		return this->neighbor_cell_base_.calculate_Quadrature_Basis(quadrature_rule);
	}

	RowMajorMatrix calculate_Neighbor_Solution_Point(const RowMajorMatrix& basis_point) const {
		return this->neighbor_cell_base_.calculate_Solution_Point(basis_point);
	}

	size_t neighbor_cell_solution_order(void) const {
		return this->neighbor_cell_base_.solution_order();
	}

	size_t neighbor_cell_index(void) const {
		return this->neighbor_cell_index_;
	}
};

//std::unordered_map<size_t, std::set<size_t>> buildNodeIndexTohMLPStencil(const std::unordered_map<size_t, const Node>& index_to_node, const std::vector<Face*>& face_set);

//std::unordered_map<size_t, std::set<std::pair<size_t, size_t>>> buildNodeIndexTohMLPBDStencil(const std::unordered_map<size_t, const Node>& index_to_node, const std::vector<Face*>& face_set);
