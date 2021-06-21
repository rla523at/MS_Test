#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/MathVector.h"

GTEST_TEST(EuclideanVector, operator_addition_1) {
	EuclideanVector v1 = { 1,2,3 };
	EuclideanVector v2 = { 4,5,6 };
	auto result = v1 + v2;

	EuclideanVector ref = { 5,7,9 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(EuclideanVector, operator_addition_2) {
	EuclideanVector v1 = { 1,2,3,4 };
	EuclideanVector v2 = { 4,5,6,7 };
	auto result = v1 + v2;

	EuclideanVector ref = { 5,7,9,11 };
	EXPECT_EQ(result, ref);
}

GTEST_TEST(EuclideanVector, operator_substraction_1) {
	EuclideanVector v1 = { 1,2,3 };
	EuclideanVector v2 = { 4,5,6 };
	auto result = v1 - v2;

	EuclideanVector ref = { -3,-3,-3 };
	EXPECT_EQ(result, ref);
}



//GTEST_TEST(MATHVECTOR, CONSTRUCTOR1) {
//	std::vector<double> v = { 1,2,3,4,5 };
//	MathVector result = v;
//
//	MathVector ref = { 1,2,3,4,5 };
//	EXPECT_EQ(result, ref);
//}
//
//GTEST_TEST(MATHVECTOR, CONSTRUCTOR) {
//	{
//		MathVector v1 = { 1E-1,2E-1,3E-1,4E-1,5E-1,6E-1,7E-1,8E-1,9E-1,10E-1 };
//		for (size_t i = 0; i < 10; ++i)
//			EXPECT_DOUBLE_EQ(v1[i], 1E-1 * (i + 1));
//	}
//	{
//		MathVector v1 = { 1E-2,2E-2,3E-2,4E-2,5E-2,6E-2,7E-2,8E-2,9E-2,10E-2 };
//		for (size_t i = 0; i < 10; ++i)
//			EXPECT_DOUBLE_EQ(v1[i], 1E-2 * (i + 1));
//	}
//	{
//		MathVector v1 = { 1E-15,2E-15,3E-15,4E-15,5E-15,6E-15,7E-15,8E-15,9E-15,10E-15 };
//		for (size_t i = 0; i < 10; ++i)
//			EXPECT_DOUBLE_EQ(v1[i], 1E-15 * (i + 1));
//	}
//	{
//		MathVector v1 = { 1E-20,2E-20,3E-20,4E-20,5E-20,6E-20,7E-20,8E-20,9E-20,10E-20 };
//		for (size_t i = 0; i < 10; ++i)
//			EXPECT_DOUBLE_EQ(v1[i], 1E-20 * (i + 1));
//	}
//	{
//		MathVector v1 = { 1E-100,2E-100,3E-100,4E-100,5E-100,6E-100,7E-100,8E-100,9E-100,10E-100 };
//		for (size_t i = 0; i < 10; ++i)
//			EXPECT_DOUBLE_EQ(v1[i], 1E-100 * (i + 1));
//	}
//	{
//		MathVector v1 = { 1,2,3,4,5,6,7,8,9,10 };
//		for (size_t i = 0; i < 10; ++i)
//			EXPECT_EQ(v1[i], (i + 1));
//	}
//	{
//		MathVector v(5, 2.1);
//		MathVector ref = { 2.1,2.1,2.1,2.1,2.1 };
//		EXPECT_EQ(v, ref);
//	}
//	{
//		MathVector v(100, 2.1);
//		for (size_t i = 0; i < v.size(); ++i)
//			EXPECT_EQ(v[i], 2.1);
//	}
//	{
//		MathVector v(100, 2.112345678997);
//		for (size_t i = 0; i < v.size(); ++i)
//			EXPECT_EQ(v[i], 2.112345678997);
//	}
//	{
//		std::vector<double> v1(1234, 18.54789);
//		MathVector v(v1.begin(), v1.end());
//		MathVector ref(1234, 18.54789);
//		EXPECT_EQ(v, ref);
//	}
//	{
//		EXPECT_ANY_THROW(MathVector v(-1));
//	}
//	{
//		//prevent by delete!
//		//EXPECT_ANY_THROW(MathVector v(1.1));
//	}
//	{
//		MathVector v1(100, 2.112345678997);
//		MathVector v2(v1.begin(), v1.end());
//		EXPECT_EQ(v1, v2);
//	}
//	{
//		//// casting 되는걸 막아야 되나? 
//		//std::vector<size_t> v1 = { 1,2,3,4,5 };
//		//MathVector v(v1.begin(), v1.end());
//		//MathVector ref = { 1,2,3,4,5 };
//		//EXPECT_EQ(v, ref);
//	}
//}
//
//GTEST_TEST(MATHVECTOR, SIZE) {
//	{
//		const MathVector v1 = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 };
//		const size_t ref = 10;
//		EXPECT_EQ(v1.size(), ref);
//	}
//	{
//		const MathVector v1 = { 1,2,3,4,5,6,7,8,9,10 };
//		const size_t ref = 10;
//		EXPECT_EQ(v1.size(), ref);
//	}
//	{
//		const MathVector v1(12342);
//		const size_t ref = 12342;
//		EXPECT_EQ(v1.size(), ref);
//	}
//}
//
//GTEST_TEST(MATHVECTOR, OPERATOR_EQUAL) {
//	{
//		const MathVector v1 = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 };
//		const MathVector v2 = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 }; 
//		EXPECT_EQ(v1, v2);
//	}
//	{
//		MathVector v1 = { 1E-15,2E-15,3E-15,4E-15,5E-15,6E-15,7E-15,8E-15,9E-15,10E-15 };
//		MathVector v2 = { 1E-15,2E-15,3E-15,4E-15,5E-15,6E-15,7E-15,8E-15,9E-15,10E-15 };
//		EXPECT_EQ(v1, v2);
//	}
//	{
//		MathVector v1 = { 1E-100,2E-100,3E-100,4E-100,5E-100,6E-100,7E-100,8E-100,9E-100,10E-100 };
//		MathVector v2 = { 1E-100,2E-100,3E-100,4E-100,5E-100,6E-100,7E-100,8E-100,9E-100,10E-100 };
//		EXPECT_EQ(v1, v2);
//	}
//	{
//		MathVector v1 = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 };
//		const MathVector v2 = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 };
//		const size_t new_capcity = 10000;
//		v1.reserve(new_capcity);
//		EXPECT_EQ(v1, v2);
//	}
//	{
//		const MathVector v1 = { 1,2,3,4,5,6,7,8,9,10 };
//		const MathVector v2 = { 1,2,3,4,5,6,7,8,9,10 };
//		EXPECT_EQ(v1, v2);
//	}
//}
//
//
//GTEST_TEST(MATHVECTOR, OPERATOR_ADDITION_ASSIGNMENT) {
//	{
//		MathVector v1 = { 0.1 };
//		MathVector v2 = { 0.1,0.4 };
//		MathVector ref = { 0.2,0.4};
//		EXPECT_ANY_THROW(v1 += v2);
//	}
//	{
//		MathVector v1 = { 0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0 };
//		MathVector ref = { 0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0 };
//		v1 += v1;
//		EXPECT_EQ(v1, ref);
//	}	
//}
//
//
//GTEST_TEST(MATHVECTOR, operation_addition_1) {
//	MathVector v1 = { -1,-2,-3 };
//	MathVector v2 = { 1,2,3 };
//	const auto result = v1 + v2;
//	//result = add(v1,v2);
//
//	MathVector ref = { 0,0,0 };
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(MATHVECTOR, ABS1) {
//	MathVector v1 = { -1,-2,-3 };
//	auto v = ms::abs(v1);
//
//	MathVector ref = { 1,2,3 };
//	EXPECT_EQ(v, ref);
//}
//GTEST_TEST(MATHVECTOR, ABS2) {
//	MathVector v1 = { -1,-2,-3 };
//	v1.abs();
//
//	MathVector ref = { 1,2,3 };
//	EXPECT_EQ(v1, ref);
//}
//
//
//GTEST_TEST(MATHVECTOR, L2NORM1) {
//	MathVector v = { 3,4 };
//	const auto result = v.L2_Norm();
//
//	const double ref = 5;
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(MATHVECTOR, L2NORM2) {
//	MathVector v = { 0.1 };
//	const auto result = v.L2_Norm();
//
//	const double ref = 0.1;
//	EXPECT_EQ(result, ref);
//}