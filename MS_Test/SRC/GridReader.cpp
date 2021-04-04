#include "../INC/GridReader.h"

namespace Editor {
	std::string to_String(const ElementType element_type) {
		switch (element_type) {
		case ElementType::Cell:
			return "Cell";
		case ElementType::SlipWall2D:
			return "SlipWall2D";
		case ElementType::SuperSonicOutlet2D:
			return "SuperSonicOutlet2D";
		case ElementType::SuperSonicOutlet3D:
			return "SuperSonicOutlet3D";
		case ElementType::Xperiodic:
			return "Xperiodic";
		case ElementType::Yperiodic:
			return "Yperiodic";
		case ElementType::Zperiodic:
			return "Zperiodic";
		default:
			FATAL_TYPE_ERROR;
			return std::string();
		}
	}
}

GridData GridReader::read(const std::string& grid_file_name) const {
	const auto grid_file_path = "Grid/" + grid_file_name + ".msh";

	auto node_data_set = this->read_Node_Data(grid_file_path);
	auto element_data_set = this->read_Element_Data(grid_file_path);
	return { node_data_set, element_data_set[0], element_data_set[1], element_data_set[2] };
}

ElementType GridReader::Name_to_Element_Type(const std::string& name) const {
	if (StringEditor::is_same_without_Case_Sensitivity(name, "Unspecified"))
		return ElementType::Cell;
	else if (StringEditor::is_same_without_Case_Sensitivity(name, "SlipWall2D"))
		return ElementType::SlipWall2D;
	else if (StringEditor::is_same_without_Case_Sensitivity(name, "SuperSonicOutlet2D"))
		return ElementType::SuperSonicOutlet2D;
	else if (StringEditor::is_same_without_Case_Sensitivity(name, "SuperSonicOutlet3D"))
		return ElementType::SuperSonicOutlet3D;
	else if (StringEditor::is_same_without_Case_Sensitivity(name, "Xperiodic"))
		return ElementType::Xperiodic;
	else if (StringEditor::is_same_without_Case_Sensitivity(name, "Yperiodic"))
		return ElementType::Yperiodic;
	else if (StringEditor::is_same_without_Case_Sensitivity(name, "Zperiodic"))
		return ElementType::Zperiodic;
	else {
		FATAL_ERROR(name + "is not prorper element type\n");
		return ElementType::NotInList;
	}
}

std::unique_ptr<GridReader> GridReaderFactory::order(const std::string& grid_file_type) const {
	if (StringEditor::is_same_without_Case_Sensitivity(grid_file_type, "gmsh"))
		return std::make_unique<GmshGridReader>();
	else
		return nullptr;
}

size_t GmshGridReader::Figure_Type_Index_to_Figure_Order(const size_t element_type_indx) const {
	switch (static_cast<GmshFigureType>(element_type_indx)) {
	case GmshFigureType::POINT:			return 0;
	case GmshFigureType::LINE_P1:
	case GmshFigureType::TRIS_P1:
	case GmshFigureType::QUAD_P1:
	case GmshFigureType::TETS_P1:
	case GmshFigureType::HEXA_P1:
	case GmshFigureType::PRIS_P1:
	case GmshFigureType::PYRA_P1:		return 1;
	case GmshFigureType::LINE_P2:
	case GmshFigureType::TRIS_P2:
	case GmshFigureType::QUAD_P2:
	case GmshFigureType::TETS_P2:
	case GmshFigureType::HEXA_P2:
	case GmshFigureType::PRIS_P2:
	case GmshFigureType::PYRA_P2:		return 2;
	case GmshFigureType::LINE_P3:
	case GmshFigureType::TRIS_P3:
	case GmshFigureType::QUAD_P3:
	case GmshFigureType::TETS_P3:
	case GmshFigureType::HEXA_P3:
	case GmshFigureType::PRIS_P3:
	case GmshFigureType::PYRA_P3:		return 3;
	case GmshFigureType::LINE_P4:
	case GmshFigureType::TRIS_P4:
	case GmshFigureType::QUAD_P4:
	case GmshFigureType::TETS_P4:
	case GmshFigureType::HEXA_P4:
	case GmshFigureType::PRIS_P4:
	case GmshFigureType::PYRA_P4:		return 4;
	case GmshFigureType::LINE_P5:
	case GmshFigureType::TRIS_P5:
	case GmshFigureType::QUAD_P5:
	case GmshFigureType::TETS_P5:
	case GmshFigureType::HEXA_P5:
	case GmshFigureType::PRIS_P5:		return 5;
	case GmshFigureType::LINE_P6:
	case GmshFigureType::QUAD_P6:		return 6;
	default:
		FATAL_TYPE_ERROR;
		return NULL;
	}
}

std::string GmshGridReader::Figure_Type_Index_to_Figure_Type_Name(const size_t element_type_index) const {
	switch (static_cast<GmshFigureType>(element_type_index)) {
	case GmshFigureType::POINT:		return "Point";
	case GmshFigureType::LINE_P1:
	case GmshFigureType::LINE_P2:
	case GmshFigureType::LINE_P3:
	case GmshFigureType::LINE_P4:
	case GmshFigureType::LINE_P5:
	case GmshFigureType::LINE_P6:		return "Line";
	case GmshFigureType::TRIS_P1:
	case GmshFigureType::TRIS_P2:
	case GmshFigureType::TRIS_P3:
	case GmshFigureType::TRIS_P4:
	case GmshFigureType::TRIS_P5:		return "Triangle";
	case GmshFigureType::QUAD_P1:
	case GmshFigureType::QUAD_P2:
	case GmshFigureType::QUAD_P3:
	case GmshFigureType::QUAD_P4:
	case GmshFigureType::QUAD_P5:
	case GmshFigureType::QUAD_P6:		return "Quadrilateral";
	case GmshFigureType::TETS_P1:
	case GmshFigureType::TETS_P2:
	case GmshFigureType::TETS_P3:
	case GmshFigureType::TETS_P4:
	case GmshFigureType::TETS_P5:		//return ReferenceFigureType::TETS;
	case GmshFigureType::HEXA_P1:
	case GmshFigureType::HEXA_P2:
	case GmshFigureType::HEXA_P3:
	case GmshFigureType::HEXA_P4:
	case GmshFigureType::HEXA_P5:		//return ReferenceFigureType::HEXA;
	case GmshFigureType::PRIS_P1:
	case GmshFigureType::PRIS_P2:
	case GmshFigureType::PRIS_P3:
	case GmshFigureType::PRIS_P4:
	case GmshFigureType::PRIS_P5:		//return ReferenceFigureType::PRIS;
	case GmshFigureType::PYRA_P1:
	case GmshFigureType::PYRA_P2:
	case GmshFigureType::PYRA_P3:
	case GmshFigureType::PYRA_P4:		//return ReferenceFigureType::PYRA;
	default:
		FATAL_TYPE_ERROR;
		return "NotInList";
	}
}

Text GmshGridReader::read_About(const std::string& grid_file_path, const std::string& target) const {
	std::ifstream grid_file_stream(grid_file_path);
	std::string target_string = "$" + target;

	const auto position = Tool::find_First_Position(grid_file_stream, target_string);
	if (position == std::string::npos)
		FATAL_ERROR("can't find " + target_string + " in grid file");

	constexpr size_t num_info_line = 2;
	const Text info_text(grid_file_stream, position, num_info_line);
	const auto num_data = StringEditor::toValue<size_t>(info_text.back());

	return Text(grid_file_stream, num_data);
}

std::vector<NodeGridData> GmshGridReader::read_Node_Data(const std::string& grid_file_path) const {
	const auto node_text = this->read_About(grid_file_path, "Nodes");
	const auto num_node = node_text.size();

	std::vector<NodeGridData> node_data;
	node_data.reserve(num_node);

	for (const auto& node_sentence : node_text) {
		const char delimiter = ' ';
		auto parsed_sentence_set = StringEditor::parse(node_sentence, delimiter);

		auto value_set = StringEditor::toValue<double>(parsed_sentence_set);

		const auto node_index = static_cast<size_t>(value_set.front());
		value_set.erase(value_set.begin());
		auto& node_coord = value_set;

		node_data.emplace_back(NodeGridData{ node_index, std::move(node_coord) });
	}

	return node_data;
}

std::array<std::vector<ElementGridData>, 3> GmshGridReader::read_Element_Data(const std::string& grid_file_path) const {
	std::map<size_t, ElementType> physical_group_index_to_element_type;

	const auto physical_name_text = this->read_About(grid_file_path, "PhysicalNames");

	for (const auto& physical_name_sentence : physical_name_text) {
		const char delimiter = ' ';
		auto parsed_sentence_set = StringEditor::parse(physical_name_sentence, delimiter);

		//const size_t dimension = parsed_sentence_set[0].toValue<size_t>();
		const auto index = StringEditor::toValue<size_t>(parsed_sentence_set[1]);
		const auto& name = Editor::remove(parsed_sentence_set[2], "\"");
		const auto element_type = this->Name_to_Element_Type(name);

		physical_group_index_to_element_type.emplace(index, element_type);
	}

	const auto element_text = this->read_About(grid_file_path, "Elements");

	std::vector<ElementGridData> cell_data;
	std::vector<ElementGridData> boundary_face_data;
	std::vector<ElementGridData> periodic_face_data;

	for (const auto& element_sentence : element_text) {
		const auto delimiter = ' ';
		auto parsed_sentence_set = StringEditor::parse(element_sentence, delimiter);

		auto value_set = StringEditor::toValue<size_t>(parsed_sentence_set);

		const auto index = value_set[0];
		const auto figure_type_index = value_set[1];
		//const auto num_tag = value[2];
		const auto physical_gorup_index = value_set[3];
		//const auto element_group_index = value[4];

		constexpr size_t num_index = 5;
		value_set.erase(value_set.begin(), value_set.begin() + num_index);

		const auto figure_type_name = this->Figure_Type_Index_to_Figure_Type_Name(figure_type_index);
		const auto figure_order = this->Figure_Type_Index_to_Figure_Order(figure_type_index);
		const auto& element_type = physical_group_index_to_element_type.at(physical_gorup_index);
		auto& consisting_node_index_set = value_set;

		ElementGridData element_data = { index, figure_type_name, figure_order, element_type, std::move(consisting_node_index_set) };

		switch (element_type) {
		case ElementType::Cell:
			cell_data.emplace_back(std::move(element_data));
			break;
		case ElementType::Xperiodic:
		case ElementType::Yperiodic:
		case ElementType::Zperiodic:
			periodic_face_data.emplace_back(std::move(element_data));
			break;
		default:
			boundary_face_data.emplace_back(std::move(element_data));
			break;
		}
	}

	return { cell_data, boundary_face_data, periodic_face_data };
}