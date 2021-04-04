#pragma once

#include "Element.h"
#include "Test.h"

void Test1(void) {
	Test_OBJ("build cell");

	const MathVector x0 = { 1,1,0 };
	const MathVector x1 = { 2,2,0 };
	const MathVector x2 = { 1,3,0 };

	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	std::vector<const MathVector*> consisting_node_set = { &x0,&x1,&x2 };
	std::vector<size_t> consisting_node_index_set = { 1,2,3 };
	const IndexedFigure owner_cell_geometry(figure_type, figure_order, std::move(consisting_node_set), std::move(consisting_node_index_set));

	const size_t dimension = 2;
	const auto initial_condition = INITIALCONDITION_FACTORY.order("SineAdvection", 2);

	const size_t solution_index = 1;
	const size_t solution_order = 2;
	CellData cell_data = { solution_index,owner_cell_geometry,solution_order,*initial_condition };

	Cell cell(std::move(cell_data));
}

void Test2(void) {
	Test_OBJ("build Element set");

	const std::string grid_file_name = "Quad_Sample.msh";
	const std::string grid_file_type = "gmsh";

	const auto grid_reader = GRIDREADER_FACTORY.order(grid_file_type);
	auto grid_data = grid_reader->read(grid_file_name);

	const size_t solution_order = 2;
	const auto initial_condition = INITIALCONDITION_FACTORY.order("SineAdvection", 2);

	ElementBuilder element_builder;
	const auto element_set = element_builder.build_Element_Set(grid_data, solution_order, *initial_condition);
	std::cout << "Test..";
}