#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Polynomial.h"

GTEST_TEST(MONOMIAL, CONSTRUCTOR1) {
	Monomial m1;
	Monomial m2 = { 0 };
	EXPECT_EQ(m1, m2);
}
GTEST_TEST(MONOMIAL, CONSTRUCTOR2){
	Monomial m1(0);
	Monomial m2 = { 1 };
	EXPECT_EQ(m1, m2);
}
GTEST_TEST(MONOMIAL, CONSTRUCTOR4) {
	Monomial m1;
	Monomial m2{ 0,0,0 };
	EXPECT_EQ(m1, m2);
}
GTEST_TEST(MONOMIAL, CONSTRUCTOR5) {
	Monomial m1{ 1 };
	Monomial m2{ 1,0,0 };
	EXPECT_EQ(m1, m2);
}
GTEST_TEST(MONOMIAL, CONSTRUCTOR6) {
	Monomial m1(4);
	Monomial m2 = { 0,0,0,0,1 };
	EXPECT_EQ(m1, m2);
}
GTEST_TEST(MONOMIAL, CONSTRUCTOR7) {
	Monomial m1 = X;
	Monomial m2 = { 1 };
	EXPECT_EQ(m1, m2);
}
GTEST_TEST(MONOMIAL, CONSTRUCTOR8) {
	Monomial m1 = Y;
	Monomial m2 = { 0,1 };
	EXPECT_EQ(m1, m2);
}
GTEST_TEST(MONOMIAL, CONSTRUCTOR9) {
	Monomial m1 = Z;
	Monomial m2 = { 0,0,1 };
	EXPECT_EQ(m1, m2);
}


GTEST_TEST(MONOMIAL, REMOVE_MEANINGLESS_ZERO1) {
	Monomial m = { 0,0,1,0,0,0 };
	m.remove_meaningless_zero();

	const Monomial ref = { 0,0,1 };
	EXPECT_EQ(m, ref);
}
GTEST_TEST(MONOMIAL, REMOVE_MEANINGLESS_ZERO2) {
	Monomial m = { 0,0,1 };
	m.remove_meaningless_zero();

	const Monomial ref = { 0,0,1 };
	EXPECT_EQ(m, ref);
}
GTEST_TEST(MONOMIAL, REMOVE_MEANINGLESS_ZERO3) {
	Monomial m = { 0,0,0 };
	m.remove_meaningless_zero();

	const Monomial ref;
	EXPECT_EQ(m, ref);
}


GTEST_TEST(MONOMIAL, IS_CONSTANT1) {
	Monomial m = { 0,0,1,0,0,0 };
	const auto result = m.is_constant();

	const bool ref = false;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, IS_CONSTANT2) {
	Monomial m = { 0,0,1 };
	const auto result = m.is_constant();

	const bool ref = false;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, IS_CONSTANT3) {
	Monomial m = { 0,0,0 };
	const auto result = m.is_constant();

	const bool ref = true;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MONOMIAL, EXPONENT1) {
	Monomial m1;
	const size_t variable_index = 0;
	const auto result = m1.exponent(variable_index);

	const size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, EXPONENT2) {
	Monomial m1 = { 1 };
	size_t variable_index = 0;
	const auto result = m1.exponent(variable_index);

	const size_t ref = 1;
	EXPECT_EQ(m1.exponent(variable_index), ref);
}
GTEST_TEST(MONOMIAL, EXPONENT3) {
	Monomial m1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
	const std::vector<size_t> exponent_set = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
	for (size_t i = 0; i < exponent_set.size(); ++i)
		EXPECT_EQ(m1.exponent(i), exponent_set[i]);
}
GTEST_TEST(MONOMIAL, EXPONENT4) {
	Monomial m1 = { 0,1,2 };
	size_t variable_index = 5;
	const auto result = m1.exponent(variable_index);

	size_t ref = 0;
	EXPECT_EQ(result,ref);
}


GTEST_TEST(MONOMIAL, MONOMIAL_ORDER1) {
	Monomial m;
	const auto result = m.monomial_order();

	constexpr size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER2) {
	Monomial m(0);
	const auto result = m.monomial_order();

	constexpr size_t ref = 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER3) {
	Monomial m = { 0 };
	const auto result = m.monomial_order();

	constexpr size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER4) {
	Monomial m = { 0,0,0,0,0,0,0,0,0 };
	const auto result = m.monomial_order();

	constexpr size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER5) {
	Monomial m(128);
	const auto result = m.monomial_order();

	constexpr size_t ref = 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER6) {
	Monomial m = { 1,2,3,4,5 };
	const auto result = m.monomial_order();

	constexpr size_t ref = 15;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MONOMIAL, DOMAIN_DIMENSION1) {
	Monomial m;
	const auto result = m.domain_dimension();

	size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, DOMAIN_DIMENSION2) {
	Monomial m = { 1 };
	const auto result = m.domain_dimension();

	size_t ref = 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, DOMAIN_DIMENSION3) {
	Monomial m(2);
	const auto result = m.domain_dimension();

	size_t ref = 3;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, DOMAIN_DIMENSION4) {
	Monomial m = { 3 };
	const auto result = m.domain_dimension();

	size_t ref = 1;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MONOMIAL, REDUCE_ORDER1) {
	Monomial m;
	EXPECT_ANY_THROW(m.reduce_order(0));
}
GTEST_TEST(MONOMIAL, REDUCE_ORDER2) {
	Monomial m(0);
	m.reduce_order(0);

	Monomial ref;
	EXPECT_EQ(m, ref);
}
GTEST_TEST(MONOMIAL, REDUCE_ORDER3) {
	Monomial m(10);
	m.reduce_order(10);

	Monomial ref;
	EXPECT_EQ(m, ref);
}
GTEST_TEST(MONOMIAL, REDUCE_ORDER4) {
	Monomial m = { 1,0,3,0,5 };
	m.reduce_order(0);

	Monomial ref{ 0,0,3,0,5 };
	EXPECT_EQ(m, ref);
}
GTEST_TEST(MONOMIAL, REDUCE_ORDER5) {
	Monomial m = { 1,0,3,0,5 };
	EXPECT_ANY_THROW(m.reduce_order(1));
}		
GTEST_TEST(MONOMIAL, REDUCE_ORDER6) {
	Monomial m = { 1,0,3,0,5 };
	m.reduce_order(2);

	Monomial ref = { 1,0,2,0,5 };
	EXPECT_EQ(m, ref);
}


GTEST_TEST(MONOMIAL, TO_STRING1) {
	Monomial m = { 1,0,3,0,5 };
	const auto result = m.to_string();

	const std::string ref = "(x0)^1(x2)^3(x4)^5";
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, TO_STRING2) {
	Monomial m(128);
	const auto result = m.to_string();

	const std::string ref = "(x128)^1";
	EXPECT_EQ(result, ref);
} 
GTEST_TEST(MONOMIAL, TO_STRING3) {
	Monomial m = { 1,2,3,4 };
	const auto result = m.to_string();

	const std::string ref = "(x0)^1(x1)^2(x2)^3(x3)^4";
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, TO_STRING4) {
	Monomial m;
	const auto result = m.to_string();

	const std::string ref = "(1)";
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION_ASSIGN1) {
	Monomial m1;
	Monomial m2;
	m1 *= m2;

	Monomial ref;
	EXPECT_EQ(m1, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION_ASSIGN2) {
	Monomial m1;
	Monomial m2 = { 3 };
	m1 *= m2;

	Monomial ref = { 3 };
	EXPECT_EQ(m1, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION_ASSIGN3) {
	Monomial m1 = { 1,2,3,4 };
	Monomial m2 = { 1,2 };
	m1 *= m2;

	Monomial ref{ 2,4,3,4 };
	EXPECT_EQ(m1, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION_ASSIGN4) {
	Monomial m1{ 1,2,3,4 };
	Monomial m2{ 1,2,0,1 };
	m1 *= m2;

	Monomial ref{ 2,4,3,5 };
	EXPECT_EQ(m1, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION_ASSIGN5) {
	Monomial m1{ 1,2,3,4 };
	Monomial m2{ 1,2,0,1,8,3 };
	m1 *= m2;

	Monomial ref{ 2,4,3,5,8,3 };
	EXPECT_EQ(m1, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION_ASSIGN6) {
	Monomial m1{ 1,2,3,4 };
	Monomial m2;
	m1 *= m2;

	Monomial ref{ 1,2,3,4 };
	EXPECT_EQ(m1, ref);
}


GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION1){
	Monomial m1;
	Monomial m2;
	const auto result = m1 * m2;

	Monomial ref;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION2) {
	Monomial m1 = { 1,2,3,4 };
	Monomial m2 = { 1,2 };
	const auto result = m1 * m2;

	Monomial ref = { 2,4,3,4 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION3) {
	Monomial m1 = { 1,2,3,4 };
	Monomial m2 = { 1,2,0,1 };
	const auto result = m1 * m2;

	Monomial ref = { 2,4,3,5 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION4) {
	Monomial m1 = { 1,2,3,4 };
	Monomial m2 = { 1,2,0,1,8,3 };
	const auto result = m1 * m2;

	Monomial ref = { 2,4,3,5,8,3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION5) {
	Monomial m1 = { 1,2,3,4 };
	Monomial m2 = { 1,2,0,1,8,3 };
	Monomial m3 = { 1,2 };
	const auto result = m1 * m2 * m3;

	Monomial ref{ 3,6,3,5,8,3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION6) {
	Monomial m1 = { 1,2,3,4 };
	Monomial m2 = { 1,2,0,1,8,3 };
	Monomial m3;
	const auto result = m1 * m2 * m3;

	Monomial ref = { 2,4,3,5,8,3 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MONOMIAL, OPERATOR_POWER1) {
	Monomial m(0);
	const auto result = m^3;

	Monomial ref = { 3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_POWER2) {
	Monomial m(3);
	const auto result = m^7;

	Monomial ref = { 0,0,0,7 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_POWER3) {
	Monomial m = { 1,2,3 };
	const auto result = m ^ 2;

	Monomial ref = { 2,4,6 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MONOMIAL, FUNCTION_CALL_1) {
	const Monomial m;
	const MathVector v1 = { 1,2,3 };

	const double ref = 1.0;
	EXPECT_EQ(m(v1), ref);
}
GTEST_TEST(MONOMIAL, FUNCTION_CALL_2) {
	const Monomial m;
	const MathVector v4;

	const double ref = 1.0;
	EXPECT_EQ(m(v4), ref);
}
GTEST_TEST(MONOMIAL, FUNCTION_CALL_3) {
	const Monomial m;

	const double ref = 1.0;
	EXPECT_EQ(m(), ref);
}
GTEST_TEST(MONOMIAL, FUNCTION_CALL_4) {
	const Monomial m = { 1,2,3 };
	const MathVector v1;
	EXPECT_ANY_THROW(m(v1));
}
GTEST_TEST(MONOMIAL, FUNCTION_CALL_5) {
	const Monomial m = { 1,2,3 };
	const MathVector v2 = { 1,2,3 };

	const double ref2 = 108;
	EXPECT_EQ(m(v2), ref2);
}
GTEST_TEST(MONOMIAL, FUNCTION_CALL_6) {
	const Monomial m = { 1,2,3 };
	const MathVector v3 = { 1.84,2.789,3.487946 };

	const double ref3 = 6.073291260986822e+02;
	EXPECT_DOUBLE_EQ(m(v3), ref3);
}


GTEST_TEST(SIMPLE_POLYNOMIAL, CONSTRUCTOR1) {
	SimplePolynomial p1;
	SimplePolynomial p2(0.0);
	EXPECT_EQ(p1, p2);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, CONSTRUCTOR2) {
	SimplePolynomial p1;
	SimplePolynomial p2(1.0);
	EXPECT_NE(p1, p2);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, CONSTRUCTOR3) {
	SimplePolynomial p1(0.0);
	SimplePolynomial p2(1.0);
	EXPECT_NE(p1, p2);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, CONSTRUCTOR4) {
	SimplePolynomial p1((Monomial()));
	SimplePolynomial p2(1.0);
	EXPECT_EQ(p1, p2);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, CONSTRUCTOR5) {
	std::vector<double> v_c = { 1,2 };
	std::vector<Monomial> v_m = { Monomial(), Monomial{1,2,3} };
	SimplePolynomial p(v_c, v_m);

	SimplePolynomial ref = { {1,2},{ {0}, {1,2,3} } };
	EXPECT_EQ(p, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, CONSTRUCTOR6) {
	SimplePolynomial p1 = { { 1,2 },{ {0},{1} } };
	SimplePolynomial p2 = { { 2,1 },{ {1},{0} } };
	EXPECT_EQ(p1, p2);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, CONSTRUCTOR7) {
	std::vector<Monomial> m_set2 = { {1}, {0} };
	std::vector<double> c_set2 = { 1, 1, 1 };
	EXPECT_ANY_THROW(SimplePolynomial p2(c_set2, m_set2));
}
GTEST_TEST(SIMPLE_POLYNOMIAL, CONSTRUCTOR8) {
	SimplePolynomial p = { { 1, 1} ,{ {0}, {0} } };
	
	SimplePolynomial ref = { {2},{0} };
	EXPECT_EQ(p, ref);
}


GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_ADDITION_ASSIGN1) {
	SimplePolynomial result = { { 1,1 }, {{2},{1}} };
	SimplePolynomial p2 = { { 1,1 },{X,{0}} };
	result += p2;

	SimplePolynomial ref = { {1,2,1},{{2},{1},{0}} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_ADDITION_ASSIGN2) {
	SimplePolynomial result;
	SimplePolynomial p2;
	result += p2;

	SimplePolynomial ref;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_ADDITION_ASSIGN3) {
	SimplePolynomial p1{ {-12,45},{ {2,1},{1} } };
	SimplePolynomial p2{ {12,-45},{ {2,1},{1} } };
	SimplePolynomial ref;

	p1 += p2;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_ADDITION_ASSIGN4) {
	SimplePolynomial result = (X ^ 2) + X;
	SimplePolynomial p2 = X + 1;
	result += p2;

	SimplePolynomial ref = { {1,2,1},{{2},{1},{0}} };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_ADDITION_ASSIGN5) {
	SimplePolynomial p1 = -12 * ((X ^ 2) * Y) + 45;
	SimplePolynomial p2 = 12 * ((X ^ 2) * Y) - 45;
	p1 += p2;

	SimplePolynomial ref;
	EXPECT_EQ(p1, ref);		 
}


GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_SUBSTRACTION_ASSIGN1) {
	SimplePolynomial p1 = X + Y;
	SimplePolynomial p2 = X + 3 * Y;
	p1 -= p2;

	SimplePolynomial ref = -2 * Y;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_SUBSTRACTION_ASSIGN2) {
	SimplePolynomial p1 = X * Y;
	SimplePolynomial p2 = X + 3 * Y;
	p1 -= p2;

	SimplePolynomial ref = X * Y - X - 3 * Y;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_SUBSTRACTION_ASSIGN3) {
	SimplePolynomial p1 = X * Y;
	SimplePolynomial p2 = X;
	p1 -= p2;

	SimplePolynomial ref = X * Y - X;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_SUBSTRACTION_ASSIGN4) {
	SimplePolynomial p1;
	SimplePolynomial p2 = X;
	p1 -= p2;

	SimplePolynomial ref = -1 * X;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_SUBSTRACTION_ASSIGN5) {
	SimplePolynomial p1;
	SimplePolynomial p2 = 3 * (X * Y) - 3 * ((X ^ 2) * Z);
	p1 -= p2;

	SimplePolynomial ref = -3 * (X * Y) + 3 * ((X ^ 2) * Z);
	EXPECT_EQ(p1, ref);
}


GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_SCALAR_MULTIPLICATION_ASSIGN1) {
	SimplePolynomial p1;
	double scalar = 5;
	p1 *= scalar;

	SimplePolynomial ref = 5;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, OPERATOR_SCALAR_MULTIPLICATION_ASSIGN2) {
	SimplePolynomial p1 = (X ^ 2) + X + 1;
	double scalar = 5;
	p1 *= scalar;

	SimplePolynomial ref = 5 * (X ^ 2) + 5 * X + 5;
	EXPECT_EQ(p1, ref);
}



GTEST_TEST(SIMPLE_POLYNOMIAL, DIFFERENTIATE1) {
	SimplePolynomial p1 = { { 1, 1, 1 }, { {2}, {1}, {0} } };

	SimplePolynomial ref({ 2,1 }, { {1},{0} });
	size_t variable_index = 0;
	EXPECT_EQ(p1.differentiate(variable_index), ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, DIFFERENTIATE2) {
	SimplePolynomial p1({ 1, 1, 1 }, { {2}, {1}, {0} });

	SimplePolynomial ref;
	size_t variable_index = 1;
	EXPECT_EQ(p1.differentiate(variable_index), ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, DIFFERENTIATE3) {
	SimplePolynomial p1({ 1, 1, 1 }, { {2}, {1}, {0} });
	SimplePolynomial p2({ 2,1 }, { {1},{0} });
	p1 += p2;

	SimplePolynomial ref({ 2,3 }, { {1},{0} });
	size_t variable_index = 0;
	EXPECT_EQ(p1.differentiate(variable_index), ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, DIFFERENTIATE4) {
	SimplePolynomial p1 = (X ^ 2) + X + 1;
	size_t variable_index = 0;
	p1.differentiate(variable_index);

	SimplePolynomial ref({ 2,1 }, { {1},{0} });	
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, DIFFERENTIATE5) {
	SimplePolynomial p1 = (X ^ 2) + X + 1;
	size_t variable_index = 1;
	p1.differentiate(variable_index);

	SimplePolynomial ref;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, DIFFERENTIATE6) {
	SimplePolynomial p1 = (X ^ 2) + X + 1;
	SimplePolynomial p2 = 2 * X + 1;
	p1 += p2;
	size_t variable_index = 0;
	p1.differentiate(variable_index);

	SimplePolynomial ref({ 2,3 }, { {1},{0} });	
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, DIFFERENTIATE7) {
	SimplePolynomial p1 = { {1,1,2},{X,Y,{0}} };
	constexpr size_t variable_index = 1;
	p1.differentiate(variable_index);

	SimplePolynomial ref = 1;
	EXPECT_EQ(p1, ref);
}


GTEST_TEST(SIMPLE_POLYNOMIAL, GRADIENT1) {
	SimplePolynomial p = { {1,1,1},{{0},{1},{0,1}} };
	auto result = p.gradient();

	VectorFunction<SimplePolynomial> ref = { { { 1 }, { {0} } }, { { 1 }, { {0} } } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, GRADIENT2) {
	SimplePolynomial p = { {1,1,1},{{0},{2},{1,1}} };
	auto result = p.gradient();

	VectorFunction<SimplePolynomial> ref = { { { 2,1 }, { {1},{0,1} } }, { { 1 }, { {1} } } };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, GRADIENT3) {
	SimplePolynomial p = { {1,1,1},{{0},{2},{1,1}} };
	const size_t domain_dimension = 3;
	auto result = p.gradient(domain_dimension);

	VectorFunction<SimplePolynomial> ref = { { { 2,1 }, { {1},{0,1} } }, { { 1 }, { {1} } },{ {0},{ {0} } } };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(SIMPLE_POLYNOMIAL, TO_STRING1) {
	const SimplePolynomial p = { {1,2,1},{{2},{1},{0}} };
	const auto result = p.to_string();

	const std::string ref = "[" + ms::double_to_string(1.0) + "(x0)^2 + " + ms::double_to_string(2.0) + "(x0)^1 + " + ms::double_to_string(1.0) + "(1)]";
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SIMPLE_POLYNOMIAL, TO_STRING2) {
	const SimplePolynomial p = (X ^ 2) + 2 * X + 1;
	const auto result = p.to_string();

	const std::string ref = "[" + ms::double_to_string(1.0) + "(x0)^2 + " + ms::double_to_string(2.0) + "(x0)^1 + " + ms::double_to_string(1.0) + "(1)]";
	EXPECT_EQ(result, ref);
}

//
//
//
//
//GTEST_TEST(SIMPLE_POLYNOMIAL, SCALAR_ADD_MONOMIAL1) {
//	const double scalar = 1;
//	const Monomial monomial;
//	const auto result = scalar + monomial;
//
//	SimplePolynomial ref = 2;
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(SIMPLE_POLYNOMIAL, MONOMIAL_SUBSTRACT_MONOMIAL1) {
//	const Monomial monomial1 = { 1 };
//	const Monomial monomial2 = { 1 };
//	const auto result = monomial1 - monomial2;
//
//	Polynomial ref;
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, COMPARE1) {
//	Polynomial p1(1);
//	Polynomial p2 = { {1,1},{{1},{0}} };
//	EXPECT_NE(p1, p2);
//}
//
//
//
//
//

//
//
//GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_ASSIGN1) {
//	Polynomial p1 = { {1,1},{{1},{0}} };
//	Polynomial p2 = { {1,1},{{1},{0}} };
//	p1 *= p2;
//
//	Polynomial ref = { {1,2,1},{{2},{1},{0}} };
//	EXPECT_EQ(p1, ref);
//}
//GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_ASSIGN2) {
//	Polynomial p1;
//	Polynomial p2;
//	p1 *= p2;
//	
//	Polynomial ref;
//	EXPECT_EQ(p1, ref);
//}
//GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_ASSIGN3) {
//	Polynomial p1(1);
//	Polynomial p2 = { {1,1},{{1},{0}} };
//	p1 *= p2;
//
//	Polynomial ref= { {1,1},{{1},{0}} };
//	EXPECT_EQ(p2, ref);
//}
//GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_ASSIGN4) {
//	Polynomial p1 = { {1,1},{{1},{0}} };
//	Polynomial p2;
//	p1 *= p2;
//
//	Polynomial ref;
//	EXPECT_EQ(p1, ref);
//}
//GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_ASSIGN5) {
//	Polynomial p1 = { {1,1,2},{X,Y,{0}} };
//	Polynomial p2 = { {1,1},{X,{0}} };
//
//	constexpr size_t variable_index = 1;
//	p1.differentiate(variable_index);
//	p1 *= p2;
//
//	Polynomial ref = { {1,1},{X,{0}} };
//	EXPECT_EQ(p1, ref);
//}
//GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_ASSIGN6) {
//	Polynomial p1 = { {1,1,2},{X,Y,{0}} };
//	Polynomial p2 = (X * Y) ^ 2;
//	p1 *= p2;	
//	EXPECT_TRUE(p1.is_simple_polynomial());
//}
//GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_ASSIGN7) {
//	Polynomial p1 = { {1,1,2},{X,Y,{0}} };
//	Polynomial p2 = X * (Y ^ 2);
//	p1 *= p2;
//
//	const Polynomial ref = (X ^ 2) * (Y ^ 2) + X * (Y ^ 3) + 2 * X * (Y ^ 2);
//	EXPECT_EQ(p1, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, POWER1) {
//	Polynomial p = { { 1,1 }, { {1},{0} } };
//	p.power(2);
//
//	Polynomial ref = { {1,2,1}, { {2},{1},{0} } };
//	EXPECT_EQ(p, ref);
//}
//GTEST_TEST(POLYNOMIAL, POWER2) {
//	Polynomial p = { { 1,1 }, { {0,1},{0} } };
//	p.power(2);
//
//	Polynomial ref = { {1,2,1}, { {0,2},{0,1},{0} } };
//	EXPECT_EQ(p, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, TO_STRING1) {
//	Polynomial p = { {1,2,1},{{2},{1},{0}} };
//
//	std::string ref = "[" + ms::double_to_string(1.0) + "(x0)^2 + " + ms::double_to_string(2.0) + "(x0)^1 + " + ms::double_to_string(1.0) + "(1)]";
//	EXPECT_EQ(p.to_string(), ref);
//}
//GTEST_TEST(POLYNOMIAL, TO_STRING2) {
//	Polynomial p = { {1,1},{{1},{0}} };
//	p *= p;
//
//	std::string ref = "[" + ms::double_to_string(1.0) + "(x0)^1 + 1(1)]^(2)";
//	EXPECT_EQ(p.to_string(), ref);
//}
//GTEST_TEST(POLYNOMIAL, TO_STRING3) {
//	Polynomial p1 = { {1,1},{{1},{0}} };
//	Polynomial p2 = { {1,1},{{1},{0}} };
//	p1.power(2);
//	
//	std::string ref = p2.to_string() + "^(2)";
//	EXPECT_EQ(p1.to_string(), ref);
//}
//GTEST_TEST(POLYNOMIAL, TO_STRING4) {
//	Polynomial p1 = { {1,1},{{1},{0}} };
//	Polynomial p2 = { {1,1},{{1},{0}} };
//	p1.power(2);
//	p2 *= p2;
//
//	EXPECT_EQ(p1.to_string(), p2.to_string());
//}
//GTEST_TEST(POLYNOMIAL, TO_STRING5) {
//	Polynomial p1 = { {1,2,1},{ {2},{1},{0} } };
//	Polynomial p2 = { {2,1,1},{ {1},{2},{0} } };
//	
//	EXPECT_EQ(p1.to_string(), p2.to_string());
//}
//GTEST_TEST(POLYNOMIAL, TO_STRING6) {
//	Polynomial p({ 1,1 }, { {1},{0} });
//	p.power(2);
//	size_t variable_index = 1;
//	p.differentiate(variable_index);
//
//	std::string ref = Polynomial().to_string();
//	EXPECT_EQ(p.to_string(), ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL1) {
//	Polynomial p = { {1,2,1},{{2},{1},{0}} };
//
//	const MathVector v;
//	EXPECT_ANY_THROW(p(v));
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL2) {
//	Polynomial p = { {1,2,1},{{2},{1},{0}} };
//	
//	const MathVector v0 = { 0 };
//	const double ref0 = 1;
//	EXPECT_EQ(p(v0), ref0);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL3) {
//	Polynomial p = { {1,2,1},{{2},{1},{0}} };
//
//	const MathVector v1 = { 1 };
//	const double ref1 = 4;
//	EXPECT_EQ(p(v1), ref1);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL4) {
//	Polynomial p = { {1,2,1},{{2},{1},{0}} };
//
//	const MathVector v2 = { 1.87945646187 };
//	const double ref2 = 8.291269515804899;
//	EXPECT_DOUBLE_EQ(p(v2), ref2);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL5) {
//	Polynomial p = { {1,-2.0e3,1.0e6},{{2},{1},{0}} };
//
//	const MathVector v3 = { 1000.1 };
//	//const double ref = 0.01;
//	const double ref = 0.01000000000931323;	// too large round off error
//	EXPECT_DOUBLE_EQ(p(v3), ref);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL6) {
//	Polynomial p = { { 1,-1.0e3 }, { {1},{0} } };
//	p.power(2);
//	const MathVector v = { 1000.1 };
//	//const double ref = 0.01;
//	const double ref = 0.01000000000000455;	// still too large round off error
//	EXPECT_DOUBLE_EQ(p(v), ref);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL7) {
//	Polynomial p1 = { {1}, {{1}} };
//	Polynomial p2(-1.0e3);
//
//	p1 += p2;
//	p1.power(2);
//
//	const MathVector v = { 1000.1 };
//	const double ref = 0.01000000000000455;	// still too large round off error
//	EXPECT_DOUBLE_EQ(p1(v), ref);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL8) {
//	Polynomial p1 = { { 1, 1, 1 }, { {2}, {1}, {0} } };
//	Polynomial p2 = { { 1, 1 }, { { 1 }, { 0 } } };
//	p1 *= p2;
//
//	const MathVector v = { -1 };
//	const double ref = 0.0;
//	EXPECT_DOUBLE_EQ(p1(v), ref);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL9) {
//	Polynomial p1 = { { 1, 1 }, { {1}, {0} } };
//	p1.power(10);
//
//	const MathVector v = { -1 };
//	const double ref = 0.0;
//	EXPECT_DOUBLE_EQ(p1(v), ref);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL10) {
//	Polynomial p1 = { { 1, 1 }, { {1}, {0} } };
//	p1.power(10);
//
//	const MathVector v = { 0 };
//	const double ref = 1.0;
//	EXPECT_DOUBLE_EQ(p1(v), ref);
//}
//GTEST_TEST(POLYNOMIAL, FUNCTION_CALL11) {
//	Polynomial p1 = { { 1, 1 }, { {1}, {0} } };
//	p1.power(10);
//
//	const MathVector v = { 1 };
//	const double ref = 1024;
//	EXPECT_DOUBLE_EQ(p1(v), ref);
//}
//
//GTEST_TEST(POLYNOMIAL, DOMAIN_ORDER1) {
//	Polynomial p1({ 1,1 }, { {1},{0} });
//	Polynomial p2({ 1,1 }, { {0,1},{0} });
//	auto p3 = (p1 + p2) * p1;
//
//	size_t variable_index = 1;
//	p3.differentiate(variable_index);
//	const auto result = p3.domain_dimension();
//
//	const size_t ref = 1;
//	EXPECT_EQ(result, ref);
//}
//
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE1) {
//	Polynomial p1({ 1, 1, 1 }, { {2}, {1}, {0} });
//
//	Polynomial ref({ 2,1 }, { {1},{0} });
//	size_t variable_index = 0;
//	EXPECT_EQ(p1.differentiate(variable_index), ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE2) {
//	Polynomial p1({ 1, 1, 1 }, { {2}, {1}, {0} });
//
//	Polynomial ref;
//	size_t variable_index = 1;
//	EXPECT_EQ(p1.differentiate(variable_index), ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE3) {
//	Polynomial p1({ 1, 1, 1 }, { {2}, {1}, {0} });
//	Polynomial p2({ 2,1 }, { {1},{0} });
//	p1 += p2;
//
//	Polynomial ref({ 2,3 }, { {1},{0} });
//	size_t variable_index = 0;
//	EXPECT_EQ(p1.differentiate(variable_index), ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE4) {
//	Polynomial p({ 1,1 }, { {1},{0} });
//	p *= p;
//
//	size_t variable_index = 0;
//	auto result = p.differentiate(variable_index);
//
//	Polynomial ref({ 2,2 }, { {1},{0} });	
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE5) {
//	Polynomial p1({ 1,1 }, { {1},{0} });
//	Polynomial p2({ 1,1 }, { {0,1},{0} });	
//	auto result = (p1 + p2) * p1;
//
//	size_t variable_index = 0;
//	result.differentiate(variable_index);
//
//	Polynomial ref({ 2,1,3 }, { {1},{0,1},{0} });	
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE6) {
//	Polynomial p1({ 1,1 }, { {1},{0} });
//	Polynomial p2({ 1,1 }, { {0,1},{0} });
//	auto result = (p1 + p2) * p1;
//
//	size_t variable_index = 1;
//	result.differentiate(variable_index);
//
//	Polynomial ref({ 1,1 }, { {1},{0} });
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE7) {
//	Polynomial p1({ 1,1 }, { {1},{0} });
//	Polynomial p2({ 1,1 }, { {0,1},{0} });
//	auto result = (p1 * p2) + p1;
//
//	size_t variable_index = 0;
//	result.differentiate(variable_index);
//
//	Polynomial ref({ 1,2 }, { {0,1},{0} });
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE8) {
//	Polynomial p1({ 1,1 }, { {1},{0} }); 
//	Polynomial p2({ 1,1 }, { {0,1},{0} });
//	auto result = (p1 * p2) + p1;
//
//	size_t variable_index = 1;
//	result.differentiate(variable_index);
//
//	Polynomial ref({ 1,1 }, { {1},{0} });	
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE9) {
//	Polynomial p({ 1,1 }, { {1},{0} });
//	p.power(2);
//
//	Polynomial ref({ 2,2 }, { {1},{0} });
//	size_t variable_index = 0;
//	EXPECT_EQ(p.differentiate(variable_index), ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE10) {
//	Polynomial p({ 1,1 }, { {1},{0} });
//	p.power(2);
//
//	Polynomial ref;
//	size_t variable_index = 1;
//	EXPECT_EQ(p.differentiate(variable_index), ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE11) {
//	Polynomial p1({ 1,1 }, { {1},{0} });
//	Polynomial p2({ 1,1 }, { {0,1},{0} });
//	auto result = p1 * p2;
//
//	size_t variable_index = 1;
//	result.differentiate(variable_index);
//
//	Polynomial ref({ 1,1 }, { {1},{0} });
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE12) {
//	Polynomial p1({ 1,1 }, { {1},{0} });
//	Polynomial p2({ 1,1 }, { {0,1},{0} });
//	auto result = p1 * p2;
//
//	size_t variable_index = 2;
//	result.differentiate(variable_index);
//
//	Polynomial ref;
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, DIFFERENTIATE13) {
//	Polynomial p1 = { {1,1,2},{X,Y,{0}} };
//	constexpr size_t variable_index = 1;
//	const auto result = p1.differentiate(variable_index);
//
//	Polynomial ref = 1;
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, EXTEND1) {
//	Polynomial p1 = { {1,1,2},{X,Y,{0}} };
//	Polynomial p2 = { {1,1},{X,{0}} };
//
//	constexpr size_t variable_index = 1;
//	p1.differentiate(variable_index);
//	p1 *= p2;
//	const auto result = p1.extend();
//
//	Polynomial ref = { {1,1},{X,{0}} };
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, EXTEND2) {
//	Polynomial p1({ 1,1 }, { {1},{0} });
//	Polynomial p2({ 1,1 }, { {0,1},{0} });
//	auto p3 = (p1 + p2) * p1;
//
//	size_t variable_index = 1;
//	p3.differentiate(variable_index);
//	const auto result = p3.extend();
//
//	Polynomial ref = { { 1,1 }, { {1},{0} } };
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, EXTEND3) {
//	Polynomial p2 = { {1,1},{X,{0}} };
//	const auto result = p2.extend();
//
//	Polynomial ref = { {1,1},{X,{0}} };
//	EXPECT_EQ(p2, ref);
//}
//GTEST_TEST(POLYNOMIAL, EXTEND4) {
//	Polynomial p1 = -12 * (X ^ 2) * Y + 45;
//	Polynomial p2 = 12 * (X ^ 2) * Y - 45;
//	p1 += p2;
//	const auto result = p1.extend();
//
//	Polynomial ref;
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, MULTIPLICATION) {
//	Polynomial p1 = 1;
//	Polynomial p2 = { {1,1},{X,{0}} };
//	p1.multiply(p2);
//
//	Polynomial ref = { {1,1},{X,{0}} };
//	EXPECT_EQ(p1, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, GRADIENT1) {
//	Polynomial p = { {1,1,1},{{0},{1},{0,1}} };
//	auto result = p.gradient();
//
//	VectorFunction<Polynomial> ref = { { { 1 }, { {0} } }, { { 1 }, { {0} } } };
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, GRADIENT2) {
//	Polynomial p = { {1,1,1},{{0},{2},{1,1}} };
//	auto result = p.gradient();
//
//	VectorFunction<Polynomial> ref = { { { 2,1 }, { {1},{0,1} } }, { { 1 }, { {1} } } };
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, GRADIENT3) {
//	Polynomial p = { {1,1,1},{{0},{2},{1,1}} };
//	const size_t domain_dimension = 3;
//	auto result = p.gradient(domain_dimension);
//
//	VectorFunction<Polynomial> ref = { { { 2,1 }, { {1},{0,1} } }, { { 1 }, { {1} } },{ {0},{ {0} } } };
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, SCALAR_PLUS_MONOMIAL1) {
//	const double scalar = 3;
//	Monomial m = { 3 };
//	const auto result = scalar + m;
//
//	const Polynomial ref = { {1,scalar},{m,{0}} };
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, SCALAR_MULTIPLY_MONOMIAL1) {
//	const double scalar = 3;
//	Monomial m = { 3 };
//	const auto result = scalar * m;
//
//	const Polynomial ref(scalar,m);
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, MONOMIAL_PLUS_MONOMIAL) {
//	Monomial m1 = { 0,0,1 };
//	Monomial m2 = { 1 };
//	const auto result = m1 + m2;
//
//	const Polynomial ref = { { 1,1 }, { m1,m2 } };
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(POLYNOMIAL, MONOMIAL_PLUS_POLYNOMIAL) {
//	Monomial m = { 1 };
//	Polynomial p = X + 1;
//	const auto result = m + p;
//
//	const Polynomial ref = { {2,1}, {{1},{0}} };
//	EXPECT_EQ(result, ref);
//}
//









//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR1_1) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator1(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR1_2) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator1(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR1_3) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator1(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR1_4) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator1(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR1_5) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator1(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR1_1_many_zero) {
//	Monomial m1{ 1,0,0,0,5,0,0,3,4,0,1,0,0,0,5 };
//	MathVector v1 = { 0,2,3,4,0,1,2,0,4,0,1,2,3,4,0 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator1(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR1_2_many_zero) {
//	Monomial m1{ 1,0,0,0,5,0,0,3,4,0,1,0,0,0,5 };
//	MathVector v1 = { 0,2,3,4,0,1,2,0,4,0,1,2,3,4,0 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator1(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR1_3_many_zero) {
//	Monomial m1{ 1,0,0,0,5,0,0,3,4,0,1,0,0,0,5 };
//	MathVector v1 = { 0,2,3,4,0,1,2,0,4,0,1,2,3,4,0 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator1(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR2_1) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator2(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR2_2) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator2(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR2_3) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator2(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR2_4) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator2(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR2_5) {
//	Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	MathVector v1 = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator2(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR2_1_many_zero) {
//	Monomial m1{ 1,0,0,0,5,0,0,3,4,0,1,0,0,0,5 };
//	MathVector v1 = { 0,2,3,4,0,1,2,0,4,0,1,2,3,4,0 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator2(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR2_2_many_zero) {
//	Monomial m1{ 1,0,0,0,5,0,0,3,4,0,1,0,0,0,5 };
//	MathVector v1 = { 0,2,3,4,0,1,2,0,4,0,1,2,3,4,0 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator2(v1);
//}
//
//GTEST_TEST(MONOMIAL, PERFORMANCE_CALL_OPERATOR2_3_many_zero) {
//	Monomial m1{ 1,0,0,0,5,0,0,3,4,0,1,0,0,0,5 };
//	MathVector v1 = { 0,2,3,4,0,1,2,0,4,0,1,2,3,4,0 };
//	double result = 0;
//	size_t num_iter = 1000000000;
//	for (size_t i = 0; i < num_iter; ++i)
//		result += m1.call_operator2(v1);
//}