#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Polynomial.h"

#include <algorithm>
#include <random>

const size_t num_value = 100;
const size_t num_vec = 10000000;
const double value = 1.0E-6;
const size_t num_iter = 1;

GTEST_TEST(SPACE_LOCALITY, VECTOR_1) {
	const size_t num_total_value = num_value * num_vec;
	std::vector<double> v(num_total_value, value);

	const size_t num_indicator = num_vec + 1;
	std::vector<size_t> indicator(num_indicator);
	for (size_t i = 0; i < num_indicator; ++i)
		indicator[i] = num_value * i;

	double result = 0;
	for (size_t iter = 0; iter < num_iter; ++iter) {
		for (size_t i = 0; i < num_vec; ++i) {
			for (size_t index = indicator[i]; index < indicator[i + 1]; ++index)
				result += v[index];
		}
	}
	std::cout << result << "\n";
}

GTEST_TEST(SPACE_LOCALITY, DOUBLE_VECTOR_1) {
	std::vector<std::vector<double>> vv(num_vec);
	for (auto& vec : vv)
		vec.resize(num_value, value);

	std::vector<size_t> index_set(num_vec);
	for (size_t i = 0; i < num_vec; ++i)
		index_set[i] = i;

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(index_set.begin(), index_set.end(), g);

	std::vector<std::vector<double>*> ptr_set(num_vec);
	for (size_t i = 0; i < num_vec; ++i)
		ptr_set[i] = &vv[index_set[i]];

	double result = 0;
	for (size_t iter = 0; iter < num_iter; ++iter) {
		for (size_t i = 0; i < num_vec; ++i) {
			for (const auto& value : *ptr_set[i])
				result += value;
		}
	}
	std::cout << result << "\n";
}

GTEST_TEST(SPACE_LOCALITY, DOUBLE_VECTOR_2) {
	std::vector<std::vector<double>> vv(num_vec);
	for (auto& vec : vv)
		vec.resize(num_value, value);

	std::vector<size_t> index_set(num_vec);
	for (size_t i = 0; i < num_vec; ++i)
		index_set[i] = i;

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(index_set.begin(), index_set.end(), g);

	std::vector<std::vector<double>*> ptr_set(num_vec);
	for (size_t i = 0; i < num_vec; ++i)
		ptr_set[i] = &vv[index_set[i]];

	double result = 0;
	for (size_t iter = 0; iter < num_iter; ++iter) {
		for (size_t i = 0; i < num_vec; ++i) {
			for (const auto& value : *ptr_set[i])
				result += value;
		}
	}
	std::cout << result << "\n";
}

GTEST_TEST(SPACE_LOCALITY, VECTOR_2) {
	const size_t num_total_value = num_value * num_vec;
	std::vector<double> v(num_total_value, 1.0E-6);

	const size_t num_indicator = num_vec + 1;
	std::vector<size_t> indicator(num_indicator);
	for (size_t i = 0; i < num_indicator; ++i)
		indicator[i] = num_value * i;

	double result = 0;
	for (size_t iter = 0; iter < num_iter; ++iter) {
		for (size_t i = 0; i < num_vec; ++i) {
			for (size_t index = indicator[i]; index < indicator[i + 1]; ++index)
				result += v[index];
		}
	}
	std::cout << result << "\n";
}