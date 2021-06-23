#pragma once
#include "EuclideanVector.h"

#include <algorithm>
#include <vector>

//#define static_require static_assert
#define dynamic_require(requirement, message)  if (!(requirement)) throw std::runtime_error(message)



class IrrationalFunction;
template <size_t DomainDim>
class Polynomial
{
public: // for test
	class SimplePolyTerm;
	class PoweredPolyTerm;
	class PolyTerm;

//public:
//	Polynomial(const double coeeficient) : simple_poly_term_(coeeficient) {};
//	Polynomial(const std::string& variable) : simple_poly_term_(variable) {};
//
//	Polynomial& operator+=(const Polynomial& other);
//	Polynomial& operator-=(const Polynomial& other);
//	Polynomial& operator*=(const double constant);
//	Polynomial operator+(const Polynomial& other) const;
//	Polynomial operator-(const Polynomial& other) const;
//	Polynomial operator*(const Polynomial& other) const;
//	Polynomial operator*(const double constant) const;
//	Polynomial operator^(const size_t power_index) const;
//	double operator()(const MathVector& variable_vector) const;
//	bool operator==(const Polynomial& other) const;
//
//	size_t domain_dimension(void) const;
//	Polynomial& be_derivative(const size_t variable_index);
//	Polynomial differentiate(const size_t variable_index) const;
//	VectorFunction<Polynomial> gradient(void) const;
//	VectorFunction<Polynomial> gradient(const size_t domain_dimension) const;
//	IrrationalFunction root(const double root_index) const;
//	size_t order(void) const;
//	std::string to_string(void) const;
//
////private: for test
//	void add_assign_poly_term(const PolyTerm& term);


	class SimplePolyTerm
	{
	public:
		SimplePolyTerm(const double constant) : constant_(constant) {};
		SimplePolyTerm(const std::string& variable);

		SimplePolyTerm& operator+=(const SimplePolyTerm& other);
		SimplePolyTerm& operator-=(const SimplePolyTerm& other);
		SimplePolyTerm& operator*=(const double constant);
		double operator()(const EuclideanVector<DomainDim>& domain_vector) const;
		bool operator==(const SimplePolyTerm& other) const;
		bool operator!=(const SimplePolyTerm& other) const;

		double be_constant(void) const;
		template <size_t VariableIndex, std::enable_if_t<VariableIndex < DomainDim, bool> = true> double differentiate(void) const;
		size_t order(void) const;
		bool is_constant(void) const;
		std::string to_string(void) const;

	private:
		std::array<double, DomainDim> coefficients_ = { 0 };
		double constant_ = 0.0;		

		//SimplePolyTerm(const SimplePolyTerm& other);
		//SimplePolyTerm operator*(const double constant) const;
		//size_t domain_dimension(void) const;
	};


	class PoweredPolyTerm
	{
	public:
		PoweredPolyTerm(const double constant) : base_(constant) {};
		PoweredPolyTerm(const SimplePolyTerm& simple_poly_term) : base_(simple_poly_term) {};

		void multiply_assign_with_same_base(const PoweredPolyTerm& other);
		double operator()(const EuclideanVector<DomainDim>& domain_vector) const;
		bool operator==(const PoweredPolyTerm& other) const;

		double be_constant(void) const;
		template <size_t VariableIndex, std::enable_if_t<VariableIndex < DomainDim, bool> = true> PolyTerm differentiate(void) const;
		bool has_same_base(const PoweredPolyTerm& other) const;
		bool is_constant(void) const;
		size_t order(void) const;
		std::string to_string(void) const;

	private:
		SimplePolyTerm base_ = 1.0;
		size_t exponent_ = 1;

		//size_t domain_dimension(void) const;
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
		double operator()(const EuclideanVector<DomainDim>& domain_vector) const;
		bool operator==(const PolyTerm& other) const; // compare node set 생성에 문제가 있다. compare node set을 생성하는게 잘못된것인가? 아니면 compare node를 dynamic하게 생성하지 못하는게 잘못된 것인가? // domain dimension이 dynamic인게 잘못된것인가?
		bool operator!=(const PolyTerm& other) const;
		PolyTerm& operator=(const PolyTerm& other);

		double be_constant(void) const;
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

		//size_t domain_dimension(void) const;
	};
//
////private: //for test
//	std::vector<PolyTerm> added_poly_term_set_;
//	SimplePolyTerm simple_poly_term_ = 0.0;
//
//	static size_t num_count_; //test
};
//std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial);
//Polynomial operator+(const double constant, const Polynomial& compact_polynomial);
//Polynomial operator*(const double constant, const Polynomial& compact_polynomial);


template <size_t DomainDim> Polynomial<DomainDim>::SimplePolyTerm::SimplePolyTerm(const std::string& variable) {
	dynamic_require(variable.front() == 'x', "variable should be start with 'x'");

	constexpr size_t index_pos = 1;
	const auto variable_index = std::stoull(variable.substr(index_pos));
	dynamic_require(variable_index < DomainDim, "variable index should be less than domain dimension");

	this->coefficients_[variable_index] = 1.0;
}

template <size_t DomainDim> typename Polynomial<DomainDim>::SimplePolyTerm& Polynomial<DomainDim>::SimplePolyTerm::operator+=(const SimplePolyTerm& other) {
	this->constant_ += other.constant_;
	for (size_t i = 0; i < DomainDim; ++i)
		this->coefficients_[i] += other.coefficients_[i];
	return *this;
}

template <size_t DomainDim> typename Polynomial<DomainDim>::SimplePolyTerm& Polynomial<DomainDim>::SimplePolyTerm::operator-=(const SimplePolyTerm& other) {
	this->constant_ -= other.constant_;
	for (size_t i = 0; i < DomainDim; ++i)
		this->coefficients_[i] -= other.coefficients_[i];
	return *this;
}

template <size_t DomainDim> typename Polynomial<DomainDim>::SimplePolyTerm& Polynomial<DomainDim>::SimplePolyTerm::operator*=(const double constant) {
	this->constant_ *= constant;
	for (size_t i = 0; i < DomainDim; ++i)
		this->coefficients_[i] *= constant;
	return *this;
}

template <size_t DomainDim> double Polynomial<DomainDim>::SimplePolyTerm::operator()(const EuclideanVector<DomainDim>& domain_vector) const {
	auto result = this->constant_;
	for (size_t i = 0; i < DomainDim; ++i)
		result += this->coefficients_[i] * domain_vector[i];
	return result;
}

template <size_t DomainDim> bool Polynomial<DomainDim>::SimplePolyTerm::operator==(const SimplePolyTerm& other) const {
	return this->constant_ == other.constant_ && this->coefficients_ == other.coefficients_;
}

template <size_t DomainDim> bool Polynomial<DomainDim>::SimplePolyTerm::operator!=(const SimplePolyTerm& other) const {
	return !(*this == other);
}

template <size_t DomainDim> double Polynomial<DomainDim>::SimplePolyTerm::be_constant(void) const {
	return this->constant_;
}

template <size_t DomainDim> template <size_t VariableIndex, std::enable_if_t<VariableIndex < DomainDim, bool>> 
	double Polynomial<DomainDim>::SimplePolyTerm::differentiate() const {
	return this->coefficients_[VariableIndex];
}

//size_t Polynomial::SimplePolyTerm::domain_dimension(void) const {
//	return this->domain_dimension_;
//}

template <size_t DomainDim> size_t Polynomial<DomainDim>::SimplePolyTerm::order(void) const {
	if (this->is_constant())
		return 0;
	else
		return 1;
}

template <size_t DomainDim> bool Polynomial<DomainDim>::SimplePolyTerm::is_constant(void) const {
	return this->coefficients_ == std::array<double, DomainDim>();

/*	for (const auto& coefficient : this->coefficients_) {
		if (coefficient != 0)
			return false;
	}
	return true*/;
}

template <size_t DomainDim> std::string Polynomial<DomainDim>::SimplePolyTerm::to_string(void) const {
	if (this->is_constant())
		return +"[" + ms::double_to_string(this->constant_) + "]";

	std::string str = "[";
	for (size_t i = 0; i < DomainDim; ++i){
		if (this->coefficients_[i] == 0.0)
			continue;
		else if (this->coefficients_[i] == 1.0)
			str += "+x" + std::to_string(i);
		else if (this->coefficients_[i] > 0)
			str += "+" + ms::double_to_string(this->coefficients_[i]) + "x" + std::to_string(i);
		else if (this->coefficients_[i] == -1.0)
			str += "-x" + std::to_string(i);
		else
			str += ms::double_to_string(this->coefficients_[i]) + "x" + std::to_string(i);
	}

	constexpr size_t position = 1;
	constexpr size_t size = 1;
	if (str.at(position) == '+')
		str.erase(position, size);

	if (this->constant_ == 0)
		return str += "]";
	else if (this->constant_ > 0)
		return str += '+' + ms::double_to_string(this->constant_) + "]";
	else
		return str += ms::double_to_string(this->constant_) + "]";
}

template <size_t DomainDim> void Polynomial<DomainDim>::PoweredPolyTerm::multiply_assign_with_same_base(const PoweredPolyTerm& other) {
	this->exponent_ += other.exponent_;
}

template <size_t DomainDim> double Polynomial<DomainDim>::PoweredPolyTerm::operator()(const EuclideanVector<DomainDim>& domain_vector) const {
	return std::pow(this->base_(domain_vector), this->exponent_);
}

template <size_t DomainDim> bool Polynomial<DomainDim>::PoweredPolyTerm::operator==(const PoweredPolyTerm& other) const {
	return this->base_ == other.base_ && this->exponent_ == other.exponent_;
}

template <size_t DomainDim> double Polynomial<DomainDim>::PoweredPolyTerm::be_constant(void) const {
	return std::pow(this->base_.be_constant(), this->exponent_);
}

//template <size_t DomainDim> size_t Polynomial<DomainDim>::PoweredPolyTerm::domain_dimension(void) const {
//	return DomainDim;
//}

template <size_t DomainDim> template <size_t VariableIndex, std::enable_if_t<VariableIndex < DomainDim, bool>>
	typename Polynomial<DomainDim>::PolyTerm Polynomial<DomainDim>::PoweredPolyTerm::differentiate(void) const {
	const auto base_derivative = this->base_.differentiate<VariableIndex>();
	if (base_derivative == 0.0)
		return 0.0;

	if (this->exponent_ == 1)
		return base_derivative;
	else {
		auto tmp = *this;	
		tmp.exponent_--;
		PolyTerm result = tmp;
		return result * (base_derivative * this->exponent_);
	}
}

template <size_t DomainDim> bool Polynomial<DomainDim>::PoweredPolyTerm::has_same_base(const PoweredPolyTerm& other) const {
	return this->base_ == other.base_;
}

template <size_t DomainDim> bool Polynomial<DomainDim>::PoweredPolyTerm::is_constant(void) const {
	return this->base_.is_constant();
}

template <size_t DomainDim> size_t Polynomial<DomainDim>::PoweredPolyTerm::order(void) const {
	if (this->is_constant())
		return 0;
	else
		return this->exponent_;
}

template <size_t DomainDim> std::string Polynomial<DomainDim>::PoweredPolyTerm::to_string(void) const {
	auto str = this->base_.to_string();
	if (this->exponent_ != 1)
		return  str + "^" + std::to_string(this->exponent_);
	else
		return str;
}


template <size_t DomainDim> Polynomial<DomainDim>::PolyTerm::PolyTerm(const SimplePolyTerm& simple_poly_term) {
	if (simple_poly_term.is_constant())
		this->coefficient_ = simple_poly_term.be_constant();
	else
		this->data_ptr_[this->num_term_++] = simple_poly_term;
}

template <size_t DomainDim> Polynomial<DomainDim>::PolyTerm::PolyTerm(const PoweredPolyTerm& powered_poly_term) {
	if (powered_poly_term.is_constant())
		this->coefficient_ = powered_poly_term.be_constant();
	else
		this->data_ptr_[this->num_term_++] = powered_poly_term;
}

template <size_t DomainDim> Polynomial<DomainDim>::PolyTerm::PolyTerm(const PolyTerm& other) {
	this->coefficient_ = other.coefficient_;
	this->num_term_ = other.num_term_;
	if (this->is_small()) {
		this->small_buffer_ = other.small_buffer_;
		this->data_ptr_ = this->small_buffer_.data();
	}
	else {
		this->multiplied_powered_poly_term_set_ = other.multiplied_powered_poly_term_set_;
		this->data_ptr_ = this->multiplied_powered_poly_term_set_.data();
	}
}

template <size_t DomainDim> void Polynomial<DomainDim>::PolyTerm::add_assign_with_same_form(const PolyTerm& other) {
	this->coefficient_ += other.coefficient_;
	if (this->coefficient_ == 0.0)
		*this = 0;
}

template <size_t DomainDim> typename Polynomial<DomainDim>::PolyTerm& Polynomial<DomainDim>::PolyTerm::operator*=(const PolyTerm& other) {
	this->coefficient_ *= other.coefficient_;
	if (this->coefficient_ == 0.0)
		return *this = 0.0;

	for (size_t i = 0; i < other.num_term_; ++i)
		this->multiply_assign_powered_poly_term(other.data_ptr_[i]);

	if (!this->coefficient_.empty())
		std::sort(this->coefficient_.begin(), this->coefficient_.end());
	std::sort(this->small_buffer_.begin(), this->small_buffer_.end());
	return *this;
}

template <size_t DomainDim> bool Polynomial<DomainDim>::PolyTerm::operator==(const PolyTerm& other) const {
	return this->coefficient_ == other.coefficient_ && this->coefficient_ == other.coefficient_ && this->small_buffer_ && other.small_buffer_;
	//
	//const auto max_order = std::max(this->order(), other.order());
	//const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
	//const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);

	//for (const auto& compare_node : compare_node_set) {
	//	if (!ms::compare_double((*this)(compare_node), other(compare_node)))
	//		return false;
	//}

	//return true;	
}

template <size_t DomainDim> bool Polynomial<DomainDim>::PolyTerm::operator!=(const PolyTerm& other) const {
	return !(*this == other);
}

template <size_t DomainDim> typename Polynomial<DomainDim>::PolyTerm& Polynomial<DomainDim>::PolyTerm::operator=(const PolyTerm& other) {
	this->coefficient_ = other.coefficient_;
	this->num_term_ = other.num_term_;
	if (other.is_small()) {
		this->small_buffer_ = other.small_buffer_;
		this->data_ptr_ = this->small_buffer_.data();
	}
	else {
		this->multiplied_powered_poly_term_set_ = other.multiplied_powered_poly_term_set_;
		this->data_ptr_ = this->multiplied_powered_poly_term_set_.data();
	}

	return *this;
}

template <size_t DomainDim> double Polynomial<DomainDim>::PolyTerm::be_constant(void) const {
	return this->coefficient_;
}

template <size_t DomainDim> typename Polynomial<DomainDim> Polynomial<DomainDim>::PolyTerm::differentiate(const size_t variable_index) const {
	Polynomial result = 0.0;
	
	for (size_t i = 0; i < this->num_term_; ++i) {
		const auto diff_term = this->data_ptr_[i].differentiate(variable_index);
		
		if (!diff_term.is_zero()) {
			PolyTerm derivative = this->coefficient_;
			if (this->is_small()) {
				size_t index = 0;
				for (size_t j = 0; j < this->num_term_; ++j) {
					if (j == i)
						continue;
					else {
						derivative.small_buffer_[index++] = this->data_ptr_[j];
						derivative.num_term_++;
					}
				}
			}
			else {
				derivative = *this;
				derivative.multiplied_powered_poly_term_set_.erase(derivative.multiplied_powered_poly_term_set_.begin() + i);
			}

			derivative *= diff_term;

			if (derivative.is_constant())
				result.simple_poly_term_ += derivative.be_constant();
			else
				result.add_assign_poly_term(derivative);
		}
	}

	return result;
}

template <size_t DomainDim> size_t Polynomial<DomainDim>::PolyTerm::order(void) const {
	size_t result = 0;
	for (size_t i = 0; i < this->num_term_; ++i)
		result += this->data_ptr_[i].order();
	return result;
}

template <size_t DomainDim> bool Polynomial<DomainDim>::PolyTerm::has_same_form(const PolyTerm& other) const {
	return this->coefficient_ == other.coefficient_ && this->small_buffer_ && other.small_buffer_;

	//
	//if (this->num_term_ != other.num_term_)
	//	return false;

	//for (size_t i = 0; i < this->num_term_; ++i) {
	//	bool is_here = false;
	//	for (size_t j = 0; j < other.num_term_; ++j) {
	//		if (this->data_ptr_[i] == other.data_ptr_[j])
	//			is_here = true;
	//	}
	//	if (!is_here)
	//		return false;
	//}
	//return true;
}

template <size_t DomainDim> bool Polynomial<DomainDim>::PolyTerm::is_constant(void) const {
	return this->num_term_ == 0;
}

template <size_t DomainDim> bool Polynomial<DomainDim>::PolyTerm::is_zero(void) const {
	return this->coefficient_ == 0.0;
}

template <size_t DomainDim> std::string Polynomial<DomainDim>::PolyTerm::to_string(void) const {
	std::string str;
	if (std::abs(this->coefficient_) != 1.0) {
		if (this->coefficient_ > 0)
			str += "+";

		str += ms::double_to_string(this->coefficient_);
	}
	else if (this->coefficient_ == 1.0)
		str += "+";
	else
		str += "-";

	for (size_t i = 0; i < this->num_term_; ++i)
		str += this->data_ptr_[i].to_string();

	return str;
}

template <size_t DomainDim> void Polynomial<DomainDim>::PolyTerm::multiply_assign_powered_poly_term(const PoweredPolyTerm& power_poly_term) {
	for (size_t i = 0; i < this->num_term_; ++i) {
		if (this->data_ptr_[i].has_same_base(power_poly_term)) {
			this->data_ptr_[i].multiply_assign_with_same_base(power_poly_term);
			return;
		}
	}

	if (this->small_buffer_.size() < ++this->num_term_) {
		this->multiplied_powered_poly_term_set_.resize(this->num_term_, 0.0);

		if (this->num_term_ == this->small_buffer_.size() + 1) {
			std::copy(this->small_buffer_.begin(), this->small_buffer_.end(), this->multiplied_powered_poly_term_set_.begin());
			this->data_ptr_ = this->multiplied_powered_poly_term_set_.data();
		}
	}

	const auto position = this->num_term_ - 1;
	this->data_ptr_[position] = power_poly_term;
}

//
//template <size_t DomainDim> double Polynomial<DomainDim>::PolyTerm::operator()(const EuclideanVector<DomainDim>& domain_vector) const {
//	auto result = this->coefficient_;
//	for (size_t i = 0; i < this->num_term_; ++i)
//		result *= this->data_ptr_[i](domain_vector);
//	return result;
//}

//class IrrationalFunction
//{
//	class PoweredPolynomial;
//	class Term;
//
//public:
//	IrrationalFunction(const Polynomial& polynomial, const double root_index = 1.0);
//
//	double operator()(const MathVector& value_vector) const;
//	bool operator==(const IrrationalFunction& other) const;
//		
//	size_t domain_dimension(void) const;
//	size_t order(void) const;
//	std::string to_string(void) const;
//
//private:
//	std::vector<Term> added_irrational_term_set_;
//	Polynomial polynomial_term_ = 0.0;
//};
//std::ostream& operator<<(std::ostream& ostream, const IrrationalFunction& irrational_function);
//
//
//class IrrationalFunction::PoweredPolynomial
//{
//public:
//	PoweredPolynomial(const Polynomial& base, const double exponent = 1.0) : base_(base), exponent_(exponent) {};
//
//	double operator()(const MathVector& value_vector) const;
//
//	size_t domain_dimension(void) const;
//	size_t order(void) const;
//	std::string to_string(void) const;
//
//private:
//	Polynomial base_ = 0.0;
//	double exponent_ = 1.0;
//};
//
//
//class IrrationalFunction::Term
//{
//public:
//	Term(const PoweredPolynomial& powered_polynomial);
//
//	double operator()(const MathVector& value_vector) const;
//
//	size_t domain_dimension(void) const;
//	size_t order(void) const;
//	std::string to_string(void) const;
//
//private:
//	double coefficient_ = 1.0;
//	std::vector<PoweredPolynomial> multiplied_powered_polynomial_set_;
//};
//
//
//template <size_t DomainDim, size_t RangeDim> class PolynomialVectorFunction
//{
//public:
//	template <typename... Args> VectorFunction(Args... args) : elements_{ args... } {};
//
//	EuclideanVector<RangeDim> operator()(const EuclideanVector < )
//		const Function& operator[](const size_t position) const;
//
//private:
//	std::array<Function, RangeDim> elements_;
//};
//template <typename... Args> struct ms::Enforce_polynomial_type;
//template <size_t DomainDim, size_t RangeDim, typename... Args> PolynomialVectorFunction(Args...)->PolynomialVectorFunction<typename ms::Enforce_polynomial_type<Args>::type, 1 + sizeof...(Rest)>; // user defined deduction guides
//
//
//
//namespace ms {
//	template <typename... Args> struct Enforce_polynomial_type {
//		static_assert((std::is_same_v<Polynomial, Args>&&...), "Polynomial Vector Function constructor requires all the arguments should be polynomial.");
//		using type = First;
//	};
//
//	std::vector<MathVector> polynomial_compare_node_set(const size_t polynomial_order, const size_t domain_dimension);
//	size_t combination(const size_t n, const size_t k);
//	size_t combination_with_repetition(const size_t n, const size_t k);
//	bool is_positive_odd_number(const double val);
//	bool is_natural_number(const double val);
//}