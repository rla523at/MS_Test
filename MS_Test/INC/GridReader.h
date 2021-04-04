#pragma once

#include "Text.h"	//figure type

#include <array>
#include <map>


struct NodeGridData
{
	size_t index;
	std::vector<double> coord_set;
};


enum class ElementType
{
	Cell,
	SlipWall2D,
	SuperSonicOutlet2D, SuperSonicOutlet3D,
	Xperiodic, Yperiodic, Zperiodic,
	NotInList
};


namespace Editor {
	std::string to_String(const ElementType element_type);
}


struct ElementGridData
{
	size_t index;
	std::string figure_type_name;
	size_t figure_order;
	ElementType element_type;
	std::vector<size_t> node_index_set;
};


struct GridData
{
	std::vector<NodeGridData> node_grid_data_set;
	std::vector<ElementGridData> cell_grid_data_set;
	std::vector<ElementGridData> boundary_face_grid_data_set;
	std::vector<ElementGridData> periodic_face_grid_data_set;
};


class GridReader
{
public:
	GridData read(const std::string& grid_file_name) const;

private:
	virtual std::vector<NodeGridData> read_Node_Data(const std::string& grid_file_path) const = 0;

	virtual std::array<std::vector<ElementGridData>, 3> read_Element_Data(const std::string& grid_file_path) const = 0;

protected:
	ElementType Name_to_Element_Type(const std::string& name) const;
};


#define GRIDREADER_FACTORY GridReaderFactory::instance()
class GridReaderFactory {
private:
	GridReaderFactory(void) = default;

public:
	GridReaderFactory(const GridReaderFactory& other) = delete;
	GridReaderFactory(GridReaderFactory&& other) = delete;

	static GridReaderFactory& instance(void) {
		static GridReaderFactory instance;
		return instance;
	}

	std::unique_ptr<GridReader> order(const std::string& grid_file_type) const;
};


enum class GmshFigureType
{
	POINT = 0,
	LINE_P1 = 1, LINE_P2 = 8, LINE_P3 = 26, LINE_P4 = 27, LINE_P5 = 28, LINE_P6 = 62,
	TRIS_P1 = 2, TRIS_P2 = 9, TRIS_P3 = 21, TRIS_P4 = 23, TRIS_P5 = 25,
	QUAD_P1 = 3, QUAD_P2 = 10, QUAD_P3 = 36, QUAD_P4 = 37, QUAD_P5 = 38, QUAD_P6 = 47,
	TETS_P1 = 4, TETS_P2 = 11, TETS_P3 = 29, TETS_P4 = 30, TETS_P5 = 31,
	HEXA_P1 = 5, HEXA_P2 = 12, HEXA_P3 = 92, HEXA_P4 = 93, HEXA_P5 = 94,
	PRIS_P1 = 6, PRIS_P2 = 13, PRIS_P3 = 90, PRIS_P4 = 91, PRIS_P5 = 106,
	PYRA_P1 = 7, PYRA_P2 = 14, PYRA_P3 = 118, PYRA_P4 = 119
};


class GmshGridReader : public GridReader
{
private:
	std::vector<NodeGridData> read_Node_Data(const std::string& grid_file_path) const override;

	std::array<std::vector<ElementGridData>, 3> read_Element_Data(const std::string& grid_file_path) const override;

private:
	size_t Figure_Type_Index_to_Figure_Order(const size_t figure_type_indx) const;

	std::string Figure_Type_Index_to_Figure_Type_Name(const size_t figure_type_index) const;

	Text read_About(const std::string& grid_file_path, const std::string& target) const;
};
