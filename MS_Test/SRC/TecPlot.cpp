#include "../INC/TecPlot.h"

std::vector<std::vector<double>> TecPlot2DConservativeSolutionData::build(const std::vector<const Cell*>& post_target, const size_t num_total_post_point) const {
	std::vector<double> rho_set, rhou_set, rhov_set, rhoE_set;
	rho_set.reserve(num_total_post_point);
	rhou_set.reserve(num_total_post_point);
	rhov_set.reserve(num_total_post_point);
	rhoE_set.reserve(num_total_post_point);

	for (const auto& cell : post_target) {
		const auto solution_post_point = cell->calculate_Post_Point_Solution();

		constexpr size_t rho_index = 0;
		constexpr size_t rhou_index = 1;
		constexpr size_t rhov_index = 2;
		constexpr size_t rhoE_index = 3;

		Editor::merge(rho_set, solution_post_point.row(rho_index));
		Editor::merge(rhou_set, solution_post_point.row(rhou_index));
		Editor::merge(rhov_set, solution_post_point.row(rhov_index));
		Editor::merge(rhoE_set, solution_post_point.row(rhoE_index));
	}

	return { rho_set,rhou_set,rhov_set,rhoE_set };
}

std::vector<std::vector<double>> TecPlot2DPrimitiveSolutionData::build(const std::vector<const Cell*>& post_target, const size_t num_total_post_point) const {
	std::vector<double> rho_set, u_set, v_set, p_set;
	rho_set.reserve(num_total_post_point);
	u_set.reserve(num_total_post_point);
	v_set.reserve(num_total_post_point);
	p_set.reserve(num_total_post_point);

	for (const auto& cell : post_target) {
		const auto solution_post_point = cell->calculate_Post_Point_Solution();

		const auto [num_equation, num_post_point_in_cell] = solution_post_point.size();
		for (size_t i = 0; i < num_post_point_in_cell; ++i) {
			const auto post_point_solution = solution_post_point.column(i);
			const auto primitive_solution = EOS::convert_To_Primitive_2D(post_point_solution);

			const auto rho = primitive_solution[0];
			const auto u = primitive_solution[1];
			const auto v = primitive_solution[2];
			const auto p = primitive_solution[3];

			rho_set.push_back(rho);
			u_set.push_back(u);
			v_set.push_back(v);
			p_set.push_back(p);
		}
	}

	return { rho_set,u_set,v_set,p_set };
}

void TecPlot::initialize(PostOption&& post_option_data, std::vector<Cell>& cell_set) {
	auto& [post_file_name, post_order, post_solution_type_name, dimension] = post_option_data;

	//set post conditions
	this->post_file_name_ = std::move(post_file_name);
	this->post_order_ = post_order;

	switch (dimension) {
	case 2: {
		this->grid_variables_ = "\"X\" \"Y\"";
		this->zone_type_ = "FETRIANGLE";

		if (StringEditor::is_same_without_Case_Sensitivity(post_solution_type_name, "conservative"))
			this->solution_data_builder_ = std::make_unique<TecPlot2DConservativeSolutionData>();
		else if (StringEditor::is_same_without_Case_Sensitivity(post_solution_type_name, "primitive"))
			this->solution_data_builder_ = std::make_unique<TecPlot2DPrimitiveSolutionData>();
		else
			FATAL_TYPE_ERROR;
		break;
	}
	default:
		FATAL_TYPE_ERROR;
		break;
	}

	//set post target
	const auto num_cell = cell_set.size();
	this->post_target_.resize(num_cell);

	for (size_t i = 0; i < num_cell; ++i)
		this->post_target_[i] = &cell_set[i];

	//build grid zone data
	std::vector<MathVector> total_post_point_set;
	std::vector<std::vector<double>> total_connectivity_set;

	for (auto& cell : cell_set) {
		const auto figure_type = cell.figure_type();

		auto connectivity_start_index = total_post_point_set.size();
		Editor::merge(total_connectivity_set, cell.calculate_Connecitivity_Set(this->post_order_, connectivity_start_index));

		auto cell_post_point_set = cell.calculate_Post_Point_Set(this->post_order_);

		//initalize post point solution calculator
		cell.initialize_Post_Point_Solution_Calculator(cell_post_point_set);

		Editor::merge(total_post_point_set, std::move(cell_post_point_set));

		connectivity_start_index += total_post_point_set.back().size();
	}

	this->num_total_post_point_ = total_post_point_set.size();
	this->num_simplex_ = total_connectivity_set.size();

	std::vector<std::vector<double>> grid_data_block_set(dimension);
	grid_data_block_set.reserve(dimension + this->num_simplex_);

	for (const auto& post_point : total_post_point_set)
		for (size_t d = 0; d < dimension; ++d)
			grid_data_block_set[d].emplace_back(post_point[d]);

	Editor::merge(grid_data_block_set, std::move(total_connectivity_set));


	//write grid file header text
	const auto file_title = "Grid_" + this->post_file_name_;
	const auto file_type = "Grid";
	const auto variables = this->grid_variables_;

	const auto file_header_text = this->write_File_Header_Text(file_title, file_type, variables);

	//write grid zone header text
	const auto zone_title = file_title;
	const auto zone_type = this->zone_type_;
	const auto num_nodes = this->num_total_post_point_;
	const auto num_elements = this->num_simplex_;
	const auto data_packing_type = "Block";
	const size_t strand_id = 1;
	const double solution_time = 0.0;

	const auto zone_header_text = this->write_Zone_Header_Text(zone_title, zone_type, num_nodes, num_elements, data_packing_type, strand_id, solution_time);

	//write grid zone data text
	const auto zone_data_text = this->write_Zone_Data_Text(grid_data_block_set);

	//write grid file
	const auto post_file_directory = "Post/" + this->post_file_name_;
	std::filesystem::create_directories(post_file_directory);

	const auto grid_file_path = post_file_directory + "/" + file_title + ".plt";

	file_header_text.write(grid_file_path);
	zone_header_text.add_Write(grid_file_path);
	zone_data_text.add_Write(grid_file_path);
};

void TecPlot::post_Solution_File(const double solution_time, const std::string& comment) const {
	//write file header text
	std::string file_title;
	if (comment.empty())
		file_title << "Solution_" << this->post_file_name_ << "_" << solution_time;
	else
		file_title << "Solution_" << this->post_file_name_ + "__" + comment;

	const auto file_type = "Solution";
	const auto variables = this->solution_data_builder_->variables();

	const auto file_header_text = this->write_File_Header_Text(file_title, file_type, variables);

	//write zone header text
	const auto zone_title = file_title;
	const auto zone_type = this->zone_type_;
	const auto num_nodes = this->num_total_post_point_;
	const auto num_elements = this->num_simplex_;
	const auto data_packing_type = "Block";
	const auto strand_id = this->num_post_;

	const auto zone_header_text = this->write_Zone_Header_Text(zone_title, zone_type, num_nodes, num_elements, data_packing_type, strand_id, solution_time);

	//write zone data text
	const auto zone_data_block_set = this->solution_data_builder_->build(this->post_target_, this->num_total_post_point_);

	const auto zone_data_text = this->write_Zone_Data_Text(zone_data_block_set);

	//write solution file
	const auto post_file_directory = "Post/" + this->post_file_name_;
	const auto solution_file_path = post_file_directory + "/" + file_title + ".plt";

	file_header_text.write(solution_file_path);
	zone_header_text.add_Write(solution_file_path);
	zone_data_text.add_Write(solution_file_path);

	this->num_post_++;
}

Text TecPlot::write_File_Header_Text(const std::string& file_title, const std::string& file_type, const std::string& variables) const {
	const auto title_sentence = "Title = \"" + file_title + "\"";
	const auto file_type_sentence = "FileType = " + file_type;
	const auto variable_sentence = "Variables = " + variables + "\n";

	return { title_sentence, file_type_sentence, variable_sentence };
}

Text TecPlot::write_Zone_Header_Text(const std::string& zone_title, const std::string& zone_type, const size_t num_nodes, const size_t num_elements, const std::string& data_packing_type, const size_t strand_id, const double solution_time) const {
	const auto zone_title_sentence = "Zone T = \"" + zone_title + "\"";
	const auto zone_type_sentence = "ZoneType = " + zone_type;
	const auto node_sentence = "Nodes = " + Editor::to_String(num_nodes);
	const auto element_sentence = "Elements = " + Editor::to_String(num_elements);
	const auto data_packing_sentence = "DataPacking = " + data_packing_type;
	const auto strand_id_sentence = "StrandID = " + Editor::to_String(strand_id);
	const auto solution_time_sentence = "SolutionTime = " + Editor::to_String(solution_time) + "\n\n";

	return { zone_title_sentence, zone_type_sentence, node_sentence, element_sentence ,data_packing_sentence ,strand_id_sentence, solution_time_sentence };
}

Text TecPlot::write_Zone_Data_Text(const std::vector<std::vector<double>>& data_block_set) const {
	constexpr size_t maximum_number_in_line = 10;

	Text zone_data_text;
	for (const auto& data_block : data_block_set) {
		std::string data_sentence;
		size_t data_in_line = 0;
		for (const auto& data : data_block) {
			data_sentence << data << " ";
			if (++data_in_line == maximum_number_in_line) {
				zone_data_text << std::move(data_sentence);
				data_in_line = 0;
			}
		}
		zone_data_text << std::move(data_sentence);
	}

	return zone_data_text;
}





//
//std::vector<std::vector<double>> TecPlotWriter::build_Cell_Index_File_Zone_Data_Block_Set(const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_, const size_t num_post_point) const
//{
//	std::vector<double> cell_index_set;
//	cell_index_set.reserve(num_post_point);
//
//	for (const auto& pair : cell_index_to_post_point_set_)
//	{
//		const auto& cell_index = pair.first;
//		const auto& post_point_set = pair.second;
//		const auto num_post_point_in_cell = post_point_set.size();
//		
//		for (size_t i = 0; i < num_post_point_in_cell; ++i)
//			cell_index_set.push_back(static_cast<double>(cell_index));
//	}
//
//	std::vector<std::vector<double>> data_block;
//	data_block.push_back(cell_index_set);
//
//	return data_block;
//}
//
//std::vector<std::vector<double>> TecPlotWriter2D::build_Grid_File_Zone_Data_Block_Set(const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set, const size_t num_post_point) const
//{
//
//}
//
//std::string TecPlotWriter::write_File_Title(const std::string& title) const
//{
//	std::string file_title_sentence;
//	return file_title_sentence << "Title= \"" << title << "\"";
//};
//
//std::string TecPlotWriter::write_File_Type(const std::string& type) const
//{
//	std::string file_type_sentence;
//	return file_type_sentence << "FileType = " << type.returnUpperCase();
//}
//
//std::string TecPlotWriter::write_Num_Zone_Node(const size_t num_node) const
//{
//	std::string zone_node_sentence;
//	return zone_node_sentence << "Nodes = " << num_node;
//}
//
//std::string TecPlotWriter::write_Num_Zone_Element(const size_t num_element) const
//{
//	std::string zone_element_sentence;
//	return zone_element_sentence << "Elements = " << num_element;
//}
//
//std::string TecPlotWriter::write_Zone_Title(const std::string& title) const
//{
//	std::string zone_title_sentence;
//	return zone_title_sentence << "Zone T= \"" << title << "\"";
//}
//
//std::vector<std::vector<double>> ConservativeVariableWriter2D::build_Solution_File_Zone_Data_Block_Set
//(
//	const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_,
//	const std::unordered_map<size_t, Cell>& index_to_cell,
//	const size_t num_post_point
//)	const
//{
//
//}
//
//std::vector<std::vector<double>> PrimitiveVariableWriter2D::build_Solution_File_Zone_Data_Block_Set
//(
//	const std::unordered_map<size_t, std::vector<Node>>& cell_index_to_post_point_set_,
//	const std::unordered_map<size_t, Cell>& index_to_cell,
//	const size_t num_post_point
//)	const
//{
//
//}
//
//
//
//
//
////
////std::vector<std::vector<double>> ConservativeVariableWriter2D::buildModeAnalysisDataBlockSet(const SolutionFunction& solution_function, const std::vector<Node>& post_point_set, const size_t mode) const
////{
////
////	const auto num_post_point = post_point_set.size();
////
////
////	std::vector<double> x1_coord_set, x2_coord_set, Pn_mode_solution_set;
////
////	x1_coord_set.reserve(num_post_point);
////	x2_coord_set.reserve(num_post_point);
////	Pn_mode_solution_set.reserve(num_post_point);
////
////
////	for (const auto& post_point : post_point_set)
////	{
////
////		x1_coord_set.push_back(post_point.getFirstCoord());
////		x2_coord_set.push_back(post_point.getSecondCoord());
////
////
////		const auto solution_at_post_point = solution_function.calPnModeSolution(mode, post_point);
////
////		constexpr size_t mode_analysis_variable_index = 0;
////
////		Pn_mode_solution_set.emplace_back(solution_at_post_point[mode_analysis_variable_index]);
////
////	}
////
////
////	constexpr size_t num_block = 3;
////
////	std::vector<std::vector<double>> mode_analysis_data_block_set;
////
////	mode_analysis_data_block_set.emplace_back(x1_coord_set);
////	mode_analysis_data_block_set.emplace_back(x2_coord_set);
////	mode_analysis_data_block_set.emplace_back(Pn_mode_solution_set);
////
////
////	return mode_analysis_data_block_set;
////
////}
////
////
////
////
////std::vector<std::vector<double>> PrimitiveVariableWriter2D::buildModeAnalysisDataBlockSet(const SolutionFunction& solution_function, const std::vector<Node>& post_point_set, const size_t mode) const
////{
////
////	const auto num_post_point = post_point_set.size();
////
////
////	std::vector<double> x1_coord_set, x2_coord_set, Pn_mode_solution_set;
////
////	x1_coord_set.reserve(num_post_point);
////	x2_coord_set.reserve(num_post_point);
////	Pn_mode_solution_set.reserve(num_post_point);
////
////
////	for (const auto& post_point : post_point_set)
////	{
////
////		x1_coord_set.push_back(post_point.getFirstCoord());
////		x2_coord_set.push_back(post_point.getSecondCoord());
////
////
////		const auto solution_at_post_point = solution_function.calPnModeSolution(mode, post_point);
////
////		constexpr size_t mode_analysis_variable_index = 0;
////
////		Pn_mode_solution_set.emplace_back(solution_at_post_point[mode_analysis_variable_index]);
////
////	}
////
////
////	constexpr size_t num_block = 3;
////
////	std::vector<std::vector<double>> mode_analysis_data_block_set;
////
////	mode_analysis_data_block_set.emplace_back(x1_coord_set);
////	mode_analysis_data_block_set.emplace_back(x2_coord_set);
////	mode_analysis_data_block_set.emplace_back(Pn_mode_solution_set);
////
////
////	return mode_analysis_data_block_set;
//
////}
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//Post::Post(const size_t dimension, const std::string& post_file_name, const size_t post_order, const std::string& post_variable_type_name)
//	: post_file_name_(post_file_name), this->post_order_(post_order)
//{
//	if (post_order == 0)
//	{
//		ERROR_NOTICE << "post order 0 is meaningless";
//		FATAL_ERROR;
//	}
//
//	if (post_variable_type_name.isNoCaseSensitivitySame("Conservative"))
//	{
//		switch (dimension)
//		{
//
//		case 2:
//		{
//			this->writer_ = new ConservativeVariableWriter2D();
//			break;
//		}
//
//		default:
//			FATAL_ERROR;
//
//		}
//	}
//	else if (post_variable_type_name.isNoCaseSensitivitySame("Primitive"))
//	{
//		switch (dimension)
//		{
//
//		case 2:
//		{
//			this->writer_ = new PrimitiveVariableWriter2D();
//			break;
//		}
//
//		default:
//			FATAL_ERROR;
//		}
//	}
//	else
//		FATAL_ERROR;
//	
//}
//
//void Post::preCalculate(std::unordered_map<size_t, Cell>& index_to_cell)
//{
//	size_t connectivity_start_index = 0;
//
//	for (auto& [cell_index, cell] : index_to_cell)
//	{		
//		auto& solution_function = cell.solution_function();
//		const auto& cell_geometry = cell.geometry();
//
//		const auto figure_type = cell_geometry.figure_type();
//		const auto& transformation_function = cell_geometry.transformation_function();
//
//		if (this->figure_type_to_reference_post_point_set_.find(figure_type) == this->figure_type_to_reference_post_point_set_.end())
//			this->add_Reference_Post_Point_Set(figure_type, this->this->post_order_);
//
//		const auto& reference_post_point_set = this->figure_type_to_reference_post_point_set_.at(figure_type);
//		const auto post_point_set = transformation_function(reference_post_point_set);
//
// 		this->cell_index_to_post_point_set_.emplace(cell_index, post_point_set);
//		this->record_Connectivity(figure_type, connectivity_start_index);
//		
//		//update connecitivity start index after record connecitivity
//		connectivity_start_index += post_point_set.size();
//
//		solution_function.set_BasisPostPoint(post_point_set);
//	}
//}
//
//void Post::write_Cell_Index_File(void) const
//{
//	++this->num_post_;
//
//	const auto num_post_point = this->calculate_Num_Post_Point();
//	const auto num_simplex = this->connectivity_set_.size();
//	const auto cell_index_zone_data_block_set = this->writer_->build_Cell_Index_File_Zone_Data_Block_Set(this->cell_index_to_post_point_set_, num_post_point);
//
//
//	std::string title;
//	title << this->post_file_name_ << "_cell_index";
//
//	//write file header 
//	const auto file_title = this->writer_->write_File_Title(title);
//	const auto file_type = this->writer_->write_File_Type("Solution");
//	const auto variable_sentence = this->writer_->write_Cell_Index_File_Variable();
//
//	const auto cell_index_file_header_text = this->write_File_Header_Text(file_title, file_type, variable_sentence);
//
//	//write zone header
//	auto cell_index_zone_header_text = this->write_Zone_Header_Text(title, num_post_point, num_simplex);
//
//	//write zone data 
//	const auto cell_index_zone_data_text = this->write_Zone_Data_Text(cell_index_zone_data_block_set);
//
//
//	const auto cell_index_file_size = cell_index_file_header_text.size() + cell_index_zone_header_text.size() + cell_index_zone_data_text.size();
//
//	Text cell_index_text;
//	cell_index_text.reserve(cell_index_file_size);
//
//	cell_index_text.merge(cell_index_file_header_text);
//	cell_index_text.merge(cell_index_zone_header_text);
//	cell_index_text.merge(cell_index_zone_data_text);
//
//	std::string cell_index_file_path;
//	cell_index_file_path << "Post/" << this->post_file_name_ << "_cell_index" << this->num_post_ << ".plt";
//
//	cell_index_text.write(cell_index_file_path);
//}
//
//void Post::write(void) const
//{
//	const auto title = this->post_file_name_;
//
//	//write file header 
//	const auto file_title = this->writer_->write_File_Title(title);
//	const auto file_type = this->writer_->write_File_Type("Grid");
//	const auto variable_sentence = this->writer_->write_Grid_File_Variable();
//
//	const auto grid_file_header_text = this->write_File_Header_Text(file_title, file_type, variable_sentence);
//
//	//write zone header
//	const auto num_post_point = this->calculate_Num_Post_Point();
//	const auto num_simplex = this->connectivity_set_.size();
//
//	const auto grid_zone_header_text = this->write_Zone_Header_Text(title, num_post_point, num_simplex);
//
//	//write zone data 
//	const auto grid_zone_data_block_set = this->writer_->build_Grid_File_Zone_Data_Block_Set(this->cell_index_to_post_point_set_, num_post_point);
//
//	const auto grid_zone_data_text = this->write_Zone_Data_Text(grid_zone_data_block_set);
//
//	//write connectivity 
//	const auto grid_connectivity_text = this->write_Connectivity_Text(this->connectivity_set_);
//
//	const auto grid_file_size = grid_file_header_text.size() + grid_zone_header_text.size() + grid_zone_data_text.size() + grid_connectivity_text.size();
//
//	Text grid_text;
//	grid_text.reserve(grid_file_size);
//	grid_text.merge(grid_file_header_text);
//	grid_text.merge(grid_zone_header_text);
//	grid_text.merge(grid_zone_data_text);
//	grid_text.merge(grid_connectivity_text);
//
//	std::string grid_file_path;
//	grid_file_path << "Post/_" << this->post_file_name_ << "_grid.plt";
//
//	grid_text.write(grid_file_path);
//}
//
//void Post::write_Solution_File(const std::unordered_map<size_t, Cell>& index_to_cell, const double current_time) const
//{
//	++this->num_post_;
//
//	std::string title;
//	title << this->post_file_name_ << "_" << current_time << "s";
//
//	//write file header 
//	const auto file_title = this->writer_->write_File_Title(title);
//	const auto file_type = this->writer_->write_File_Type("Solution");
//	const auto variable_sentence = this->writer_->write_Solution_File_Variable();
//
//	const auto solution_file_header_text = this->write_File_Header_Text(file_title, file_type, variable_sentence);
//
//	//write zone header
//	const auto num_post_point = this->calculate_Num_Post_Point();
//	const auto num_simplex = this->connectivity_set_.size();
//
//	std::string additional_zone_header;
//	additional_zone_header << "SolutionTime = " << current_time;
//	additional_zone_header << "StrandId = " << num_post_;
//
//	auto solution_zone_header_text = this->write_Zone_Header_Text(title, num_post_point, num_simplex);
//	solution_zone_header_text << additional_zone_header;
//
//	//write zone data 
//	const auto solution_zone_data_block_set = this->writer_->build_Solution_File_Zone_Data_Block_Set(this->cell_index_to_post_point_set_, index_to_cell, num_post_point);
//
//	const auto solution_zone_data_text = this->write_Zone_Data_Text(solution_zone_data_block_set);
//
//
//	//write solution text
//	const auto solution_file_size = solution_file_header_text.size() + solution_zone_header_text.size() + solution_zone_data_text.size();
//
//	Text solution_text;
//	solution_text.reserve(solution_file_size);
//	solution_text.merge(solution_file_header_text);
//	solution_text.merge(solution_zone_header_text);
//	solution_text.merge(solution_zone_data_text);
//
//	std::string solution_file_path;
//	solution_file_path << "Post/" << this->post_file_name_ << "_solution_" << this->num_post_ << ".plt";
//
//	solution_text.write(solution_file_path);
//}
//
//
//
//
//
//
//
//
////
////
////void Post::writeModeAnalysisFile(const Geometry& geometry, const SolutionFunction& solution_function)
////{
////
////	++this->num_post_;
////
////	const auto& transformation_function = geometry.getTransformationFunction();
////	const auto& figure_type = geometry.getFigureType();
////	const auto& solution_order = solution_function.getSolutionOrder();
////	
////	const auto& reference_post_point_set = this->figure_type_to_reference_post_point_set_.at(figure_type);
////	const auto post_point_set = transformation_function(reference_post_point_set);
////	const auto num_post_point = post_point_set.size();
////
////	const auto& reference_connectivity = this->figure_type_to_reference_connectivity_.at(figure_type);
////	const auto connectivity_text = this->writeConnectivityText(reference_connectivity);
////	const auto num_simplex = reference_connectivity.size();
////
////
////	const auto num_mode = solution_order + 1;
////	for (size_t mode = 0; mode < num_mode; ++mode)
////	{
////
////		const auto title = "P" + TOSTRING(mode) + "mode solution";
////	
////		std::string title_sentence, file_type_sentence, variable_sentence;
		////title_sentence		<< "Title= \"" << title << "\"";
		////file_type_sentence	<< "FileType = FULL";
		////variable_sentence	<< this->writer_->writeModeAnalysisVariablestd::string();
////
////		Text file_header_text;
////		file_header_text << title_sentence
////						 << file_type_sentence 
////						 << variable_sentence;
////
////
////		std::string zone_title_sentence, zone_type_sentence, node_sentence, element_sentence, data_packing_sentence, strand_id_sentence, solution_time_sentece;
////		zone_title_sentence		<< "Zone T = \"" << title << "\"";
////		zone_type_sentence		<< this->writer_->writeZoneType();
////		node_sentence			<< "Nodes = " << num_post_point;
////		element_sentence		<< "Elements = " << num_simplex;
////		data_packing_sentence	<< "DataPacking = Block";
////		strand_id_sentence		<< "StrandID = " << this->num_post_;
////		solution_time_sentece	<< "SolutionTime = " << 99;
////
////		Text zone_header_text;
////		zone_header_text << title_sentence
////						 << variable_sentence
////						 << zone_title_sentence
////						 << zone_type_sentence
////						 << node_sentence
////						 << element_sentence
////						 << data_packing_sentence
////						 << strand_id_sentence
////						 << solution_time_sentece;
////		
////
////
////		const auto mode_analysis_data_block_set = this->writer_->buildModeAnalysisDataBlockSet(solution_function, post_point_set, mode);
////		const auto data_text = this->writeDataText(mode_analysis_data_block_set);
////
////
////		const auto num_line = file_header_text.size() + zone_header_text.size() + data_text.size() + connectivity_text.size();
////
////		Text post_text;
////		post_text.reserve(num_line);
////
////		post_text.merge(zone_header_text);
////		post_text.merge(data_text);
////		post_text.merge(connectivity_text);
////
////
////		std::string post_file_path = "Post/" + this->post_file_name_ + "_" + title + "_" + TOSTRING(this->num_post_) + ".plt";
////
////		TextWriter::write(post_file_path, post_text);
////
////	}
////
////}
////
//
//
//

//
//

//
//Text Post::write_Zone_Data_Text(const std::vector<std::vector<double>>& data_block_set) const

//
//Text Post::write_Connectivity_Text(const std::vector<std::vector<size_t>>& connectivity_set) const
//{
//	Text connectivity_text;
//
//	for (const auto& connectivity : connectivity_set)
//	{
//		std::string connectivity_sentence;
//		
//		for (const auto& node_index : connectivity)
//			connectivity_sentence << node_index << " ";
//
//		connectivity_text << connectivity_sentence;
//	}
//
//	return connectivity_text;
//}
//
