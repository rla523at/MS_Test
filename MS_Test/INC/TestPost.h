#pragma once

#include "../INC/Test.h"
#include "../INC/TecPlot.h"

void Test1(void) {
	Test_OBJ("post sine on triangle grid");

	const std::string grid_file_name = "10X10_Triangle.msh";
	const std::string grid_file_type = "gmsh";

	const auto grid_reader = GRIDREADER_FACTORY.order(grid_file_type);
	auto grid_data = grid_reader->read(grid_file_name);

	const size_t solution_order = 3;
	const auto initial_condition = INITIALCONDITION_FACTORY.order("SineAdvection", 2);

	ElementBuilder element_builder;
	auto element_set = element_builder.build_Element_Set(grid_data, solution_order, *initial_condition);

	const std::string post_file_name = "test_post_triangle";
	const size_t post_order = 3;
	const std::string post_solution_type = "conservative";
	const size_t dimension = 2;

	PostOptionData option = { post_file_name, post_order, post_solution_type, dimension };

	TECPLOT.initialize(std::move(option), element_set.cell_set);
	TECPLOT.post_Solution_File(0.0, "Initial");
}

void Test2(void) {
	Test_OBJ("post sine on quadrilateral grid");

	const std::string grid_file_name = "2D_SineAdvection_20X20.msh";
	const std::string grid_file_type = "gmsh";

	const auto grid_reader = GRIDREADER_FACTORY.order(grid_file_type);
	auto grid_data = grid_reader->read(grid_file_name);

	const size_t solution_order = 3;
	const auto initial_condition = INITIALCONDITION_FACTORY.order("SineAdvection", 2);

	ElementBuilder element_builder;
	auto element_set = element_builder.build_Element_Set(grid_data, solution_order, *initial_condition);


	const std::string post_file_name = "test_post_20_Quad";
	const size_t post_order = 3;
	const std::string post_solution_type = "conservative";
	const size_t dimension = 2;

	PostOptionData option = { post_file_name, post_order, post_solution_type, dimension };

	TECPLOT.initialize(std::move(option), element_set.cell_set);
	TECPLOT.post_Solution_File(0.0, "Initial");
}

void Test3(void) {
	Test_OBJ("post sine on mixed grid");

	const std::string grid_file_name = "10X10_Mix.msh";
	const std::string grid_file_type = "gmsh";

	const auto grid_reader = GRIDREADER_FACTORY.order(grid_file_type);
	auto grid_data = grid_reader->read(grid_file_name);

	const size_t solution_order = 3;
	const auto initial_condition = INITIALCONDITION_FACTORY.order("SineAdvection", 2);

	ElementBuilder element_builder;
	auto element_set = element_builder.build_Element_Set(grid_data, solution_order, *initial_condition);


	const std::string post_file_name = "test_post_mix";
	const size_t post_order = 3;
	const std::string post_solution_type = "conservative";
	const size_t dimension = 2;

	PostOptionData option = { post_file_name, post_order, post_solution_type, dimension };

	TECPLOT.initialize(std::move(option), element_set.cell_set);
	TECPLOT.post_Solution_File(0.0, "Initial");
}