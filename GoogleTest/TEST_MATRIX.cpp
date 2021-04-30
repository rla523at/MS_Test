#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Matrix.h"

GTEST_TEST(MATRIX, CONSTRUCTOR) {
	RowMajorMatrix m1(2, 2, { 1,0,0,1 });
	RowMajorMatrix m2(2);

	const auto [rows, cols] = m2.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), m2.at(i, j));
}


GTEST_TEST(MATRIX, TRANSPOSE1) {
	RowMajorMatrix m(3, 2, { 1,2,3,4,5,6 });
	m.transpose();

	RowMajorMatrix ref(2, 3, { 1,3,5,2,4,6 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, TRANSPOSE2) {
	RowMajorMatrix m(3, 2, { 1,2,3,4,5,6 });
	m.transpose();
	m.transpose();

	RowMajorMatrix ref(3, 2, { 1,2,3,4,5,6 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m.at(i, j), ref.at(i, j));
}


GTEST_TEST(MATRIX, PLUS_ASSIGNMENT1) {
	RowMajorMatrix m1(2, 2, { 1,2,3,4 });
	RowMajorMatrix m2(2, 2, { 1,3,4,5 });
	m1 += m2;

	RowMajorMatrix ref(2, 2, { 2,5,7,9 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
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
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, PLUS_ASSIGNMENT4) {
	RowMajorMatrix m1(2, 3, { 1,2,3,4,5,6 });
	RowMajorMatrix m2(3, 2, { 1,3,4,5,5,6 });
	m2.transpose();
	m2 += m1;

	RowMajorMatrix ref(2, 3, { 2,6,8,7,10,12 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m2.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, PLUS_ASSIGNMENT5) {
	RowMajorMatrix m1(2, 2, { 1,2,3,4 });
	RowMajorMatrix m2(2, 2, { 1,3,4,5 });
	m1.transpose();
	m2.transpose();
	m1 += m2;

	RowMajorMatrix ref(2, 2, { 2,7,5,9 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, PLUS_ASSIGNMENT6) {
	RowMajorMatrix m1(2, 5, { 1.2345, 2.346345, 6.3262345, 8.5674567, 6.23452345, 2.53462, 6.432452345, 2.345345, 1.234563245, 7.3245345 });
	RowMajorMatrix m2(5, 2, { 1.234234, 2.3462345, 345.324, 2.6345345, 634523.5, 2345345.3,	 23453.345, 234534.6,	 234523.5, 623452.1 });
	m2.transpose();
	m1 += m2;

	RowMajorMatrix ref(2, 5, { 2.468734,  347.670345, 634529.8262345,    23461.9124567, 234529.73452345,4.8808545, 9.066986845, 2345347.645345, 234535.834563245,  623459.4245345 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, PLUS_ASSIGNMENT7) {
	RowMajorMatrix m1(3, 5, { 1.2345, 2.346345, 6.3262345, 8.5674567, 6.23452345, 2.53462, 6.432452345, 2.345345, 1.234563245, 7.3245345,  789.45978, 74.5789123, 74.23541, 4.7894113, 7894.5134 });
	RowMajorMatrix m2(5, 3, { 1.234234, 2.3462345, 789456.0,   345.324, 2.6345345, 74.48651,  634523.5, 2345345.3, 710.1846, 23453.345,  234534.6,  12.5487,  234523.5,  623452.1, 421.7456 });
	m2.transpose();
	m1 += m2;

	RowMajorMatrix ref(3, 5, { 2.468734,  347.670345, 634529.8262345,    23461.9124567, 234529.73452345,    4.8808545, 9.066986845, 2345347.645345, 234535.834563245,  623459.4245345, 790245.45978, 149.0654223,      784.42001,       17.3381113,        8316.259 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}


GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT1) {
	RowMajorMatrix m1(2, 2, { 1,2,3,4 });
	RowMajorMatrix m2(2, 2, { 1,3,4,5 });
	m1 *= m2;

	RowMajorMatrix ref(2, 2, { 9,13,19,29 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT2) {
	RowMajorMatrix m1(2, 8, { 1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1 });
	RowMajorMatrix m2(8, 2, { 1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8 });
	m1 *= m2;

	RowMajorMatrix ref(2, 2, { 204,204,120,120 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT3) {
	RowMajorMatrix m1(2, 2, { 1,2,3,4 });
	RowMajorMatrix m2(2, 2, { 1,3,4,5 });
	m1.transpose();
	m1 *= m2;

	RowMajorMatrix ref(2, 2, { 13,18,18,26 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT4) {
	RowMajorMatrix m1(2, 2, { 1,2,3,4 });
	RowMajorMatrix m2(2, 2, { 1,3,4,5 });
	m2.transpose();
	m1 *= m2;

	RowMajorMatrix ref(2, 2, { 7,14,15,32 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT5) {
	RowMajorMatrix m1(2, 2, { 1,2,3,4 });
	RowMajorMatrix m2(2, 2, { 1,3,4,5 });
	m1.transpose();
	m2.transpose();
	m1 *= m2;

	RowMajorMatrix ref(2, 2, { 10,19,14,28 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT6) {
	RowMajorMatrix m1(2, 3, { 1,2,3,4,5,6 });
	RowMajorMatrix m2(2, 3, { 1,2,3,3,2,1 });
	m2.transpose();
	m1 *= m2;

	RowMajorMatrix ref(2, 2, { 14,10,32,28 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT7) {
	RowMajorMatrix m1(2, 3, { 1,2,3,4,5,6 });
	RowMajorMatrix m2(2, 3, { 1,2,3,3,2,1 });
	m1.transpose();
	m1 *= m2;

	RowMajorMatrix ref(3, 3, { 13,10,7,17,14,11,21,18,15 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT8) {
	RowMajorMatrix m1(2, 3, { 1.5479,2.4567123,3.414878,4.41487,5.121,6.15789 });
	RowMajorMatrix m2(2, 3, { 1.1244,2.48711,3.12314,3.789413,2.9135491,1.264863 });
	m1.transpose();
	m1 *= m2;

	RowMajorMatrix ref(3, 3, { 18.470224531309999,  16.712738084116999,  10.418514118810000,  22.167911283120002,  21.030398669553001,  14.150019875622000,  27.174477241769999,  26.434492089979003,  18.454029295989997 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, MULTIPLY_ASSIGNMENT9) {
	RowMajorMatrix m1(2, 3, { 1.5479,2.4567123,3.414878,4.41487,5.121,6.15789 });
	RowMajorMatrix m2(2, 3, { 1.1244,2.48711,3.12314,3.789413,2.9135491,1.264863 });
	m2.transpose();
	m1 *= m2;

	RowMajorMatrix ref(2, 2, { 18.515714565372999,  17.342737125037932, 36.932522712599997 , 39.438937931479998 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m1.at(i, j), ref.at(i, j));
}

GTEST_TEST(MATRIX, INVERSE1) {
	RowMajorMatrix m(2, 2, { 1,2,3,4 });
	m.inverse();

	RowMajorMatrix ref(2, 2, { -2,1,1.5,-0.5 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, INVERSE2) {
	RowMajorMatrix m(2, 2, { 1,2,3,4 });
	m.transpose().inverse();

	RowMajorMatrix ref(2, 2, { -2,1.5,1,-0.5 });
	const auto [rows, cols] = ref.size();
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_DOUBLE_EQ(m.at(i, j), ref.at(i, j));
}
GTEST_TEST(MATRIX, INVERSE3) {
	RowMajorMatrix m(5, 5, { 1, 2, 3, 4, 5,  2, 13, 4, 11, 6, 1, 6, 5, 4, 3, 4, 1, 12, 7, 8, 9, 8, 7, 6, 5 });
	m.inverse();

	RowMajorMatrix ref(5, 5, { -0.03333333333333333, 3.426508252053199e-18, -0.1666666666666667, 2.669049661773008e-17,   0.1333333333333333,              0.09375,               -0.0625,                0.25,                -0.125,              0.03125,  -0.1541666666666667,  -0.04166666666666667,  0.1666666666666667,   0.08333333333333333, -0.02916666666666667,  -0.3395833333333333,    0.2708333333333333, -0.4166666666666667,    0.2083333333333333,             -0.06875,   0.5333333333333333,   -0.1666666666666667,  0.1666666666666667,   -0.1666666666666667,  0.03333333333333333 });
	const auto [rows, cols] = ref.size();
	const double epsilon = 1.0E-15;
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_NEAR(m.at(i, j), ref.at(i, j), epsilon); 	
	
	// suffering by machine error!
	//const size_t ULP_factor = 10000000;
	//for (size_t i = 0; i < rows; ++i) {
	//	for (size_t j = 0; j < cols; ++j) {
	//		if (!ms::compare_double(m.at(i, j), ref.at(i, j), ULP_factor)) {
	//			const auto lower_ULP = m.at(i, j) - std::nextafter(m.at(i, j), std::numeric_limits<double>::lowest());
	//			const auto upper_ULP = std::nextafter(m.at(i, j), std::numeric_limits<double>::max()) - m.at(i, j);

	//			const auto lower_bound = m.at(i, j) - ULP_factor * lower_ULP;
	//			const auto upper_bound = m.at(i, j) + ULP_factor * upper_ULP;

	//			std::cout << std::setprecision(20);
	//			std::cout << "value of m : " << m.at(i, j) << "\n";
	//			std::cout << "lower ULP : " << lower_ULP << "\n";
	//			std::cout << "upper ULP : " << upper_ULP << "\n";
	//			std::cout << "lower bound : " << lower_bound << "\n";
	//			std::cout << "upper bound : " << upper_bound << "\n";
	//			std::cout << "value : " << ref.at(i, j) << "\n";
	//		}
	//	}
	//}
}
GTEST_TEST(MATRIX, INVERSE4) {
	RowMajorMatrix m(5, 5, { 1, 2, 3, 4, 5,  2, 3, 4, 1, 6, 1, 6, 5, 4, 3, 4, 1, 2, 7, 8, 9, 8, 7, 6, 5 });
	m.inverse();

	RowMajorMatrix ref(5, 5, { -0.033333333333333,  -0.000000000000000,  -0.166666666666667,  -0.000000000000000,   0.133333333333333,  -1.291666666666667,   0.250000000000000,   0.666666666666667,   0.500000000000000,  -0.208333333333333,   1.616666666666666,  -0.250000000000000,  -0.666666666666667,  -0.750000000000000,   0.283333333333333,   0.275000000000000,  -0.250000000000000,   0.000000000000000,   0.000000000000000,   0.025000000000000,  -0.466666666666667,   0.250000000000000,   0.166666666666667,   0.250000000000000,  -0.133333333333333 });
	const auto [rows, cols] = ref.size();
	const double epsilon = 1.0E-15;
	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < cols; ++j)
			EXPECT_NEAR(m.at(i, j), ref.at(i, j), epsilon);// suffering by machine error!
			//EXPECT_DOUBLE_EQ(m.at(i, j), ref.at(i, j));
}