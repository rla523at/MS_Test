#include "gtest/gtest.h"
#include "../MS_Test/INC/Figure.h"

#define X Polynomial("x0")
#define Y Polynomial("x1")
#define Z Polynomial("x2")

GTEST_TEST(ReferenceFigure, CONSTRUCTOR1) {
	const FigureType figure_type = FigureType::Point;
	const size_t figure_order = 0;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}
GTEST_TEST(ReferenceFigure, CONSTRUCTOR2) {
	const FigureType figure_type = FigureType::Point;
	const size_t figure_order = 1;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}
GTEST_TEST(ReferenceFigure, CONSTRUCTOR3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 7;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}
GTEST_TEST(ReferenceFigure, CONSTRUCTOR4) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 6;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}
GTEST_TEST(ReferenceFigure, CONSTRUCTOR5) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 7;
	EXPECT_ANY_THROW(ReferenceFigure ref_fig(figure_type, figure_order));
}

GTEST_TEST(ReferenceFigure_transformation_node_set, line_P1) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, line_P2) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 2;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{0,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, line_P3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 3;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{-0.33333333333333333333333333,0,0},{0.33333333333333333333333333,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, line_P4) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 4;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{-0.5,0,0}, {0,0,0},{0.5,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, line_P5) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 5;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{-0.6,0,0},{-0.2,0,0},{0.2,0,0}, {0.6,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, line_P6) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 6;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,0,0},{1,0,0},{-0.66666666666666667,0,0},{-0.33333333333333333,0,0},{0,0,0},{0.33333333333333333,0,0},{0.66666666666666667,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, triangle_P1) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,-1,0},{1,-1,0},{-1,1,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, triangle_P2) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 2;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,-1,0},{1,-1,0},{-1,1,0},{0,-1,0},{0,0,0},{-1,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, triangle_P3) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 3;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,-1,0},{1,-1,0},{-1,1,0},{-0.33333333333333333333,-1,0},{0.33333333333333333333,-1,0},{0.33333333333333333333,-0.33333333333333333333,0},{-0.33333333333333333333,0.33333333333333333333,0},{-1,0.33333333333333333333,0} ,{-1,-0.33333333333333333333,0},{-0.33333333333333333333,-0.33333333333333333333,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, triangle_P4) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 4;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { {-1,-1,0},{1,-1,0},{-1,1,0},{-0.5,-1,0},{0,-1,0},{0.5,-1,0},{0.5,-0.5,0},{0,0,0},{-0.5,0.5,0},{-1,0.5,0},{-1,0,0},{-1,-0.5,0},{-0.5,-0.5,0},{0,-0.5,0},{-0.5,0,0} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, triangle_P5) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 5;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { { -1,-1,0, }, { 1,-1,0, }, { -1,1,0, }, { -0.6,-1,0, }, { -0.2,-1,0, }, { 0.2,-1,0, }, { 0.6,-1,0, }, { 0.6,-0.6,0, }, { 0.2,-0.2,0, }, { -0.2,0.2,0, }, { -0.6,0.6,0, }, { -1,0.6,0, }, { -1,0.2,0, }, { -1,-0.2,0, }, { -1,-0.6,0, }, { -0.6,-0.6,0, }, { 0.2,-0.6,0, }, { -0.6,0.2,0, }, { -0.2,-0.6,0, }, { -0.2,-0.2,0, }, { -0.6,-0.2,0, } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, quadrilateral_P1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, quadrilateral_P2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 2;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { 0,-1,0 }, { 1,0,0 }, { 0,1,0 }, { -1,0,0 }, { 0,0,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, quadrilateral_P3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 3;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { -0.33333333333333333,-1,0 }, { 0.33333333333333333,-1,0 }, { 1,-0.33333333333333333,0 }, { 1,0.33333333333333333,0 }, { 0.33333333333333333,1,0 }, { -0.33333333333333333,1,0 }, { -1,0.33333333333333333,0 }, { -1,-0.33333333333333333,0 }, { 0.33333333333333333,-0.33333333333333333,0 }, { 0.33333333333333333,0.33333333333333333,0 }, { -0.33333333333333333,0.33333333333333333,0 }, { -0.33333333333333333,-0.33333333333333333,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, quadrilateral_P4) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 4;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { -0.5,-1,0 }, { 0,-1,0 }, { 0.5,-1,0 }, { 1,-0.5,0 }, { 1,0,0 }, { 1,0.5,0 }, { 0.5,1,0 }, { 0,1,0 }, { -0.5,1,0 }, { -1,0.5,0 }, { -1,0,0 }, { -1,-0.5,0 }, { 0.5,-0.5,0 }, { 0.5,0.5,0 }, { -0.5,0.5,0 }, { -0.5,-0.5,0 }, { 0.5,0,0 }, { 0,0.5,0 }, { -0.5,0,0 }, { 0,-0.5,0 }, { 0,0,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, quadrilateral_P5) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 5;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { -0.6,-1,0 }, { -0.2,-1,0 }, { 0.2,-1,0 }, { 0.6,-1,0 }, { 1,-0.6,0 }, { 1,-0.2,0 }, { 1,0.2,0 }, { 1,0.6,0 }, { 0.6,1,0 }, { 0.2,1,0 }, { -0.2,1,0 }, { -0.6,1,0 }, { -1,0.6,0 }, { -1,0.2,0 }, { -1,-0.2,0 }, { -1,-0.6,0 }, { 0.6,-0.6,0 }, { 0.6,0.6,0 }, { -0.6,0.6,0 }, { -0.6,-0.6,0 }, { 0.6,-0.2,0 }, { 0.6,0.2,0 }, { 0.2,0.6,0 }, { -0.2,0.6,0 }, { -0.6,0.2,0 }, { -0.6,-0.2,0 }, { -0.2,-0.6,0 }, { 0.2,-0.6,0 }, { 0.2,-0.2,0 }, { 0.2,0.2,0 }, { -0.2,0.2,0 }, { -0.2,-0.2,0 } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_node_set, quadrilateral_P6) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 6;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto result = ref_fig.node_set();

	const std::vector<MathVector> ref = { { -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, { -0.66666666666666667,-1,0 }, { -0.33333333333333333,-1,0 }, { 0,-1,0 }, { 0.33333333333333333,-1,0 }, { 0.66666666666666667,-1,0 }, { 1,-0.66666666666666667,0 }, { 1,-0.33333333333333333,0 }, { 1,0,0 }, { 1,0.33333333333333333,0 }, { 1,0.66666666666666667,0 }, { 0.66666666666666667,1,0 }, { 0.33333333333333333,1,0 }, { 0,1,0 }, { -0.33333333333333333,1,0 }, { -0.66666666666666667,1,0 }, { -1,0.66666666666666667,0 }, { -1,0.33333333333333333,0 }, { -1,0,0 }, { -1,-0.33333333333333333,0 }, { -1,-0.66666666666666667,0 }, { 0.66666666666666667,-0.66666666666666667,0 }, { 0.66666666666666667,0.66666666666666667,0 }, { -0.66666666666666667,0.66666666666666667,0 }, { -0.66666666666666667,-0.66666666666666667,0 }, { 0.66666666666666667,-0.33333333333333333,0 }, { 0.66666666666666667,0,0 }, { 0.66666666666666667,0.33333333333333333,0 }, { 0.33333333333333333,0.66666666666666667,0 }, { 0,0.66666666666666667,0 }, { -0.33333333333333333,0.66666666666666667,0 }, { -0.66666666666666667,0.33333333333333333,0 }, { -0.66666666666666667,0,0 }, { -0.66666666666666667,-0.33333333333333333,0 }, { -0.33333333333333333,-0.66666666666666667,0 }, { 0,-0.66666666666666667,0 }, { 0.33333333333333333,-0.66666666666666667,0 }, { 0.33333333333333333,-0.33333333333333333,0 }, { 0.33333333333333333,0.33333333333333333,0 }, { -0.33333333333333333,0.33333333333333333,0 }, { -0.33333333333333333,-0.33333333333333333,0 }, { 0.33333333333333333,0,0 }, { 0,0.33333333333333333,0 }, { -0.33333333333333333,0,0 }, { 0,-0.33333333333333333,0 }, { 0,0,0 } };
	EXPECT_EQ(result, ref);
}

GTEST_TEST(ReferenceFigure_transformation_monomial_vector, line_P1) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	const ReferenceFigure ref_fig(figure_type, figure_order);
	const auto& result = ref_fig.mapping_monomial_vector();

	VectorFunction<Polynomial> ref = { 1,X };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_vector, line_P2) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.mapping_monomial_vector();

	VectorFunction<Polynomial> ref = { 1,X,X ^ 2 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_vector, line_P3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.mapping_monomial_vector();

	VectorFunction<Polynomial> ref = { 1,X,X ^ 2,X ^ 3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_vector, triangle_P1) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.mapping_monomial_vector();

	VectorFunction<Polynomial> ref = { 1,X,Y };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_vector, triangle_P2) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.mapping_monomial_vector();

	VectorFunction<Polynomial> ref = { 1,X,Y,X ^ 2,X * Y,Y ^ 2 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_vector, triangle_P3) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.mapping_monomial_vector();

	VectorFunction<Polynomial> ref = { 1,X,Y,X ^ 2,X * Y,Y ^ 2,X ^ 3,(X ^ 2) * Y,X * (Y ^ 2),Y ^ 3 };
	EXPECT_EQ(result, ref);
}

GTEST_TEST(ReferenceFigure_reference_quadrature_rule, line_P0) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 0;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { 0.000000000000000,0,0 } },{ 2.000000000000000} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, line_P1) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 1;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { 0.000000000000000,0,0 } },{ 2.000000000000000} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, line_P2) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 2;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.577350269189626,0,0 }, { 0.577350269189626,0,0 } },{ 1.000000000000000, 1.000000000000000} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, line_P3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 3;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.577350269189626,0,0 }, { 0.577350269189626,0,0 } },{ 1.000000000000000, 1.000000000000000} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, line_P4) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 4;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.774596669241483,0,0 }, { 0.000000000000000,0,0 }, { 0.774596669241483,0,0 } },{ 0.555555555555554, 0.888888888888889, 0.555555555555554} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, line_P5) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 5;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.774596669241483,0,0 }, { 0.000000000000000,0,0 }, { 0.774596669241483,0,0 } },{ 0.555555555555554, 0.888888888888889, 0.555555555555554} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, triangle_P0) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 0;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.5,0,0 } },{ 2} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, triangle_P1) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 1;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.6666666666666667,-0.577350269189626,0 }, { -0.9106836025229592,0.577350269189626,0 }, { 0.2440169358562927,-0.577350269189626,0 }, { -0.6666666666666667,0.577350269189626,0 } },{ 0.788675134594813, 0.211324865405187, 0.788675134594813, 0.211324865405187} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, triangle_P2) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 2;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.6666666666666667,-0.577350269189626,0 }, { -0.9106836025229592,0.577350269189626,0 }, { 0.2440169358562927,-0.577350269189626,0 }, { -0.6666666666666667,0.577350269189626,0 } },{ 0.788675134594813, 0.211324865405187, 0.788675134594813, 0.211324865405187} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, triangle_P3) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 3;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.7999999999999996,-0.774596669241483,0 }, { -0.8872983346207415,0,0 }, { -0.9745966692414834,0.774596669241483,0 }, { -0.1127016653792585,-0.774596669241483,0 }, { -0.5,0,0 }, { -0.8872983346207415,0.774596669241483,0 }, { 0.5745966692414826,-0.774596669241483,0 }, { -0.1127016653792585,0,0 }, { -0.7999999999999996,0.774596669241483,0 } },{ 0.2738575106854125, 0.2469135802469129, 0.03478446462322775, 0.4381720170966613, 0.3950617283950618, 0.05565514339716456, 0.2738575106854125, 0.2469135802469129, 0.03478446462322775} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, triangle_P4) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 4;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.7999999999999996,-0.774596669241483,0 }, { -0.8872983346207415,0,0 }, { -0.9745966692414834,0.774596669241483,0 }, { -0.1127016653792585,-0.774596669241483,0 }, { -0.5,0,0 }, { -0.8872983346207415,0.774596669241483,0 }, { 0.5745966692414826,-0.774596669241483,0 }, { -0.1127016653792585,0,0 }, { -0.7999999999999996,0.774596669241483,0 } },{ 0.2738575106854125, 0.2469135802469129, 0.03478446462322775, 0.4381720170966613, 0.3950617283950618, 0.05565514339716456, 0.2738575106854125, 0.2469135802469129, 0.03478446462322775} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, quadrilateral_P0) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 0;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { 0,0,0 } },{ 4} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, quadrilateral_P1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 1;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { 0,0,0 } },{ 4} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, quadrilateral_P2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 2;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.577350269189626,-0.577350269189626,0 }, { -0.577350269189626,0.577350269189626,0 }, { 0.577350269189626,-0.577350269189626,0 }, { 0.577350269189626,0.577350269189626,0 } },{ 1, 1, 1, 1} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, quadrilateral_P3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 3;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.577350269189626,-0.577350269189626,0 }, { -0.577350269189626,0.577350269189626,0 }, { 0.577350269189626,-0.577350269189626,0 }, { 0.577350269189626,0.577350269189626,0 } },{ 1, 1, 1, 1} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, quadrilateral_P4) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 4;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.774596669241483,-0.774596669241483,0 }, { -0.774596669241483,0,0 }, { -0.774596669241483,0.774596669241483,0 }, { 0,-0.774596669241483,0 }, { 0,0,0 }, { 0,0.774596669241483,0 }, { 0.774596669241483,-0.774596669241483,0 }, { 0.774596669241483,0,0 }, { 0.774596669241483,0.774596669241483,0 } },{ 0.3086419753086403, 0.4938271604938259, 0.3086419753086403, 0.4938271604938259, 0.7901234567901235, 0.4938271604938259, 0.3086419753086403, 0.4938271604938259, 0.3086419753086403} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
}
GTEST_TEST(ReferenceFigure_reference_quadrature_rule, quadrilateral_P5) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const size_t integrand_order = 5;
	const auto result = ref_fig.reference_quadrature_rule(integrand_order);

	const QuadratureRule ref = { { { -0.774596669241483,-0.774596669241483,0 }, { -0.774596669241483,0,0 }, { -0.774596669241483,0.774596669241483,0 }, { 0,-0.774596669241483,0 }, { 0,0,0 }, { 0,0.774596669241483,0 }, { 0.774596669241483,-0.774596669241483,0 }, { 0.774596669241483,0,0 }, { 0.774596669241483,0.774596669241483,0 } },{ 0.3086419753086403, 0.4938271604938259, 0.3086419753086403, 0.4938271604938259, 0.7901234567901235, 0.4938271604938259, 0.3086419753086403, 0.4938271604938259, 0.3086419753086403} };
	EXPECT_EQ(result.node_set, ref.node_set);
	EXPECT_EQ(result.weight_set, ref.weight_set);
} 


GTEST_TEST(ReferenceFigure_center_node, line) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto center_node = ref_fig.center_node();
	MathVector ref = { 0,0,0 };

	EXPECT_EQ(center_node, ref);
}
GTEST_TEST(ReferenceFigure_center_node, triangle) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto center_node = ref_fig.center_node();
	MathVector ref = { -1.0 / 3.0, -1.0 / 3.0, 0 };

	EXPECT_EQ(center_node, ref);
}
GTEST_TEST(ReferenceFigure_center_node, quadrilateral) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto center_node = ref_fig.center_node();
	MathVector ref = { 0,0,0 };

	EXPECT_EQ(center_node, ref);
}

GTEST_TEST(ReferenceFigure_face_figure_type, line) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto face_figure_type_set = ref_fig.face_figure_type_set();

	std::vector<FigureType> ref = { FigureType::Point,FigureType::Point };
	EXPECT_EQ(face_figure_type_set, ref);
}
GTEST_TEST(ReferenceFigure_face_figure_type, triangle) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto face_figure_type_set = ref_fig.face_figure_type_set();

	std::vector<FigureType> ref = { FigureType::Line,FigureType::Line,FigureType::Line };
	EXPECT_EQ(face_figure_type_set, ref);
}
GTEST_TEST(ReferenceFigure_face_figure_type, quadrilateral) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	auto face_figure_type_set = ref_fig.face_figure_type_set();

	std::vector<FigureType> ref = { FigureType::Line,FigureType::Line,FigureType::Line,FigureType::Line };
	EXPECT_EQ(face_figure_type_set, ref);
}

GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, line_P1) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);
	
	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(2, 2, { 0.5,-0.5,0.5,0.5 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, line_P2) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(3, 3, { 0, -0.5,  0.5,  0,  0.5,  0.5,1.0,    0, -1.0 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, line_P3) {
	const FigureType figure_type = FigureType::Line;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(4, 4, { -0.0625,  0.0625,  0.5625, -0.5625,-0.0625, -0.0625,  0.5625,  0.5625, 0.5625, -1.6875, -0.5625,  1.6875, 0.5625,  1.6875, -0.5625, -1.6875 });
	EXPECT_TRUE(result.compare_with_finitie_precision(ref, 16)); // 15ULP roud off error !
}
GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, triangle_P1) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(3, 3, { 0, -0.5, -0.5,0.5,  0.5,    0,0.5,    0,  0.5 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, triangle_P2) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(6, 6, { 0,  0.5,  0.5,  0.5,  1.0,  0.5,  0,  0.5,    0,  0.5,    0,    0,  0,    0,  0.5,    0,    0,  0.5,  0, -1.0, -1.0, -1.0, -1.0,    0,1.0,  1.0,  1.0,    0,  1.0,    0,  0, -1.0, -1.0,    0, -1.0, -1.0 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, triangle_P3) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(10, 10, { 0,    -0.5,    -0.5, -1.125, -2.25, -1.125, -0.5625, -1.6875, -1.6875, -0.5625,-0.0625, -0.0625,       0, 0.5625,     0,      0,  0.5625,       0,       0,       0,-0.0625,       0, -0.0625,      0,     0, 0.5625,       0,       0,       0,  0.5625,      0,   1.125,   1.125, 2.8125,   4.5, 1.6875,  1.6875,   3.375,  1.6875,       0,      0, -0.5625, -0.5625,  -2.25, -2.25,      0, -1.6875, -1.6875,       0,       0, 0.5625,    2.25,  0.5625, 1.6875,  2.25,      0,       0,  1.6875,       0,       0, 0.5625,  0.5625,    2.25,      0,  2.25, 1.6875,       0,       0,  1.6875,       0,      0, -0.5625, -0.5625,      0, -2.25,  -2.25,       0,       0, -1.6875, -1.6875,      0,   1.125,   1.125, 1.6875,   4.5, 2.8125,       0,  1.6875,   3.375,  1.6875,      0,  -3.375,  -3.375, -3.375, -6.75, -3.375,       0,  -3.375,  -3.375,       0 });
	const auto [row, column] = ref.size();
	const double epsilon = 1.0E-13;
	for (size_t i = 0; i < row; ++i)
		for (size_t j = 0; j < column; ++j)
			EXPECT_NEAR(result.at(i, j), ref.at(i, j), epsilon); //severe round off error how can i fix it ??
}
GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, quadrilateral_P1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(4, 4, { 0.25, -0.25,  0.25, -0.25,  0.25,  0.25, -0.25, -0.25,  0.25,  0.25,  0.25,  0.25,  0.25, -0.25, -0.25,  0.25 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, quadrilateral_P2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 2;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(9, 9, { 0,    0,  0.25,    0,    0, -0.25, 0.25, -0.25,    0,  0,    0, -0.25,    0,    0, -0.25, 0.25,  0.25,    0,  0,    0,  0.25,    0,    0,  0.25, 0.25,  0.25,    0,  0,    0, -0.25,    0,    0,  0.25, 0.25, -0.25,    0,  0,    0,     0, -0.5,    0,   0.5, -0.5,     0,  0.5,  0,  0.5,     0,    0,  0.5,     0, -0.5,  -0.5,    0,  0,    0,     0,  0.5,    0,  -0.5, -0.5,     0,  0.5,  0, -0.5,     0,    0,  0.5,     0, -0.5,   0.5,    0,1.0,    0,     0,    0, -1.0,     0,  1.0,     0, -1.0 });
	EXPECT_EQ(result, ref);
}
GTEST_TEST(ReferenceFigure_transformation_monomial_matrix, quadrilateral_P3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 3;
	ReferenceFigure ref_fig(figure_type, figure_order);

	const auto key = std::make_pair(figure_type, figure_order);
	const auto& result = ref_fig.inverse_mapping_monomial_matrix();

	RowMajorMatrix ref(16, 16, { 0.00390625, -0.00390625,  0.00390625, -0.00390625, -0.03515625,  0.03515625,  0.31640625,  0.03515625, -0.03515625,  0.03515625, -0.03515625, -0.31640625,  0.31640625, -0.31640625, -0.03515625,  0.03515625, 0.00390625,  0.00390625, -0.00390625, -0.00390625, -0.03515625,  0.03515625,  0.31640625, -0.03515625, -0.03515625, -0.03515625,  0.03515625,  0.31640625, -0.31640625, -0.31640625,  0.03515625,  0.03515625, 0.00390625,  0.00390625,  0.00390625,  0.00390625, -0.03515625, -0.03515625,  0.31640625, -0.03515625, -0.03515625, -0.03515625, -0.03515625,  0.31640625,  0.31640625,  0.31640625, -0.03515625, -0.03515625, 0.00390625, -0.00390625, -0.00390625,  0.00390625, -0.03515625, -0.03515625,  0.31640625,  0.03515625, -0.03515625,  0.03515625,  0.03515625, -0.31640625, -0.31640625,  0.31640625,  0.03515625, -0.03515625,-0.03515625,  0.10546875, -0.10546875,  0.03515625,  0.03515625, -0.03515625, -0.31640625, -0.94921875,  0.31640625, -0.10546875,  0.10546875,  0.94921875, -0.94921875,  0.31640625,  0.94921875, -0.31640625,-0.03515625, -0.10546875,  0.10546875,  0.03515625,  0.03515625, -0.03515625, -0.31640625,  0.94921875,  0.31640625,  0.10546875, -0.10546875, -0.94921875,  0.94921875,  0.31640625, -0.94921875, -0.31640625,-0.03515625, -0.03515625,  0.10546875,  0.10546875,  0.31640625, -0.94921875, -0.31640625,  0.03515625,  0.03515625,  0.31640625, -0.94921875, -0.31640625,  0.94921875,  0.94921875, -0.10546875, -0.10546875,-0.03515625, -0.03515625, -0.10546875, -0.10546875,  0.31640625,  0.94921875, -0.31640625,  0.03515625,  0.03515625,  0.31640625,  0.94921875, -0.31640625, -0.94921875, -0.94921875,  0.10546875,  0.10546875,-0.03515625, -0.10546875, -0.10546875, -0.03515625,  0.03515625,  0.03515625, -0.31640625,  0.94921875,  0.31640625,  0.10546875,  0.10546875, -0.94921875, -0.94921875, -0.31640625,  0.94921875,  0.31640625,-0.03515625,  0.10546875,  0.10546875, -0.03515625,  0.03515625,  0.03515625, -0.31640625, -0.94921875,  0.31640625, -0.10546875, -0.10546875,  0.94921875,  0.94921875, -0.31640625, -0.94921875,  0.31640625,-0.03515625,  0.03515625,  0.10546875, -0.10546875,  0.31640625,  0.94921875, -0.31640625, -0.03515625,  0.03515625, -0.31640625, -0.94921875,  0.31640625,  0.94921875, -0.94921875, -0.10546875,  0.10546875,-0.03515625,  0.03515625, -0.10546875,  0.10546875,  0.31640625, -0.94921875, -0.31640625, -0.03515625,  0.03515625, -0.31640625,  0.94921875,  0.31640625, -0.94921875,  0.94921875,  0.10546875, -0.10546875, 0.31640625,  0.94921875, -2.84765625, -0.94921875, -0.31640625,  0.94921875,  0.31640625, -0.94921875, -0.31640625, -0.94921875,  2.84765625,  0.94921875, -2.84765625, -0.94921875,  2.84765625,  0.94921875, 0.31640625,  0.94921875,  2.84765625,  0.94921875, -0.31640625, -0.94921875,  0.31640625, -0.94921875, -0.31640625, -0.94921875, -2.84765625,  0.94921875,  2.84765625,  0.94921875, -2.84765625, -0.94921875, 0.31640625, -0.94921875, -2.84765625,  0.94921875, -0.31640625, -0.94921875,  0.31640625,  0.94921875, -0.31640625,  0.94921875,  2.84765625, -0.94921875, -2.84765625,  0.94921875,  2.84765625, -0.94921875, 0.31640625, -0.94921875,  2.84765625, -0.94921875, -0.31640625,  0.94921875,  0.31640625,  0.94921875, -0.31640625,  0.94921875, -2.84765625, -0.94921875,  2.84765625, -0.94921875, -2.84765625,  0.94921875 });
	const auto [row, column] = ref.size();
	const double epsilon = 1.0E-14;
	for (size_t i = 0; i < row; ++i)
		for (size_t j = 0; j < column; ++j)
			EXPECT_NEAR(result.at(i, j), ref.at(i, j), epsilon); //severe round off error how can i fix it ??
}


GTEST_TEST(ReferenceFigure, transformation_function_1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure reference_fig(figure_type, figure_order);

	MathVector tp1 = { -1,-1,0 };
	MathVector tp2 = { 1,-1,0 };
	MathVector tp3 = { -1,1,0 };
	MathVector tp4 = { -1,1,0 };
	std::vector<const MathVector*> pv = { &tp1,&tp2,&tp3,&tp4 };
	const auto transformation_function = reference_fig.mapping_function(pv);

	MathVector p1 = { -1,-1,0 };
	MathVector p2 = { 1,-1,0 };
	MathVector p3 = { 1,1,0 };
	MathVector p4 = { -1,1,0 };

	EXPECT_EQ(tp1, transformation_function(p1));
	EXPECT_EQ(tp2, transformation_function(p2));
	EXPECT_EQ(tp3, transformation_function(p3));
	EXPECT_EQ(tp4, transformation_function(p4));
}
GTEST_TEST(ReferenceFigure, transformation_function_2) {
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
	const auto transformation_function = reference_fig.mapping_function(pv);

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
GTEST_TEST(ReferenceFigure, transformation_function_3) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;
	ReferenceFigure ref_fig(figure_type, figure_order);

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 4,2,0 };
	MathVector p3 = { 2,4,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3 };

	const auto result= ref_fig.mapping_function(pv);
	//std::cout << "\n" << result << "\n";

	MathVector rp1 = { -1,-1,0 };
	MathVector rp2 = { 1,-1,0 };
	MathVector rp3 = { -1,1,0 };

	EXPECT_EQ(p1, result(rp1));
	EXPECT_EQ(p2, result(rp2));
	EXPECT_EQ(p3, result(rp3));
}
GTEST_TEST(ReferenceFigure, transformation_function_4) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure reference_fig(figure_type, figure_order);

	MathVector n1 = { 1,2,0 };
	MathVector n2 = { 3,1,0 };
	MathVector n3 = { 4,1,0 };
	MathVector n4 = { 1,3,0 };
	std::vector<const MathVector*> nv = { &n1,&n2,&n3,&n4 };
	const auto transformation_function = reference_fig.mapping_function(nv);

	MathVector p1 = { -1,-1,0 };
	MathVector p2 = { 1,-1,0 };
	MathVector p3 = { 1,1,0 };
	MathVector p4 = { -1,1,0 };

	EXPECT_EQ(n1, transformation_function(p1));
	EXPECT_EQ(n2, transformation_function(p2));
	EXPECT_EQ(n3, transformation_function(p3));
	EXPECT_EQ(n4, transformation_function(p4));
}


GTEST_TEST(ReferenceFigure, transformation_scale_function_1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	ReferenceFigure reference_fig(figure_type, figure_order);

	MathVector n1 = { 1,2,0 };
	MathVector n2 = { 3,1,0 };
	MathVector n3 = { 4,1,0 };
	MathVector n4 = { 1,3,0 };
	std::vector<const MathVector*> nv = { &n1,&n2,&n3,&n4 };
	const auto transformation_function = reference_fig.mapping_function(nv);
	const auto transformation_scale_function = reference_fig.scale_function(transformation_function).value();

	IrrationalFunction ref = (0.25 * Y + 1.25) * (-0.25 * X + 0.25) - (0.25 * X + 0.25) * (-0.25 * Y - 0.75);
	EXPECT_EQ(transformation_scale_function, ref);
}


GTEST_TEST(Figure, calculate_quadrature_rule_1) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 4,2,0 };
	MathVector p3 = { 2,4,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 1;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	double area = 0;
	for (const auto& weight : quadrature_rule.weight_set)
		area += weight;

	const double ref = 3;
	EXPECT_EQ(area, ref);
}
GTEST_TEST(Figure, calculate_quadrature_rule_2) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 1.5,1.257,0 };
	MathVector p3 = { 2.4874,1.24,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 1;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	double area = 0;
	for (const auto& weight : quadrature_rule.weight_set)
		area += weight;

	const double ref = 0.362569100000000;
	EXPECT_DOUBLE_EQ(area, ref);
}
GTEST_TEST(Figure, calculate_quadrature_rule_3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 2.4874,1.257,0 };
	MathVector p3 = { 3.4874,1.24,0 };
	MathVector p4 = { 1,2.574,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 1;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	double area = 0;
	for (const auto& weight : quadrature_rule.weight_set)
		area += weight;

	const double ref = 1.072740900000000; 
	EXPECT_DOUBLE_EQ(area, ref);
}
GTEST_TEST(Figure, calculate_quadrature_rule_4) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 1;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	double area = 0;
	for (const auto& weight : quadrature_rule.weight_set)
		area += weight;

	const double ref = 2;
	EXPECT_DOUBLE_EQ(area, ref);
}
GTEST_TEST(Figure, calculate_quadrature_rule_5) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 2;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	MathVector ref_n0 = { 1.238104996137775, 1.987298334620742,  0 };
	MathVector ref_n1 = { 1.281754163448146, 2.330947501931113,  0 };
	MathVector ref_n2 = { 1.325403330758517, 2.674596669241483,  0 };
	MathVector ref_n3 = { 2.056350832689629, 1.556350832689629,  0 };
	MathVector ref_n4 = { 2.250000000000000, 1.750000000000000,  0 };
	MathVector ref_n5 = { 2.443649167310371, 1.943649167310371,  0 };
	MathVector ref_n6 = { 2.874596669241483, 1.125403330758517,  0 };
	MathVector ref_n7 = { 3.218245836551854, 1.169052498068887,  0 };
	MathVector ref_n8 = { 3.561895003862225, 1.212701665379258,  0 };
		
	std::vector<MathVector> ref_node_set = { ref_n0,ref_n1,ref_n2,ref_n3,ref_n4,ref_n5,ref_n6,ref_n7,ref_n8 };
	std::vector<double> ref_weight_set = { 0.154320987654321,0.294728189459351,0.214089249169867,0.199098971034476,0.395061728395062,0.294728189459351,0.094552726138774,0.199098971034476,0.154320987654321 };
	constexpr double epsilon = 1.0E-15;
	for (size_t i = 0; i < ref_node_set.size(); ++i) {
		for (size_t j = 0; j < 3; ++j)
			EXPECT_NEAR(quadrature_rule.node_set[i][j],ref_node_set[i][j], epsilon);

		EXPECT_NEAR(quadrature_rule.weight_set[i],ref_weight_set[i], epsilon);
	}

	//round off error..
	//for (size_t i = 0; i < ref_node_set.size(); ++i) {
	//	for (size_t j = 0; j < 3; ++j)
	//		EXPECT_EQ(quadrature_rule.node_set[i][j] - ref_node_set[i][j], 0);

	//	EXPECT_EQ(quadrature_rule.weight_set[i] - ref_weight_set[i], 0);
	//}
}
GTEST_TEST(Figure, calculate_quadrature_rule_6) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 5;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	double area = 0;
	for (const auto& weight : quadrature_rule.weight_set)
		area += weight;

	const double ref = 2;
	EXPECT_DOUBLE_EQ(area, ref);
}

GTEST_TEST(Figure, initial_basis_1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	
	MathVector p1 = { 0,0,0 };
	MathVector p2 = { 2,0,0 };
	MathVector p3 = { 2,2,0 };
	MathVector p4 = { 0,2,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 1;
	const auto result = fig.initial_basis_vector(polynomial_order);
	
	VectorFunction<Polynomial> ref = { 1, X - 1, Y - 1 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(Figure, initial_basis_2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 0,0,0 };
	MathVector p2 = { 2,0,0 };
	MathVector p3 = { 2,2,0 };
	MathVector p4 = { 0,2,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 2;
	const auto result = fig.initial_basis_vector(polynomial_order);

	VectorFunction<Polynomial> ref = { 1, X - 1, Y - 1, (X - 1) ^ 2,(X - 1) * (Y - 1),(Y - 1) ^ 2 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(Figure, initial_basis_3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 2.4874,1.257,0 };
	MathVector p3 = { 3.4874,1.24,0 };
	MathVector p4 = { 1,2.574,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 1;
	const auto result = fig.initial_basis_vector(polynomial_order);

	VectorFunction<Polynomial> ref = { 1, X - 1.9937, Y - 1.76775 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(Figure, initial_basis_4) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 2.4874,1.257,0 };
	MathVector p3 = { 3.4874,1.24,0 };
	MathVector p4 = { 1,2.574,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 2;
	const auto result = fig.initial_basis_vector(polynomial_order);

	VectorFunction<Polynomial> ref = { 1, X - 1.9937, Y - 1.76775, (X - 1.9937) ^ 2,(X - 1.9937) * (Y - 1.76775),(Y - 1.76775) ^ 2 };
	EXPECT_EQ(result, ref);
}

GTEST_TEST(MS, inner_product_1) {
	Polynomial f0 = 0.70710678118654746;
	Polynomial f1 = 0.94868329805051632 * X - 2.0554804791094519;
	Polynomial f2 = 1.6710199556880552 * X + 3.0382181012510086 * Y - 9.1906097562843012;
	Polynomial f3 = 1.3103156645083498 * ((X - 2.25) ^ 2) - 0.24190443037077192 * X - 0.33261859175981184 * Y + 0.39687445607705152;
	VectorFunction<Polynomial> vf = { f0,f1,f2,f3 };

	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };
	Figure fig(figure_type, figure_order, std::move(pv));

	const auto num_func = vf.size();
	constexpr double error = 9.0E-14;
	for (size_t i = 0; i < num_func; ++i) {
		for (size_t j = 0; j < num_func; ++j) {
			const auto result = ms::inner_product(vf[i], vf[j], fig);

			if (i == j)
				EXPECT_NEAR(result, 1, error);
			else
				EXPECT_NEAR(result, 0, error);
		}
	}
}
GTEST_TEST(MS, inner_product_2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 2;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	Polynomial f0 = 0.70710678118654746;
	Polynomial f1 = (X - 2.25) * (Y - 1.75);
	const auto result = ms::inner_product(f0, f1, quadrature_rule);
	
	const double ref = -0.441941738241592;
	//EXPECT_EQ(result, ref);
	EXPECT_NEAR(result, ref, 9.0E-15); // round off error ?? bug ?
}
GTEST_TEST(MS, inner_product_3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 2;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	Polynomial f0 = 0.94868329805051632 * X - 2.0554804791094519;
	Polynomial f1 = (X - 2.25) * (Y - 1.75);
	const auto result = ms::inner_product(f0, f1, quadrature_rule);

	const double ref = 0.039528470752105;
	//EXPECT_EQ(result, ref);
	EXPECT_NEAR(result, ref, 9.0E-15); // round off error ?? bug ?
}
GTEST_TEST(MS, inner_product_4) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 2;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	Polynomial f0 = 1.6710199556880552 * X + 3.0382181012510086 * Y - 9.1906097562843012;
	Polynomial f1 = (X - 2.25) * (Y - 1.75);
	const auto result = ms::inner_product(f0, f1, quadrature_rule);

	const double ref = -0.158240526106823;
	//EXPECT_EQ(result, ref);
	EXPECT_NEAR(result, ref, 9.0E-15); // round off error ?? bug ?
}
GTEST_TEST(MS, inner_product_5) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));
	const size_t polynomial_order = 2;
	const auto quadrature_rule = fig.quadrature_rule(polynomial_order);

	Polynomial f0 = 1.3103156645083498 * ((X - 2.25) ^ 2) - 0.24190443037077192 * X - 0.33261859175981184 * Y + 0.39687445607705152;
	Polynomial f1 = (X - 2.25) * (Y - 1.75);
	const auto result = ms::inner_product(f0, f1, quadrature_rule);

	const double ref = -0.383687304838086;
	//EXPECT_EQ(result, ref);
	EXPECT_NEAR(result, ref, 9.0E-15); // round off error ?? bug ?
}

GTEST_TEST(MS, Gram_Schmidt_process_1) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));

	const size_t polynomial_order = 2;
	const auto orthonormal_basis = fig.orthonormal_basis_vector(polynomial_order);

	double max_error = 0.0;
	for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
		for (size_t j = 0; j <= i; ++j) {
			const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], fig);

			if (i == j)
				max_error = std::max(max_error, std::abs(1 - result));
			else
				max_error = std::max(max_error, std::abs(result));
		}
	}

	constexpr double allowable_error = 9.0E-13;
	EXPECT_LE(max_error, allowable_error);
}
GTEST_TEST(MS, Gram_Schmidt_process_2) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));

	const size_t polynomial_order = 3;
	const auto orthonormal_basis = fig.orthonormal_basis_vector(polynomial_order);

	double max_error = 0.0;
	for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
		for (size_t j = 0; j <= i; ++j) {
			const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], fig);

			if (i == j)
				max_error = std::max(max_error, std::abs(1 - result));
			else
				max_error = std::max(max_error, std::abs(result));
		}
	}

	constexpr double allowable_error = 9.0E-13;
	EXPECT_LE(max_error, allowable_error);
}
GTEST_TEST(MS, Gram_Schmidt_process_3) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 3,1,0 };
	MathVector p3 = { 4,1,0 };
	MathVector p4 = { 1,3,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));

	const size_t polynomial_order = 5;
	const auto orthonormal_basis = fig.orthonormal_basis_vector(polynomial_order);

	double max_error = 0.0;
	for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
		for (size_t j = 0; j <= i; ++j) {
			const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], fig);

			if (i == j)
				max_error = std::max(max_error, std::abs(1 - result));
			else
				max_error = std::max(max_error, std::abs(result));
		}
	}

	constexpr double allowable_error = 9.0E-13;
	EXPECT_LE(max_error, allowable_error);
}
GTEST_TEST(MS, Gram_Schmidt_process_4) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 0,0,0 };
	MathVector p2 = { 2,0,0 };
	MathVector p3 = { 2,2,0 };
	MathVector p4 = { 0,2,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));

	const size_t polynomial_order = 5;
	const auto orthonormal_basis = fig.orthonormal_basis_vector(polynomial_order);

	double max_error = 0.0;
	for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
		for (size_t j = 0; j <= i; ++j) {
			const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], fig);

			if (i == j)
				max_error = std::max(max_error, std::abs(1 - result));
			else
				max_error = std::max(max_error, std::abs(result));
		}
	}

	constexpr double allowable_error = 9.0E-13;
	EXPECT_LE(max_error, allowable_error);
}
GTEST_TEST(MS, Gram_Schmidt_process_5) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	MathVector p1 = { 1,2,0 };
	MathVector p2 = { 2.4874,1.257,0 };
	MathVector p3 = { 3.4874,1.24,0 };
	MathVector p4 = { 1,2.574,0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));

	const size_t polynomial_order = 5;
	const auto orthonormal_basis = fig.orthonormal_basis_vector(polynomial_order);

	double max_error = 0.0;
	for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
		for (size_t j = 0; j <= i; ++j) {
			const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], fig);

			if (i == j)
				max_error = std::max(max_error, std::abs(1 - result));
			else
				max_error = std::max(max_error, std::abs(result));
		}
	}

	constexpr double allowable_error = 9.0E-13;
	EXPECT_LE(max_error, allowable_error);
}
GTEST_TEST(MS, Gram_Schmidt_process_6) {
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	const MathVector p1 = { 0.3635520579711813,		0.2973431147402148,		0 };
	const MathVector p2 = { 0.3512301560533574,		0.3184608229801218,		0 };
	const MathVector p3 = { 0.3309655464243111,		0.3010404355350647,		0 };
	const MathVector p4 = { 0.3359655464243111,		0.2910404355350647,		0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	Figure fig(figure_type, figure_order, std::move(pv));

	const size_t polynomial_order = 5;
	const auto orthonormal_basis = fig.orthonormal_basis_vector(polynomial_order);

	double max_error = 0.0;
	for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
		for (size_t j = 0; j <= i; ++j) {
			const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], fig);

			if (i == j)
				max_error = std::max(max_error, std::abs(1 - result));
			else
				max_error = std::max(max_error, std::abs(result));
		}
	}

	constexpr double allowable_error = 9.0E-13;
	EXPECT_LE(max_error, allowable_error);
}
GTEST_TEST(MS, Gram_Schmidt_process_7) {
	const FigureType figure_type = FigureType::Triangle;
	const size_t figure_order = 1;

	const MathVector p1 = { 0.3635520579711813,		0.2973431147402148,		0 };
	const MathVector p2 = { 0.3512301560533574,		0.3184608229801218,		0 };
	const MathVector p3 = { 0.3309655464243111,		0.3010404355350647,		0 };
	std::vector<const MathVector*> pv = { &p1,&p2,&p3 };

	Figure fig(figure_type, figure_order, std::move(pv));

	const size_t polynomial_order = 5;
	const auto orthonormal_basis = fig.orthonormal_basis_vector(polynomial_order);;

	double max_error = 0.0;
	for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
		for (size_t j = 0; j <= i; ++j) {
			const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], fig);

			if (i == j)
				max_error = std::max(max_error, std::abs(1 - result));
			else
				max_error = std::max(max_error, std::abs(result));
		}
	}

	constexpr double allowable_error = 9.0E-13;
	EXPECT_LE(max_error, allowable_error);
}



GTEST_TEST(JACOBIAN, CONSTRUCTOR2) {
	Polynomial p1 = 1 + X + Y;
	Polynomial p2 = X * Z;
	Polynomial p3 = Z ^ 3;
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
	const auto solution = ms::Newton_Raphson(f, initial_guess, convergence_criteria);
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

//#include <random>
//GTEST_TEST(MS, choice_of_initial_basis) {
//	const FigureType figure_type = FigureType::Quadrilateral;
//	const size_t figure_order = 1;
//
//	const size_t polynomial_order = 5;
//	const auto integrand_order = 2 * polynomial_order;
//	
//	MathVector center = { 100000,10000,0 };
//	const size_t num_case = 10;
//	std::string result_str;
//
//	std::cout << "center coordinate : " << center << "\n";
//	std::cout << "Polynomial order : " << std::to_string(polynomial_order) << "\n";
//	result_str += "\n==================== description ===================\n";
//	result_str += "center coordinate : " + center.to_string() + "\n";
//	result_str += "Polynomial order : " + std::to_string(polynomial_order) + "\n";
//
//	for (size_t i = 0; i < num_case; ++i) {
//		std::random_device rd;
//		std::mt19937 gen(rd());
//		std::uniform_real_distribution<double> dis(0, 100000);
//
//		const MathVector d1 = { -1 - dis(gen), -1 - dis(gen), 0 };
//		const MathVector d2 = { 1 + dis(gen), -1 - dis(gen), 0 };
//		const MathVector d3 = { 1 + dis(gen), 1 + dis(gen), 0 };
//		const MathVector d4 = { -1 - dis(gen), 1 + dis(gen), 0 };
//
//
//		//std::uniform_real_distribution<double> dis(-0.4, 0.4);
//
//		//const MathVector d1 = { -1 + dis(gen), -1 + dis(gen), 0 };
//		//const MathVector d2 = { 1 + dis(gen), -1 + dis(gen), 0 };
//		//const MathVector d3 = { 1 + dis(gen), 1 + dis(gen), 0 };
//		//const MathVector d4 = { -1 + dis(gen), 1 + dis(gen), 0 };
//
//		const auto p1 = center + d1;
//		const auto p2 = center + d2;
//		const auto p3 = center + d3;
//		const auto p4 = center + d4;
//		std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };
//
//		Figure fig(figure_type, figure_order, std::move(pv));
//		const auto quadrature_rule = fig.calculate_quadrature_rule(integrand_order);
//
//		const auto initial_basis1 = fig.initial_basis_vector1(polynomial_order);
//		const auto orthonormal_basis1 = ms::Gram_Schmidt_Process(initial_basis1, quadrature_rule);
//		double max_error1 = 0.0;
//		for (size_t i = 0; i < orthonormal_basis1.size(); ++i) {
//			for (size_t j = 0; j <= i; ++j) {
//				const auto result = ms::inner_product(orthonormal_basis1[i], orthonormal_basis1[j], quadrature_rule);
//
//				if (i == j)
//					max_error1 = std::max(max_error1, std::abs(1 - result));
//				else
//					max_error1 = std::max(max_error1, std::abs(result));
//			}
//		}
//
//		const auto initial_basis2 = fig.initial_basis_vector2(polynomial_order);
//		const auto orthonormal_basis2 = ms::Gram_Schmidt_Process(initial_basis2, quadrature_rule);
//		double max_error2 = 0.0;
//		for (size_t i = 0; i < orthonormal_basis2.size(); ++i) {
//			for (size_t j = 0; j <= i; ++j) {
//				const auto result = ms::inner_product(orthonormal_basis2[i], orthonormal_basis2[j], quadrature_rule);
//
//				if (i == j)
//					max_error2 = std::max(max_error2, std::abs(1 - result));
//				else
//					max_error2 = std::max(max_error2, std::abs(result));
//			}
//		}
//
//		const auto initial_basis3 = fig.initial_basis_vector3(polynomial_order);
//		const auto orthonormal_basis3 = ms::Gram_Schmidt_Process(initial_basis3, quadrature_rule);
//		double max_error3 = 0.0;
//		for (size_t i = 0; i < orthonormal_basis3.size(); ++i) {
//			for (size_t j = 0; j <= i; ++j) {
//				const auto result = ms::inner_product(orthonormal_basis3[i], orthonormal_basis3[j], quadrature_rule);
//
//				if (i == j)
//					max_error3 = std::max(max_error3, std::abs(1 - result));
//				else
//					max_error3 = std::max(max_error3, std::abs(result));
//			}
//		}
//
//		std::cout << "\n==================== case" << std::to_string(i) << " ====================\n";
//		std::cout << "==================== point ====================\n";
//		std::cout << p1 << "\n";
//		std::cout << p2 << "\n";
//		std::cout << p3 << "\n";
//		std::cout << p4 << "\n";
//		std::cout << "==================== result ===================\n";
//		std::cout << "1 error: " << max_error1 << "\n";
//		std::cout << "2 error: " << max_error2 << "\n";
//		std::cout << "3 error: " << max_error3 << "\n";
//		std::cout << "\n===================== end =====================\n";
//
//		result_str += "\n==================== case" + std::to_string(i) + " ====================\n";
//		result_str += "==================== point ====================\n";
//		result_str += p1.to_string() + "\n";
//		result_str += p2.to_string() + "\n";
//		result_str += p3.to_string() + "\n";
//		result_str += p4.to_string() + "\n";
//		result_str += "==================== result ===================\n";
//		result_str += "1 error: " + ms::double_to_string(max_error1) + "\n";
//		result_str += "2 error: " + ms::double_to_string(max_error2) + "\n";
//		result_str += "3 error: " + ms::double_to_string(max_error3) + "\n";
//		result_str += "\n===================== end =====================\n";
//	}
//	Text result_txt = { result_str };
//	result_txt.add_write("./Initial Basis Test.txt");
//}