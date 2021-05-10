#pragma once

#include <algorithm>
#include <fstream>
#include <iomanip>	//set precision
#include <mkl.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

class MathVector : public std::vector<double>
{
public:
	template <typename ... Vals>
	explicit MathVector(Vals&&... values) :std::vector<double>(std::forward<Vals>(values)...) {};
	MathVector(std::initializer_list<double> list) :std::vector<double>(list) {};
	MathVector(const std::vector<double>& vec) :std::vector<double>(vec) {};
	MathVector(const double) = delete;
	

	MathVector& operator+=(const MathVector& y);
	MathVector& operator-=(const MathVector& y);
	MathVector& operator*=(const double scalar);
	MathVector operator+(const MathVector& y) const;
	MathVector operator-(const MathVector& y) const;
	MathVector operator*(const double scalar) const;


	MathVector& abs(void);	
	bool compare_with_finite_precision(const MathVector& other, const size_t ULP_precision = 4) const;
	double inner_product(const MathVector& other) const;
	double L2_Norm(void) const;
	void merge(const MathVector& other);
	void merge(MathVector&& other);
	MathVector& normalize(void);
	std::string to_string(void) const;
};


std::ostream& operator<<(std::ostream& os, const MathVector& x);
MathVector operator*(const double scalar, const MathVector& x);


namespace ms {
	MathVector abs(const MathVector& x);
	MathVector normalize(const MathVector& x);
	std::string double_to_string(const double val, const size_t precision = 17);
	bool compare_double(const double d1, const double d2, const size_t ULP_precision = 4);
}


template <typename T>
class VectorFunction : public std::vector<T>
{
public:
	template <typename ... Vals>
	VectorFunction(Vals&&... values) :std::vector<T>(std::forward<Vals>(values)...) {};
	VectorFunction(std::initializer_list<T> list) :std::vector<T>(list) {};

	MathVector operator()(const MathVector& variable_vector) const {
		MathVector result;
		result.reserve(this->size());
		for (const auto& function : *this)
			result.push_back(function(variable_vector));
		return result;
	}

	std::vector<MathVector> operator()(const std::vector<MathVector>& variable_vector_set) const {
		std::vector<MathVector> result;
		result.reserve(variable_vector_set.size());
		for (const auto& variable_vector : variable_vector_set)
			result.push_back((*this)(variable_vector));
		return result;
	}

	size_t domain_dimension(void) const {
		const size_t num_function = this->size();

		std::vector<size_t> domain_dimension_set(num_function);
		for (size_t i = 0; i < num_function; ++i)
			domain_dimension_set[i] = (*this)[i].domain_dimension();

		return *std::max_element(domain_dimension_set.begin(), domain_dimension_set.end());
	}

	std::string to_string(void) const {
		std::string result = "{ ";
		for (const auto& func : *this)
			result += func.to_string() + ", ";
		result.erase(result.end() - 2, result.end());
		result += " }";
		return result;
	}
};


template <typename T>
std::ostream& operator<<(std::ostream& os, const VectorFunction<T>& x) {
	return os << x.to_string();
};