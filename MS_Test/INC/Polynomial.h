#pragma once
#include "MathVector.h"

#include <algorithm>

class SimplePolynomial;
class PolyTerm;
class CompactPolynomial;

class MonoTerm
{
	double coefficient_ = 1.0;
	std::vector<size_t> monomial_exponents_;

public:
	MonoTerm(const double coefficient) : coefficient_(coefficient) {};
	MonoTerm(const std::string& variable);

	void add_assign_with_same_monomial(const MonoTerm& other);
	MonoTerm& operator*=(const MonoTerm& other);
	SimplePolynomial operator+(const double& constant) const;
	SimplePolynomial operator+(const MonoTerm& mono_term) const;
	SimplePolynomial operator+(const SimplePolynomial& simple_polynomial) const;
	SimplePolynomial operator-(const SimplePolynomial& simple_polynomial) const;
	MonoTerm operator*(const double constant) const;
	MonoTerm operator*(const MonoTerm& other) const;
	PolyTerm operator*(const SimplePolynomial& simple_polynomial) const;
	MonoTerm operator^(const size_t power_index) const;
	double operator()(const MathVector& variable_vector) const;
	bool operator==(const MonoTerm& other) const;
	bool operator<(const MonoTerm& other) const;

	double be_constant(void) const;
	size_t domain_dimension(void) const;
	MonoTerm& be_derivative(const size_t variable_index);
	MonoTerm be_derivative(const size_t variable_index) const;
	bool has_same_monomial(const MonoTerm& other) const;
	bool is_constant(void) const;
	size_t order(void) const;
	MonoTerm& power(const size_t power_index);
	std::string to_string(void) const;
};
std::ostream& operator<<(std::ostream& os, const MonoTerm& mono_term);
MonoTerm operator* (const double constant, const MonoTerm& mono_term);


class SimplePolynomial
{
	std::vector<MonoTerm> added_mono_term_set_;
	double constant_ = 0.0;

public:
	SimplePolynomial(const double constant) : constant_(constant) {};
	SimplePolynomial(const MonoTerm& mono_term);
		
	SimplePolynomial& operator+=(const SimplePolynomial& other);
	SimplePolynomial& operator-=(const SimplePolynomial& other);
	SimplePolynomial& operator*=(const double constant);
	SimplePolynomial operator+(const SimplePolynomial& other) const;
	SimplePolynomial operator-(const SimplePolynomial& other) const;
	SimplePolynomial operator*(const double constant) const;
	PolyTerm operator*(const SimplePolynomial& other) const;
	PolyTerm operator^(const size_t power_index) const;
	double operator()(const MathVector& variable_vector) const;
	bool operator==(const SimplePolynomial& other) const;
	bool operator!=(const SimplePolynomial& other) const;

	double be_constant(void) const;
	SimplePolynomial& be_derivative(const size_t variable_index);
	SimplePolynomial differentiate(const size_t variable_index) const;
	size_t domain_dimension(void) const;
	bool is_constant(void) const;
	size_t order(void) const;
	std::string to_string(void) const;

private:
	void add_assign_mono_term(const MonoTerm& other);
};
std::ostream& operator<<(std::ostream& os, const SimplePolynomial& simple_polynomial);
SimplePolynomial operator*(const double constant, const SimplePolynomial& simple_polynomial);


class PolyTerm
{
	class PoweredPolynomial;

	double coefficient_ = 1.0;
	std::vector<PoweredPolynomial> multiplied_power_poly_term_set_;

public:
	//PolyTerm(const double coeeficient) : coefficient_(coeeficient) {};
	//PolyTerm(const SimplePolynomial& simple_polynomial);

	void add_assign_with_same_form(const PolyTerm& other);
	PolyTerm& operator*=(const PolyTerm& other);
	CompactPolynomial operator+(const CompactPolynomial& compact_polynomial) const;
	CompactPolynomial operator-(const CompactPolynomial& compact_polynomial) const;
	PolyTerm operator*(const PolyTerm& other) const;
	PolyTerm operator^(const size_t power_index) const;
	double operator()(const MathVector& variable_vector) const;
	bool operator==(const PolyTerm& other) const;
	bool operator!=(const PolyTerm& other) const;
	
	size_t domain_dimension(void) const;
	CompactPolynomial differentiate(const size_t variable_index) const;
	size_t order(void) const;
	bool has_same_form(const PolyTerm& other) const;
	PolyTerm& power(const size_t power_index);
	std::string to_string(void) const;

private:
	PolyTerm(const PoweredPolynomial& simple_polynomial);
	void multiply_assign(const PoweredPolynomial& power_poly_term);
};
std::ostream& operator<<(std::ostream& ostream, const PolyTerm& poly_term);
PolyTerm operator*(const double constant, const PolyTerm& other);


class PolyTerm::PoweredPolynomial
{
	SimplePolynomial base_ = 1.0;
	size_t exponent_ = 1;

public:
	PoweredPolynomial(const SimplePolynomial& simple_poly_term) : base_(simple_poly_term) {};

	void multiply_assign_with_same_base(const PoweredPolynomial& other);
	double operator()(const MathVector& variable_vector) const;
	bool operator==(const PoweredPolynomial& other) const;

	double be_constant(void) const;
	size_t domain_dimension(void) const;
	PolyTerm differentiate(const size_t variable_index) const;
	bool has_same_base(const PoweredPolynomial& other) const;
	bool is_constant(void) const;
	size_t order(void) const;
	void power(const size_t power_index);
	std::string to_string(void) const;
};


class CompactPolynomial
{
private:
	std::vector<PolyTerm> added_poly_term_set_;
	SimplePolynomial simple_poly_term_ = 0.0;

public:
	explicit CompactPolynomial(void) = default;
	CompactPolynomial(const double coeeficient) : simple_poly_term_(coeeficient) {};
	CompactPolynomial(const MonoTerm& mono_term) : simple_poly_term_(mono_term) {};
	CompactPolynomial(const SimplePolynomial& simple_poly_term) : simple_poly_term_(simple_poly_term) {};
	CompactPolynomial(const PolyTerm& poly_term);

	CompactPolynomial& operator+=(const CompactPolynomial& other);
	CompactPolynomial& operator-=(const CompactPolynomial& other);
	CompactPolynomial& operator*=(const double constant);
	CompactPolynomial operator+(const CompactPolynomial& other) const;
	CompactPolynomial operator-(const CompactPolynomial& other) const;
	CompactPolynomial operator*(const CompactPolynomial& other) const;
	CompactPolynomial operator*(const double constant) const;
	double operator()(const MathVector& variable_vector) const;
	bool operator==(const CompactPolynomial& other) const;

	size_t domain_dimension(void) const;
	CompactPolynomial differentiate(const size_t variable_index) const;
	VectorFunction<CompactPolynomial> gradient(void) const;
	VectorFunction<CompactPolynomial> gradient(const size_t domain_dimension) const;
	size_t order(void) const;
	std::string to_string(void) const;

	//private: // for test
	void add_assign_poly_term(const PolyTerm& term);
};
std::ostream& operator<<(std::ostream& ostream, const CompactPolynomial& polynomial);
CompactPolynomial operator*(const double constant, const CompactPolynomial& compact_polynomial);


//CompactPolynomial operator+(const PolyTerm& poly_term, const CompactPolynomial& compact_polynomial);


//Polynomial(const double scalar);
//CompactPolynomial& power(const double power_index);
//CompactPolynomial operator^(const double power_index) const;
//CompactPolynomial& operator*=(const CompactPolynomial& other);








































//class Monomial
//{
//private:
//	std::vector<size_t> exponent_set_;
//
//public:
//	explicit Monomial(void) = default;
//	explicit Monomial(const size_t variable_index);			// Monomial(0) => x0	/ Monomial(2) => x2
//	Monomial(const std::initializer_list<size_t> list);		// Monomial{0} => 1		/ Monomial{2} => x0^2 , without explicit m = {1,2,3} work!
//		
//	Monomial& operator*=(const Monomial& other);
//	Monomial operator*(const Monomial& other) const;
//	Monomial operator^(const size_t power) const;
//	double operator()(void) const;
//	double operator()(const MathVector& variable_vector) const;
//	bool operator<(const Monomial& other) const;
//	bool operator==(const Monomial& other) const;
//
//	size_t domain_dimension(void) const;
//	size_t exponent(size_t variable_index) const;
//	bool is_one(void) const;
//	size_t order(void) const;	
//	Monomial& reduce_order(const size_t variable_index);
//	std::string to_string(void) const;
//
////private: //for test
//	void remove_meaningless_zero(void);	
//};
//std::ostream& operator<<(std::ostream& ostream, const Monomial& monomial);
//
//
//class SimplePolyTerm 
//{
//private:
//	MathVector coefficient_vector_;
//	VectorFunction<Monomial> monomial_vector_function_;
//
//public:
//	explicit SimplePolyTerm(void);
//	SimplePolyTerm(const double scalar);
//	SimplePolyTerm(const Monomial& monomial);
//	SimplePolyTerm(const double scalar,const Monomial& monomial);
//	SimplePolyTerm(const std::vector<double>& coefficient_set, const std::vector<Monomial>& monomial_set);
//
//	SimplePolyTerm& operator+=(const SimplePolyTerm& other);
//	SimplePolyTerm& operator*=(const double constant);
//	SimplePolyTerm operator*(const double constant) const;
//	double operator()(const MathVector& variable_vector) const;
//	bool operator==(const SimplePolyTerm& other) const;
//	bool operator!=(const SimplePolyTerm& other) const;
//
//	double be_constant(void) const;
//	SimplePolyTerm& differentiate(const size_t variable_index);
//	size_t domain_dimension(void) const;
//	bool is_constant(void) const;
//	size_t order(void) const;
//	std::string to_string(void) const;
//	
////private:
//	void add_mono_term(const double coefficient, const Monomial& monomial);
//	SimplePolyTerm differentiate_monomial(const Monomial& monomial, const size_t variable_index) const;
//};


//
//class Polynomial;
//class PolyTerm
//{
//	friend class Polynomial;
//
////private:
//public: // for test
//	MathVector coefficient_vector_;
//	VectorFunction<Monomial> monomial_vector_function_;
//	double power_index_ = 1.0;
//	std::vector<PolyTerm> multiplied_term_set_; // to minimize truncation error
//
//public:
//	explicit PolyTerm(void);
//	PolyTerm(const double scalar);
//	PolyTerm(const Monomial& monomial);
//	PolyTerm(const std::vector<double>& coefficient_set, const std::vector<Monomial>& monomial_set);
//
//	PolyTerm& operator*=(const PolyTerm& other);
//	PolyTerm operator*(const PolyTerm& other) const;
//	PolyTerm operator^(const double power_index) const;
//	double operator()(const MathVector& variable_vector) const;
//	bool operator==(const PolyTerm& other) const;
//	bool operator!=(const PolyTerm& other) const;
//
//	size_t domain_dimension(void) const;
//	size_t order(void) const;	
//	std::string to_string(void) const;
//
////private: //for test
//	void add_mono_term(const double coefficient, const Monomial& monomial);
//	PolyTerm base(void) const;
//	void be_simple(void);
//	bool can_be_simple(void) const;
//	double constant_front(void) const;
//	double& constant_front(void);
//	bool has_constant_front(void) const;
//	bool is_simple(void) const;
//	bool is_single_term(void) const;
//	void make_constant_front(void);
//	PolyTerm& power(const double power_index);
//	PolyTerm& simple_term_addition(const PolyTerm& other);
//	double simple_term_calculation(const MathVector& variable_vector) const;
//	PolyTerm& simple_term_differentiate(const size_t variable_index);
//	PolyTerm& simple_term_multiplication(const PolyTerm& other);
//	size_t simple_term_domain_dimension(void) const;
//	size_t simple_term_order(void) const;
//	std::string simple_term_to_string(void) const;
//};
//std::ostream& operator<<(std::ostream& ostream, const PolyTerm& term);


//class Polynomial
//{
//private:
//	MathVector coefficient_vector_;
//	VectorFunction<PolyTerm> poly_term_vector_function_;
//	PolyTerm simple_term_;
//
//public:
//	explicit Polynomial(void) = default;
//	Polynomial(const double scalar);
//	Polynomial(const Monomial& monomial);
//	Polynomial(const PolyTerm& poly_term);
//
//	Polynomial& operator+=(const Polynomial& other);
//	Polynomial& operator-=(const Polynomial& other);
//	Polynomial& operator*=(const Polynomial& other);
//	Polynomial operator+(const Polynomial& other) const;
//	Polynomial operator-(const Polynomial& other) const;
//	Polynomial operator*(const Polynomial& other) const;
//	Polynomial operator^(const double power_index) const;
//	double operator()(const MathVector& variable_vector) const;
//	bool operator==(const Polynomial& other) const;
//
//	size_t domain_dimension(void) const;
//	Polynomial& differentiate(const size_t variable_index);
//	VectorFunction<Polynomial> gradient(void) const;
//	VectorFunction<Polynomial> gradient(const size_t domain_dimension) const;
//	size_t order(void) const;
//	Polynomial& power(const double power_index);
//	std::string to_string(void) const;
//
////private: // for test
//	Polynomial& add_term(const double coefficient, const PolyTerm& term);
//	Polynomial differentiate(const size_t variable_index) const;
//	//Polynomial poly_term_differentiate(const PolyTerm& polyn_term, const size_t variable_index) const;
//	static Polynomial poly_term_differentiate(const PolyTerm& polyn_term, const size_t variable_index);
//};
//std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial);
//Polynomial operator+(const PolyTerm& poly_term, const Polynomial& polynomial);
//Polynomial operator-(const PolyTerm& poly_term, const Polynomial& polynomial);
//Polynomial operator*(const PolyTerm& poly_term, const Polynomial& polynomial);


namespace ms {
	std::vector<MathVector> polynomial_compare_node_set(const size_t polynomial_order, const size_t domain_dimension);
	size_t combination(const size_t n, const size_t k);
	size_t combination_with_repetition(const size_t n, const size_t k);
//	Polynomial differentiate(const Polynomial& polynomial, const size_t variable_index);
//	Polynomial sqrt(const Polynomial& polynomial);
//	bool is_positive_odd_number(const double val);
//	bool is_natural_number(const double val);
}



















//class Polynomial
//{
//	enum class BinaryOperator{
//		addition,
//		multiplication
//	};
//
//	using CalculatedPolynomial = std::pair<BinaryOperator, Polynomial>;
//
//private:
//	MathVector coefficient_vector_;
//	VectorFunction<Monomial> monomial_vector_function_;	
//	std::vector<CalculatedPolynomial> calculated_polynomial_set_; // to minimize truncation error
//	double power_index_ = 1.0;
//	bool absoulte_flag = false;
//
//public:
	//explicit Polynomial(void) = default;
	//Polynomial(const double coefficient);
	//Polynomial(const Monomial& monomial);
	//Polynomial(const double coefficient, const Monomial& monomial);
	//Polynomial(const std::vector<double>& coefficient_set, const std::vector<Monomial>& monomial_set);
//	
//	Polynomial& operator+=(const Polynomial& other);
//	Polynomial& operator-=(const Polynomial& other);
//	Polynomial& operator*=(const double scalar);
//	Polynomial& operator*=(const Polynomial& other);
//
//	Polynomial operator+(const Polynomial& other) const;
//	Polynomial operator-(const Polynomial& other) const;
//	Polynomial operator*(const double scalar) const;
//	Polynomial operator*(const Polynomial& other) const;
//	Polynomial operator^(const double power_index) const;
//
//	double operator()(const MathVector& variable_vector) const;
//	bool operator==(const Polynomial& other) const;
//	bool operator!=(const Polynomial& other) const;
//
//	Polynomial& differentiate(const size_t variable_index);
	//VectorFunction<Polynomial> gradient(void) const;
	//VectorFunction<Polynomial> gradient(const size_t domain_dimension) const;
//	Polynomial& power(const double power_index);
//	std::string to_string(void) const;
//
//	size_t domain_dimension(void) const;
//	size_t polynomial_order(void) const;
//
////private:
//	bool compare_v1(const Polynomial& other) const;
//	bool compare_v2(const Polynomial& other) const;
//		
//	Polynomial& be_zero(void);
//	bool is_zero(void) const;
//	bool is_one(void) const;
//	bool is_operable(void) const;
//	bool is_simple_polynomial(void) const;
//	bool is_single_term(void) const;
//	bool is_scalar(void) const;
//	bool is_same_term(const Polynomial& other) const;
//	bool has_addable_simple_polynomial(void) const;
//	bool has_scalar_front_term(void) const;
//
//	Polynomial& simple_polynomial_addition_single_term(const double coefficient, const Monomial& monomial);
//	Polynomial& simple_polynomial_addition_simple_polynomial(const Polynomial& other);
//	Polynomial& nested_polynomial_addition_simple_polynomial(const Polynomial& other);
//	Polynomial& nested_polynomial_addition_nested_polynomial(const Polynomial& other);
//
//	//void simple_polynomial_scalar_multiplication(const double scalar);
//	Polynomial& simple_polynomial_multiplication(const Polynomial& other);
//	double simple_polynomial_calculation(const MathVector& variable_vector) const;
//	Polynomial to_simple_polynomial(void) const;
//
//	Polynomial& simple_polynomial_differentiate(const size_t variable_index);
//	size_t simple_polynomial_domain_dimension(void) const;
//	size_t simple_polynomial_order(void) const;
//	std::string simple_polynomial_string(void) const;
//};
//
//
//Polynomial operator+(const double scalar, const Polynomial& polynomial);
//Polynomial operator+(const Monomial& monomial, const Polynomial& polynomial);
//Polynomial operator-(const Monomial& monomial, const Polynomial& polynomial);
//Polynomial operator*(const double scalar, const Polynomial& monomial);
//
//std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial);
//
//




