#pragma once
#include "MathVector.h"

#include <algorithm> //for min max
#include <map>

// if need highly optimized code
// make Monomial3D 

#define X Monomial(0)
#define Y Monomial(1)
#define Z Monomial(2)

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
	Monomial operator^(const size_t power) const;
	double operator()(void) const;
	double operator()(const MathVector& variable_vector) const;
	bool operator<(const Monomial& other) const;
	bool operator==(const Monomial& other) const;

	size_t exponent(size_t variable_index) const;
	size_t monomial_order(void) const;
	size_t domain_order(void) const;
	Monomial& reduce_order(const size_t variable_index);
	std::string to_string(void) const;

//private:
	void remove_meaningless_zero(void);
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
	Polynomial(const double coefficient);
	Polynomial(const Monomial& monomial);
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
	Polynomial extend(void) const;
	VectorFunction<Polynomial> gradient(void) const;
	VectorFunction<Polynomial> gradient(const size_t domain_dimension) const;
	Polynomial& power(const size_t power_index);
	std::string to_string(void) const;

	size_t polynomial_order(void) const;
	size_t domain_dimension(void) const;

	//performance test
	bool compare_v1(const Polynomial& other) const;
	bool compare_v2(const Polynomial& other) const;


//private:
	std::vector<MathVector> build_compare_node_set(void) const;
	
	double calculate(const MathVector& variable_vector) const;
	Polynomial& differentiate_simple_poly(const size_t variable_index);

	void addition(const Polynomial& other);
	void scalar_multiplication(const double scalar);
	void multiplication(const Polynomial& other);
	
	size_t simple_domain_order(void) const;
	size_t simple_polynomial_order(void) const;

	void insert(const double coefficient, const Monomial& monomial);
	bool is_zero(void) const;
	bool is_simple_polynomial(void) const;

	std::string to_poly_string(void) const;
};


Polynomial operator+(const double scalar, const Monomial& monomial);
Polynomial operator+(const Monomial& monomial, const Polynomial& polynomial);
Polynomial operator*(const double scalar, const Monomial& monomial);

std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial);


namespace ms {
	size_t combination(const size_t n, const size_t k);
	size_t combination_with_repetition(const size_t n, const size_t k);
	Polynomial differentiate(const Polynomial& other, const size_t variable_index);
}