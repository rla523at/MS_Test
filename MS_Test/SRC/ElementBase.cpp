#include "../INC/ElementBase.h"

CellBase::CellBase(CellData&& cell_data)
	: index_(cell_data.index), IndexedFigure(std::move(cell_data.figure)) {
	// calculate orthonormal basis function set
	const auto solution_order = cell_data.solution_order;
	const auto& initial_condition_function = cell_data.initial_condition_function;

	const auto integrand_order = 2 * solution_order;
	const auto quadrature_rule = this->calculate_Quadrature_Rule(integrand_order);

	const auto initial_basis_function_set = Math::calculate_Initial_Basis_Function_Set(*this, solution_order);
	const auto orthonormal_basis_function_set = Math::Gram_Schmidt_Process(initial_basis_function_set, quadrature_rule);

	for (const auto& basis_function : orthonormal_basis_function_set)
		this->order_to_basis_function_.emplace(basis_function.order(), basis_function);

	// calculate initial solution coefficient
	const auto& QP_set = quadrature_rule.quadrature_point_set;
	const auto num_QP = QP_set.size();

	const auto one_of_initial_solution = initial_condition_function(QP_set.front());
	const auto num_equation = one_of_initial_solution.size();

	RowMajorMatrix solution_QP(MatrixType::Full, num_equation, num_QP);
	solution_QP.change_Column(0, one_of_initial_solution);
	for (size_t q = 1; q < num_QP; ++q)
		solution_QP.change_Column(q, initial_condition_function(QP_set[q]));

	const auto QW_basis = this->calculate_Quadrature_Basis(quadrature_rule);
	this->solution_coefficient_ = solution_QP * QW_basis;

	// set order to order end index
	size_t end_index = 0;
	for (size_t order = 0; order <= this->solution_order(); ++order) {
		end_index += order_to_basis_function_.count(order);

		this->order_to_order_end_index_.emplace(order, end_index);
	}
}

RowMajorMatrix CellBase::calculate_Basis_Point(const std::vector<MathVector>& node_set) const {
	const auto num_basis = this->order_to_basis_function_.size();
	const auto num_point = node_set.size();

	RowMajorMatrix basis_point(MatrixType::Full, num_basis, num_point);
	size_t b = 0;
	for (const auto& [order, basis_function] : this->order_to_basis_function_) {
		for (size_t p = 0; p < num_point; ++p)
			basis_point.at(b, p) = basis_function(node_set[p]);

		b++;
	}

	return basis_point;
}

RowMajorMatrix CellBase::calculate_Quadrature_Basis(const QuadratureRule& quadrature_rule) const {
	const auto& QP_set = quadrature_rule.quadrature_point_set;
	const auto& QW_set = quadrature_rule.quadrature_weight_set;

	const auto num_basis = this->order_to_basis_function_.size();
	const auto num_QP = QP_set.size();

	RowMajorMatrix QW_basis(MatrixType::Full, num_QP, num_basis);
	size_t b = 0;
	for (const auto& [order, basis_function] : this->order_to_basis_function_) {
		for (size_t q = 0; q < num_QP; ++q)
			QW_basis.at(q, b) = basis_function(QP_set[q]) * QW_set[q];

		b++;
	}

	return QW_basis;
}

RowMajorMatrix CellBase::calculate_Quadrature_Gradient_Basis(const QuadratureRule& quadrature_rule) const {
	const auto num_variable = this->dimension();

	const auto& QP_set = quadrature_rule.quadrature_point_set;
	const auto& QW_set = quadrature_rule.quadrature_weight_set;

	const auto num_QP = QP_set.size();
	const auto num_basis = this->order_to_basis_function_.size();

	size_t b = 0;
	RowMajorMatrix gradeint_basis_QW(MatrixType::Full, num_QP * num_variable, num_basis);
	for (const auto& [order, basis_function] : this->order_to_basis_function_) {
		const auto gradient_basis_function = Math::gradient(basis_function, num_variable);

		for (size_t q = 0; q < num_QP; ++q) {
			MathVector gradient_ibasis_QW_value = gradient_basis_function(QP_set[q]) * QW_set[q];

			for (size_t v = 0; v < num_variable; ++v)
				gradeint_basis_QW.at(v + q * num_variable, b) = gradient_ibasis_QW_value[v];
		}

		b++;
	}

	return gradeint_basis_QW;
}

std::vector<const Polynomial*> CellBase::Pn_mode_basis_function_set(const size_t Pn_mode) const {
	if (this->solution_order() < Pn_mode)
		FATAL_ERROR("requested order(" + Editor::to_String(Pn_mode) + ") is exceed solution order(" + Editor::to_String(this->solution_order()) + ")");

	const auto range = this->order_to_basis_function_.equal_range(Pn_mode);

	std::vector<const Polynomial*> Pn_mode_basis_function_set;
	for (auto iter = range.first; iter != range.second; ++iter) {
		const auto& [order, basis_function] = *iter;
		Pn_mode_basis_function_set.push_back(&basis_function);
	}

	return Pn_mode_basis_function_set;
}

RowMajorMatrix CellBase::Pn_mode_coefficient(const size_t Pn_mode) const {
	if (this->solution_order() < Pn_mode)
		FATAL_ERROR("requested order(" + Editor::to_String(Pn_mode) + ") is exceed solution order(" + Editor::to_String(this->solution_order()) + ")");

	const auto start_index = this->order_to_order_end_index_.at(Pn_mode - 1);
	const auto end_index = this->order_to_order_end_index_.at(Pn_mode);

	return this->solution_coefficient_.column(start_index, end_index);
}

FaceBase::FaceBase(FaceData&& face_data)
	:IndexedFigure(std::move(face_data.figure_data)), owner_cell_index_(face_data.owner_cell_index), owner_cell_base_(face_data.owner_cell) {
	const auto figure_face_type = this->owner_cell_base_.examine_Figure_Face_Type(*this);

	switch (figure_face_type) {
	case FigureFaceType::InwardFace:
		this->align_normal_coefficient_ = -1;
		break;
	case FigureFaceType::OutwardFace:
		this->align_normal_coefficient_ = 1;
		break;
	default:
		FATAL_TYPE_ERROR;
		break;
	}
}

MathVector FaceBase::calculate_Normal_Vector(const MathVector& node) const {
	const auto reference_normal_vector = ReferenceFigure::calculate_Normal_Vector(this->figure_type_);
	const auto transformation_normal_matrix = this->calculate_Transformation_Normal_Matrix(node);
	return this->align_normal_coefficient_ * Math::normalize(transformation_normal_matrix * reference_normal_vector);
}

std::vector<MathVector> FaceBase::calculate_Normal_Vector(const std::vector<MathVector>& node_set) const {
	std::vector<MathVector> normal_vector_set;
	normal_vector_set.reserve(node_set.size());

	for (const auto& node : node_set)
		normal_vector_set.emplace_back(this->calculate_Normal_Vector(node));

	return normal_vector_set;
}


//std::unordered_map<size_t, std::set<size_t>> GmshGridBuilder::buildNodeIndexTohMLPStencil(const std::unordered_map<size_t, const Node>& index_to_node, const std::vector<Face*>& face_set)
//{
//	std::unordered_map<size_t, std::set<size_t>> vertex_node_index_to_hMLP_stencil = this->vertex_node_index_to_cell_index_;
//
//	for (const auto face : face_set)
//	{
//		if (face->type() != FaceType::PeriodicBoundaryFace)	 continue;
//
//		const auto periodic_boundary_face = dynamic_cast<PeriodicBoundaryFace*>(face);
//
//		const auto& owner_cell_index = periodic_boundary_face->owner_cell_index();
//		const auto& neighbor_cell_index = periodic_boundary_face->neighbor_cell_index();
//
//		const auto& owner_cell_side_vertex_consisting_node_index_set = periodic_boundary_face->geometry().calculate_Vertex_Node_Index_Set();
//		const auto& neighbor_cell_side_vertex_consisting_node_index_set = periodic_boundary_face->neighbor_cell_side_figure().calculate_Vertex_Node_Index_Set();
//
//		for (const auto owner_cell_side_vertex_consisting_node_index : owner_cell_side_vertex_consisting_node_index_set)
//			vertex_node_index_to_hMLP_stencil.at(owner_cell_side_vertex_consisting_node_index).emplace(neighbor_cell_index);
//
//		for (const auto neighbor_cell_side_vertex_consisting_node_index : neighbor_cell_side_vertex_consisting_node_index_set)
//			vertex_node_index_to_hMLP_stencil.at(neighbor_cell_side_vertex_consisting_node_index).emplace(owner_cell_index);
//	}
//
//	for (const auto face : face_set)
//	{
//		if (face->type() != FaceType::PeriodicBoundaryFace)	 continue;
//
//		const auto periodic_boundary_face = dynamic_cast<PeriodicBoundaryFace*>(face);
//
//		const auto matched_node_index_set = periodic_boundary_face->calculate_Matching_Node_Index(index_to_node);
//
//		for (const auto& [owner_cell_side_node_index, neighbor_cell_side_node_index] : matched_node_index_set)
//		{
//			auto& stencil1 = vertex_node_index_to_hMLP_stencil.at(owner_cell_side_node_index);
//			auto& stencil2 = vertex_node_index_to_hMLP_stencil.at(neighbor_cell_side_node_index);
//
//			if (stencil1 == stencil2) continue;
//
//			stencil1.insert(stencil2.begin(), stencil2.end());
//			stencil2.insert(stencil1.begin(), stencil1.end());
//		}
//	}
//
//	return vertex_node_index_to_hMLP_stencil;
//}
//
//
//
//std::unordered_map<size_t, std::set<std::pair<size_t, size_t>>> GmshGridBuilder::buildNodeIndexTohMLPBDStencil(const std::unordered_map<size_t, const Node>& index_to_node, const std::vector<Face*>& face_set)
//{
//	std::unordered_map<size_t, std::set<std::pair<size_t, size_t>>> node_index_to_hMLPBD_stencil;
//
//	for (const auto& [node_index, cell_index_set] : this->vertex_node_index_to_cell_index_)
//	{
//		std::set<std::pair<size_t, size_t>> stencil;
//		for (const auto& cell_index : cell_index_set)
//			stencil.emplace(cell_index, node_index);
//
//		node_index_to_hMLPBD_stencil.emplace(node_index, stencil);
//	}
//
//	for (const auto face : face_set)
//	{
//		if (face->type() != FaceType::PeriodicBoundaryFace)	 continue;
//
//		const auto periodic_boundary_face = dynamic_cast<PeriodicBoundaryFace*>(face);
//
//		const auto& owner_cell_index = periodic_boundary_face->owner_cell_index();
//		const auto& neighbor_cell_index = periodic_boundary_face->neighbor_cell_index();
//
//		const auto matched_node_index_set = periodic_boundary_face->calculate_Matching_Node_Index(index_to_node);
//
//		for (const auto& [owner_cell_side_node_index, neighbor_cell_side_node_index] : matched_node_index_set)
//		{
//			node_index_to_hMLPBD_stencil.at(owner_cell_side_node_index).emplace(neighbor_cell_index, neighbor_cell_side_node_index);
//			node_index_to_hMLPBD_stencil.at(neighbor_cell_side_node_index).emplace(owner_cell_index, owner_cell_side_node_index);
//		}
//	}
//
//	for (const auto face : face_set)
//	{
//		if (face->type() != FaceType::PeriodicBoundaryFace)	 continue;
//
//		const auto periodic_boundary_face = dynamic_cast<PeriodicBoundaryFace*>(face);
//
//		const auto matched_node_index_set = periodic_boundary_face->calculate_Matching_Node_Index(index_to_node);
//
//		for (const auto& [owner_cell_side_node_index, neighbor_cell_side_node_index] : matched_node_index_set)
//		{
//			auto& stencil1 = node_index_to_hMLPBD_stencil.at(owner_cell_side_node_index);
//			auto& stencil2 = node_index_to_hMLPBD_stencil.at(neighbor_cell_side_node_index);
//
//			if (stencil1 == stencil2) continue;
//
//			stencil1.insert(stencil2.begin(), stencil2.end());
//			stencil2.insert(stencil1.begin(), stencil1.end());
//		}
//	}
//
//	return node_index_to_hMLPBD_stencil;
//}