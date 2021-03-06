#pragma once

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
	MathVector(Vals&&... values) :std::vector<double>(std::forward<Vals>(values)...) {};
	MathVector(std::initializer_list<double> list) :std::vector<double>(list) {};
	MathVector(const double) = delete;
	

	MathVector& operator+=(const MathVector& y);
	MathVector& operator-=(const MathVector& y);
	MathVector& operator*=(const double scalar);
	MathVector operator+(const MathVector& y) const;
	MathVector operator-(const MathVector& y) const;
	MathVector operator*(const double scalar) const;


	MathVector& abs(void);	
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
	std::string double_to_string(const double val, const size_t precision = 15);
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
			result.emplace_back(function(variable_vector));
		return result;
	}
};