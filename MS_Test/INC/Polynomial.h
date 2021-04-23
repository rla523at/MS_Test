#pragma once
#include "MathVector.h"

#include <algorithm> //for min max
#include <map>


class Monomial
{
private:
	std::vector<size_t> exponent_set_;
	bool is_constant_ = false;

public:
	explicit Monomial(void);
	explicit Monomial(const size_t variable_index);			// Monomial(0) => x_0	/ Monomial(2) => x_2
	Monomial(const std::initializer_list<size_t> list);		// Monomial{0} => 1		/ Monomial{2} => x^2 , without explicit m = {1,2,3} work!
	Monomial(std::vector<size_t>&& exponent_set);

	Monomial& operator*=(const Monomial& other);
	Monomial operator*(const Monomial& other) const;
	double operator()(void) const;
	double operator()(const MathVector& variable_vector) const;
	bool operator<(const Monomial& other) const;
	bool operator==(const Monomial& other) const;

	size_t exponent(size_t variable_index) const;
	size_t order(void) const;
	Monomial& reduce_order(const size_t variable_index);
	Monomial reduce_order(const size_t variable_index) const;
	std::string to_string(void) const;

	////for performance test
	//double call_operator1(const MathVector& variable_vector) const;
	//double call_operator2(const MathVector& variable_vector) const;

private:
	bool check_constant(void) const;
};

std::ostream& operator<<(std::ostream& ostream, const Monomial& monomial);


class Polynomial
{
	enum class BinaryOperator{
		addition,
		multiplication
	};

	using CalculatedPolynomial = std::pair<BinaryOperator, Polynomial>;

private:
	MathVector coefficient_vector_;
	std::vector<Monomial> monomial_set_;
	
	size_t power_index_ = 1;
	std::vector<CalculatedPolynomial> calculated_polynomial_set_; // to minimize truncation error

public:
	explicit Polynomial(void);
	explicit Polynomial(const double coefficient);
	explicit Polynomial(const Monomial& monomial);
	Polynomial(const double coefficient, const Monomial& monomial);
	Polynomial(const std::vector<double>& coefficient_set, const std::vector<Monomial>& monomial_set);
	
	Polynomial& operator+=(const Polynomial& other);
	Polynomial& operator-=(const Polynomial& other);
	Polynomial& operator*=(const double scalar);
	Polynomial& operator*=(const Polynomial& other);
	Polynomial operator+(const Polynomial& other) const;
	Polynomial operator-(const Polynomial& other) const;
	Polynomial operator*(const double scalar) const;
	Polynomial operator*(const Polynomial& other) const;	
	double operator()(const MathVector& variable_vector) const;
	bool operator==(const Polynomial& other) const;
	bool operator!=(const Polynomial& other) const;

	Polynomial& differentiate(const size_t variable_index);
	Polynomial& power(const size_t power_index);
	std::string to_string(void) const;

private:
	double calculate(const MathVector& variable_vector) const;
	Polynomial& differentiate_simple_poly(const size_t variable_index);

	void addition(const Polynomial& other);
	void scalar_multiplication(const double scalar);
	void multiplication(const Polynomial& other);

	Polynomial extend(void) const;
	void insert(const double coefficient, const Monomial& monomial);
	bool is_simple_polynomial(void) const;

	std::string to_poly_string(void) const;
};

std::ostream& operator<<(std::ostream& ostream, const Polynomial& monomial);