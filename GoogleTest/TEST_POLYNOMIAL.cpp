#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/SRC/Polynomial.cpp"

GTEST_TEST(MONOMIAL, CONSTRUCTOR) {
	{
		Monomial m1;
		Monomial m2{ 0 };
		EXPECT_EQ(m1, m2);
	}
	{
		Monomial m1(0);
		Monomial m2{ 1 };
		EXPECT_EQ(m1, m2);
	}
	{
		Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
		std::vector<size_t> exponent_set = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
		Monomial m2(std::move(exponent_set));
		EXPECT_EQ(m1, m2);
	}
}

GTEST_TEST(MONOMIAL, EXPONENT) {
	{
		Monomial m1;
		size_t variable_index = 0;
		EXPECT_ANY_THROW(m1.exponent(variable_index));
	}
	{
		Monomial m1(0);
		Monomial m2{ 1 };
		size_t variable_index = 0;
		const size_t ref = 1;
		EXPECT_EQ(m1.exponent(variable_index), ref);
		EXPECT_EQ(m2.exponent(variable_index), ref);
	}
	{
		Monomial m1{ 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
		std::vector<size_t> exponent_set = { 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5 };
		for (size_t i = 0; i < exponent_set.size(); ++i)
			EXPECT_EQ(m1.exponent(i), exponent_set[i]);
	}
	{
		Monomial m1{ 0,1,2 };
		EXPECT_EQ(m1.exponent(2), 2);
	}
}

GTEST_TEST(MONOMIAL, IS_CONSTANT) {
	Monomial m1;
	Monomial m2(0);
	Monomial m3{ 0 };
	Monomial m4{ 0,0,0,0,0,0,0,0,0 };

	EXPECT_TRUE(m1.is_constant());
	EXPECT_FALSE(m2.is_constant());
	EXPECT_TRUE(m3.is_constant());
	EXPECT_TRUE(m4.is_constant()); 
} 

//GTEST_TEST(MONOMIAL, NUM_VARIABLE) {
//	Monomial m1;
//	Monomial m2(0);
//	Monomial m3{ 0 };
//	Monomial m4{ 0,0,0,0,0,0,0,0,0 };
//	Monomial m5(128);
//	Monomial m6{ 1,2,3,4,5 };
//
//	EXPECT_EQ(m1.num_variable(), 0);
//	EXPECT_EQ(m2.num_variable(), 1);
//	EXPECT_EQ(m3.num_variable(), 0);
//	EXPECT_EQ(m4.num_variable(), 0);
//	EXPECT_EQ(m5.num_variable(), 1);
//	EXPECT_EQ(m6.num_variable(), 5);
//}

GTEST_TEST(MONOMIAL, ORDER) {
	Monomial m1;
	Monomial m2(0);
	Monomial m3{ 0 };
	Monomial m4{ 0,0,0,0,0,0,0,0,0 };
	Monomial m5(128);
	Monomial m6{ 1,2,3,4,5 };

	EXPECT_EQ(m1.order(), 0);
	EXPECT_EQ(m2.order(), 1);
	EXPECT_EQ(m3.order(), 0);
	EXPECT_EQ(m4.order(), 0);
	EXPECT_EQ(m5.order(), 1);
	EXPECT_EQ(m6.order(), 15);
}

GTEST_TEST(MONOMIAL, REDUCE_ORDER) {
	{
		Monomial m1;
		Monomial m2(0);
		Monomial m3{ 0 };
		Monomial m4{ 0,0,0,0,0,0,0,0,0 };

		EXPECT_ANY_THROW(m1.reduce_order(0));
		
		Monomial ref;
		EXPECT_EQ(m2.reduce_order(0), ref); 

		EXPECT_ANY_THROW(m3.reduce_order(0));

		EXPECT_ANY_THROW(m4.reduce_order(0));
	}
	{
		Monomial m(10);

		for (size_t i = 0; i < 10; ++i)
			EXPECT_ANY_THROW(m.reduce_order(i));

		Monomial ref;
 		EXPECT_EQ(m.reduce_order(10), ref);
	}
	{
		const Monomial m{ 1,0,3,0,5 };
		
		Monomial ref1{ 0,0,3,0,5 };
		EXPECT_EQ(m.reduce_order(0), ref1);
		
		EXPECT_ANY_THROW(m.reduce_order(1));
		
		Monomial ref3{ 1,0,2,0,5 };
		EXPECT_EQ(m.reduce_order(2), ref3);
		
		EXPECT_ANY_THROW(m.reduce_order(3));
	
		Monomial ref5{ 1,0,3,0,4 };
		EXPECT_EQ(m.reduce_order(4), ref5);
	}
	{
		Monomial m{ 1,0,3,0,5 };
		Monomial ref1{ 0,0,3,0,5 };
		EXPECT_EQ(m.reduce_order(0), ref1);
	}
	{
		Monomial m{ 1,0,3,0,5 };
		EXPECT_ANY_THROW(m.reduce_order(1)); 
	}
	{
		Monomial m{ 1,0,3,0,5 };
		Monomial ref3{ 1,0,2,0,5 };
		EXPECT_EQ(m.reduce_order(2), ref3);
	}
	{
		Monomial m{ 1,0,3,0,5 };
		EXPECT_ANY_THROW(m.reduce_order(3));
	}
	{
		Monomial m{ 1,0,3,0,5 };
		Monomial ref5{ 1,0,3,0,4 };
		EXPECT_EQ(m.reduce_order(4), ref5);
	}
}

GTEST_TEST(MONOMIAL, TO_STRING) {
	{
		Monomial m{ 1,0,3,0,5 };
		EXPECT_EQ(m.to_string(), "(x0)^1(x2)^3(x4)^5");
	}
	{
		Monomial m(128);
		EXPECT_EQ(m.to_string(), "(x128)^1"); 
	}
	{
		Monomial m{ 1,2,3,4 };
		EXPECT_EQ(m.to_string(), "(x0)^1(x1)^2(x2)^3(x3)^4");
	}
	{
		Monomial m;
		EXPECT_EQ(m.to_string(), "(1)");
	}
}

GTEST_TEST(MONOMIAL, MULTIPLICATION_ASSIGN) {
	{
		Monomial m1; 
		Monomial m2;		
		Monomial ref;		

		EXPECT_EQ(m1 *= m2, ref);
	}
	{
		Monomial m1{ 1,2,3,4 };
		Monomial m2{ 1,2 };
		Monomial ref{ 2,4,3,4 };

		EXPECT_EQ(m1 *= m2, ref);
	}
	{
		Monomial m1{ 1,2,3,4 };
		Monomial m2{ 1,2,0,1 };
		Monomial ref{ 2,4,3,5 };

		EXPECT_EQ(m1 *= m2, ref);
	}
	{
		Monomial m1{ 1,2,3,4 };
		Monomial m2{ 1,2,0,1,8,3 };
		Monomial ref{ 2,4,3,5,8,3 };

		EXPECT_EQ(m1 *= m2, ref);
	}
	{
		Monomial m1{ 1,2,3,4 };
		Monomial m2{ 1,2,0,1,8,3 };
		Monomial m3{ 1,2 };
		Monomial ref{ 3,6,3,5,8,3 };

		EXPECT_EQ(m1 *= m2 *= m3, ref);
	}
}

GTEST_TEST(MONOMIAL, MULTIPLICATION) {
	{
		Monomial m1;
		Monomial m2;
		Monomial ref;

		EXPECT_EQ(m1 * m2, ref);
	}
	{
		Monomial m1{ 1,2,3,4 };
		Monomial m2{ 1,2 };
		Monomial ref{ 2,4,3,4 };

		EXPECT_EQ(m1 * m2, ref);
	}
	{
		Monomial m1{ 1,2,3,4 };
		Monomial m2{ 1,2,0,1 };
		Monomial ref{ 2,4,3,5 };

		EXPECT_EQ(m1 * m2, ref);
	}
	{
		Monomial m1{ 1,2,3,4 };
		Monomial m2{ 1,2,0,1,8,3 };
		Monomial ref{ 2,4,3,5,8,3 };

		EXPECT_EQ(m1 * m2, ref);
	}
	{
		Monomial m1{ 1,2,3,4 };
		Monomial m2{ 1,2,0,1,8,3 };
		Monomial m3{ 1,2 };
		Monomial ref{ 3,6,3,5,8,3 };

		EXPECT_EQ(m1 * m2 * m3, ref);
	}
}

GTEST_TEST(MONOMIAL, FUNCTION_CALL) {
	{
		const Monomial m1;
		const MathVector v1 = { 1,2,3 };
		const MathVector v2 = { 1 };
		const MathVector v3 = { 1,2,3,4,5,6 };
		const MathVector v4;
		const double ref = 1.0;
		EXPECT_ANY_THROW(m1(v1));
		EXPECT_ANY_THROW(m1(v2));
		EXPECT_ANY_THROW(m1(v3));
		EXPECT_ANY_THROW(m1(v4));
		EXPECT_EQ(m1(), ref);
	}
	{

	}
}
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