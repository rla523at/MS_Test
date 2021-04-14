#pragma once

#include "MathVector.h"

#include <array>
#include <algorithm> //for min max
#include <map>


class Monomial
{
	using Iter = std::vector<size_t>::iterator;

private:
	std::vector<size_t> exponent_set_;
	bool is_constant_ = false;

	//for optimization	
	Iter start_iter_;
	Iter end_iter_;
	
public:
	explicit Monomial(void);
	explicit Monomial(const size_t variable_index);			// Monomial(0) => x_0	/ Monomial(2) => x_2
	Monomial(const std::initializer_list<size_t> list);		// Monomial{0} => 1		/ Monomial{2} => x^2 , without explicit m = {1,2,3} work!
	Monomial(std::vector<size_t>&& exponent_set);
	Monomial(const Monomial& other);
	Monomial(Monomial&& other) noexcept;

	Monomial& operator*=(const Monomial& other);
	Monomial operator*(const Monomial& other) const;
	double operator()(void) const;
	double operator()(const MathVector& variable_vector) const;
	Monomial& operator=(const Monomial& other);
	Monomial& operator=(Monomial&& other) noexcept;
	bool operator<(const Monomial& other) const;
	bool operator==(const Monomial& other) const;

	size_t exponent(size_t variable_index) const;
	size_t order(void) const;
	Monomial& reduce_order(const size_t variable_index);
	Monomial reduce_order(const size_t variable_index) const;
	std::string to_string(void) const;

	//bool is_constant(void) const;		//seem useless
	//size_t num_variable(void) const;	//seem useless

	////for performance test
	//double call_operator1(const MathVector& variable_vector) const;
	//double call_operator2(const MathVector& variable_vector) const;

private:
	bool check_constant(void) const;
	void update_iterator(void); //consdier iterator invalidation
	size_t size(void) const;
};

std::ostream& operator<<(std::ostream& ostream, const Monomial& monomial);

//class Monomial3D
//{
//private:
//	std::array<size_t, 3> exponent_set_ = { 0,0,0 };
//	bool is_constant_ = false;
//
//public:
//	explicit Monomial3D(void) {};
//	Monomial3D(const std::initializer_list<size_t> list);	
//
//	double operator()(const MathVector& variable_vector) const;
//	double call_operator1(const MathVector& variable_vector) const;
//	double call_operator2(const MathVector& variable_vector) const;
//
//private:
//	bool check_constant(void) const;
//};

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
	std::vector<CalculatedPolynomial> calculated_polynomial_set_;

public:
	explicit Polynomial(void);
	explicit Polynomial(const double coefficient);
	explicit Polynomial(const Monomial& monomial);
	explicit Polynomial(const double coefficient, const Monomial& monomial);
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

	std::string to_string(void) const;

private:
	double calculate(const MathVector& variable_vector) const;

	void addition(const Polynomial& other);
	void scalar_multiplication(const double scalar);
	void multiplication(const Polynomial& other);

	Polynomial extend(void) const;
	void insert(const double coefficient, const Monomial& monomial);
	bool is_simple_polynomial(void) const;
	bool is_same_polynomial(const Polynomial& other) const;

	std::string to_poly_string(void) const;
};

std::ostream& operator<<(std::ostream& ostream, const Polynomial& monomial);


//class Polynomial
//{
//private:
//	std::map<Monomial, double> monomial_to_coefficient_;
//
//public:
//	explicit Polynomial(void)
//		: monomial_to_coefficient_{ {Monomial(), 0} } {};
//
//	explicit Polynomial(const double coefficient)
//		: monomial_to_coefficient_{ {Monomial(), coefficient} } {};
//
//	explicit Polynomial(const Monomial& monomial)
//		: monomial_to_coefficient_{ {monomial, 1} } {};
//
//	explicit Polynomial(const double coefficient, const Monomial& monomial)
//		: monomial_to_coefficient_{ {monomial, coefficient} } {};
//
//	template <typename coeffcient_container, typename monomial_container>
//	Polynomial(const coeffcient_container& coefficient_set, const monomial_container& monomial_set) {
//		if (coefficient_set.size() != monomial_set.size())
//			throw "coefficients and monomials do not 1:1 match";
//
//		for (size_t i = 0; i < coefficient_set.size(); ++i)
//			this->insert(coefficient_set[i], monomial_set[i]);
//	};
//
//	Polynomial& operator*=(const double scalar);
//
//	Polynomial operator*(const double scalar) const;
//
//	Polynomial operator*(const Polynomial& other) const;
//
//	Polynomial& operator+=(const Polynomial& other);
//
//	Polynomial& operator-=(const Polynomial& other);
//
//	Polynomial& operator*=(const Polynomial& other);
//
//	Polynomial operator-(const Polynomial& other) const {
//		auto result = *this;
//		return result -= other;
//	}
//
//	double operator()(const MathVector& variable_vector) const;
//
//	Polynomial& differentiate(const size_t differential_variable);
//
//	Polynomial differentiate(const size_t differential_variable) const;
//
//	void insert(const double coefficient, const Monomial& monomial);
//
//	void insert(const double coefficient, Monomial&& monomial);
//
//	bool is_Zero(void) const;
//
//	size_t order(void) const;
//
//	Polynomial& translate(const MathVector& translation_vector);
//
//	Polynomial translate(const MathVector& translation_vector) const {
//		Polynomial p = *this;
//		return p.translate(translation_vector);
//	};
//
//	std::string to_String(void) const;
//};
//
//
//namespace Math {
//	Polynomial differentiate(const Monomial& monomial, const size_t differential_variable);
//
//	Polynomial translate(const Monomial& monomial, const MathVector& translation_vector);
//}
//
//
//inline std::ostream& operator<<(std::ostream& os, const Polynomial& polynomial) {
//	return os << polynomial.to_String();
//}
//
//inline Polynomial operator*(const double scalar, const Polynomial& polynomial) {
//	return polynomial.operator*(scalar);
//};





//
//enum class BinaryOperator
//{
//	plus,	
//	multiply
//};
//
//class Polynomial;
//namespace Math {
//	Polynomial differentiate(const Monomial& monomial, const size_t differential_variable);
//
//	Polynomial translate(const Monomial& monomial, const MathVector& translation_vector);
//}
//
//class Polynomial
//{
//	using BOPoly = std::pair<BinaryOperator, Polynomial>;
//private:
//	std::map<Monomial, double> monomial_to_coefficient_;
//
//	std::vector<BOPoly> BOPoly_set_;
//
//public:
//	explicit Polynomial(void)
//		: monomial_to_coefficient_{ {Monomial(), 0.0} } {};
//
//	explicit Polynomial(const double coefficient)
//		: monomial_to_coefficient_{ {Monomial(), coefficient} } {};
//
//	explicit Polynomial(const Monomial& monomial)
//		: monomial_to_coefficient_{ {monomial, 1.0} } {};
//
//	explicit Polynomial(const double coefficient, const Monomial& monomial)
//		: monomial_to_coefficient_{ {monomial, coefficient} } {};
//
//	template <typename coeffcient_container, typename monomial_container>
//	explicit Polynomial(const coeffcient_container& coefficient_set, const monomial_container& monomial_set) {
//		if (coefficient_set.size() != monomial_set.size())
//			FATAL_SIZE_ERROR;
//
//		for (size_t i = 0; i < coefficient_set.size(); ++i)
//			this->insert(coefficient_set[i], monomial_set[i]);
//	};
//
//
	//Polynomial& operator+=(const Polynomial& other) {
	//	if (this->is_Simple_Poly() && other.is_Simple_Poly())
	//		this->plus(other);
	//	else
	//		this->BOPoly_set_.emplace_back(BinaryOperator::plus, other);
	//	return *this;
	//}

	//Polynomial& operator-=(const Polynomial& other) {
	//	auto temp = other * -1.0;
	//	return *this += temp;
	//}

	//Polynomial& operator*=(const double& scalar) {
	//	this->multiply(scalar);		
	//	for (auto& [BO, poly] : BOPoly_set_) {
	//		if (BO == BinaryOperator::plus)
	//			poly.multiply(scalar);
	//	}
	//	return *this;
	//}

	//Polynomial& operator*=(const Polynomial& other) {
	//	this->BOPoly_set_.emplace_back(BinaryOperator::multiply, other);
	//	return *this;
	//}
//
//	Polynomial operator+(const Polynomial& other) const {
//		Polynomial result(*this);
//		return result += other;
//	}
//
//	Polynomial operator-(const Polynomial& other) const {
//		Polynomial result(*this);
//		return result -= other;
//	}
//
//	Polynomial operator*(const Polynomial& other) const {
//		Polynomial result(*this);
//		return result *= other;
//	}
//
	//Polynomial operator*(const double scalar) const {
	//	Polynomial result(*this);
	//	return result *= scalar;
	//}

	//double operator()(const MathVector& variable_vector) const {
	//	if (this->is_Simple_Poly())
	//		return this->calculate(variable_vector);
	//	else {
	//		auto result = this->calculate(variable_vector);
	//		for (const auto& [BO, polynomial] : this->BOPoly_set_) {
	//			switch (BO) {
	//			case  BinaryOperator::plus:
	//				result += polynomial(variable_vector);
	//				break;
	//			case BinaryOperator::multiply:
	//				result *= polynomial(variable_vector);
	//				break;
	//			}
	//		}
	//		return result;
	//	}
	//}
//
//	std::string to_Extended_String(void) const {
//		auto extended_poly = this->extend();
//
//		if (extended_poly.is_Zero())
//			return "{ 0 }";
//
//		std::string str;
//
//		str += "{ ";
//		const auto start_iter = extended_poly.monomial_to_coefficient_.rbegin();
//		const auto end_iter = extended_poly.monomial_to_coefficient_.rend();
//		for (auto iter = start_iter; iter != end_iter; ++iter) {
//			const auto& [monomial, coefficient] = *iter;
//			str += Editor::to_String(coefficient) + ",\t";
//		}
//		StringEditor::erase_back(str, 2);
//		str += " } { ";
//		for (auto iter = start_iter; iter != end_iter; ++iter) {
//			const auto& [monomial, coefficient] = *iter;
//			str += Editor::to_String(monomial) + ", ";
//		}
//		StringEditor::erase_back(str, 2);
//		str += " }";
//
//		return str;
//	}
//
//
//	std::string to_String(void) const {
//		if (this->is_Simple_Poly()) {
//			return this->to_Poly_String();
//		}
//		else {
//			auto str = this->to_Poly_String();
//
//			for (const auto& [BO, poly] : this->BOPoly_set_) {
//				switch (BO) {
//				case  BinaryOperator::plus:
//					str += " + ";
//					break;
//				case BinaryOperator::multiply:
//					str += " X ";
//					break;
//				}
//				str += poly.to_String() + "]";
//			}
//			//StringEditor::erase_back(str, 1);
//			return str;
//		}
//	}
//
//private:
//	double calculate(const MathVector& variable_vector) const {
//		double result = 0.0;
//		for (const auto& [monomial, coefficient] : this->monomial_to_coefficient_)
//			result += coefficient * monomial(variable_vector);
//
//		return result;
//	}
//
//	Polynomial& differentiate(const size_t differential_variable) {
//		Polynomial result;
//
//		for (auto& [monomial, coefficient] : this->monomial_to_coefficient_)
//			result += Math::differentiate(monomial, differential_variable) * coefficient;
//			//result += coefficient * Math::differentiate(monomial, differential_variable);
//
//		*this = std::move(result);
//		return *this;
//	}
//
//	Polynomial extend(void) const {
//		auto result = *this;
//
//		for (auto& [BO, poly] : result.BOPoly_set_) {
//			if (poly.is_Simple_Poly()) {
//				switch (BO) {
//				case  BinaryOperator::plus:
//					result.plus(poly);
//					break;
//				case BinaryOperator::multiply:
//					result.multiply(poly);
//					break;
//				}
//			}
//			else {
//				const auto& extended_poly = poly.extend();
//
//				if (!extended_poly.is_Simple_Poly())
//					FATAL_ERROR("polynomial is not fully extended");
//
//				switch (BO) {
//				case  BinaryOperator::plus:
//					result.plus(extended_poly);
//					break;
//				case BinaryOperator::multiply:
//					result.multiply(extended_poly);
//					break;
//				}
//			}
//		}
//		result.BOPoly_set_.clear();
//		return result;
//	}
//
//
//	bool is_Simple_Poly(void) const {
//		return this->BOPoly_set_.empty();
//	}
//
//	bool is_Zero() const {
//		if (!this->is_Simple_Poly())
//			return false;
//
//		if (this->monomial_to_coefficient_.size() == 0)
//			return true;
//
//		if (this->monomial_to_coefficient_.size() > 1)
//			return false;
//
//		const auto& [monomial, coefficient] = *this->monomial_to_coefficient_.begin();
//
//		if (coefficient == 0)
//			return true;
//		else
//			return false;
//	}
//
//	void insert(const double coefficient, const Monomial& monomial) {
//		if (coefficient == 0)
//			return;
//
//		auto iter = this->monomial_to_coefficient_.find(monomial);
//		if (iter == this->monomial_to_coefficient_.end())
//			this->monomial_to_coefficient_.emplace(monomial, coefficient);
//		else {
//			iter->second += coefficient;
//			if (iter->second == 0)
//				this->monomial_to_coefficient_.erase(iter);
//		}
//	}
//
	//void insert(const double coefficient, Monomial&& monomial) {
	//	if (coefficient == 0)
	//		return;

	//	auto iter = this->monomial_to_coefficient_.find(monomial);
	//	if (iter == this->monomial_to_coefficient_.end())
	//		this->monomial_to_coefficient_.emplace(std::move(monomial), coefficient);
	//	else {
	//		iter->second += coefficient;
	//		if (iter->second == 0)
	//			this->monomial_to_coefficient_.erase(iter);
	//	}
	//}

	//void plus(const Polynomial& other) {
	//	for (const auto& [other_monomial, other_coefficient] : other.monomial_to_coefficient_)
	//		this->insert(other_coefficient, other_monomial);
	//}

	//void multiply(const double scalar) {
	//	for (auto& [monomial, coefficient] : this->monomial_to_coefficient_)
	//		coefficient *= scalar;
	//}

	//void multiply(const Polynomial& other) {
	//	Polynomial result;
	//	for (const auto& [this_monomial, this_coefficient] : this->monomial_to_coefficient_)
	//		for (const auto& [other_monomial, other_coefficient] : other.monomial_to_coefficient_) {
	//			const auto result_coefficient = this_coefficient * other_coefficient;
	//			auto result_monomial = this_monomial * other_monomial;
	//			result.insert(result_coefficient, std::move(result_monomial));
	//		}
	//	*this = std::move(result);
	//}
//
//
	//std::string to_Poly_String(void) const {
	//	std::string str;
	//	str += "[";
	//	const auto start_iter = this->monomial_to_coefficient_.rbegin();
	//	const auto end_iter = this->monomial_to_coefficient_.rend();
	//	for (auto iter = start_iter; iter != end_iter; ++iter) {
	//		const auto& [monomial, coefficient] = *iter;
	//		str += Editor::to_String(coefficient) + Editor::to_String(monomial) + "\t";
	//	}
	//	StringEditor::erase_back(str, 1);
	//	str += "]";
	//	return str;
	//}
//
//	//Polynomial differentiate(const size_t differential_variable) const;
//
//	//void insert(const double coefficient, const Monomial& monomial);
//
//	//void insert(const double coefficient, Monomial&& monomial);
//
//	//bool is_Zero(void) const;
//
//	//size_t order(void) const;
//
//	//Polynomial& translate(const MathVector& translation_vector);
//
//	//Polynomial translate(const MathVector& translation_vector) const {
//	//	Polynomial p = *this;
//	//	return p.translate(translation_vector);
//	//};
//
//	
//};

//
//namespace Math {
//	Polynomial differentiate(const Monomial& monomial, const size_t differential_variable);
//
//	Polynomial translate(const Monomial& monomial, const MathVector& translation_vector);
//}
//
//
//inline std::ostream& operator<<(std::ostream& os, const Polynomial& polynomial) {
//	return os << polynomial.to_String();
//}
//
//inline Polynomial operator*(const double scalar, const Polynomial& polynomial) {
//	return polynomial.operator*(scalar);
//};

//
//class PolynomialSet
//{
//	using BOPoly = std::pair<BinaryOperator, Polynomial>;
//	using BOPolys = std::pair<BinaryOperator, PolynomialSet>;
//
//private:
//	std::vector<BOPoly> BOPoly_set_;
//
//	std::vector<BOPolys> BOPolys_set_;
//
//public:
//	PolynomialSet(void) {
//		this->BOPoly_set_.emplace_back(BinaryOperator::plus, Polynomial());
//	}
//
//	PolynomialSet(const Polynomial& polynomial) {
//		this->BOPoly_set_.emplace_back(BinaryOperator::plus, polynomial);
//	}
//
//	PolynomialSet(Polynomial&& polynomial) {
//		this->BOPoly_set_.emplace_back(BinaryOperator::plus, std::move(polynomial));
//	}
//
//
//	PolynomialSet& operator+=(const Polynomial& polynomial) {
//		this->BOPoly_set_.emplace_back(BinaryOperator::plus, polynomial);
//		return *this;
//	}
//
//	PolynomialSet& operator*=(const Polynomial& polynomial) {
//		this->BOPoly_set_.emplace_back(BinaryOperator::multiply, polynomial);
//		return *this;
//	}
//
//	PolynomialSet& operator+=(const PolynomialSet& polynomial_set) {
//		this->BOPolys_set_.emplace_back(BinaryOperator::plus, polynomial_set);
//		return *this;
//	}
//
//	PolynomialSet& operator*=(const PolynomialSet& polynomial_set) {
//		this->BOPolys_set_.emplace_back(BinaryOperator::multiply, polynomial_set);
//		return *this;
//	}
//
//	PolynomialSet operator+(const Polynomial& polynomial) const {
//		PolynomialSet result(*this);
//		return result += polynomial;
//	}
//
//	PolynomialSet operator+(const PolynomialSet& polynomial) const {
//		PolynomialSet result(*this);
//		return result += polynomial;
//	}
//
//	PolynomialSet operator*(const Polynomial& polynomial) const {
//		PolynomialSet result(*this);
//		return result *= polynomial;
//	}
//
//	PolynomialSet operator*(const PolynomialSet& polynomial) const {
//		PolynomialSet result(*this);
//		return result *= polynomial;
//	}
//
//	double operator()(const MathVector& variable_vector) const {
//		if (this->BOPolys_set_.empty())
//			return this->calculate(variable_vector);
//		else {
			//auto result = this->calculate(variable_vector);
			//for (const auto& [op, function] : this->BOPolys_set_) {
			//	switch (op) {
			//	case  BinaryOperator::plus:
			//		result += function(variable_vector);
			//		break;
			//	case BinaryOperator::multiply:
			//		result *= function(variable_vector);
			//		break;
			//	}
			//}
			//return result;
//		}
//	}
//
//private:
//	double calculate(const MathVector& variable_vector) const {
//		double result = 0.0;
//		for (const auto& [op, function] : this->BOPoly_set_) {
//			switch (op) {
//			case  BinaryOperator::plus:
//				result += function(variable_vector);
//				break;
//			case BinaryOperator::multiply:
//				result *= function(variable_vector);
//				break;
//			}
//		}
//		return result;
//	}
//};















//class MonoOply
//{
//	using BiOply = std::pair<BinaryOperator, Polynomial>;
//
//private:
//	std::vector<BiOply> bioply_set_;
//
//public:
//	MonoOply(void) {
//		this->bioply_set_.emplace_back(BinaryOperator::plus, Polynomial());
//	}
//
//	MonoOply(const Polynomial& polynomial) {
//		this->bioply_set_.emplace_back(BinaryOperator::plus, polynomial);
//	}
//
//	MonoOply(Polynomial&& polynomial) {
//		this->bioply_set_.emplace_back(BinaryOperator::plus, std::move(polynomial));
//	}
//
//
//	MonoOply& operator+=(const Polynomial& polynomial) {
//		this->bioply_set_.emplace_back(BinaryOperator::plus, polynomial);
//		return *this;
//	}
//
//	MonoOply& operator*=(const Polynomial& polynomial) {
//		this->bioply_set_.emplace_back(BinaryOperator::multiply, polynomial);
//		return *this;
//	}
//
//	MonoOply operator+(const Polynomial& polynomial) const {
//		MonoOply result(*this);
//		return result += polynomial;
//	}
//
//	MonoOply operator*(const Polynomial& polynomial) const {
//		MonoOply result(*this);
//		return result *= polynomial;
//	}
//
//	double operator()(const MathVector& variable_vector) const {
//		double result = 0.0;
//		for (const auto& [op, function] : this->bioply_set_) {
//			switch (op) {
//			case  BinaryOperator::plus:
//				result += function(variable_vector);
//				break;
//			case BinaryOperator::multiply:
//				result *= function(variable_vector);
//				break;
//			}
//		}
//		return result;
//	}
//};
//
//
//class PolynomialSet
//{
//	using BiOpMonoOply = std::pair<BinaryOperator, MonoOply>;
//
//private:
//	std::vector<BiOpMonoOply> bi_op_mono_oply_set_;
//
//public:
//	PolynomialSet(void) {
//		this->bi_op_mono_oply_set_.emplace_back(BinaryOperator::plus, MonoOply());
//	}
//
//	PolynomialSet(const MonoOply& mono_oply) {
//		this->bi_op_mono_oply_set_.emplace_back(BinaryOperator::plus, mono_oply);
//	}
//
//	PolynomialSet(MonoOply&& mono_oply) {
//		this->bi_op_mono_oply_set_.emplace_back(BinaryOperator::plus, std::move(mono_oply));
//	}
//
//
//	PolynomialSet& operator+=(const MonoOply& mono_oply) {
//		this->bi_op_mono_oply_set_.emplace_back(BinaryOperator::plus, mono_oply);
//		return *this;
//	}
//
//	PolynomialSet& operator*=(const MonoOply& mono_oply) {
//		this->bi_op_mono_oply_set_.emplace_back(BinaryOperator::multiply, mono_oply);
//		return *this;
//	}
//
//	PolynomialSet operator+(const MonoOply& mono_oply) const {
//		PolynomialSet result(*this);
//		return result += mono_oply;
//	}
//
//	PolynomialSet operator*(const MonoOply& mono_oply) const {
//		PolynomialSet result(*this);
//		return result *= mono_oply;
//	}
//
//	double operator()(const MathVector& variable_vector) const {
//		double result = 0.0;
//		for (const auto& [op, mono_oply] : this->bi_op_mono_oply_set_) {
//			switch (op) {
//			case  BinaryOperator::plus:
//				result += mono_oply(variable_vector);
//				break;
//			case BinaryOperator::multiply:
//				result *= mono_oply(variable_vector);
//				break;
//			}
//		}
//		return result;
//	}
//};
//
//
//PolynomialSet operator+ (const MonoOply& mono_oply1, const MonoOply& mono_oply2) {
//	PolynomialSet result(mono_oply1);
//	return result += mono_oply2;
//}
//
//
//PolynomialSet operator* (const MonoOply& mono_oply1, const MonoOply& mono_oply2) {
//	PolynomialSet result(mono_oply1);
//	return result *= mono_oply2;
//}


//template <typename F>
//class FunctionSet
//{
//private:
//	std::vector<std::pair<BinaryOperator, F>> function_set_;
//
//public:
//	FunctionSet(void) {
//		this->function_set_.emplace_back(BinaryOperator::plus, F());
//	}
//
//	FunctionSet(const F& function) {
//		this->function_set_.emplace_back(BinaryOperator::plus, function);
//	}
//
//	FunctionSet(F&& function) {
//		this->function_set_.emplace_back(BinaryOperator::plus, std::move(function));
//	}
//
//
//	FunctionSet& operator+=(const F& function) {
//		this->function_set_.emplace_back(BinaryOperator::plus, function);
//		return *this;
//	}
//
//	FunctionSet& operator*=(const F& function) {
//		this->function_set_.emplace_back(BinaryOperator::multiply, function);
//		return *this;
//	}
//
//	double operator()(const MathVector& variable_vector) const {
//		double result = 0.0;
//		for (const auto& [op, function] : this->function_set_) {
//			switch (op) {
//			case  BinaryOperator::plus:
//				result += function(variable_vector);
//				break;
//			case BinaryOperator::multiply:
//				result *= function(variable_vector);
//				break;
//			}
//		}
//		return result;
//	}
//};




//class PolynomialSet
//{
//private:
//	std::vector<std::pair<BinaryOperator, Polynomial>> polynomial_set_;
//	
//	//std::vector<std::tuple<BinaryOperator, std::pair<size_t,size_t>, std::pair<size_t, size_t>>> junction_indicator_;
//
//	////debug
//	//std::vector<std::pair<BinaryOperator, Rational>> rational_set_;
//	////
//
//public:
//	PolynomialSet(void) {
//		this->polynomial_set_.emplace_back(BinaryOperator::plus, Polynomial());
//		//this->rational_set_.emplace_back(BinaryOperator::plus, *this);
//	}
//
//	PolynomialSet(Polynomial&& polynomial) {
//		this->polynomial_set_.emplace_back(BinaryOperator::plus, std::move(polynomial));
//		//this->rational_set_.emplace_back(BinaryOperator::plus, *this);
//	}
//
//	
//	PolynomialSet& operator+=(const Polynomial& polynomial) {
//		this->polynomial_set_.emplace_back(BinaryOperator::plus, polynomial);
//		return *this;
//	}
//
//	PolynomialSet& operator*=(const Polynomial& polynomial) {
//		this->polynomial_set_.emplace_back(BinaryOperator::multiply, polynomial);
//		return *this;
//	}
//
//	//Rational& operator+=(const Rational& other) {
//	//	const auto num_this_term = this->polynomial_set_.size();
//	//	const auto num_otehr_term = other.polynomial_set_.size();
//
//	//	for (const auto& [other_junction_binary_operator, other_junction_index] : other.junction_indicator_)
//	//		this->junction_indicator_.emplace_back(other_junction_binary_operator, other_junction_index + num_this_term);
//	//	this->junction_indicator_.emplace_back(BinaryOperator::plus, num_this_term);
//
//
//	//	this->polynomial_set_.reserve(num_this_term + num_otehr_term);
//	//	Editor::merge(this->polynomial_set_, other.polynomial_set_);
//
//
//	//	//this->rational_set_.front().second = *this;
//	//	//this->rational_set_.emplace_back(BinaryOperator::plus, other);
//
//	//	return *this;
//	//}
//
//	//Rational& minus(const Polynomial& polynomial) {
//	//	auto tmp = -1.0 * polynomial;
//	//	this->polynomial_set_.emplace_back(BinaryOperator::plus, std::move(tmp));
//	//}
//
//	//Rational& operator-=(const Rational& other) {
//	//	const size_t num_this_term = this->polynomial_set_.size();
//	//	const size_t num_otehr_term = other.polynomial_set_.size();
//
//	//	this->junction_indicator_.emplace_back(BinaryOperator::plus, num_this_term);
//
//	//	this->polynomial_set_.reserve(num_this_term + num_otehr_term);
//	//	Editor::merge(this->polynomial_set_, other.polynomial_set_);
//	//}
//
//	PolynomialSet& operator*=(const Polynomial& polynomial) {
//		this->polynomial_set_.emplace_back(BinaryOperator::multiply, polynomial);
//		return *this;
//	}
//
//	//Rational& operator*=(const Rational& other) {
//	//	const size_t num_this_term = this->polynomial_set_.size();
//	//	const size_t num_otehr_term = other.polynomial_set_.size();
//
//	//	for (const auto& [other_junction_binary_operator, other_junction_index] : other.junction_indicator_)
//	//		this->junction_indicator_.emplace_back(other_junction_binary_operator, other_junction_index + num_this_term);
//	//	this->junction_indicator_.emplace_back(BinaryOperator::multiply, num_this_term);
//
//	//	this->polynomial_set_.reserve(num_this_term + num_otehr_term);
//	//	Editor::merge(this->polynomial_set_, other.polynomial_set_);
//
//
//	//	//this->rational_set_.front().second = *this;
//	//	//this->rational_set_.emplace_back(BinaryOperator::multiply, other);
//
//	//	return *this;
//	//}
//
//	double operator()(const MathVector& variable_vector) const;
//
//private:
//	//double calculate(const MathVector& variable_vector, const size_t start_index, const size_t end_index) const;
//
//	//double calculate(const MathVector& variable_vector) const;
//
//};