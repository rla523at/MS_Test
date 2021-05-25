#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Polynomial.h"

GTEST_TEST(MONOMIAL, CONSTRUCTOR1) {
	Monomial m1;
	Monomial m2 = { 0 };
	EXPECT_EQ(m1, m2);
}
GTEST_TEST(MONOMIAL, CONSTRUCTOR2) {
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
//GTEST_TEST(MONOMIAL, CONSTRUCTOR7) {
//	Monomial m1 = X;
//	Monomial m2 = { 1 };
//	EXPECT_EQ(m1, m2);
//}
//GTEST_TEST(MONOMIAL, CONSTRUCTOR8) {
//	Monomial m1 = Y;
//	Monomial m2 = { 0,1 };
//	EXPECT_EQ(m1, m2);
//}
//GTEST_TEST(MONOMIAL, CONSTRUCTOR9) {
//	Monomial m1 = Z;
//	Monomial m2 = { 0,0,1 };
//	EXPECT_EQ(m1, m2);
//}


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
	const auto result = m.is_one();

	const bool ref = false;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, IS_CONSTANT2) {
	Monomial m = { 0,0,1 };
	const auto result = m.is_one();

	const bool ref = false;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, IS_CONSTANT3) {
	Monomial m = { 0,0,0 };
	const auto result = m.is_one();

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
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MONOMIAL, MONOMIAL_ORDER1) {
	Monomial m;
	const auto result = m.order();

	constexpr size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER2) {
	Monomial m(0);
	const auto result = m.order();

	constexpr size_t ref = 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER3) {
	Monomial m = { 0 };
	const auto result = m.order();

	constexpr size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER4) {
	Monomial m = { 0,0,0,0,0,0,0,0,0 };
	const auto result = m.order();

	constexpr size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER5) {
	Monomial m(128);
	const auto result = m.order();

	constexpr size_t ref = 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, MONOMIAL_ORDER6) {
	Monomial m = { 1,2,3,4,5 };
	const auto result = m.order();

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


GTEST_TEST(MONOMIAL, OPERATOR_MULTIPLICATION1) {
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
	const auto result = m ^ 3;

	Monomial ref = { 3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_POWER2) {
	Monomial m(3);
	const auto result = m ^ 7;

	Monomial ref = { 0,0,0,7 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_POWER3) {
	Monomial m = { 1,2,3 };
	const auto result = m ^ 2;

	Monomial ref = { 2,4,6 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MONOMIAL, OPERATOR_FUNCTION_CALL_1) {
	const Monomial m;
	const MathVector v1 = { 1,2,3 };

	const double ref = 1.0;
	EXPECT_EQ(m(v1), ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_FUNCTION_CALL_2) {
	const Monomial m;
	const MathVector v4;

	const double ref = 1.0;
	EXPECT_EQ(m(v4), ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_FUNCTION_CALL_3) {
	const Monomial m;

	const double ref = 1.0;
	EXPECT_EQ(m(), ref);
}
GTEST_TEST(MONOMIAL, OPERATOR_FUNCTION_CALL_4) {
	const Monomial m = { 1,2,3 };
	const MathVector v1;
	EXPECT_ANY_THROW(m(v1));
}
GTEST_TEST(MONOMIAL, OPERATOR_FUNCTION_CALL_5) {
	const Monomial m = { 1,2,3 };
	const MathVector v2 = { 1,2,3 };

	const double ref2 = 108;
	EXPECT_EQ(m(v2), ref2);
}
GTEST_TEST(MONOMIAL, OPERATOR_FUNCTION_CALL_6) {
	const Monomial m = { 1,2,3 };
	const MathVector v3 = { 1.84,2.789,3.487946 };

	const double ref3 = 6.073291260986822e+02;
	EXPECT_DOUBLE_EQ(m(v3), ref3);
}


GTEST_TEST(MONOMIAL, TO_STRING1) {
	Monomial m = { 1,0,3,0,5 };
	const auto result = m.to_string();

	const std::string ref = "(x0)(x2)^3(x4)^5";
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, TO_STRING2) {
	Monomial m(128);
	const auto result = m.to_string();

	const std::string ref = "(x128)";
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, TO_STRING3) {
	Monomial m = { 1,2,3,4 };
	const auto result = m.to_string();

	const std::string ref = "(x0)(x1)^2(x2)^3(x3)^4";
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MONOMIAL, TO_STRING4) {
	Monomial m;
	const auto result = m.to_string();

	const std::string ref = "(1)";
	EXPECT_EQ(result, ref);
}


//GTEST_TEST(TERM, OPERATOR_MULTIPLCATION_ASSIGN1) {
//	PolyTerm t1 = { { 1,1 }, { X,{0} } }; // x+1
//	PolyTerm t2 = { { 1,-1 }, { X,{0} } }; // x-1
//	t1 *= t2;
//
//	PolyTerm ref = { {1,-1}, {{X ^ 2},{0}} };
//	EXPECT_EQ(t1, ref);
//}
//GTEST_TEST(TERM, OPERATOR_MULTIPLCATION_ASSIGN2) {
//	PolyTerm t1 = { { 1,1 }, { X,{0} } }; // x+1
//	PolyTerm t2 = { { 1,-1 }, { X,{0} } }; // x-1
//	t1 *= t2;
//	t1 *= t1;
//
//	PolyTerm ref = { {1,-2,1}, {{X ^ 4}, {X ^ 2},{0}} };
//	EXPECT_EQ(t1, ref);
//}
//GTEST_TEST(TERM, OPERATOR_MULTIPLCATION_ASSIGN3) {
//	PolyTerm t1 = { { 1,1 }, { X,{0} } }; // x+1
//	PolyTerm t2 = { { 1,-1 }, { X,{0} } }; // x-1
//	t1 = t1 ^ 2;
//	t2 = t2 ^ 2;
//	t1 *= t2;
//
//	PolyTerm ref = { {1,-2,1}, {{X ^ 4}, {X ^ 2},{0}} };
//	EXPECT_EQ(t1, ref);
//}
//GTEST_TEST(TERM, OPERATOR_MULTIPLCATION_ASSIGN4) {
//	PolyTerm t1 = 3;
//	PolyTerm t2 = 2;
//	t1 *= t2;
//
//	PolyTerm ref = 6;
//	EXPECT_EQ(t1, ref);
//}
//GTEST_TEST(TERM, OPERATOR_MULTIPLCATION_ASSIGN5) {
//	PolyTerm t1 = 3;
//	PolyTerm t2 = { { 1,-1 }, { X,{0} } }; // x-1
//	t1 *= t2;
//
//	PolyTerm ref = { { 3,-3 }, { X,{0} } };
//	EXPECT_EQ(t1, ref);
//}
//
//
//GTEST_TEST(TERM, OPERATOR_POWER1) {
//	PolyTerm t1 = { { 1,1 }, { X,{0} } }; // x+1
//	PolyTerm t2 = { { 1,-1 }, { X,{0} } }; // x-1
//	t1 *= t2;
//	auto result = t1 ^ 2;
//
//	PolyTerm ref = { {1,-2,1}, {{X ^ 4}, {X ^ 2},{0}} };
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(TERM, SIMPLE_TERM_ADDITION1) {
//	PolyTerm t1;
//	PolyTerm t2 = { { 1,1 }, { X,{0} } }; // x+1
//	t1.simple_term_addition(t2);
//
//	PolyTerm ref = { { 1,1 }, { X,{0} } };
//	EXPECT_EQ(t1, ref);
//}
//GTEST_TEST(TERM, SIMPLE_TERM_ADDITION2) {
//	PolyTerm t1 = { { 1,1 }, { X,{0} } }; // x+1
//	PolyTerm t2;
//	t1.simple_term_addition(t2);
//
//	PolyTerm ref = { { 1,1 }, { X,{0} } };
//	EXPECT_EQ(t1, ref);
//}
//GTEST_TEST(TERM, SIMPLE_TERM_ADDITION3) {
//	PolyTerm t1 = { { 1,1 }, { X,{0} } }; // x+1
//	PolyTerm t2 = 3;
//	t1.simple_term_addition(t2);
//
//	PolyTerm ref = { { 1,4 }, { X,{0} } };
//	EXPECT_EQ(t1, ref);
//}
//
//
//GTEST_TEST(TERM, SIMPLE_TERM_DIFFERENTIATE_1) {
//	PolyTerm t1 = { { 1,1 }, { X,{0} } }; // x+1
//	
//	const size_t variable_index = 0;
//	const auto result = t1.simple_term_differentiate(variable_index);
//
//	PolyTerm ref = 1;
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(TERM, SIMPLE_TERM_DIFFERENTIATE_2) {
//	PolyTerm t1 = { { 1,1 }, { X,{0} } }; // x+1
//	PolyTerm t2 = { { 1,-1 }, { X,{0} } }; // x-1
//	auto t3 = t1 * t2;
//
//	const size_t variable_index = 0;
//	t3.multiplied_power_poly_term_set_.front().simple_term_differentiate(variable_index);
//
//	PolyTerm ref = { { 1,-1 }, { X,{0} } };
//	EXPECT_EQ(t3, ref);
//}


GTEST_TEST(POLYNOMIAL, TEMP) {
	//Polynomial p1 = X + 1;
	//Polynomial p2 = X - 1;
	//auto p3 = 3 * p1;
	//auto p4 = p3 * p2;

	Polynomial p1 = X * Y;
	std::cout << p1 << "\n";

	auto p2 = p1 - X;
	std::cout << p2 << "\n";

	auto p3 = p2 - 3 * Y;
	std::cout << p3 << "\n";

	auto p4 = p3 + Y * Z;
	std::cout << p4 << "\n";

	auto p5 = p4 + Z + X + 5;
	std::cout << p5 << "\n";
}


GTEST_TEST(POLYNOMIAL, OPERATOR_ADDITION_1) {
	Polynomial p1 = X;
	Polynomial p2 = X + Y + 1;
	const auto result = p1 + p2;

	Polynomial ref = 2 * X + Y + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_ADDITION_2) {
	Polynomial p1 = (X^2) + X;
	Polynomial p2 = X + Y + 1;
	const auto result = p1 + p2;

	Polynomial ref = (X^2) + 2 * X + Y + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_ADDITION_3) {
	Polynomial p1;
	Polynomial p2;
	const auto result = p1 + p2;

	Polynomial ref;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_ADDITION_4) {
	Polynomial p1 = -12 * (X ^ 2) * Y + 45 * X;
	Polynomial p2 = 12 * (X ^ 2) * Y - 45 * X;
	const auto result = p1 + p2;

	Polynomial ref;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_ADDITION_5) {
	Polynomial p1 = (X ^ 2) + X;
	Polynomial p2 = X + 1;
	const auto result = p1 + p2;

	Polynomial ref = (X ^ 2) + 2 * X + 1;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_1) {
	Polynomial p1 = X + Y;
	Polynomial p2 = X + 3 * Y;
	const auto result = p1 - p2;

	Polynomial ref = -2 * Y;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_2) {
	Polynomial p1 = X * Y;
	Polynomial p2 = X + 3 * Y;
	const auto result = p1 - p2;

	Polynomial ref = X * Y - X - 3 * Y;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_3) {
	Polynomial p1 = X * Y;
	Polynomial p2 = X;
	p1 -= p2;

	Polynomial ref = X * Y - X;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_4) {
	Polynomial p1;
	Polynomial p2 = X;
	p1 -= p2;

	Polynomial ref = -1 * X;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_5) {
	Polynomial p1;
	Polynomial p2 = 3 * (X * Y) - 3 * ((X ^ 2) * Z);
	p1 -= p2;

	Polynomial ref = -3 * (X * Y) + 3 * ((X ^ 2) * Z);
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_6) {
	Polynomial p = (X - 1) * (Y - 1);
	p -= 1;

	const auto ref = X * Y - X - Y;
	EXPECT_EQ(p, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_7) {
	Polynomial p = (X - 1) * (Y - 1);
	p -= X - 1;

	const auto ref = X * Y - 2 * X - Y + 2;
	EXPECT_EQ(p, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_8) {
	Polynomial p = (X - 1) * (Y - 1);
	p -= X + Y - 1;

	const auto ref = X * Y - 2 * X - 2 * Y + 2;
	EXPECT_EQ(p, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_SUBSTRACTION_9) {
	Polynomial p = (X - 1) * (Y - 1);
	p -= 2 * ((X - 1) ^ 2) + X + Y - 1;

	const auto ref = -2 * (X ^ 2) + X * Y + 2 * X - 2 * Y;
	EXPECT_EQ(p, ref);
}



GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_1) {
	Polynomial p1 = X;
	Polynomial p2 = X + Y + 1;
	const auto result = p1 * p2;

	Polynomial ref = (X^2) + X*Y + X;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_2) {
	Polynomial p1 = X + Y + 1;
	Polynomial p2 = X;
	const auto result = p1 * p2;

	Polynomial ref = (X ^ 2) + X * Y + X;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_3) {
	Polynomial p1 = X + Y;
	Polynomial p2 = Y + X;
	const auto result = p1 * p2;

	Polynomial ref = (X ^ 2) + 2 * X * Y + (Y ^ 2);
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_4) {
	Polynomial p1 = X + Y;
	Polynomial p2 = Y + X;
	Polynomial p3 = X;
	const auto result = (p1 * p3) * (p2 * p3);

	Polynomial ref = (X ^ 4) + 2 * (X^3) * Y + ((X*Y) ^ 2);
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_5) {
	Polynomial p1 = X + Y;
	const auto result = -1 * p1;

	Polynomial ref = -1 * X - 1 * Y;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_6) {
	Polynomial p1 = X + 1;
	Polynomial p2 = X + 1;
	const auto result = p1 * p2;

	Polynomial ref = (X ^ 2) + 2 * X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_7) {
	Polynomial p1;
	Polynomial p2;
	const auto result = p1 * p2;

	Polynomial ref;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_8) {
	Polynomial p1 = 1;
	Polynomial p2 = X + 1;
	const auto result = p1 * p2;

	Polynomial ref = X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_9) {
	Polynomial p1 = X + 1;
	Polynomial p2;
	const auto result = p1 * p2;

	Polynomial ref;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_10) {
	Polynomial p1 = X + 1;
	const auto p2 = p1^(0.5);
	const auto result = p2 * p2;

	Polynomial ref = X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_11) {
	Polynomial p1 = (X ^ 2) + X + 1;
	const auto result = p1 * 5;

	Polynomial ref = 5 * (X ^ 2) + 5 * X + 5;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(POLYNOMIAL, OPERATOR_POWER_1) {
	Polynomial p1 = X + 1;
	const auto result = p1 ^ 2;

	Polynomial ref = (X ^ 2) + 2 * X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_POWER_2) {
	Polynomial p = 1.5;
	Polynomial result = p ^ 2;

	Polynomial ref = 2.25;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_POWER_3) {
	Polynomial p;
	Polynomial result = p ^ 2;

	Polynomial ref;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, OPERATOR_POWER_4) {
	Polynomial p1 = (X ^ 2) + 2 * X + 1;
	const auto p2 = p1 ^ 2;
	const auto result = ms::sqrt(p2);

	Polynomial ref = (X ^ 2) + 2 * X + 1;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(POLYNOMIAL, COMPLEX_OPERATION_1) {
	const auto p1 = X + 1;
	const auto p2 = X + 2;
	const auto p3 = X + 3;
	const auto p4 = p1 * p2 + p3;
	const auto p5 = p4 ^ 2;
	const auto result = 2 * (p5 + p1);

	Polynomial ref = 2 * (X ^ 4) + 16 * (X ^ 3) + 52 * (X ^ 2) + 82 * X + 52;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, COMPLEX_OPERATION_2) {
	Polynomial p1 = (X ^ 2) + X + 1;
	const auto p2 = p1 ^ 0.7;
	const auto p3 = 5 * p2;

	for (size_t i = 0; i < 10; ++i) {
		const double val = 0.31 * i;
		const MathVector val_vec = { val };
		const double result = p3(val_vec);
		const double ref = 5 * (std::pow(val * val + val + 1, 0.7));
		EXPECT_DOUBLE_EQ(result, ref);
	}
}
GTEST_TEST(POLYNOMIAL, COMPLEX_OPERATION_3) {
	Polynomial p1 = X + 1;
	const auto p2 = p1 ^ 2;
	const auto p3 = p2 + X + 2;
	const auto p4 = p3 + 2 * X + 3;
	const auto result = 5 * p4;

	const auto ref = 5 * (X ^ 2) + 25 * X + 30;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, COMPLEX_OPERATION_4) {
	Polynomial p1 = ms::sqrt(X + 1) * X + X + 1;
	Polynomial p2 = ms::sqrt(X + 2) * X;
	const auto result = p1 * p2;

	for (size_t i = 0; i < 10; ++i) {
		const double val = 0.31 * i;
		const MathVector v = { val };

		const double ref = (std::sqrt(val + 1) * val + val + 1) * (std::sqrt(val + 2) * val);
		EXPECT_DOUBLE_EQ(result(v), ref);
	}
}


//GTEST_TEST(POLYNOMIAL, POLY_TERM_DIFFERENTIATE_1) {
//	PolyTerm pt1 = { {1,1}, {X,{0}} };
//	PolyTerm pt2 = { {1,-1}, {X,{0}} };
//	const auto pt3 = pt1 * pt2;
//	
//	const size_t variable_index = 0;
//	const auto result = Polynomial::poly_term_differentiate(pt3, variable_index);
//
//	const Polynomial ref = 2*X;
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, POLY_TERM_DIFFERENTIATE_2) {
//	PolyTerm pt1 = { {1,1}, {X,{0}} };
//	auto pt2 = pt1 ^ 2;
//
//	const size_t variable_index = 0;
//	const auto result = Polynomial::poly_term_differentiate(pt2, variable_index);
//
//	const Polynomial ref = 2 * X + 2;
//	EXPECT_EQ(result, ref);
//}


GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_1) {
	Polynomial p1 = (X ^ 2) + X + 1;
	
	size_t variable_index = 0;
	p1.differentiate(variable_index);

	Polynomial ref = 2 * X + 1;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_2) {
	Polynomial p1 = (X ^ 2) + X + 1;

	size_t variable_index = 1;
	p1.differentiate(variable_index);	

	Polynomial ref;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_3) {
	Polynomial p1 = (X ^ 2) + X + 1;
	Polynomial p2 = 2 * X + 1;
	auto p3 = p1 + p2;

	size_t variable_index = 0;
	p3.differentiate(variable_index);

	Polynomial ref = 2 * X + 3;
	EXPECT_EQ(p3, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_4) {
	Polynomial p1 = X + 1;
	auto p2 = p1 ^ 2;

	size_t variable_index = 0;
	p2.differentiate(variable_index);

	Polynomial ref = 2 * X + 2;
	EXPECT_EQ(p2, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_5) {
	Polynomial p1 = X + 1;
	Polynomial p2 = Y + 1;
	auto result = (p1 + p2) * p1;

	size_t variable_index = 0;
	result.differentiate(variable_index);

	Polynomial ref = 2 * X + Y + 3;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_6) {
	Polynomial p1 = X + 1;
	Polynomial p2 = Y + 1;
	auto result = (p1 + p2) * p1;

	size_t variable_index = 1;
	result.differentiate(variable_index);

	Polynomial ref = X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_7) {
	Polynomial p1 = X + 1;
	Polynomial p2 = Y + 1;
	auto result = (p1 * p2) + p1;

	size_t variable_index = 0;
	result.differentiate(variable_index);

	Polynomial ref = Y + 2;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_8) {
	Polynomial p1 = X + 1;
	Polynomial p2 = Y + 1;
	auto result = (p1 * p2) + p1;

	size_t variable_index = 1;
	result.differentiate(variable_index);

	Polynomial ref = X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_9) {
	Polynomial p1 = (X ^ 2) + X + 1;
	size_t variable_index = 0;
	p1.differentiate(variable_index);

	Polynomial ref = 2 * X + 1;
	EXPECT_EQ(p1, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_10) {
	const auto p1 = X + 1;
	const auto p2 = X + 2;
	auto p3 = p1 * p1 * p2;

	constexpr size_t variable_index = 0;
	p3.differentiate(variable_index);

	Polynomial ref = 3 * (X ^ 2) + 8 * X + 5;
	EXPECT_EQ(p3, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_11) {
	auto p = (2 * X + 3) ^ 2;
	constexpr size_t variable_index = 0;
	p.differentiate(variable_index);

	Polynomial ref = 8 * X + 12;
	EXPECT_EQ(p, ref);
}
GTEST_TEST(POLYNOMIAL, DIFFERENTIATE_12) {
	auto p = ((2 * X + 3) ^ 2) * X;
	constexpr size_t variable_index = 0;
	p.differentiate(variable_index);

	Polynomial ref = 12 * (X ^ 2) + 24 * X + 9;
	EXPECT_EQ(p, ref);
}


GTEST_TEST(POLYNOMIAL, GRADIENT_1) {
	Polynomial p = X + Y + 1;
	auto result = p.gradient();

	VectorFunction<Polynomial> ref = { 1, 1 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, GRADIENT2) {
	Polynomial p = (X ^ 2) + X * Y + 1;
	auto result = p.gradient();

	VectorFunction<Polynomial> ref = { 2*X+Y, X };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, GRADIENT3) {
	Polynomial p = (X ^ 2) + X * Y + 1;
	const size_t domain_dimension = 3;
	auto result = p.gradient(domain_dimension);

	VectorFunction<Polynomial> ref = { 2 * X + Y, X, 0 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(POLYNOMIAL, GRADIENT4) {
	const Polynomial p = X*Y*Z;
	const auto result = p.gradient();

	const VectorFunction<Polynomial> ref = { Y * Z,  X * Z, X * Y };
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


GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE1) {
	Polynomial p1 = X + 1;
	Polynomial p2 = X + Y + Z - 3;
	Polynomial p3 = Y + Z;
	VectorFunction<Polynomial> f = { p1,p2,p3 };

	constexpr size_t variable_index = 0;
	f.differentiate(variable_index);

	VectorFunction<Polynomial> ref = { 1,1,0 };
	EXPECT_EQ(f, ref);
}
GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE2) {
	Polynomial p1 = X * Y + 1;
	Polynomial p2 = X + Y * Z + Z - 3;
	Polynomial p3 = Y + Z;
	VectorFunction<Polynomial> f = { p1,p2,p3 };

	constexpr size_t variable_index = 0;
	f.differentiate(variable_index);

	VectorFunction<Polynomial> ref = { Y,1,0 };
	EXPECT_EQ(f, ref);
}
GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE3) {
	Polynomial p1 = X * Y + 1;
	Polynomial p2 = X + Y * Z + Z - 3;
	Polynomial p3 = Y + Z;
	VectorFunction<Polynomial> f = { p1,p2,p3 };

	constexpr size_t variable_index = 1;
	f.differentiate(variable_index);

	VectorFunction<Polynomial> ref = { X,Z,1 };
	EXPECT_EQ(f, ref);
}
GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE4) {
	Polynomial p1 = 1.5 * X + 0.5 * Y + 3;
	Polynomial p2 = Y + 3;
	Polynomial p3 = 0;
	VectorFunction<Polynomial> f = { p1,p2,p3 };

	constexpr size_t variable_index = 0;
	f.differentiate(variable_index);

	VectorFunction<Polynomial> ref = { 1.5,0,0 };
	EXPECT_EQ(f, ref);
}
GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE5) {
	Polynomial p1 = 1.5 * X + 0.5 * Y + 3;
	Polynomial p2 = Y + 3;
	Polynomial p3 = 0;
	VectorFunction<Polynomial> f = { p1,p2,p3 };

	constexpr size_t variable_index = 1;
	f.differentiate(variable_index);

	VectorFunction<Polynomial> ref = { 0.5,1,0 };
	EXPECT_EQ(f, ref);
}


GTEST_TEST(VECTORFUNCTION, CROSS_PRODUCT1) {
	VectorFunction<Polynomial> vf1 = { 1.5,0,0 };
	VectorFunction<Polynomial> vf2 = { 0.5,1,0 };
	const auto result = vf1.cross_product(vf2);

	VectorFunction<Polynomial> ref = { 0,0,1.5 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(VECTORFUNCTION, L2_NORM1) {
	VectorFunction<Polynomial> vf = { 0,0,1.5 };
	const auto result = vf.L2_norm();

	Polynomial ref = 1.5;
	EXPECT_EQ(result, ref);
}
