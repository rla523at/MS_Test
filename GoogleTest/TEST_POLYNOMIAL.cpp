#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Polynomial.h"

GTEST_TEST(MONOMIAL, EXPONENT) {
	{
		Monomial m1;
		size_t variable_index = 0;
		EXPECT_ANY_THROW(m1.exponent(variable_index));
	}
	{
		Monomial m(0); 
		size_t variable_index = 0;
		const size_t ref = 1;
		EXPECT_EQ(m.exponent(variable_index), ref); 
	}
	{
		Monomial m = { 1 };
		size_t variable_index = 0;
		const size_t ref = 1; 
		EXPECT_EQ(m.exponent(variable_index), ref);
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

GTEST_TEST(MONOMIAL, ORDER) {
	Monomial m1;
	Monomial m2(0);
	Monomial m3 = { 0 };
	Monomial m4 = { 0,0,0,0,0,0,0,0,0 };
	Monomial m5(128);
	Monomial m6 = { 1,2,3,4,5 };

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
		Monomial m3 = { 0 };
		Monomial m4 = { 0,0,0,0,0,0,0,0,0 };

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

GTEST_TEST(MONOMIAL, ASSIGN) {
	{
		Monomial m1;
		Monomial m2 = { 1,2,3 };
		m1 = m2;
		 
		EXPECT_EQ(m1, m2);
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
		EXPECT_EQ(m1(v1), ref);
		EXPECT_EQ(m1(v2), ref);
		EXPECT_EQ(m1(v3), ref);
		EXPECT_EQ(m1(v4), ref);
		EXPECT_EQ(m1(), ref);
	}
	{
		const Monomial m = { 1,2,3 };
		const MathVector v1;
		EXPECT_ANY_THROW(m(v1));
		
		const MathVector v2 = { 1,2,3 };
		const double ref2 = 108;
		EXPECT_EQ(m(v2), ref2);

		const MathVector v3 = { 1.84,2.789,3.487946 };
		const double ref3 = 6.073291260986822e+02;
		EXPECT_DOUBLE_EQ(m(v3), ref3);		

		const MathVector v4 = { 1,2,3,4,5 };
		const double ref4 = 108;
		EXPECT_EQ(m(v4), ref4);
	}
	{
		const Monomial m = { 12,123,26,4,32 };
		const MathVector v = { 0.849756587,1.123470098791,2.34596741,0.2487874441,1.22 };
		const double ref = 2.217180060803164e+15;
		EXPECT_DOUBLE_EQ(m(v), ref);
	}
}


//GTEST_TEST(MONOMIAL, IS_CONSTANT) {
//	Monomial m1;
//	Monomial m2(0);
//	Monomial m3{ 0 };
//	Monomial m4{ 0,0,0,0,0,0,0,0,0 };
//
//	EXPECT_TRUE(m1.is_constant());
//	EXPECT_FALSE(m2.is_constant());
//	EXPECT_TRUE(m3.is_constant());
//	EXPECT_TRUE(m4.is_constant()); 
//} 

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

//GTEST_TEST(MONOMIAL3D, CALL_OPERATOR1_1) {
//	{
//		Monomial3D m = { 1,1,1 };
//		std::vector<Monomial3D> vec(100);
//		std::vector<Monomial3D> vec2(100, m);
//
//		MathVector mv = { 1,2,3 };
//
//		double result = 0;
//		for (size_t i = 0; i < 10000000000; ++i) {
//			for (const auto& mono : vec)
//				result += mono.call_operator1(mv);
//			for (const auto& mono : vec2)
//				result += mono.call_operator1(mv);
//		}
//	}
//}
//
//GTEST_TEST(MONOMIAL3D, CALL_OPERATOR2_1) {
//	{
//		Monomial3D m = { 1,1,1 };
//		std::vector<Monomial3D> vec(100);
//		std::vector<Monomial3D> vec2(100, m);
//
//		MathVector mv = { 1,2,3 };
//
//		double result = 0;
//		for (size_t i = 0; i < 10000000000; ++i) {
//			for (const auto& mono : vec)
//				result += mono.call_operator2(mv);
//			for (const auto& mono : vec2)
//				result += mono.call_operator2(mv);
//		}
//	}
//}
//
//GTEST_TEST(MONOMIAL3D, CALL_OPERATOR1_2) {
//	{
//		std::vector<Monomial3D> vec(100);
//		MathVector mv = { 1,2,3 };
//
//		double result = 0;
//		for (size_t i = 0; i < 10000000000; ++i) {
//			for (const auto& monomial : vec)
//				result += monomial.call_operator1(mv);
//		}
//	}
//}
//
//GTEST_TEST(MONOMIAL3D, CALL_OPERATOR2_2) {
//	{
//		std::vector<Monomial3D> vec(100);
//		MathVector mv = { 1,2,3 };
//
//		double result = 0;
//		for (size_t i = 0; i < 10000000000; ++i) {
//			for (const auto& monomial : vec)
//				result += monomial.call_operator2(mv);
//		}
//	}
//}
//
//GTEST_TEST(MONOMIAL3D, CALL_OPERATOR1_3) {
//	{
//		Monomial3D m1 = { 1,1,1 };
//		Monomial3D m2;
//		std::vector<Monomial3D> vec;
//		vec.reserve(200);
//		for (size_t i = 0; i < 100; i++) {
//			vec.push_back(m1);
//			vec.push_back(m2);
//		}
//
//		MathVector mv = { 1,2,3 };
//		double result = 0;
//		for (size_t i = 0; i < 10000000000; ++i) {
//			for (const auto& mono : vec)
//				result += mono.call_operator1(mv);
//		}
//	}
//}
//
//GTEST_TEST(MONOMIAL3D, CALL_OPERATOR2_3) {
//	{
//		Monomial3D m1 = { 1,1,1 };
//		Monomial3D m2;
//		std::vector<Monomial3D> vec;
//		vec.reserve(200);
//		for (size_t i = 0; i < 100; i++) {
//			vec.push_back(m1);
//			vec.push_back(m2);
//		}
//
//		MathVector mv = { 1,2,3 };
//		double result = 0;
//		for (size_t i = 0; i < 10000000000; ++i) {
//			for (const auto& mono : vec)
//				result += mono.call_operator2(mv);
//		}
//	}
//}

GTEST_TEST(POLYNOMIAL, CONSTRUCTOR) {
	{
		Polynomial p1;
		Polynomial p2(0.0);
		Polynomial p3(1.0);

		Monomial m1;
		Polynomial p4(m1);

		EXPECT_EQ(p1, p2);
		EXPECT_NE(p1, p3); 
		EXPECT_NE(p2, p3);
		EXPECT_FALSE(p1 == p3);		
		EXPECT_EQ(p3, p4);
	}
	{
		std::vector<double> v_c = { 1,2 };
		std::vector<Monomial> v_m = { Monomial(), Monomial{1,2,3} };

		Polynomial p1 = { {1,2},{ Monomial(), Monomial{1,2,3} } };
		Polynomial p2(v_c, v_m);

		EXPECT_EQ(p1, p2);
	}
	{
		Polynomial p1 = { {1,2},{ Monomial(), Monomial{1} } };
		Polynomial p2 = { {2,1},{ Monomial{1},Monomial() } };
		EXPECT_EQ(p1, p2);
	}
	{
		Polynomial p1 = { {1,3},{ Monomial(), Monomial{1} } };
		Polynomial p2 = { {3,1},{ Monomial{1},Monomial() } };
		EXPECT_EQ(p1, p2);
	}
}

GTEST_TEST(POLYNOMIAL, ADDITION_ASSIGN) {
	{
		Polynomial p1 = { {1,1},{Monomial{2},Monomial{1}} };
		Polynomial p2 = { {1,1},{Monomial{1},Monomial{0}} };
		Polynomial ref = { {1,2,1},{Monomial{2},Monomial{1},Monomial{0}} };

		p1 += p2;
		EXPECT_EQ(p1, ref);
	}
	{
		Polynomial p1;
		Polynomial p2;
		Polynomial ref;

		p1 += p2;
		EXPECT_EQ(p1, ref); 
	}
	{
		Polynomial p1{ {-12,45},{Monomial{2,1},Monomial{1}} };
		Polynomial p2{ {12,-45},{Monomial{2,1},Monomial{1}} };
		Polynomial ref;

		p1 += p2;
		EXPECT_EQ(p1, ref);
	}
}

GTEST_TEST(POLYNOMIAL, MULTIPLICATION_ASSIGN) {
	{
		Polynomial p1 = { {1,1},{Monomial{1},Monomial{0}} };
		Polynomial p2 = { {1,1},{Monomial{1},Monomial{0}} };
		Polynomial ref = { {1,2,1},{Monomial{2},Monomial{1},Monomial{0}} };

		p1 *= p2;
		EXPECT_EQ(p1, ref);
	}
	{
		Polynomial p1;
		Polynomial p2;
		Polynomial ref;

		p1 *= p2;
		EXPECT_EQ(p1, ref);
	}
}

GTEST_TEST(POLYNOMIAL, TO_STRING) {
	{
		Polynomial p = { {1,2,1},{Monomial{2},Monomial{1},Monomial{0}} };
		std::string ref = "[" + std::to_string(1.0) + "(x0)^2 + " + std::to_string(2.0) + "(x0)^1 + " + std::to_string(1.0) + "(1)]";
		EXPECT_EQ(p.to_string(), ref);
	}
}


GTEST_TEST(POLYNOMIAL, CALL_OPERATOR) {
	{
		Polynomial p = { {1,2,1},{Monomial{2},Monomial{1},Monomial{0}} };

		const MathVector v;
		EXPECT_ANY_THROW(p(v));

		const MathVector v0 = { 0 };
		const double ref0 = 1;
		EXPECT_EQ(p(v0), ref0);

		const MathVector v1 = { 1 };
		const double ref1 = 4;
		EXPECT_EQ(p(v1), ref1);

		const MathVector v2 = { 1.87945646187 };
		const double ref2 = 8.291269515804899;
		EXPECT_DOUBLE_EQ(p(v2), ref2);
	}
	{
		Polynomial p = { {1,-2.0e3,1.0e6},{Monomial{2},Monomial{1},Monomial{0}} };

		const MathVector v3 = { 1000.1 };
		//const double ref = 0.01;
		const double ref = 0.01000000000931323;	// too large round off error
		EXPECT_DOUBLE_EQ(p(v3), ref);	
	}	
	{
		Polynomial p = { { 1,-1.0e3 }, { Monomial{1},Monomial{0}} };
		p *= p;
		const MathVector v = { 1000.1 };
		//const double ref = 0.01;
		const double ref = 0.01000000000000455;	// still too large round off error
		EXPECT_DOUBLE_EQ(p(v), ref);
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