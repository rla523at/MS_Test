#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Matrix.h"

GTEST_TEST(MATRIX, CONSTRUCTOR) {
	RowMajorMatrix m1(2, 2, { 1,0,0,1 });
	RowMajorMatrix m2(2);

	EXPECT_EQ(m1, m2);
}


GTEST_TEST(MATRIX, TRANSPOSE1) {
	RowMajorMatrix m(3, 2, { 1,2,3,4,5,6 });
	m.transpose();

	RowMajorMatrix ref(2, 3, { 1,3,5,2,4,6 });
	EXPECT_EQ(m, ref);
}
GTEST_TEST(MATRIX, TRANSPOSE2) {
	RowMajorMatrix m(3, 2, { 1,2,3,4,5,6 });
	m.transpose();

	RowMajorMatrix ref(2, 3, { 1,3,5,2,4,6 });
	EXPECT_EQ(ref,m);
}
GTEST_TEST(MATRIX, TRANSPOSE3) {
	RowMajorMatrix m(3, 2, { 1,2,3,4,5,6 });
	m.transpose();
	m.transpose();

	RowMajorMatrix ref(3, 2, { 1,2,3,4,5,6 });
	EXPECT_EQ(m, ref);
}


GTEST_TEST(MATRIX, PLUS_ASSIGNMENT1) {
	RowMajorMatrix m1(2, 2, { 1,2,3,4 });
	RowMajorMatrix m2(2, 2, { 1,3,4,5 });
	m1 += m2;

	RowMajorMatrix ref(2, 2, { 2,5,7,9 });
	EXPECT_EQ(m1, ref);
}
GTEST_TEST(MATRIX, PLUS_ASSIGNMENT2) {
	RowMajorMatrix m1(2, 3, { 1,2,3,4,5,6 });
	RowMajorMatrix m2(3, 2, { 1,3,4,5,5,6 });
	EXPECT_ANY_THROW(m1 += m2);
}
GTEST_TEST(MATRIX, PLUS_ASSIGNMENT3) {
	RowMajorMatrix m1(2, 3, { 1,2,3,4,5,6 });
	RowMajorMatrix m2(3, 2, { 1,3,4,5,5,6 });
	m2.transpose();
	m1 += m2;

	RowMajorMatrix ref(2, 3, { 2,6,8,7,10,12 });
	EXPECT_EQ(m1,ref);
}
GTEST_TEST(MATRIX, PLUS_ASSIGNMENT4) {
	RowMajorMatrix m1(2, 3, { 1,2,3,4,5,6 });
	RowMajorMatrix m2(3, 2, { 1,3,4,5,5,6 });
	m2.transpose();
	m2 += m1;

	RowMajorMatrix ref(2, 3, { 2,6,8,7,10,12 });
	EXPECT_EQ(m2, ref);
}