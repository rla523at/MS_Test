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


class SimplePolynomial
{
private:
	MathVector coefficient_vector_;
	std::vector<Monomial> monomial_set_;

public:
	explicit SimplePolynomial(void) = default;
	SimplePolynomial(const double coefficient);
	SimplePolynomial(const Monomial& monomial);
	SimplePolynomial(const double coefficient, const Monomial& monomial);
	SimplePolynomial(const std::vector<double>& coefficient_set, const std::vector<Monomial>& monomial_set); //이건 initialize list로 안만들어도 왜 생성이 되는거지 ?

	SimplePolynomial& operator+=(const SimplePolynomial& other);
	SimplePolynomial& operator-=(const SimplePolynomial& other);
	SimplePolynomial& operator*=(const double scalar);

	SimplePolynomial operator+(const SimplePolynomial& other) const;
	SimplePolynomial operator-(const SimplePolynomial& other) const;
	SimplePolynomial operator*(const double scalar) const;

	bool operator==(const SimplePolynomial& other) const;
	bool operator!=(const SimplePolynomial& other) const;

	SimplePolynomial& differentiate(const size_t variable_index);
	VectorFunction<SimplePolynomial> gradient(void) const;
	VectorFunction<SimplePolynomial> gradient(const size_t domain_dimension) const;
	std::string to_string(void) const;

	bool is_zero(void) const;

	//private: // for testing
	void insert(const double coefficient, const Monomial& monomial);	

	size_t domain_dimension(void) const;
	size_t num_term(void) const;
	size_t polynomial_order(void) const;
	std::map<Monomial, double> ordered_polynomial(void) const;
};


SimplePolynomial operator+(const double scalar, const SimplePolynomial& simple_polynomial);
SimplePolynomial operator+(const Monomial& monomial, const SimplePolynomial& simple_polynomial);
SimplePolynomial operator-(const Monomial& monomial, const SimplePolynomial& simple_polynomial);
SimplePolynomial operator*(const double scalar, const SimplePolynomial& simple_polynomial);
std::ostream& operator<<(std::ostream& ostream, const SimplePolynomial& simple_polynomial);


class Polynomial
{
	enum class BinaryOperator{
		addition,
		substraction,
		multiplication
	};

	using CalculatedPolynomial = std::pair<BinaryOperator, Polynomial>;

private:
	//size_t power_index_ = 1;
	std::vector<SimplePolynomial> multiplied_simple_polynomial_set_;
	std::vector<CalculatedPolynomial> calculated_polynomial_set_;

public:	
	explicit Polynomial(const SimplePolynomial& simple_polynomial);

	Polynomial& operator+=(const Polynomial& other);
	Polynomial& operator-=(const Polynomial& other);
	Polynomial& operator*=(const double scalar);
	Polynomial& operator*=(const Polynomial& other);

	Polynomial operator+(const Polynomial& other) const;
	Polynomial operator-(const Polynomial& other) const;
	Polynomial operator*(const double scalar) const;
	Polynomial operator*(const Polynomial& other) const;


	//private: // for testing
	Polynomial& differentiate(const size_t variable_index);
	// 
//	double operator()(const MathVector& variable_vector) const;
//	bool operator==(const Polynomial& other) const;
//	bool operator!=(const Polynomial& other) const;
//
//	size_t domain_dimension(void) const;
//	Polynomial extend(void) const;
//	VectorFunction<Polynomial> gradient(void) const;
//	VectorFunction<Polynomial> gradient(const size_t domain_dimension) const;
//	size_t polynomial_order(void) const;
//	Polynomial& power(const size_t power_index);
//	std::string to_string(void) const;	
//
//	performance test
//	bool compare_v1(const Polynomial& other) const;
//	bool compare_v2(const Polynomial& other) const;
//
//
//private:
//	std::vector<MathVector> build_compare_node_set(void) const;
//	
//	double calculate(const MathVector& variable_vector) const;
//	Polynomial& differentiate_simple_poly(const size_t variable_index);
//
//	void add(const Polynomial& other);
//	void substract(const Polynomial& other);
//	void multiply(const double scalar);
//	Polynomial& multiply(const Polynomial& other);
//	
//	size_t simple_polynomial_domain_dimension(void) const;
//	size_t simple_polynomial_polynomial_order(void) const;
//	size_t simple_polynomial_num_term(void) const;
//
//	void insert(const double coefficient, const Monomial& monomial);
//	bool is_zero(void) const;
//	bool is_simple_polynomial(void) const;
//	bool is_single_term(void) const;
//
//	std::string to_poly_string(void) const;
};


//Polynomial operator+(const double scalar, const Monomial& monomial);
//Polynomial operator+(const Monomial& monomial, const Polynomial& polynomial);
//Polynomial operator-(const Monomial& monomial1, const Monomial& monomial2);
//Polynomial operator*(const double scalar, const Monomial& monomial);
//Polynomial operator*(const double scalar, const Polynomial& polynomial);

std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial);


namespace ms {
	size_t combination(const size_t n, const size_t k);
	size_t combination_with_repetition(const size_t n, const size_t k);
	SimplePolynomial differentiate(const SimplePolynomial& other, const size_t variable_index);
	/*Polynomial differentiate(const Polynomial& other, const size_t variable_index);*/
}