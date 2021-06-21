#pragma once
#include "MathVector.h"

#include <algorithm>
#include <array>

class IrrationalFunction;
class Polynomial
{
public: // for test
	class VariableTerm;
	class SimplePolyTerm;
	class PoweredPolyTerm;
	class PolyTerm;

public:
	Polynomial(const double coeeficient) : simple_poly_term_(coeeficient) {};
	Polynomial(const std::string& variable) : simple_poly_term_(variable) {};

	Polynomial& operator+=(const Polynomial& other);
	Polynomial& operator-=(const Polynomial& other);
	Polynomial& operator*=(const double constant);
	Polynomial operator+(const Polynomial& other) const;
	Polynomial operator-(const Polynomial& other) const;
	Polynomial operator*(const Polynomial& other) const;
	Polynomial operator*(const double constant) const;
	Polynomial operator^(const size_t power_index) const;
	double operator()(const MathVector& variable_vector) const;
	bool operator==(const Polynomial& other) const;

	size_t domain_dimension(void) const;
	Polynomial& be_derivative(const size_t variable_index);
	Polynomial differentiate(const size_t variable_index) const;
	VectorFunction<Polynomial> gradient(void) const;
	VectorFunction<Polynomial> gradient(const size_t domain_dimension) const;
	IrrationalFunction root(const double root_index) const;
	size_t order(void) const;
	std::string to_string(void) const;

//private: for test
	void add_assign_poly_term(const PolyTerm& term);


	class SimplePolyTerm
	{
	public:
		SimplePolyTerm(const double constant) : constant_(constant) {};
		SimplePolyTerm(const std::string& variable);
		SimplePolyTerm(const SimplePolyTerm& other);

		SimplePolyTerm& operator+=(const SimplePolyTerm& other);
		SimplePolyTerm& operator*=(const double constant);
		SimplePolyTerm operator*(const double constant) const;
		double operator()(const MathVector& variable_vector) const;
		bool operator==(const SimplePolyTerm& other) const;
		bool operator!=(const SimplePolyTerm& other) const;
		SimplePolyTerm& operator=(const SimplePolyTerm& other);

		double be_constant(void) const;
		double differentiate(const size_t variable_index) const;
		size_t domain_dimension(void) const;
		size_t order(void) const;
		bool is_constant(void) const;
		std::string to_string(void) const;

	private:
		void match_size(const SimplePolyTerm& other);
		bool is_small(void) const;

	private:
		std::vector<double> coefficient_vector_; //{c_i} mean sum(c_i * x_i)
		double constant_ = 0.0;
		
		size_t domain_dimension_ = 0;
		std::array<double, 3> small_buffer_ = { 0,0,0 };
		double* data_ptr_ = small_buffer_.data();
	};

	class PoweredPolyTerm
	{
	public:
		PoweredPolyTerm(const double constant) : base_(constant) {};
		PoweredPolyTerm(const SimplePolyTerm& simple_poly_term) : base_(simple_poly_term) {};

		void multiply_assign_with_same_base(const PoweredPolyTerm& other);
		double operator()(const MathVector& variable_vector) const;
		bool operator==(const PoweredPolyTerm& other) const;

		double be_constant(void) const;
		size_t domain_dimension(void) const;
		PolyTerm differentiate(const size_t variable_index) const;
		bool has_same_base(const PoweredPolyTerm& other) const;
		bool is_constant(void) const;
		size_t order(void) const;
		std::string to_string(void) const;

	private:
		SimplePolyTerm base_ = 1.0;
		size_t exponent_ = 1;
	};

	class PolyTerm
	{
	public:
		PolyTerm(const double coeeficient) : coefficient_(coeeficient) {};
		PolyTerm(const SimplePolyTerm& simple_poly_term);
		PolyTerm(const PoweredPolyTerm& powered_poly_term);
		PolyTerm(const PolyTerm& other);

		void add_assign_with_same_form(const PolyTerm& other);
		PolyTerm& operator*=(const PolyTerm& other);
		PolyTerm operator*(const PolyTerm& other) const;
		double operator()(const MathVector& variable_vector) const;
		bool operator==(const PolyTerm& other) const;
		bool operator!=(const PolyTerm& other) const;
		PolyTerm& operator=(const PolyTerm& other);

		double be_constant(void) const;
		size_t domain_dimension(void) const;
		Polynomial differentiate(const size_t variable_index) const;
		size_t order(void) const;
		bool has_same_form(const PolyTerm& other) const;
		bool is_constant(void) const;
		bool is_zero(void) const;
		std::string to_string(void) const;

	private:
		void multiply_assign_powered_poly_term(const PoweredPolyTerm& power_poly_term);
		bool is_small(void) const;

	//private:
	public:
		double coefficient_ = 1.0;
		std::vector<PoweredPolyTerm> multiplied_powered_poly_term_set_;

		size_t num_term_ = 0;
		std::array<PoweredPolyTerm, 3> small_buffer_ = { 0,0,0 };
		PoweredPolyTerm* data_ptr_ = small_buffer_.data();
	};

//private: //for test
	std::vector<PolyTerm> added_poly_term_set_;
	SimplePolyTerm simple_poly_term_ = 0.0;

	static size_t num_count_; //test
};
std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial);
Polynomial operator+(const double constant, const Polynomial& compact_polynomial);
Polynomial operator*(const double constant, const Polynomial& compact_polynomial);


class IrrationalFunction
{
	class PoweredPolynomial;
	class Term;

public:
	IrrationalFunction(const Polynomial& polynomial, const double root_index = 1.0);

	double operator()(const MathVector& value_vector) const;
	bool operator==(const IrrationalFunction& other) const;
		
	size_t domain_dimension(void) const;
	size_t order(void) const;
	std::string to_string(void) const;

private:
	std::vector<Term> added_irrational_term_set_;
	Polynomial polynomial_term_ = 0.0;
};
std::ostream& operator<<(std::ostream& ostream, const IrrationalFunction& irrational_function);


class IrrationalFunction::PoweredPolynomial
{
public:
	PoweredPolynomial(const Polynomial& base, const double exponent = 1.0) : base_(base), exponent_(exponent) {};

	double operator()(const MathVector& value_vector) const;

	size_t domain_dimension(void) const;
	size_t order(void) const;
	std::string to_string(void) const;

private:
	Polynomial base_ = 0.0;
	double exponent_ = 1.0;
};


class IrrationalFunction::Term
{
public:
	Term(const PoweredPolynomial& powered_polynomial);

	double operator()(const MathVector& value_vector) const;

	size_t domain_dimension(void) const;
	size_t order(void) const;
	std::string to_string(void) const;

private:
	double coefficient_ = 1.0;
	std::vector<PoweredPolynomial> multiplied_powered_polynomial_set_;
};


namespace ms {
	std::vector<MathVector> polynomial_compare_node_set(const size_t polynomial_order, const size_t domain_dimension);
	size_t combination(const size_t n, const size_t k);
	size_t combination_with_repetition(const size_t n, const size_t k);
	bool is_positive_odd_number(const double val);
	bool is_natural_number(const double val);
}