#pragma once
#include "MathVector.h"

#include <algorithm> //for min max
#include <map>


#define X Monomial(0)
#define Y Monomial(1)
#define Z Monomial(2)


class Monomial
{
private:
	std::vector<size_t> exponent_set_;

public:
	explicit Monomial(void) = default;
	explicit Monomial(const size_t variable_index);			// Monomial(0) => x0	/ Monomial(2) => x2
	Monomial(const std::initializer_list<size_t> list);		// Monomial{0} => 1		/ Monomial{2} => x0^2 , without explicit m = {1,2,3} work!

	Monomial& operator*=(const Monomial& other);
	Monomial operator*(const Monomial& other) const;
	Monomial operator^(const size_t power) const;
	double operator()(void) const;
	double operator()(const MathVector& variable_vector) const;
	bool operator<(const Monomial& other) const;
	bool operator==(const Monomial& other) const;

	size_t exponent(size_t variable_index) const;
	size_t monomial_order(void) const;
	size_t domain_dimension(void) const;
	Monomial& reduce_order(const size_t variable_index);
	std::string to_string(void) const;

	//private: // for testing
	void remove_meaningless_zero(void);
	bool is_constant(void) const;
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
	VectorFunction<Monomial> monomial_vector_function_;	
	std::vector<CalculatedPolynomial> calculated_polynomial_set_; // to minimize truncation error
	double power_index_ = 1.0;
	bool absoulte_flag = false;

public:
	explicit Polynomial(void) = default;
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
	Polynomial operator^(const double power_index) const;

	double operator()(const MathVector& variable_vector) const;
	bool operator==(const Polynomial& other) const;
	bool operator!=(const Polynomial& other) const;

	Polynomial& differentiate(const size_t variable_index);
	VectorFunction<Polynomial> gradient(void) const;
	VectorFunction<Polynomial> gradient(const size_t domain_dimension) const;
	Polynomial& power(const double power_index);
	std::string to_string(void) const;

	size_t domain_dimension(void) const;
	size_t polynomial_order(void) const;

//private:
	bool compare_v1(const Polynomial& other) const;
	bool compare_v2(const Polynomial& other) const;
		
	Polynomial& be_zero(void);
	bool is_zero(void) const;
	bool is_one(void) const;
	bool is_operable(void) const;
	bool is_simple_polynomial(void) const;
	bool is_single_term(void) const;

	Polynomial& polynomial_addition(const Polynomial& other);

	Polynomial& simple_polynomial_addition(const double coefficient, const Monomial& monomial);
	Polynomial& simple_polynomial_addition(const Polynomial& other);
	void simple_polynomial_scalar_multiplication(const double scalar);
	Polynomial& simple_polynomial_multiplication(const Polynomial& other);
	double simple_polynomial_calculation(const MathVector& variable_vector) const;
	Polynomial to_simple_polynomial(void) const;

	Polynomial& simple_polynomial_differentiate(const size_t variable_index);
	size_t simple_polynomial_domain_dimension(void) const;
	size_t simple_polynomial_order(void) const;
	std::string simple_polynomial_string(void) const;
};


Polynomial operator+(const double scalar, const Polynomial& polynomial);
Polynomial operator+(const Monomial& monomial, const Polynomial& polynomial);
Polynomial operator-(const Monomial& monomial, const Polynomial& polynomial);
Polynomial operator*(const double scalar, const Polynomial& monomial);

std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial);


namespace ms {
	std::vector<MathVector> build_compare_node_set(const size_t polynomial_order, const size_t domain_dimension);
	size_t combination(const size_t n, const size_t k);
	size_t combination_with_repetition(const size_t n, const size_t k);
	Polynomial differentiate(const Polynomial& polynomial, const size_t variable_index);
	Polynomial sqrt(const Polynomial& polynomial);
	bool is_positive_odd_number(const double val);
}