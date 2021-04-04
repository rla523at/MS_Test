#include "../INC/Element.h"

void Cell::initialize_Time_Step_Calculator(const std::string& time_step_type_name, const double time_step_constant) {
	const auto dimension = this->dimension();
	const auto indent = time_step_type_name + Editor::to_String(dimension);

	if (StringEditor::is_same_without_Case_Sensitivity(indent, "CFL2"))
		this->time_step_calculator_ = std::make_unique<CFL2DCellTimeStep<CellBase>>(*this, time_step_constant);
	else if (StringEditor::is_same_without_Case_Sensitivity(time_step_type_name, "constantDt"))
		this->time_step_calculator_ = std::make_unique<ConstantCellTimeStep<CellBase>>(time_step_constant);
	else
		FATAL_TYPE_ERROR;
}



ElementSet ElementBuilder::build_Element_Set(GridData& grid_data, const size_t solution_order, const InitialCondition& initial_condition_function) {
	ElementSet element_set;
	auto& [cell_set, boundary_face_set, inner_face_set, periodic_face_set] = element_set;
	
	auto& [node_grid_data_set, cell_grid_data_set, boundary_face_grid_data_set, periodic_face_grid_data_set] = grid_data;

	const auto num_node = node_grid_data_set.size();
	const auto num_cell = cell_grid_data_set.size();
	const auto num_boundary_face = boundary_face_grid_data_set.size();
	const auto num_periodic_face = periodic_face_grid_data_set.size();

	// build index to node
	this->index_to_node_.reserve(num_node);
	for (auto& [index, coord_set] : node_grid_data_set)
		this->index_to_node_.emplace(index, std::move(coord_set));

	// build cell set
	cell_set.reserve(num_cell);
	for (auto& [cell_index, figure_type_name, figure_order, element_type, node_index_set] : cell_grid_data_set) {
		auto node_set = Tool::extract_Set_by_Index(this->index_to_node_, node_index_set);
		IndexedFigureData figure_data = { figure_type_name, figure_order, std::move(node_set), std::move(node_index_set) };
		CellData cell_data = { cell_index, std::move(figure_data), solution_order, initial_condition_function };

		cell_set.emplace_back(std::move(cell_data));
	}

	// make 'cell index to cell set index' to find owner/neighbor cell
	std::unordered_map<size_t, size_t> cell_index_to_cell_set_index;
	cell_index_to_cell_set_index.reserve(num_cell);

	// make 'vertex node index to cell index' to find owner/neighbor cell index
	std::unordered_map<size_t, std::set<size_t>> vertex_node_index_to_cell_index;
	vertex_node_index_to_cell_index.reserve(num_node);

	for (size_t i = 0; i < num_cell; ++i) {
		const auto& cell = cell_set[i];
		cell_index_to_cell_set_index.emplace(cell.index(), i);

		const auto vertex_node_index_set = cell.calculate_Vertex_Node_Index_Set();
		for (const auto& node_index : vertex_node_index_set) {
			if (vertex_node_index_to_cell_index.find(node_index) == vertex_node_index_to_cell_index.end())
				vertex_node_index_to_cell_index.emplace(node_index, std::set<size_t>());

			vertex_node_index_to_cell_index.at(node_index).insert(cell.index());
		}
	}

	// make constructed face node set list
	std::set<std::set<size_t>> constructed_face_node_index_set_list;

	// build boundary face set
	if (!boundary_face_grid_data_set.empty()) {
		boundary_face_set.reserve(num_boundary_face);
		for (auto& [index, figure_type_name, figure_order, element_type, node_index_set] : boundary_face_grid_data_set) {
			//find owner cell index
			const auto cell_index_set = this->find_Cell_Index_Has_This_Face(vertex_node_index_to_cell_index, node_index_set);

			if (cell_index_set.size() != 1)
				FATAL_ERROR(Editor::to_String(index) + " boundary does not have unique owner cell");

			const auto& owner_cell_index = cell_index_set.front();

			//update constructed face node set list
			constructed_face_node_index_set_list.emplace(node_index_set.begin(), node_index_set.end());

			//build boundary face figure data
			auto node_set = Tool::extract_Set_by_Index(this->index_to_node_, node_index_set);
			IndexedFigureData figure_data = { figure_type_name, figure_order, std::move(node_set), std::move(node_index_set) };

			//add boundary face
			const auto cell_set_index = cell_index_to_cell_set_index.at(owner_cell_index);
			const auto& owner_cell = cell_set.at(cell_set_index);

			FaceData face_data = { std::move(figure_data), owner_cell_index, owner_cell };
			boundary_face_set.emplace_back(std::move(face_data), element_type);
		}
	}

	// build periodic face set
	if (!periodic_face_grid_data_set.empty()) {
		//build data index to node set
		std::unordered_map<size_t, std::vector<const MathVector*>> data_index_to_node_set;
		for (size_t data_index = 0; data_index < num_periodic_face; ++data_index) {
			const auto& [index, figure_type_name, figure_order, element_type, node_index_set] = periodic_face_grid_data_set[data_index];
			auto node_set = Tool::extract_Set_by_Index(this->index_to_node_, node_index_set);

			data_index_to_node_set.emplace(data_index, std::move(node_set));
		}

		//match data index
		const auto data_index_to_matched_index = this->match_Periodic_Face_Grid_Data_Index(periodic_face_grid_data_set, data_index_to_node_set);

		//i : one of periodic face, j : matched periodic face of i
		for (const auto& [i_data_index, j_data_index] : data_index_to_matched_index) {
			auto& [i_index, i_figure_type_name, i_figure_order, i_element_type, i_node_index_set] = periodic_face_grid_data_set[i_data_index];
			auto& [j_index, j_figure_type_name, j_figure_order, j_element_type, j_node_index_set] = periodic_face_grid_data_set[j_data_index];

			//find owner/neighbor cell index
			const auto cell_index_set_have_i = this->find_Cell_Index_Has_This_Face(vertex_node_index_to_cell_index, i_node_index_set);
			const auto cell_index_set_have_j = this->find_Cell_Index_Has_This_Face(vertex_node_index_to_cell_index, j_node_index_set);

			if (cell_index_set_have_i.size() != 1)
				FATAL_ERROR(Editor::to_String(i_index) + " periodic boundary have not unique owner cell");
			else if (cell_index_set_have_j.size() != 1)
				FATAL_ERROR(Editor::to_String(j_index) + " periodic boundary have not unique neighbor cell");

			const auto& owner_cell_index = cell_index_set_have_i.front();
			const auto& neighbor_cell_index = cell_index_set_have_j.front();

			//update constructed face node set list
			constructed_face_node_index_set_list.emplace(i_node_index_set.begin(), i_node_index_set.end());
			constructed_face_node_index_set_list.emplace(j_node_index_set.begin(), j_node_index_set.end());

			//build owner & neighbor side periodic face figure data
			//we will designate i as owner cell side face		
			auto& i_node_set = data_index_to_node_set.at(i_data_index);
			auto& j_node_set = data_index_to_node_set.at(j_data_index);

			IndexedFigureData owner_side_figure_data = { i_figure_type_name, i_figure_order, std::move(i_node_set), std::move(i_node_index_set) };
			IndexedFigureData neighbor_side_figure_data = { j_figure_type_name, j_figure_order, std::move(j_node_set), std::move(j_node_index_set) };

			//add periodic face 
			const auto owner_cell_set_index = cell_index_to_cell_set_index.at(owner_cell_index);
			const auto neighbor_cell_set_index = cell_index_to_cell_set_index.at(neighbor_cell_index);

			const auto& owner_cell = cell_set.at(owner_cell_set_index);
			const auto& neighbor_cell = cell_set.at(neighbor_cell_set_index);

			FaceData face_data = { std::move(owner_side_figure_data), owner_cell_index, owner_cell };
			periodic_face_set.emplace_back(std::move(face_data), std::move(neighbor_side_figure_data), neighbor_cell_index, neighbor_cell, i_element_type);
		}
	}

	//build inner face
	for (const auto& owner_cell : cell_set) {
		const auto owner_cell_index = owner_cell.index();
		const auto face_index_to_node_index_set = owner_cell.calculate_Face_Index_to_Node_Index_Set();

		for (const auto& [face_index, face_node_index_set] : face_index_to_node_index_set) {
			std::set<size_t> inner_face_node_index_set(face_node_index_set.begin(), face_node_index_set.end());
			if (constructed_face_node_index_set_list.find(inner_face_node_index_set) != constructed_face_node_index_set_list.end())
				continue;
			else {
				// find neighbor cell index
				auto cell_index_set = this->find_Cell_Index_Has_This_Face(vertex_node_index_to_cell_index, face_node_index_set);

				if (cell_index_set.size() != 2)
					FATAL_ERROR("fail to construct inner face of " + Editor::to_String(owner_cell_index) + " cell");
				else
					Editor::remove(cell_index_set, owner_cell_index);

				const auto neighbor_cell_index = cell_index_set.front();

				// build inner face figure
				auto owner_side_figure = owner_cell.build_Face_Figure(face_index);

				// add inner face
				const auto neighbor_cell_set_index = cell_index_to_cell_set_index.at(neighbor_cell_index);
				const auto& neighbor_cell = cell_set.at(neighbor_cell_set_index);

				FaceData face_data = { std::move(owner_side_figure), owner_cell_index, owner_cell };
				inner_face_set.emplace_back(std::move(face_data), neighbor_cell_index, neighbor_cell);

				// update list
				constructed_face_node_index_set_list.emplace(std::move(inner_face_node_index_set));
			}
		}
	}

	return element_set;
}

std::vector<size_t> ElementBuilder::find_Cell_Index_Has_This_Face(const std::unordered_map<size_t, std::set<size_t>>& vertex_node_index_to_cell_index, const std::vector<size_t>& face_node_index_set) const {
	const auto start_node_index = face_node_index_set[0];
	const auto end_node_index = face_node_index_set[1];

	const auto& cell_index_set_have_start_node = vertex_node_index_to_cell_index.at(start_node_index);
	const auto& cell_index_set_have_end_node = vertex_node_index_to_cell_index.at(end_node_index);

	std::vector<size_t> cell_index_intersection;
	std::set_intersection(cell_index_set_have_start_node.begin(), cell_index_set_have_start_node.end(), cell_index_set_have_end_node.begin(), cell_index_set_have_end_node.end(), std::back_inserter(cell_index_intersection));

	return cell_index_intersection;
}

std::unordered_map<size_t, size_t> ElementBuilder::match_Periodic_Face_Grid_Data_Index(const std::vector<ElementGridData>& periodic_face_grid_data_set, const std::unordered_map<size_t, std::vector<const MathVector*>>& data_index_to_node_set) const {
	const auto num_periodic_face = periodic_face_grid_data_set.size();

	std::unordered_map<size_t, std::vector<const MathVector*>> data_index_to_X_periodic_face_node_set;
	std::unordered_map<size_t, std::vector<const MathVector*>> data_index_to_Y_periodic_face_node_set;
	std::unordered_map<size_t, std::vector<const MathVector*>> data_index_to_Z_periodic_face_node_set;
	for (size_t data_index = 0; data_index < num_periodic_face; ++data_index) {
		const auto& [index, figure_type, figure_order, element_type, node_index_set] = periodic_face_grid_data_set[data_index];
		switch (element_type) {
		case ElementType::Xperiodic:
			data_index_to_X_periodic_face_node_set.emplace(data_index, data_index_to_node_set.at(data_index));
			break;
		case ElementType::Yperiodic:
			data_index_to_Y_periodic_face_node_set.emplace(data_index, data_index_to_node_set.at(data_index));
			break;
		case ElementType::Zperiodic:
			data_index_to_Z_periodic_face_node_set.emplace(data_index, data_index_to_node_set.at(data_index));
			break;
		default:
			FATAL_TYPE_ERROR;
		}
	}

	const auto num_total_set = static_cast<size_t>(num_periodic_face * 0.5);

	std::unordered_map<size_t, size_t> data_index_to_matched_index;
	data_index_to_matched_index.reserve(num_total_set);
	data_index_to_matched_index.merge(this->match_Periodic_Face_Data_Index_by_Type(data_index_to_X_periodic_face_node_set, ElementType::Xperiodic));
	data_index_to_matched_index.merge(this->match_Periodic_Face_Data_Index_by_Type(data_index_to_Y_periodic_face_node_set, ElementType::Yperiodic));
	data_index_to_matched_index.merge(this->match_Periodic_Face_Data_Index_by_Type(data_index_to_Z_periodic_face_node_set, ElementType::Zperiodic));

	return data_index_to_matched_index;
}

std::unordered_map<size_t, size_t> ElementBuilder::match_Periodic_Face_Data_Index_by_Type(const std::unordered_map<size_t, std::vector<const MathVector*>>& data_index_to_node_set, const ElementType element_type) const {
	const auto num_periodic_face = data_index_to_node_set.size();

	if (num_periodic_face == 0)
		return std::unordered_map<size_t, size_t>();

	std::unordered_set<size_t> matched_index;
	matched_index.reserve(num_periodic_face);

	const auto num_set = static_cast<size_t>(0.5 * num_periodic_face);
	std::unordered_map<size_t, size_t> matched_index_set;
	matched_index_set.reserve(num_set);

	const auto start_iter = data_index_to_node_set.begin();
	const auto end_iter = data_index_to_node_set.end();
	for (auto i_iter = start_iter; i_iter != end_iter; ++i_iter) {
		const auto& [i_index, i_node_set] = *i_iter;

		if (matched_index.find(i_index) != matched_index.end())
			continue;

		for (auto j_iter = std::next(i_iter, 1); j_iter != end_iter; ++j_iter) {
			const auto& [j_index, j_node_set] = *j_iter;

			if (this->is_Matched_Periodic_Face(i_node_set, j_node_set, element_type)) {
				matched_index_set.emplace(i_index, j_index);
				matched_index.emplace(i_index);
				matched_index.emplace(j_index);
				break;
			}
		}
	}

	if (matched_index_set.size() != num_set)
		FATAL_ERROR("some periodic boundaries matched yet");

	return matched_index_set;
}

bool ElementBuilder::is_Matched_Periodic_Face(const std::vector<const MathVector*>& i_periodic_face_consisting_node_set, const std::vector<const MathVector*>& j_periodic_face_consisting_node_set, const ElementType element_type) const {
	if (i_periodic_face_consisting_node_set.size() != j_periodic_face_consisting_node_set.size())
		return false;

	for (const auto& node : i_periodic_face_consisting_node_set) {
		if (this->is_There_Mathced_Periodic_Node(j_periodic_face_consisting_node_set, *node, element_type))
			continue;
		else
			return false;
	}
	return true;
}

bool ElementBuilder::is_There_Mathced_Periodic_Node(const std::vector<const MathVector*>& node_set, const MathVector& node, const ElementType element_type) const {
	switch (element_type) {
	case ElementType::Xperiodic: {
		constexpr size_t x_axis_tag = 0;
		for (const auto& position_vector : node_set) {
			if (this->is_Axis_Translation(*position_vector, node, x_axis_tag))
				return true;
		}
		return false;
	}
	case ElementType::Yperiodic: {
		constexpr size_t y_axis_tag = 1;
		for (const auto& position_vector : node_set) {
			if (this->is_Axis_Translation(*position_vector, node, y_axis_tag))
				return true;
		}
		return false;
	}
	case ElementType::Zperiodic: {
		constexpr size_t z_axis_tag = 2;
		for (const auto& position_vector : node_set) {
			if (this->is_Axis_Translation(*position_vector, node, z_axis_tag))
				return true;
		}
		return false;
	}
	default:
		FATAL_TYPE_ERROR;
		return false;
	}
}

bool ElementBuilder::is_Axis_Translation(const MathVector& node1, const MathVector& node2, const size_t axis_tag) const {
	const auto line_vector = Math::abs(node2 - node1);

	for (size_t i = 0; i < line_vector.size(); ++i) {
		if (i == axis_tag)
			continue;

		if (line_vector[i] > 1.0E-10)
			return false;
	}
	return true;
}




//
//
////void SSPRK::update(const double dt, std::unordered_map<size_t, Cell>& index_to_cell, const std::vector<Face*>& face_set, Limiter& limiter) const
////{
////	const auto num_cell = index_to_cell.size();
////
////	std::unordered_map<size_t, RowMajorMatrix> original_coefficient_set;
////	std::unordered_map<size_t, const RowMajorMatrix&> current_coefficient_set;
////	original_coefficient_set.reserve(num_cell);
////	current_coefficient_set.reserve(num_cell);
////
////	for (const auto& pair : index_to_cell)
////	{
////		const auto& cell_index = pair.first;
////		const auto& cell = pair.second;
////
////		const auto& solution_function = cell.solution_function();
////		const auto& coefficient = solution_function.coefficient();
////
////		original_coefficient_set.emplace(cell_index, coefficient);
////		current_coefficient_set.emplace(cell_index, coefficient);
////	}
////
////
////
////	//step 1
////	for (auto& pair : index_to_cell)
////	{
////		const auto cell_index = pair.first;
////		auto& cell = pair.second;
////
////		auto& solution_function = cell.solution_function();
////		const auto& original_coefficient = original_coefficient_set.at(cell_index);
////		
////		const auto rhs = cell.calculate_RHS();
////
////		auto&& step1_coefficient = original_coefficient + rhs * (dt * 0.391752226571890);
////
////		solution_function.updateCoeffcient(std::move(step1_coefficient));
////	}
////
////	limiter.limit(index_to_cell, face_set);
////
////	for (Face* face_pointer : face_set)
////		face_pointer->calculate_Surface_Integral_Value();
////
////
////
////	//step 2
////	for (auto& pair : index_to_cell)
////	{
////
////		auto& cell = pair.second;
////		auto& solution_function = cell.solution_function();
////
////		const auto cell_index = pair.first;
////		const auto& original_coefficient = original_coefficient_set.at(cell_index);
////		const auto& current_coefficient = current_coefficient_set.at(cell_index);
////
////		const auto rhs = cell.calculate_RHS();
////		
////
////		auto&& step2_coefficient = original_coefficient * 0.444370493651235 + current_coefficient * 0.555629506348765 + rhs * (dt * 0.368410593050371);
////
////
////		solution_function.updateCoeffcient(std::move(step2_coefficient));
////
////	}
////
////	const std::unordered_map<size_t, RowMajorMatrix> step2_coefficient_set(current_coefficient_set.begin(), current_coefficient_set.end());
////	
////	limiter.limit(index_to_cell, face_set);
////
////	for (Face* face_pointer : face_set)
////		face_pointer->calculate_Surface_Integral_Value();
////
////
////
////
////	//step 3
////	for (auto& pair : index_to_cell)
////	{
////
////		auto& cell = pair.second;
////		auto& solution_function = cell.solution_function();
////
////		const auto cell_index = pair.first;
////		const auto& original_coefficient = original_coefficient_set.at(cell_index);
////		const auto& current_coefficient = current_coefficient_set.at(cell_index);
////
////		const auto rhs = cell.calculate_RHS();
////
////
////		auto&& step3_coefficient = original_coefficient * 0.620101851488403 + current_coefficient * 0.379898148511597 + rhs * (dt * 0.251891774271694);
////
////
////		solution_function.updateCoeffcient(std::move(step3_coefficient));
////
////	}
////
////	const std::unordered_map<size_t, RowMajorMatrix> step3_coefficient_set(current_coefficient_set.begin(), current_coefficient_set.end());
////
////	limiter.limit(index_to_cell, face_set);
////
////
////	for (Face* face_pointer : face_set)
////		face_pointer->calculate_Surface_Integral_Value();
////
////
////
////	//step 4
////	std::unordered_map<size_t, RowMajorMatrix> step4_rhs_set;
////
////	for (auto& pair : index_to_cell)
////	{
////
////		auto& cell = pair.second;
////		auto& solution_function = cell.solution_function();
////
////		const auto cell_index = pair.first;
////		const auto& original_coefficient = original_coefficient_set.at(cell_index);
////		const auto& current_coefficient = current_coefficient_set.at(cell_index);
////
////		step4_rhs_set.emplace(cell_index, cell.calculate_RHS());
////
////
////		auto&& step4_coefficient = original_coefficient * 0.178079954393132 + current_coefficient * 0.821920045606868 + step4_rhs_set.at(cell_index) * (dt * 0.544974750228521);
////
////
////		solution_function.updateCoeffcient(std::move(step4_coefficient));
////
////	}
////
////	limiter.limit(index_to_cell, face_set);
////
////
////	for (Face* face_pointer : face_set)
////		face_pointer->calculate_Surface_Integral_Value();
////
////
////	//step 5
////	for (auto& pair : index_to_cell)
////	{
////
////		auto& cell = pair.second;
////		auto& solution_function = cell.solution_function();
////
////		const auto cell_index = pair.first;
////		const auto& step2_coefficient = step2_coefficient_set.at(cell_index);
////		const auto& step3_coefficient = step3_coefficient_set.at(cell_index);
////		const auto& current_coefficient = current_coefficient_set.at(cell_index);
////		const auto& step4_rhs = step4_rhs_set.at(cell_index);
////
////		const auto rhs = cell.calculate_RHS();
////
////
////		auto&& updated_coefficient = step2_coefficient * 0.517231671970585 + step3_coefficient * 0.096059710526147 + current_coefficient * 0.386708617503269 +
////									 step4_rhs * (dt * 0.063692468666290) + rhs * (dt * 0.226007483236906);
////
////
////		solution_function.updateCoeffcient(std::move(updated_coefficient));
////
////	}
////
////	limiter.limit(index_to_cell, face_set);
////
////
////	for (Face* face_pointer : face_set)
////		face_pointer->calculate_Surface_Integral_Value();
////
////}