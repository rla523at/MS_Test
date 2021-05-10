#include "gtest/gtest.h"
#include "../MS_Test/INC/Figure.h"

GTEST_TEST(REFERENCE_FIGURE, CONSTRUCTOR1) {
	const FigureType figure_type = FigureType::Point;
	const size_t figure_order = 0;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}
GTEST_TEST(REFERENCE_FIGURE, CONSTRUCTOR2) {
	const FigureType figure_type = FigureType::Point;
	const size_t figure_order = 1;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}
GTEST_TEST(REFERENCE_FIGURE, CONSTRUCTOR3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 7;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}
GTEST_TEST(REFERENCE_FIGURE, CONSTRUCTOR4) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 6;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}
GTEST_TEST(REFERENCE_FIGURE, CONSTRUCTOR5) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 7;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}


GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_LINE_1) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_LINE_2) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{0,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_LINE_3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{-0.33333333333333333333333333,0,0},{0.33333333333333333333333333,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_LINE_4) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 4;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{-0.5,0,0}, {0,0,0},{0.5,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_LINE_5) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 5;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{-0.6,0,0},{-0.2,0,0},{0.2,0,0}, {0.6,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_LINE_6) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 6;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{-0.66666666666666667,0,0},{-0.33333333333333333,0,0},{0,0,0},{0.33333333333333333,0,0},{0.66666666666666667,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_TRIANGLE_1) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,-1,0},{1,-1,0},{-1,1,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_TRIANGLE_2) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,-1,0},{1,-1,0},{-1,1,0},{0,-1,0},{0,0,0},{-1,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_TRIANGLE_3) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,-1,0},{1,-1,0},{-1,1,0},{-0.33333333333333333333,-1,0},{0.33333333333333333333,-1,0},{0.33333333333333333333,-0.33333333333333333333,0},{-0.33333333333333333333,0.33333333333333333333,0},{-1,0.33333333333333333333,0} ,{-1,-0.33333333333333333333,0},{-0.33333333333333333333,-0.33333333333333333333,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_TRIANGLE_4) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 4;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { {-1,-1,0},{1,-1,0},{-1,1,0},{-0.5,-1,0},{0,-1,0},{0.5,-1,0},{0.5,-0.5,0},{0,0,0},{-0.5,0.5,0},{-1,0.5,0},{-1,0,0},{-1,-0.5,0},{-0.5,-0.5,0},{0,-0.5,0},{-0.5,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_TRIANGLE_5) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 5;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { { -1,-1,0, }, { 1,-1,0, }, { -1,1,0, }, { -0.6,-1,0, }, { -0.2,-1,0, }, { 0.2,-1,0, }, { 0.6,-1,0, }, { 0.6,-0.6,0, }, { 0.2,-0.2,0, }, { -0.2,0.2,0, }, { -0.6,0.6,0, }, { -1,0.6,0, }, { -1,0.2,0, }, { -1,-0.2,0, }, { -1,-0.6,0, }, { -0.6,-0.6,0, }, { 0.2,-0.6,0, }, { -0.6,0.2,0, }, { -0.2,-0.6,0, }, { -0.2,-0.2,0, }, { -0.6,-0.2,0, } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_QUADRILATERAL_1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_QUADRILATERAL_2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { 0,-1,0 }, { 1,0,0 }, { 0,1,0 }, { -1,0,0 }, { 0,0,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_QUADRILATERAL_3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { -0.33333333333333333,-1,0 }, { 0.33333333333333333,-1,0 }, { 1,-0.33333333333333333,0 }, { 1,0.33333333333333333,0 }, { 0.33333333333333333,1,0 }, { -0.33333333333333333,1,0 }, { -1,0.33333333333333333,0 }, { -1,-0.33333333333333333,0 }, { 0.33333333333333333,-0.33333333333333333,0 }, { 0.33333333333333333,0.33333333333333333,0 }, { -0.33333333333333333,0.33333333333333333,0 }, { -0.33333333333333333,-0.33333333333333333,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_QUADRILATERAL_4) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 4;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { -0.5,-1,0 }, { 0,-1,0 }, { 0.5,-1,0 }, { 1,-0.5,0 }, { 1,0,0 }, { 1,0.5,0 }, { 0.5,1,0 }, { 0,1,0 }, { -0.5,1,0 }, { -1,0.5,0 }, { -1,0,0 }, { -1,-0.5,0 }, { 0.5,-0.5,0 }, { 0.5,0.5,0 }, { -0.5,0.5,0 }, { -0.5,-0.5,0 }, { 0.5,0,0 }, { 0,0.5,0 }, { -0.5,0,0 }, { 0,-0.5,0 }, { 0,0,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_QUADRILATERAL_5) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 5;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { -0.6,-1,0 }, { -0.2,-1,0 }, { 0.2,-1,0 }, { 0.6,-1,0 }, { 1,-0.6,0 }, { 1,-0.2,0 }, { 1,0.2,0 }, { 1,0.6,0 }, { 0.6,1,0 }, { 0.2,1,0 }, { -0.2,1,0 }, { -0.6,1,0 }, { -1,0.6,0 }, { -1,0.2,0 }, { -1,-0.2,0 }, { -1,-0.6,0 }, { 0.6,-0.6,0 }, { 0.6,0.6,0 }, { -0.6,0.6,0 }, { -0.6,-0.6,0 }, { 0.6,-0.2,0 }, { 0.6,0.2,0 }, { 0.2,0.6,0 }, { -0.2,0.6,0 }, { -0.6,0.2,0 }, { -0.6,-0.2,0 }, { -0.2,-0.6,0 }, { 0.2,-0.6,0 }, { 0.2,-0.2,0 }, { 0.2,0.2,0 }, { -0.2,0.2,0 }, { -0.2,-0.2,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_NODE_SET_QUADRILATERAL_6) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 6;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto result = ReferenceFigure::key_to_transformation_node_set_.at(key);

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { -0.66666666666666667,-1,0 }, { -0.33333333333333333,-1,0 }, { 0,-1,0 }, { 0.33333333333333333,-1,0 }, { 0.66666666666666667,-1,0 }, { 1,-0.66666666666666667,0 }, { 1,-0.33333333333333333,0 }, { 1,0,0 }, { 1,0.33333333333333333,0 }, { 1,0.66666666666666667,0 }, { 0.66666666666666667,1,0 }, { 0.33333333333333333,1,0 }, { 0,1,0 }, { -0.33333333333333333,1,0 }, { -0.66666666666666667,1,0 }, { -1,0.66666666666666667,0 }, { -1,0.33333333333333333,0 }, { -1,0,0 }, { -1,-0.33333333333333333,0 }, { -1,-0.66666666666666667,0 }, { 0.66666666666666667,-0.66666666666666667,0 }, { 0.66666666666666667,0.66666666666666667,0 }, { -0.66666666666666667,0.66666666666666667,0 }, { -0.66666666666666667,-0.66666666666666667,0 }, { 0.66666666666666667,-0.33333333333333333,0 }, { 0.66666666666666667,0,0 }, { 0.66666666666666667,0.33333333333333333,0 }, { 0.33333333333333333,0.66666666666666667,0 }, { 0,0.66666666666666667,0 }, { -0.33333333333333333,0.66666666666666667,0 }, { -0.66666666666666667,0.33333333333333333,0 }, { -0.66666666666666667,0,0 }, { -0.66666666666666667,-0.33333333333333333,0 }, { -0.33333333333333333,-0.66666666666666667,0 }, { 0,-0.66666666666666667,0 }, { 0.33333333333333333,-0.66666666666666667,0 }, { 0.33333333333333333,-0.33333333333333333,0 }, { 0.33333333333333333,0.33333333333333333,0 }, { -0.33333333333333333,0.33333333333333333,0 }, { -0.33333333333333333,-0.33333333333333333,0 }, { 0.33333333333333333,0,0 }, { 0,0.33333333333333333,0 }, { -0.33333333333333333,0,0 }, { 0,-0.33333333333333333,0 }, { 0,0,0 } };
	EXPECT_EQ(result, ref);
}


//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_LINE_P0) {
//	const FigureType figure_type = FigureType::Line;
//	const size_t figure_order = 1;
//
//	MathVector p1 = { -1,0,0, };
//	MathVector p2 = { 1,0,0, };
//	std::vector<const MathVector*> pv = { &p1,&p2 };
//	Figure fig(figure_type, figure_order,std::move(pv));
//
//	const size_t integrand_order = 0;
//	const auto result = fig.calculate_quadrature_rule(integrand_order);
//
//	const QuadratureRule ref = { { { 0.000000000000000,0,0 } },{ 2.000000000000000} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_LINE_P1) {
//	const FigureType figure_type = FigureType::Line;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 1;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { 0.000000000000000,0,0 } },{ 2.000000000000000} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_LINE_P2) {
//	const FigureType figure_type = FigureType::Line;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 2;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.577350269189626,0,0 }, { 0.577350269189626,0,0 } },{ 1.000000000000000, 1.000000000000000} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_LINE_P3) {
//	const FigureType figure_type = FigureType::Line;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 3;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.577350269189626,0,0 }, { 0.577350269189626,0,0 } },{ 1.000000000000000, 1.000000000000000} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_LINE_P4) {
//	const FigureType figure_type = FigureType::Line;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 4;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.774596669241483,0,0 }, { 0.000000000000000,0,0 }, { 0.774596669241483,0,0 } },{ 0.555555555555554, 0.888888888888889, 0.555555555555554} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_LINE_P5) {
//	const FigureType figure_type = FigureType::Line;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 5;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.774596669241483,0,0 }, { 0.000000000000000,0,0 }, { 0.774596669241483,0,0 } },{ 0.555555555555554, 0.888888888888889, 0.555555555555554} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_TRIANGLE_P0) {
//	const FigureType figure_type = FigureType::Triangle;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 0;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.5,0,0 } },{ 2} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_TRIANGLE_P1) {
//	const FigureType figure_type = FigureType::Triangle;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 1;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.6666666666666667,-0.577350269189626,0 }, { -0.9106836025229592,0.577350269189626,0 }, { 0.2440169358562927,-0.577350269189626,0 }, { -0.6666666666666667,0.577350269189626,0 } },{ 0.788675134594813, 0.211324865405187, 0.788675134594813, 0.211324865405187} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_TRIANGLE_P2) {
//	const FigureType figure_type = FigureType::Triangle;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 2;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.6666666666666667,-0.577350269189626,0 }, { -0.9106836025229592,0.577350269189626,0 }, { 0.2440169358562927,-0.577350269189626,0 }, { -0.6666666666666667,0.577350269189626,0 } },{ 0.788675134594813, 0.211324865405187, 0.788675134594813, 0.211324865405187} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_TRIANGLE_P3) {
//	const FigureType figure_type = FigureType::Triangle;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 3;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.7999999999999996,-0.774596669241483,0 }, { -0.8872983346207415,0,0 }, { -0.9745966692414834,0.774596669241483,0 }, { -0.1127016653792585,-0.774596669241483,0 }, { -0.5,0,0 }, { -0.8872983346207415,0.774596669241483,0 }, { 0.5745966692414826,-0.774596669241483,0 }, { -0.1127016653792585,0,0 }, { -0.7999999999999996,0.774596669241483,0 } },{ 0.2738575106854125, 0.2469135802469129, 0.03478446462322775, 0.4381720170966613, 0.3950617283950618, 0.05565514339716456, 0.2738575106854125, 0.2469135802469129, 0.03478446462322775} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_TRIANGLE_P4) {
//	const FigureType figure_type = FigureType::Triangle;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 4;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.7999999999999996,-0.774596669241483,0 }, { -0.8872983346207415,0,0 }, { -0.9745966692414834,0.774596669241483,0 }, { -0.1127016653792585,-0.774596669241483,0 }, { -0.5,0,0 }, { -0.8872983346207415,0.774596669241483,0 }, { 0.5745966692414826,-0.774596669241483,0 }, { -0.1127016653792585,0,0 }, { -0.7999999999999996,0.774596669241483,0 } },{ 0.2738575106854125, 0.2469135802469129, 0.03478446462322775, 0.4381720170966613, 0.3950617283950618, 0.05565514339716456, 0.2738575106854125, 0.2469135802469129, 0.03478446462322775} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_QUADRILATERAL_P0) {
//	const FigureType figure_type = FigureType::Quadrilateral;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 0;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { 0,0,0 } },{ 4} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_QUADRILATERAL_P1) {
//	const FigureType figure_type = FigureType::Quadrilateral;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 1;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { 0,0,0 } },{ 4} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_QUADRILATERAL_P2) {
//	const FigureType figure_type = FigureType::Quadrilateral;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 2;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.577350269189626,-0.577350269189626,0 }, { -0.577350269189626,0.577350269189626,0 }, { 0.577350269189626,-0.577350269189626,0 }, { 0.577350269189626,0.577350269189626,0 } },{ 1, 1, 1, 1} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_QUADRILATERAL_P3) {
//	const FigureType figure_type = FigureType::Quadrilateral;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 3;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.577350269189626,-0.577350269189626,0 }, { -0.577350269189626,0.577350269189626,0 }, { 0.577350269189626,-0.577350269189626,0 }, { 0.577350269189626,0.577350269189626,0 } },{ 1, 1, 1, 1} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_QUADRILATERAL_P4) {
//	const FigureType figure_type = FigureType::Quadrilateral;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 4;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.774596669241483,-0.774596669241483,0 }, { -0.774596669241483,0,0 }, { -0.774596669241483,0.774596669241483,0 }, { 0,-0.774596669241483,0 }, { 0,0,0 }, { 0,0.774596669241483,0 }, { 0.774596669241483,-0.774596669241483,0 }, { 0.774596669241483,0,0 }, { 0.774596669241483,0.774596669241483,0 } },{ 0.3086419753086403, 0.4938271604938259, 0.3086419753086403, 0.4938271604938259, 0.7901234567901235, 0.4938271604938259, 0.3086419753086403, 0.4938271604938259, 0.3086419753086403} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//}
//GTEST_TEST(REFERENCE_FIGURE, REFERENCE_QUADRATURE_RULE_QUADRILATERAL_P5) {
//	const FigureType figure_type = FigureType::Quadrilateral;
//	const size_t figure_order = 1;
//	ReferenceFigure ref_fig(figure_type, figure_order);
//
//	const size_t integrand_order = 5;
//	const auto key = std::make_pair(figure_type, integrand_order);
//	const auto result = ReferenceFigure::key_to_quadrature_rule_.at(key);
//
//	const QuadratureRule ref = { { { -0.774596669241483,-0.774596669241483,0 }, { -0.774596669241483,0,0 }, { -0.774596669241483,0.774596669241483,0 }, { 0,-0.774596669241483,0 }, { 0,0,0 }, { 0,0.774596669241483,0 }, { 0.774596669241483,-0.774596669241483,0 }, { 0.774596669241483,0,0 }, { 0.774596669241483,0.774596669241483,0 } },{ 0.3086419753086403, 0.4938271604938259, 0.3086419753086403, 0.4938271604938259, 0.7901234567901235, 0.4938271604938259, 0.3086419753086403, 0.4938271604938259, 0.3086419753086403} };
//	EXPECT_EQ(result.quadrature_point_set, ref.quadrature_point_set);
//	EXPECT_EQ(result.quadrature_weight_set, ref.quadrature_weight_set);
//} 


GTEST_TEST(REFERENCE_FIGURE, CENTER_NODE_LINE) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto center_node = ref_fig.center_node();
	MathVector ref = { 0,0,0 };

	EXPECT_EQ(center_node, ref);
}
GTEST_TEST(REFERENCE_FIGURE, CENTER_NODE_TRIANGLE) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto center_node = ref_fig.center_node();
	MathVector ref = { -1.0 / 3.0, -1.0 / 3.0, 0 };

	EXPECT_EQ(center_node, ref);
}
GTEST_TEST(REFERENCE_FIGURE, CENTER_NODE_QUADRILATERAL) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto center_node = ref_fig.center_node();
	MathVector ref = { 0,0,0 };

	EXPECT_EQ(center_node, ref);
}


GTEST_TEST(REFERENCE_FIGURE, FACE_FIGURE_TYPE_SET_LINE) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto face_figure_type_set = ref_fig.face_figure_type_set();

	std::vector<FigureType> ref = { FigureType::Point,FigureType::Point };
	EXPECT_EQ(face_figure_type_set, ref);
}
GTEST_TEST(REFERENCE_FIGURE, FACE_FIGURE_TYPE_SET_TRIANGLE) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto face_figure_type_set = ref_fig.face_figure_type_set();

	std::vector<FigureType> ref = { FigureType::Line,FigureType::Line,FigureType::Line };
	EXPECT_EQ(face_figure_type_set, ref);
}
GTEST_TEST(REFERENCE_FIGURE, FACE_FIGURE_TYPE_SET_QUADRILATERAL) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto face_figure_type_set = ref_fig.face_figure_type_set();

	std::vector<FigureType> ref = { FigureType::Line,FigureType::Line,FigureType::Line,FigureType::Line };
	EXPECT_EQ(face_figure_type_set, ref);
}


GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_VECTOR_LINE1) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_transformation_monomial_vector_.at(key);
	
	VectorFunction<Monomial> ref = { Monomial{0},Monomial{1} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_VECTOR_LINE2) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_transformation_monomial_vector_.at(key);

	VectorFunction<Monomial> ref = { Monomial{0},Monomial{1},Monomial{2} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_VECTOR_LINE3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_transformation_monomial_vector_.at(key);

	VectorFunction<Monomial> ref = { Monomial{0},Monomial{1},Monomial{2},Monomial{3} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_VECTOR_TRIANGLE1) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_transformation_monomial_vector_.at(key);

	VectorFunction<Monomial> ref = { Monomial{0},Monomial{1},Monomial{0,1} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_VECTOR_TRIANGLE2) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_transformation_monomial_vector_.at(key);

	VectorFunction<Monomial> ref = { Monomial{0},Monomial{1},Monomial{0,1},Monomial{2},Monomial{1,1},Monomial{0,2} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_VECTOR_TRIANGLE3) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_transformation_monomial_vector_.at(key);

	VectorFunction<Monomial> ref = { Monomial{0},Monomial{1},Monomial{0,1},Monomial{2},Monomial{1,1},Monomial{0,2},Monomial{3},Monomial{2,1},Monomial{1,2},Monomial{0,3} };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_LINE_P1) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);
	
	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(2, 2, { 0.5,-0.5,0.5,0.5 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_LINE_P2) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(3, 3, { 0, -0.5,  0.5,  0,  0.5,  0.5,1.0,    0, -1.0 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_LINE_P3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(4, 4, { -0.0625,  0.0625,  0.5625, -0.5625,-0.0625, -0.0625,  0.5625,  0.5625, 0.5625, -1.6875, -0.5625,  1.6875, 0.5625,  1.6875, -0.5625, -1.6875 });
	EXPECT_TRUE(result.compare_with_finitie_precision(ref, 16)); // 15ULP roud off error !
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_TRIANGLE_P1) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(3, 3, { 0, -0.5, -0.5,0.5,  0.5,    0,0.5,    0,  0.5 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_TRIANGLE_P2) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(6, 6, { 0,  0.5,  0.5,  0.5,  1.0,  0.5,  0,  0.5,    0,  0.5,    0,    0,  0,    0,  0.5,    0,    0,  0.5,  0, -1.0, -1.0, -1.0, -1.0,    0,1.0,  1.0,  1.0,    0,  1.0,    0,  0, -1.0, -1.0,    0, -1.0, -1.0 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_TRIANGLE_P3) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(10, 10, { 0,    -0.5,    -0.5, -1.125, -2.25, -1.125, -0.5625, -1.6875, -1.6875, -0.5625,-0.0625, -0.0625,       0, 0.5625,     0,      0,  0.5625,       0,       0,       0,-0.0625,       0, -0.0625,      0,     0, 0.5625,       0,       0,       0,  0.5625,      0,   1.125,   1.125, 2.8125,   4.5, 1.6875,  1.6875,   3.375,  1.6875,       0,      0, -0.5625, -0.5625,  -2.25, -2.25,      0, -1.6875, -1.6875,       0,       0, 0.5625,    2.25,  0.5625, 1.6875,  2.25,      0,       0,  1.6875,       0,       0, 0.5625,  0.5625,    2.25,      0,  2.25, 1.6875,       0,       0,  1.6875,       0,      0, -0.5625, -0.5625,      0, -2.25,  -2.25,       0,       0, -1.6875, -1.6875,      0,   1.125,   1.125, 1.6875,   4.5, 2.8125,       0,  1.6875,   3.375,  1.6875,      0,  -3.375,  -3.375, -3.375, -6.75, -3.375,       0,  -3.375,  -3.375,       0 });
	const auto [row, column] = ref.size();
	const double epsilon = 1.0E-13;
	for (size_t i = 0; i < row; ++i)
		for (size_t j = 0; j < column; ++j)
			EXPECT_NEAR(result.at(i, j), ref.at(i, j), epsilon); //severe round off error how can i fix it ??
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_QUADRILATERAL_P1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(4, 4, { 0.25, -0.25,  0.25, -0.25,  0.25,  0.25, -0.25, -0.25,  0.25,  0.25,  0.25,  0.25,  0.25, -0.25, -0.25,  0.25 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_QUADRILATERAL_P2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(9, 9, { 0,    0,  0.25,    0,    0, -0.25, 0.25, -0.25,    0,  0,    0, -0.25,    0,    0, -0.25, 0.25,  0.25,    0,  0,    0,  0.25,    0,    0,  0.25, 0.25,  0.25,    0,  0,    0, -0.25,    0,    0,  0.25, 0.25, -0.25,    0,  0,    0,     0, -0.5,    0,   0.5, -0.5,     0,  0.5,  0,  0.5,     0,    0,  0.5,     0, -0.5,  -0.5,    0,  0,    0,     0,  0.5,    0,  -0.5, -0.5,     0,  0.5,  0, -0.5,     0,    0,  0.5,     0, -0.5,   0.5,    0,1.0,    0,     0,    0, -1.0,     0,  1.0,     0, -1.0 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_MONOMIAL_MATRIX_QUADRILATERAL_P3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);

	RowMajorMatrix ref(16, 16, { 0.00390625, -0.00390625,  0.00390625, -0.00390625, -0.03515625,  0.03515625,  0.31640625,  0.03515625, -0.03515625,  0.03515625, -0.03515625, -0.31640625,  0.31640625, -0.31640625, -0.03515625,  0.03515625, 0.00390625,  0.00390625, -0.00390625, -0.00390625, -0.03515625,  0.03515625,  0.31640625, -0.03515625, -0.03515625, -0.03515625,  0.03515625,  0.31640625, -0.31640625, -0.31640625,  0.03515625,  0.03515625, 0.00390625,  0.00390625,  0.00390625,  0.00390625, -0.03515625, -0.03515625,  0.31640625, -0.03515625, -0.03515625, -0.03515625, -0.03515625,  0.31640625,  0.31640625,  0.31640625, -0.03515625, -0.03515625, 0.00390625, -0.00390625, -0.00390625,  0.00390625, -0.03515625, -0.03515625,  0.31640625,  0.03515625, -0.03515625,  0.03515625,  0.03515625, -0.31640625, -0.31640625,  0.31640625,  0.03515625, -0.03515625,-0.03515625,  0.10546875, -0.10546875,  0.03515625,  0.03515625, -0.03515625, -0.31640625, -0.94921875,  0.31640625, -0.10546875,  0.10546875,  0.94921875, -0.94921875,  0.31640625,  0.94921875, -0.31640625,-0.03515625, -0.10546875,  0.10546875,  0.03515625,  0.03515625, -0.03515625, -0.31640625,  0.94921875,  0.31640625,  0.10546875, -0.10546875, -0.94921875,  0.94921875,  0.31640625, -0.94921875, -0.31640625,-0.03515625, -0.03515625,  0.10546875,  0.10546875,  0.31640625, -0.94921875, -0.31640625,  0.03515625,  0.03515625,  0.31640625, -0.94921875, -0.31640625,  0.94921875,  0.94921875, -0.10546875, -0.10546875,-0.03515625, -0.03515625, -0.10546875, -0.10546875,  0.31640625,  0.94921875, -0.31640625,  0.03515625,  0.03515625,  0.31640625,  0.94921875, -0.31640625, -0.94921875, -0.94921875,  0.10546875,  0.10546875,-0.03515625, -0.10546875, -0.10546875, -0.03515625,  0.03515625,  0.03515625, -0.31640625,  0.94921875,  0.31640625,  0.10546875,  0.10546875, -0.94921875, -0.94921875, -0.31640625,  0.94921875,  0.31640625,-0.03515625,  0.10546875,  0.10546875, -0.03515625,  0.03515625,  0.03515625, -0.31640625, -0.94921875,  0.31640625, -0.10546875, -0.10546875,  0.94921875,  0.94921875, -0.31640625, -0.94921875,  0.31640625,-0.03515625,  0.03515625,  0.10546875, -0.10546875,  0.31640625,  0.94921875, -0.31640625, -0.03515625,  0.03515625, -0.31640625, -0.94921875,  0.31640625,  0.94921875, -0.94921875, -0.10546875,  0.10546875,-0.03515625,  0.03515625, -0.10546875,  0.10546875,  0.31640625, -0.94921875, -0.31640625, -0.03515625,  0.03515625, -0.31640625,  0.94921875,  0.31640625, -0.94921875,  0.94921875,  0.10546875, -0.10546875, 0.31640625,  0.94921875, -2.84765625, -0.94921875, -0.31640625,  0.94921875,  0.31640625, -0.94921875, -0.31640625, -0.94921875,  2.84765625,  0.94921875, -2.84765625, -0.94921875,  2.84765625,  0.94921875, 0.31640625,  0.94921875,  2.84765625,  0.94921875, -0.31640625, -0.94921875,  0.31640625, -0.94921875, -0.31640625, -0.94921875, -2.84765625,  0.94921875,  2.84765625,  0.94921875, -2.84765625, -0.94921875, 0.31640625, -0.94921875, -2.84765625,  0.94921875, -0.31640625, -0.94921875,  0.31640625,  0.94921875, -0.31640625,  0.94921875,  2.84765625, -0.94921875, -2.84765625,  0.94921875,  2.84765625, -0.94921875, 0.31640625, -0.94921875,  2.84765625, -0.94921875, -0.31640625,  0.94921875,  0.31640625,  0.94921875, -0.31640625,  0.94921875, -2.84765625, -0.94921875,  2.84765625, -0.94921875, -2.84765625,  0.94921875 });
	const auto [row, column] = ref.size();
	const double epsilon = 1.0E-14;
	for (size_t i = 0; i < row; ++i)
		for (size_t j = 0; j < column; ++j)
			EXPECT_NEAR(result.at(i, j), ref.at(i, j), epsilon); //severe round off error how can i fix it ??
}


GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_FUNCTION1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure reference_fig(figure_type, figure_order);

	MathVector tp1 = { -1,-1,0 };
	MathVector tp2 = { 1,-1,0 };
	MathVector tp3 = { -1,1,0 };
	MathVector tp4 = { -1,1,0 };
	std::vector<const MathVector*> pv = { &tp1,&tp2,&tp3,&tp4 };
	const auto transformation_function = reference_fig.calculate_transformation_function(pv);

	std::cout << "\n" << transformation_function << "\n";

	MathVector p1 = { -1,-1,0 };
	MathVector p2 = { 1,-1,0 };
	MathVector p3 = { 1,1,0 };
	MathVector p4 = { -1,1,0 };

	EXPECT_EQ(tp1, transformation_function(p1));
	EXPECT_EQ(tp2, transformation_function(p2));
	EXPECT_EQ(tp3, transformation_function(p3));
	EXPECT_EQ(tp4, transformation_function(p4));
}
GTEST_TEST(REFERENCE_FIGURE, TRANSFORMATION_FUNCTION2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 2;
	ReferenceFigure reference_fig(figure_type, figure_order);

	MathVector tp1 = { -1,-1,0 };
	MathVector tp2 = { 1,-1,0 };
	MathVector tp3 = { -1,1,0 };
	MathVector tp4 = { -1,1,0 };
	MathVector tp5 = { -1,0,0 };
	MathVector tp6 = { 0,0,0 };
	MathVector tp7 = { -1,1,0 };
	MathVector tp8 = { -1,0,0 };
	MathVector tp9 = { -1,0,0 };
	std::vector<const MathVector*> pv = { &tp1,&tp2,&tp3,&tp4,&tp5,&tp6,&tp7,&tp8,&tp9 };
	const auto transformation_function = reference_fig.calculate_transformation_function(pv);

	std::cout << "\n" << transformation_function <<"\n";

	MathVector p1 = { -1,-1,0 };
	MathVector p2 = { 1,-1,0 };
	MathVector p3 = { 1,1,0 };
	MathVector p4 = { -1,1,0 };
	MathVector p5 = { 0, -1, 0 };
	MathVector p6 = { 1, 0, 0 };
	MathVector p7 = { 0, 1, 0 };
	MathVector p8 = { -1, 0, 0 };
	MathVector p9 = { 0, 0, 0 };

	EXPECT_EQ(tp1, transformation_function(p1));
	EXPECT_EQ(tp2, transformation_function(p2));
	EXPECT_EQ(tp3, transformation_function(p3));
	EXPECT_EQ(tp4, transformation_function(p4));
	EXPECT_EQ(tp5, transformation_function(p5));
	EXPECT_EQ(tp6, transformation_function(p6));
	EXPECT_EQ(tp7, transformation_function(p7));
	EXPECT_EQ(tp8, transformation_function(p8));
	EXPECT_EQ(tp9, transformation_function(p9));
}


GTEST_TEST(FIGURE, INITIAL_BASIS1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	
	MathVector p1 = { 0,0,0 };
	MathVector p2 = { 2,0,0 };
	MathVector p3 = { 2,2,0 };
	MathVector p4 = { 0,2,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 1;

	VectorFunction<Polynomial> result;
	const auto center_node = fig.calculate_center_node();
	for (size_t a = 0; a <= polynomial_order; ++a)
		for (size_t b = 0; b <= a; ++b)
			result.push_back(((X - center_node[0]) ^ (a - b)) * ((Y - center_node[1]) ^ b));

	VectorFunction<Polynomial> ref = { 1, X - 1, Y - 1 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(FIGURE, MULTIPLY_MATRIX_VECTOR_FUNCTION1) {
	VectorFunction<Monomial> v = { {0},{1},{2} };
	RowMajorMatrix m(2, 3, { 1,2,1,4,4,1 });
	const auto result = m * v;

	VectorFunction<Polynomial> ref = { {{1,2,1}, { {2},{1},{0} }}, {{1,4,4}, { {2},{1},{0} }} };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(VECTORFUNCTION, OPERATOR_CALL1) {
	Polynomial p1 = (X ^ 2) + 3 * (X ^ 2) * Y + (Y ^ 3) + (Z ^ 2) - 6;
	Polynomial p2 = X + Y + Z - 3;
	Polynomial p3 = (Y ^ 2) * Z + X * Z - 2;
	VectorFunction<Polynomial> f = { p1,p2,p3 };
	MathVector node = { 1,1,1 };
	const auto result = f(node);

	MathVector ref = { 0,0,0 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(JACOBIAN, CONSTRUCTOR1) {
	Polynomial p1 = { {1,1,1},{{0},{1},{0,1}} };
	Polynomial p2 = { {1},{{1,0,1}} };
	Polynomial p3 = { {1},{{0,3}} };
	VectorFunction<Polynomial> f = { p1,p2,p3 };
	const auto result = JacobianFunction(f);

	VectorFunction<Polynomial> vp1 = { {{1}, { {0} }}, {{1}, { {0} }}, {{0}, { {0} }} };
	VectorFunction<Polynomial> vp2 = { {{1}, { {0,0,1} }}, {{0}, { {0} }}, {{1}, { {1} }} };
	VectorFunction<Polynomial> vp3 = { {{0}, { {0} }}, {{3}, { {0,2} }}, {{0}, { {0} }} };
	JacobianFunction ref = { vp1,vp2,vp3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(JACOBIAN, CONSTRUCTOR2) {
	Polynomial p1 = 1 + X + Y;
	Polynomial p2 = X*Z;
	Polynomial p3 = Z^3;
	VectorFunction<Polynomial> f = { p1,p2,p3 };
	const auto result = JacobianFunction(f);

	VectorFunction<Polynomial> vp1 = { 1,1,0 };
	VectorFunction<Polynomial> vp2 = { Z,0,X };
	VectorFunction<Polynomial> vp3 = { 0,0,3 * (Z ^ 2) };
	JacobianFunction ref = { vp1,vp2,vp3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(JACOBIAN, CONSTRUCTOR3) {
	Polynomial p1 = (X ^ 2) + 3 * (X ^ 2) * Y + (Y ^ 3) + (Z ^ 2) - 6;
	Polynomial p2 = X + Y + Z - 3;
	Polynomial p3 = (Y ^ 2) * Z + X * Z - 2;
	VectorFunction<Polynomial> f = { p1,p2,p3 };
	const auto result = JacobianFunction(f);

	VectorFunction<Polynomial> vp1 = { 2 * X + 6 * X * Y,3 * (X ^ 2) + 3 * (Y ^ 2),2 * Z };
	VectorFunction<Polynomial> vp2 = { 1,1,1 };
	VectorFunction<Polynomial> vp3 = { Z,2 * Y * Z,(Y ^ 2) + X };
	JacobianFunction ref = { vp1,vp2,vp3 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(JACOBIAN, NEWTON_RAPHSON1) {
	Polynomial p1 = X + Y - 1;
	Polynomial p2 = X - Y - 3;
	VectorFunction<Polynomial> f = { p1,p2 };
	MathVector initial_guess = { 0,0 };
	const auto result = ms::Newton_Raphson(f, initial_guess);

	MathVector ref = { 2,-1 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(JACOBIAN, NEWTON_RAPHSON2) {
	Polynomial p1 = (X ^ 2) + X + Y - 7;
	Polynomial p2 = X - Y - 1;
	VectorFunction<Polynomial> f = { p1,p2 };
	MathVector initial_guess = { 0,0 };
	const auto result = ms::Newton_Raphson(f, initial_guess);

	MathVector ref = { 2,1 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(JACOBIAN, NEWTON_RAPHSON3) {
	Polynomial p1 = (X ^ 2) + 3 * (X ^ 2) * Y + (Y ^ 3) + (Z ^ 2) - 6;
	Polynomial p2 = X + Y + Z - 3;
	Polynomial p3 = (Y ^ 2) * Z + X * Z - 2;
	VectorFunction<Polynomial> f = { p1,p2,p3 };
	MathVector initial_guess = { 1.1,1.1,1.1 };
	const double convergence_criteria = 1.0E-13;
	const auto solution = ms::Newton_Raphson(f,initial_guess,convergence_criteria);
	const auto result = f(solution).L2_Norm();
	
	const auto ref = 1.0E-15;
	EXPECT_LE(result, ref);
}
GTEST_TEST(JACOBIAN, NEWTON_RAPHSON4) {
	Polynomial p1 = (X ^ 2) + 3 * (X ^ 2) * Y + (Y ^ 3) + (Z ^ 2) - 6;
	Polynomial p2 = X + Y + Z - 3;
	Polynomial p3 = (Y ^ 2) * Z + X * Z - 2;
	VectorFunction<Polynomial> f = { p1,p2,p3 };
	//MathVector initial_guess = { 0,0,0 }; bad initial guess
	MathVector initial_guess = { 0.5,0.5,0.5 };
	const auto solution = ms::Newton_Raphson(f, initial_guess);
	const auto result = f(solution).L2_Norm();

	const auto ref = 1.0E-14; // 1.0E-15º¸´Ù Å­
	EXPECT_LE(result, ref);
}