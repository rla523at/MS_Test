#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Polynomial.h"

#include <algorithm>
#include <random>

GTEST_TEST(SPACE_LOCALITY, DOUBLE_VECTOR_1) {
	std::vector<std::vector<double>> vv(10);
	for (auto& vec : vv)
		vec.resize(10,0.001);

	std::vector<size_t> index_set(10);
	for (size_t i = 0; i < 10; ++i)
		index_set[i] = i;

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(index_set.begin(), index_set.end(), g);

	std::vector<std::vector<double>*> ptr_set(10);
	for (size_t i = 0; i < 10; ++i)
		ptr_set[i] = &vv[index_set[i]];

	double result = 0;	
	for (size_t iter = 0; iter < 100000000; ++iter) {
		for (size_t i = 0; i < 10; ++i) {
			for (const auto& value : *ptr_set[i])
				result += value;
		}
	}
	std::cout << result << "\n";
}

GTEST_TEST(SPACE_LOCALITY, VECTOR_1) {
	std::vector<double> v(100, 0.001);

	std::vector<size_t> indicator(11);
	for (size_t i = 0; i < 11; ++i)
		indicator[i] = 10 * i;

	double result = 0;
	for (size_t iter = 0; iter < 100000000; ++iter) {
		for (size_t i = 0; i < 10; ++i) {
			for (size_t index = indicator[i]; index < indicator[i + 1]; ++index)
				result += v[index];
		}
	}
	std::cout << result << "\n";
}

GTEST_TEST(SPACE_LOCALITY, DOUBLE_VECTOR_2) {
	std::vector<std::vector<double>> vv(10);
	for (auto& vec : vv)
		vec.resize(10, 0.001);

	std::vector<size_t> index_set(10);
	for (size_t i = 0; i < 10; ++i)
		index_set[i] = i;

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(index_set.begin(), index_set.end(), g);

	std::vector<std::vector<double>*> ptr_set(10);
	for (size_t i = 0; i < 10; ++i)
		ptr_set[i] = &vv[index_set[i]];

	double result = 0;
	for (size_t iter = 0; iter < 100000000; ++iter) {
		for (size_t i = 0; i < 10; ++i) {
			for (const auto& value : *ptr_set[i])
				result += value;
		}
	}
	std::cout << result << "\n";
}

GTEST_TEST(SPACE_LOCALITY, VECTOR_2) {
	std::vector<double> v(100, 0.001);

	std::vector<size_t> indicator(11);
	for (size_t i = 0; i < 11; ++i)
		indicator[i] = 10 * i;

	double result = 0;
	for (size_t iter = 0; iter < 100000000; ++iter) {
		for (size_t i = 0; i < 10; ++i) {
			for (size_t index = indicator[i]; index < indicator[i + 1]; ++index)
				result += v[index];
		}
	}
	std::cout << result << "\n";
}