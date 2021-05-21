#include "../INC/Polynomial.h"

Monomial::Monomial(const size_t variable_index) {
	this->exponent_set_.resize(variable_index + 1);
	this->exponent_set_.back() = 1;
};

Monomial::Monomial(const std::initializer_list<size_t> list)
	: exponent_set_{ list } {
	this->remove_meaningless_zero();
};

Monomial& Monomial::operator*=(const Monomial& other) {
	if (other.is_one())
		return *this;

	if (this->is_one())
		return *this = other;

	const auto other_size = other.exponent_set_.size();
	if (this->exponent_set_.size() <= other_size)
		this->exponent_set_.resize(other_size);

	for (size_t i = 0; i < other_size; ++i)
		this->exponent_set_[i] += other.exponent_set_[i];

	this->remove_meaningless_zero();
	return *this;
}

Monomial Monomial::operator*(const Monomial& other) const {
	Monomial result = *this;
	return result *= other;
}

Monomial Monomial::operator^(const size_t power) const {
	if (this->is_one())
		return *this;
		
	if (power == 0)
		return Monomial();
	
	Monomial result(*this);
	for (auto& exponent : result.exponent_set_)
		exponent *= power;

	return result;
}

double Monomial::operator()(void) const {
	if (!this->is_one())
		throw std::invalid_argument("variable monomial need variable values");
	else
		return 1.0;
}

double Monomial::operator()(const MathVector& variable_value_vector) const {
	if (this->exponent_set_.size() > variable_value_vector.size())
		throw std::invalid_argument("given variables are not enough");

	if (this->is_one())
		return 1.0;

	double result = 1.0;
	for (size_t i = 0; i < this->exponent_set_.size(); ++i)
		result *= std::pow(variable_value_vector[i], this->exponent_set_[i]);

	return result;
}

bool Monomial::operator<(const Monomial& other) const {
	const auto this_order = this->order();
	const auto other_order = other.order();

	if (this_order == other_order) {
		const auto min_set_size = std::min(this->exponent_set_.size(), other.exponent_set_.size());
		for (size_t i = 0; i < min_set_size; ++i) {
			if (this->exponent_set_[i] == other.exponent_set_[i])
				continue;

			return this->exponent_set_[i] < other.exponent_set_[i];
		}
		return false;
	}

	return this_order < other_order;
}

bool Monomial::operator==(const Monomial& other) const {
	return  this->exponent_set_ == other.exponent_set_;
}

size_t Monomial::exponent(size_t variable_index) const {
	if (this->exponent_set_.size() <= variable_index || this->is_one())
		return 0;

	return this->exponent_set_[variable_index];
}

size_t Monomial::order(void) const {
	if (this->is_one())
		return 0;

	size_t order = 0;
	for (const auto exponent : this->exponent_set_)
		order += exponent;
	return order;
}

size_t Monomial::domain_dimension(void) const {
	return this->exponent_set_.size();
}

Monomial& Monomial::reduce_order(const size_t variable_index) {
	if (this->exponent(variable_index) == 0)
		throw "exponent can not be negative";
	else
		this->exponent_set_[variable_index]--;

	this->remove_meaningless_zero();
	return *this;
}

std::string Monomial::to_string(void) const {
	if (this->is_one())
		return "(1)";

	std::string str;
	for (size_t i = 0; i < this->exponent_set_.size(); ++i) {
		if (this->exponent_set_[i] == 0)
			continue;
		
		if (this->exponent_set_[i] == 1)
			str += "(x" + std::to_string(i) + ")";
		else
			str += "(x" + std::to_string(i) + ")^" + std::to_string(this->exponent_set_[i]);
	}

	return str;
}

void Monomial::remove_meaningless_zero(void) {
	const auto is_zero = [](size_t exponent) {return exponent == 0; };
	const auto pos_iter = std::find_if_not(this->exponent_set_.rbegin(), this->exponent_set_.rend(), is_zero);
	const auto num_zero = pos_iter - this->exponent_set_.rbegin();

	this->exponent_set_.erase(this->exponent_set_.end() - num_zero, this->exponent_set_.end());
}

bool Monomial::is_one(void) const {
	return this->exponent_set_.empty();
}

std::ostream& operator<<(std::ostream& ostream, const Monomial& monomial) {
	return ostream << monomial.to_string();
}

PolyTerm::PolyTerm(void) {
	this->coefficient_vector_.push_back(0.0);
	this->monomial_vector_function_.push_back(Monomial());
}

PolyTerm::PolyTerm(const double scalar) {
	this->coefficient_vector_.push_back(scalar);
	this->monomial_vector_function_.push_back(Monomial());
}

PolyTerm::PolyTerm(const Monomial& monomial) {
	this->coefficient_vector_.push_back(1.0);
	this->monomial_vector_function_.push_back(monomial);
}

PolyTerm::PolyTerm(const std::vector<double>& coefficient_vector, const std::vector<Monomial>& monomial_set) {
	const auto num_term = coefficient_vector.size();
	if (num_term != monomial_set.size())
		throw std::length_error("Size Error");

	for (size_t i = 0; i < num_term; ++i)
		this->add_mono_term(coefficient_vector[i], monomial_set[i]);
}

PolyTerm& PolyTerm::operator*=(const PolyTerm& other) {
	if (!this->has_constant_front())
		this->make_constant_front();

	if (other.is_simple() && other.has_constant_front()) {
		this->constant_front() *= other.constant_front();
		return *this;
	}
	
	if (other.is_simple())
		this->simple_term_multiplication(other);
	else {		
		this->constant_front() *= other.constant_front();
		for (const auto& other_term : other.multiplied_term_set_)
			this->simple_term_multiplication(other_term);
	}
	return *this;
}

PolyTerm PolyTerm::operator*(const PolyTerm& other) const {
	PolyTerm result(*this);
	return result *= other;
}

PolyTerm PolyTerm::operator^(const double power_index) const {
	PolyTerm result(*this);
	return result.power(power_index);
}

double PolyTerm::operator()(const MathVector& variable_vector) const {
	if (this->is_simple())
		return this->simple_term_calculation(variable_vector);

	double result = this->constant_front();
	for (const auto& term : this->multiplied_term_set_) 
		result *= term.simple_term_calculation(variable_vector);	
	return result;
}


bool PolyTerm::operator==(const PolyTerm& other) const {
	const auto max_order = std::max(this->order(), other.order());
	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);

	for (const auto& compare_node : compare_node_set) {
		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
			return false;
	}

	return true;
}

size_t PolyTerm::domain_dimension(void) const {
	if (this->is_simple())
		return this->simple_term_domain_dimension();

	size_t result = 0;
	for (const auto& term : this->multiplied_term_set_)
		result = std::max(result, term.simple_term_domain_dimension());
	return result;
}

size_t PolyTerm::order(void) const {
	if (this->is_simple())
		return this->simple_term_order();

	size_t result = 0;
	for (const auto& term : this->multiplied_term_set_)
		result += term.simple_term_order();
	return result;
}

std::string PolyTerm::to_string(void) const {
	if (this->is_simple())
		return this->simple_term_to_string();

	std::string str;
	if (this->coefficient_vector_.front() != 1.0)
		str += ms::double_to_string(this->coefficient_vector_.front());

	for (const auto& term : this->multiplied_term_set_)
		str += "X" + term.simple_term_to_string();	

	const auto pos = str.find('X');
	const auto size = 1;
	str.erase(pos,size);

	return str;
}

void PolyTerm::add_mono_term(const double coefficient, const Monomial& monomial) {
	if (coefficient == 0.0)
		return;

	if (this->has_constant_front() && this->constant_front() == 0.0) {
		this->coefficient_vector_.front() = coefficient;
		this->monomial_vector_function_.front() = monomial;
		return;
	}

	const auto monomial_iter = std::find(this->monomial_vector_function_.begin(), this->monomial_vector_function_.end(), monomial);
	if (monomial_iter == this->monomial_vector_function_.end()) {
		this->monomial_vector_function_.push_back(monomial);
		this->coefficient_vector_.push_back(coefficient);
	}
	else {
		const auto coefficient_iter = coefficient_vector_.begin() + (monomial_iter - monomial_vector_function_.begin());
		*coefficient_iter += coefficient;
		if (*coefficient_iter == 0) {
			this->monomial_vector_function_.erase(monomial_iter);
			this->coefficient_vector_.erase(coefficient_iter);
		}
	}
}

PolyTerm PolyTerm::base(void) const {
	PolyTerm result = *this;
	result.power_index_ = 1.0;
	return result;
}

double PolyTerm::constant_front(void) const {
	return this->coefficient_vector_.front();
}

double& PolyTerm::constant_front(void) {
	return this->coefficient_vector_.front();
}

void PolyTerm::make_constant_front(void) {
	PolyTerm result = 1;
	result.multiplied_term_set_.push_back(std::move(*this));
	*this = std::move(result);
}

//std::vector<Term> Term::differentiate(const size_t variable_index) const {
//	std::vector<Term> result;
//	if (this->is_constant()) {
//		result.push_back(Term());
//		return result;
//	}
//	if (this->is_simple()) {
//		auto tmp = *this;
//		result.push_back(std::move(tmp.simple_term_differentiate(variable_index)));
//		return result;
//	}
//
//	for (size_t i = 0; i < this->multiplied_term_set_.size(); ++i) {
//		auto tmp = *this;
//		tmp.multiplied_term_set_[i]
//	}
//}

PolyTerm& PolyTerm::power(const double power_index) {
	if (power_index == 0)
		return *this = 1;

	if (!this->has_constant_front())
		this->make_constant_front();

	this->constant_front() = std::pow(this->constant_front(), power_index);
	for (auto& term : this->multiplied_term_set_)
		term.power_index_ *= power_index;
	return *this;
}

bool PolyTerm::has_constant_front(void) const {
	return this->coefficient_vector_.size() == 1
		&& this->monomial_vector_function_.size() == 1
		&& this->monomial_vector_function_.front().is_one();
}

bool PolyTerm::is_simple(void) const {
	return this->multiplied_term_set_.empty();
}

PolyTerm& PolyTerm::simple_term_addition(const PolyTerm& other) {
	const auto num_other_term = other.coefficient_vector_.size();
	for (size_t i = 0; i < num_other_term; ++i)
		this->add_mono_term(other.coefficient_vector_[i], other.monomial_vector_function_[i]);
	return *this;
}

double PolyTerm::simple_term_calculation(const MathVector& variable_vector) const {
	const auto monomial_value_vector = this->monomial_vector_function_(variable_vector);
	const auto base_value = this->coefficient_vector_.inner_product(monomial_value_vector);
	return std::pow(base_value, this->power_index_); 
}

PolyTerm& PolyTerm::simple_term_differentiate(const size_t variable_index) {
	//differetiate term
	for (size_t i = 0; i < this->coefficient_vector_.size(); ++i) {
		const auto exponent = this->monomial_vector_function_[i].exponent(variable_index);

		this->coefficient_vector_[i] *= exponent;
		if (coefficient_vector_[i] != 0)
			this->monomial_vector_function_[i].reduce_order(variable_index);
	}

	//remove zero term
	constexpr double zero_coefficient = 0.0;
	for (;;) {
		auto iter = std::find(this->coefficient_vector_.begin(), this->coefficient_vector_.end(), zero_coefficient);
		if (iter == this->coefficient_vector_.end())
			break;

		const auto position = iter - this->coefficient_vector_.begin();
		this->coefficient_vector_.erase(this->coefficient_vector_.begin() + position);
		this->monomial_vector_function_.erase(this->monomial_vector_function_.begin() + position);
	}
	return *this;
}

PolyTerm& PolyTerm::simple_term_multiplication(const PolyTerm& other) {
	for (auto& term : this->multiplied_term_set_) {
		if (term.base() == other.base()) {
			term.power_index_ += other.power_index_;
			return *this;
		}
	}
	this->multiplied_term_set_.push_back(other);
	return *this;
}

size_t PolyTerm::simple_term_domain_dimension(void) const {
	std::vector<size_t> monomial_domain_dimension_set;
	for (const auto& monomial : this->monomial_vector_function_)
		monomial_domain_dimension_set.push_back(monomial.domain_dimension());

	return *std::max_element(monomial_domain_dimension_set.begin(), monomial_domain_dimension_set.end());
}

size_t PolyTerm::simple_term_order(void) const {
	std::vector<size_t> monomial_order_set;
	for (const auto& monomial : this->monomial_vector_function_)
		monomial_order_set.push_back(monomial.order());

	return *std::max_element(monomial_order_set.begin(), monomial_order_set.end());
}

std::string PolyTerm::simple_term_to_string(void) const {
	std::string str = "[";

	const auto num_term = this->coefficient_vector_.size();
	std::map<Monomial, double> ordered_term;
	for (size_t i = 0; i < num_term; ++i)
		ordered_term.try_emplace(this->monomial_vector_function_[i], this->coefficient_vector_[i]);

	for (auto iter = ordered_term.rbegin(); iter != ordered_term.rend(); ++iter) {
		const auto& [monomial, coefficient] = *iter;
		if (iter != ordered_term.rbegin() && coefficient > 0)
			str += "+";
		if (monomial.is_one())
			str += ms::double_to_string(coefficient);
		else if (coefficient == 1)
			str += monomial.to_string();
		else if (coefficient == -1)
			str += "-" + monomial.to_string();
		else
			str += ms::double_to_string(coefficient) + monomial.to_string();
	}

	str += "]";

	if (this->power_index_ != 1.0)
		str += "^" + ms::double_to_string(this->power_index_);

	return str;
}

std::ostream& operator<<(std::ostream& ostream, const PolyTerm& term) {
	return ostream << term.to_string();
}

//PolyTerm operator+(const Monomial& monomial1, const Monomial& monomial2) {
//	PolyTerm result;
//	result.add_mono_term(1.0, monomial1);
//	result.add_mono_term(1.0, monomial2);
//	return result;
//}
//
//PolyTerm operator-(const Monomial& monomial1, const Monomial& monomial2) {
//	PolyTerm result;
//	result.add_mono_term(1.0, monomial1);
//	result.add_mono_term(-1.0, monomial2);
//	return result;
//}

PolyTerm operator*(const double scalar, const PolyTerm& poly_term){
	return poly_term * scalar;
}


Polynomial::Polynomial(const PolyTerm& poly_term) {
	this->add_term(1.0, poly_term);
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
	const auto num_other_term = other.coefficient_vector_.size();
	this->simple_term_.simple_term_addition(other.simple_term_);

	for (size_t i = 0; i < num_other_term; ++i)
		this->add_term(other.coefficient_vector_[i], other.poly_term_vector_function_[i]);
	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
	return *this += (-1 * other);
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {
	Polynomial result;

	const auto num_this_term = this->coefficient_vector_.size();
	const auto num_other_term = other.coefficient_vector_.size();
	for (size_t i = 0; i < num_this_term; ++i)
		for (size_t j = 0; j < num_other_term; ++j)
			result.add_term(this->coefficient_vector_[i] * other.coefficient_vector_[j], this->poly_term_vector_function_[i] * other.poly_term_vector_function_[j]);

	for (size_t i = 0; i < num_this_term; ++i)
		result.add_term(this->coefficient_vector_[i], this->poly_term_vector_function_[i] * other.simple_term_);

	for (size_t i = 0; i < num_other_term; ++i)
		result.add_term(other.coefficient_vector_[i], other.poly_term_vector_function_[i] * this->simple_term_);

	result.add_term(1.0, this->simple_term_ * other.simple_term_);

	return *this = std::move(result);
}

Polynomial Polynomial::operator+(const PolyTerm& poly_term) const { //scalar, monomial 등이 polyterm으로 형변환 한번 되고
	Polynomial result(*this);
	return result += poly_term; // polyterm이 polynomial로 형변환 한번 되고
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
	Polynomial result(*this);
	return result += other;
}

Polynomial Polynomial::operator-(const PolyTerm& poly_term) const {
	Polynomial result(*this);
	return result -= poly_term;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
	Polynomial result(*this);
	return result -= other;
}

Polynomial Polynomial::operator*(const PolyTerm& poly_term) const {
	Polynomial result(*this);
	return result *= poly_term;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
	Polynomial result(*this);
	return result *= other;
}

double Polynomial::operator()(const MathVector& variable_vector) const {
	const auto poly_term_value_vector = this->poly_term_vector_function_(variable_vector);
	return this->coefficient_vector_.inner_product(poly_term_value_vector) + this->simple_term_(variable_vector);
}

bool Polynomial::operator==(const Polynomial& other) const {
	const auto max_order = std::max(this->order(), other.order());
	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);

	for (const auto& compare_node : compare_node_set) {
		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
			return false;
	}

	return true;
}

size_t Polynomial::domain_dimension(void) const {
	size_t result = this->simple_term_.domain_dimension();
	for (const auto& term : this->poly_term_vector_function_)
		result = std::max(result, term.domain_dimension());
	return result;

}

size_t Polynomial::order(void) const {
	size_t result = this->simple_term_.order();
	for (const auto& term : this->poly_term_vector_function_)
		result = std::max(result, term.order());
	return result;
}

std::string Polynomial::to_string(void) const {
	std::string str;

	const auto num_term = this->coefficient_vector_.size();
	for (size_t i = 0; i < num_term; ++i) {
		const auto coefficient = this->coefficient_vector_[i];
		if (coefficient == 1.0)
			str += "+" + this->poly_term_vector_function_[i].to_string();
		else
			str += "+" + ms::double_to_string(coefficient) + this->poly_term_vector_function_[i].to_string();
	}

	if (!this->simple_term_.has_constant_front() || this->simple_term_.constant_front() != 0.0)
		str += "+" + this->simple_term_.to_string();

	str.erase(str.begin());
	return str;
}

Polynomial& Polynomial::add_term(const double coefficient, const PolyTerm& term) {
	if (term.is_simple()) {
		this->simple_term_.simple_term_addition(term);
		return *this;
	}

	auto new_coefficient = coefficient * term.constant_front();
	auto new_term = term;	
	new_term.coefficient_vector_.front() = 1.0;
	
	if (new_coefficient == 0.0)
		return *this;

	const auto poly_term_iter = std::find(this->poly_term_vector_function_.begin(), this->poly_term_vector_function_.end(), new_term);
	if (poly_term_iter == this->poly_term_vector_function_.end()) {
		this->poly_term_vector_function_.push_back(std::move(new_term));
		this->coefficient_vector_.push_back(new_coefficient);
	}
	else {
		const auto position = poly_term_iter - this->poly_term_vector_function_.begin();
		const auto coefficient_iter = this->coefficient_vector_.begin() + position;
		*coefficient_iter += new_coefficient;

		if (*coefficient_iter == 0) {
			this->poly_term_vector_function_.erase(poly_term_iter);
			this->coefficient_vector_.erase(coefficient_iter);
		}
	}
	return *this;
}

std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial) {
	return ostream << polynomial.to_string();
}

//Polynomial operator+(const PolyTerm& polyterm1, const PolyTerm& polyterm2) {
//	Polynomial result = polyterm1;
//	return result += polyterm2;
//}

//Polynomial operator+(const PolyTerm& poly_term, const Polynomial& polynomial) {
//	return polynomial + poly_term;
//}
//
//Polynomial operator-(const PolyTerm& poly_term, const Polynomial& polynomial) {
//	return -1 * polynomial += poly_term;
//}

Polynomial operator+(const PolyTerm& poly_term1, const PolyTerm& poly_term2) {
	Polynomial result = poly_term1;
	return result += poly_term2;
}

Polynomial operator-(const PolyTerm& poly_term1, const PolyTerm& poly_term2) {
	Polynomial result = poly_term1;
	return result -= poly_term2;

}

Polynomial operator*(const PolyTerm& poly_term, const Polynomial& polynomial) {
	return polynomial * poly_term;
}
































































//polynomial !
//const auto num_this_term = this->coefficient_vector_.size();
//const auto num_other_term = other.coefficient_vector_.size();

//Term result;
//for (size_t i = 0; i < num_this_term; ++i) {
//	const auto& this_coefficient = this->coefficient_vector_[i];
//	const auto& this_monomial = this->monomial_vector_function_[i];
//	for (size_t j = 0; j < num_other_term; ++j) {
//		const auto& other_coefficient = other.coefficient_vector_[j];
//		const auto& other_monomial = other.monomial_vector_function_[j];

//		const auto result_coefficient = this_coefficient * other_coefficient;
//		const auto result_monomial = this_monomial * other_monomial;
//		result.add_term(result_coefficient, result_monomial);
//	}
//}
//return *this = std::move(result);



//
//Polynomial::Polynomial(const double coefficient) {
//	this->simple_polynomial_addition_single_term(coefficient, Monomial());
//}
//
//Polynomial::Polynomial(const Monomial& monomial) {
//	this->simple_polynomial_addition_single_term(1.0, monomial);
//}
//
//Polynomial::Polynomial(const double coefficient, const Monomial& monomial) {
//	this->simple_polynomial_addition_single_term(coefficient, monomial);
//}
//
//Polynomial::Polynomial(const std::vector<double>& coefficient_vector, const std::vector<Monomial>& monomial_set) {
//	const size_t num_term = coefficient_vector.size();
//	if (num_term != monomial_set.size())
//		throw std::length_error("Size Error");
//
//	for (size_t i = 0; i < num_term; ++i)
//		this->simple_polynomial_addition_single_term(coefficient_vector[i], monomial_set[i]);
//}
//
//Polynomial& Polynomial::operator+=(const Polynomial& other) {
//	if (this->is_zero())
//		return *this = other;
//
//	if (other.is_zero())
//		return *this;
//
//	if (this->is_operable()) {
//		if (this->is_simple_polynomial() && other.is_simple_polynomial())
//			return this->simple_polynomial_addition_simple_polynomial(other);
//		else if (this->is_simple_polynomial() && !other.is_simple_polynomial()) {
//			auto tmp = other;
//			tmp.nested_polynomial_addition_simple_polynomial(*this);
//			return *this = std::move(tmp);
//		}
//		else if (!this->is_simple_polynomial() && other.is_simple_polynomial())
//			return this->nested_polynomial_addition_simple_polynomial(other);
//		else
//			return this->nested_polynomial_addition_nested_polynomial(other);
//	}
//	else {
//		Polynomial tmp = 1;
//		tmp.calculated_polynomial_set_.reserve(3);
//		if (!other.is_simple_polynomial()) {
//			if (other.has_addable_simple_polynomial()) {
//				tmp.calculated_polynomial_set_.emplace_back(BinaryOperator::addition, other); // a += a를 다루기 위해 순서가 이렇게 됨
//				tmp.calculated_polynomial_set_.back().second.calculated_polynomial_set_.pop_back();
//				tmp.calculated_polynomial_set_.emplace_back(other.calculated_polynomial_set_.back());
//			}
//			else
//				tmp.calculated_polynomial_set_.emplace_back(BinaryOperator::addition, other);
//		}
//		else
//			tmp.calculated_polynomial_set_.emplace_back(BinaryOperator::addition, other);
//		tmp.calculated_polynomial_set_.emplace(tmp.calculated_polynomial_set_.begin(), BinaryOperator::multiplication, std::move(*this));
//		return *this = std::move(tmp);
//	}
//}
//
//Polynomial& Polynomial::operator-=(const Polynomial& other) {
//	auto temp = other * -1.0;
//	return *this += temp;
//}
//
//Polynomial& Polynomial::operator*=(const double scalar) {
//	if (this->is_operable() && this->is_scalar()) {
//		this->coefficient_vector_.front() *= scalar;
//		return *this;
//	}
//
//	if (this->is_operable() && this->has_scalar_front_term()) {
//		this->coefficient_vector_.front() *= scalar;
//		for (auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
//			if (binary_operator == BinaryOperator::addition)
//				polynomial *= scalar;
//		}
//		return *this;
//	}
//	else {
//		Polynomial tmp = scalar;
//		tmp.calculated_polynomial_set_.emplace_back(BinaryOperator::multiplication, std::move(*this));
//		return *this = std::move(tmp);
//	}
//}
//
//Polynomial& Polynomial::operator*=(const Polynomial& other) {	
//	if (this->is_zero() || other.is_zero())
//		return this->be_zero();
//
//	if (other.is_one())
//		return *this;
//
//	if (this->is_one())
//		return *this = other;
//
//	if (this->is_operable()) {
//		if (this->is_simple_polynomial()) {
//			if (other.is_single_term())
//				return this->simple_polynomial_multiplication(other);
//			else {
//				Polynomial tmp = 1 * (*this) * other;
//				return *this = std::move(tmp);
//			}
//		}
//		else {
//			this->calculated_polynomial_set_.emplace_back(BinaryOperator::multiplication, other);
//			return *this;
//		}
//
//
//
//		//if (this->is_simple_polynomial() && other.is_single_term())
//		//	return this->simple_polynomial_multiplication(other);
//
//		//this->calculated_polynomial_set_.emplace_back(BinaryOperator::multiplication, other);
//		//return *this;
//	}
//	else {
//		Polynomial tmp = 1 * (*this) * other;
//		return *this = std::move(tmp);
//
//		//Polynomial tmp = 1;
//		//tmp.calculated_polynomial_set_.reserve(2);
//		//tmp.calculated_polynomial_set_.emplace_back(BinaryOperator::multiplication, other);
//		//tmp.calculated_polynomial_set_.emplace(tmp.calculated_polynomial_set_.begin(), BinaryOperator::multiplication, std::move(*this));
//		//return *this = std::move(tmp);
//	}
//}
//
//Polynomial Polynomial::operator+(const Polynomial& other) const {
//	Polynomial result = *this;
//	return result += other;
//}
//
//Polynomial Polynomial::operator-(const Polynomial& other) const {
//	Polynomial result = *this;
//	return result -= other;
//}
//
//Polynomial Polynomial::operator*(const double scalar) const {
//	Polynomial result(*this);
//	return result *= scalar;
//}
//
//Polynomial Polynomial::operator*(const Polynomial& other) const {
//	Polynomial result(*this);
//	return result *= other;
//}
//
//Polynomial Polynomial::operator^(const double power_index) const {
//	Polynomial result(*this);
//	return result.power(power_index);
//}
//
//double Polynomial::operator()(const MathVector& variable_vector) const {
//	auto result = this->simple_polynomial_calculation(variable_vector);
//	for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
//		if (binary_operator == BinaryOperator::addition)
//			result += polynomial(variable_vector);
//		else
//			result *= polynomial(variable_vector);
//	}
//	if (this->absoulte_flag)
//		return std::abs(std::pow(result, this->power_index_));
//	else
//		return std::pow(result, this->power_index_);
//}
//
//bool Polynomial::operator==(const Polynomial& other) const {
//	//return this->compare_v1(other);
//	return this->compare_v2(other);
//}
//
//bool Polynomial::operator!=(const Polynomial& other) const {
//	return !((*this) == other);
//}
//
//Polynomial& Polynomial::differentiate(const size_t variable_index) {
//	if (this->absoulte_flag)
//		throw std::runtime_error("absolute function is not differentiable");
//
//	if (this->power_index_ == 1.0) {
//		if (this->is_simple_polynomial())
//			return this->simple_polynomial_differentiate(variable_index);
//		else {
//			auto [binary_operator, polynomial] = this->calculated_polynomial_set_.back();
//			this->calculated_polynomial_set_.pop_back();
//			if (binary_operator == BinaryOperator::addition)
//				return this->differentiate(variable_index) += polynomial.differentiate(variable_index);
//			else {
//				auto tmp_poly = ms::differentiate(*this, variable_index);
//				tmp_poly *= polynomial;
//
//				*this *= polynomial.differentiate(variable_index);
//				return *this += tmp_poly;
//			}
//		}
//	}
//	else {
//		auto tmp_poly = *this;
//		tmp_poly.power_index_ = 1.0;
//		tmp_poly *= this->power_index_;
//		tmp_poly.differentiate(variable_index);
//
//		--this->power_index_;
//		return *this *= tmp_poly;
//	}
//}
//
//VectorFunction<Polynomial> Polynomial::gradient(void) const {
//	const auto domain_dimension = this->domain_dimension();
//	return this->gradient(domain_dimension);
//}
//
//VectorFunction<Polynomial> Polynomial::gradient(const size_t domain_dimension) const {
//	VectorFunction<Polynomial> result;
//	result.reserve(domain_dimension);
//
//	for (size_t i = 0; i < domain_dimension; ++i)
//		result.push_back(ms::differentiate(*this, i));
//
//	return result;
//}
//
//Polynomial& Polynomial::power(const double power_index) {
//	if (power_index == 0)
//		return *this = 1;
//
//	if (this->is_zero()||this->is_one())
//		return *this;
//
//	if (ms::is_positive_odd_number(this->power_index_))
//		this->absoulte_flag = true;
//
//	this->power_index_ *= power_index;
//	return *this;
//}
//
//std::string Polynomial::to_string(void) const {
//	auto str = this->simple_polynomial_string();	
//	for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
//		switch (binary_operator) {
//		case  BinaryOperator::addition: {
//			str += "+" + polynomial.to_string();
//			break;
//		}
//		case BinaryOperator::multiplication: {
//			if (str.back() == ']') 
//				str += "X[" + polynomial.to_string() + "]";
//			else {
//				str.insert(0, "[");
//				str += "]X[" + polynomial.to_string() + "]";
//			}		
//			break;
//		}
//		}
//	}
//	if (this->absoulte_flag) {
//		str.insert(0, "|");
//		str += "|";
//	}
//	if (this->power_index_ != 1.0) {
//		str.insert(0, "[");
//		str += "]^" + ms::double_to_string(this->power_index_);
//	}
//
//	if (this->has_scalar_front_term() && this->coefficient_vector_.front() == 1) {
//		const auto pos = str.find("[1]X");
//		const auto size = 4;
//		str.erase(pos, size);
//	}
//
//	return str;
//}
//
//size_t Polynomial::domain_dimension(void) const {
//	if (this->is_simple_polynomial())
//		return this->simple_polynomial_domain_dimension();
//	else {
//		auto result = this->simple_polynomial_domain_dimension();
//		for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) 
//				result = std::max(result, polynomial.domain_dimension());
//		return result;
//	}
//}
//
//bool Polynomial::compare_v1(const Polynomial& other) const {
//	//유리함수는 to simple polynomial이 안됨!
//	const auto simplified_this_polynomial = this->to_simple_polynomial();
//	const auto simplified_other_polynomial = other.to_simple_polynomial();
//
//	const auto num_term = simplified_this_polynomial.coefficient_vector_.size();
//	if (num_term != simplified_other_polynomial.coefficient_vector_.size())
//		return false;
//
//	std::map<Monomial, double> ordered_this_polynomial;
//	std::map<Monomial, double> ordered_other_polynomial;
//	for (size_t i = 0; i < num_term; ++i) {
//		ordered_this_polynomial.try_emplace(simplified_this_polynomial.monomial_vector_function_[i], simplified_this_polynomial.coefficient_vector_[i]);
//		ordered_other_polynomial.try_emplace(simplified_other_polynomial.monomial_vector_function_[i], simplified_other_polynomial.coefficient_vector_[i]);
//	}
//
//	return ordered_this_polynomial == ordered_other_polynomial;
//}
//
//bool Polynomial::compare_v2(const Polynomial& other) const {
//	//유리함수의 basis는 뭐지 ??????????????
//	//power가 있을때 비교 어떻게 하지 ???????????
//	const auto max_polynomial_order = std::max(this->polynomial_order(), other.polynomial_order());
//	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
//	const auto compare_node_set = ms::polynomial_compare_node_set(max_polynomial_order,max_domain_dimension);
//
//	for (const auto& compare_node : compare_node_set) {
//		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
//			return false;
//	}
//
//	return true;
//}
//
//size_t Polynomial::polynomial_order(void) const {	
//	auto result = this->simple_polynomial_order();
//	if (this->is_simple_polynomial()) 
//		return result * static_cast<size_t>(std::ceil(this->power_index_));
//	else {		
//		for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
//			switch (binary_operator) {
//			case  BinaryOperator::addition:
//				result = std::max(result, polynomial.polynomial_order());
//				break;
//			case BinaryOperator::multiplication:
//				result += polynomial.polynomial_order();
//				break;
//			}
//		}
//		return result * static_cast<size_t>(std::ceil(this->power_index_));
//	}
//}
//
//double Polynomial::simple_polynomial_calculation(const MathVector& variable_vector) const {
//	const auto monomial_value_vector = this->monomial_vector_function_(variable_vector);
//	return this->coefficient_vector_.inner_product(monomial_value_vector);
//}
//
//Polynomial& Polynomial::simple_polynomial_differentiate(const size_t variable_index) {
//	//differetiate term
//	for (size_t i = 0; i < this->coefficient_vector_.size(); ++i) {
//		const auto exponent = this->monomial_vector_function_[i].exponent(variable_index);
//
//		this->coefficient_vector_[i] *= exponent;
//		if (coefficient_vector_[i] != 0)
//			this->monomial_vector_function_[i].reduce_order(variable_index);
//	}
//
//	//remove zero term
//	constexpr double zero_coefficient = 0.0;
//	for (;;) {
//		auto iter = std::find(this->coefficient_vector_.begin(), this->coefficient_vector_.end(), zero_coefficient);
//		if (iter == this->coefficient_vector_.end())
//			break;
//
//		const auto position = iter - this->coefficient_vector_.begin();
//		this->coefficient_vector_.erase(this->coefficient_vector_.begin() + position);
//		this->monomial_vector_function_.erase(this->monomial_vector_function_.begin() + position);
//	}
//
//	return *this;
//}
//
//Polynomial& Polynomial::simple_polynomial_addition_simple_polynomial(const Polynomial& other) {
//	for (size_t i = 0; i < other.coefficient_vector_.size(); ++i)
//		this->simple_polynomial_addition_single_term(other.coefficient_vector_[i], other.monomial_vector_function_[i]);
//	return *this;
//}
//
//Polynomial& Polynomial::nested_polynomial_addition_simple_polynomial(const Polynomial& other) {
//	if (this->has_addable_simple_polynomial()) {
//		auto& [binary_operator, simple_polynomial] = this->calculated_polynomial_set_.back();
//		simple_polynomial.simple_polynomial_addition_simple_polynomial(other);
//	}
//	else
//		this->calculated_polynomial_set_.emplace_back(BinaryOperator::addition, other);
//	return *this;
//}
//
//Polynomial& Polynomial::nested_polynomial_addition_nested_polynomial(const Polynomial& other) {
//	if (this->has_addable_simple_polynomial()) {
//		this->calculated_polynomial_set_.emplace(this->calculated_polynomial_set_.end() - 1, BinaryOperator::addition, other);
//		if (other.has_addable_simple_polynomial()) {
//			const auto& [other_binary_operator, other_simple_polynomial] = other.calculated_polynomial_set_.back();
//			auto& [this_binary_operator, this_simple_polynomial] = this->calculated_polynomial_set_.back();
//			this_simple_polynomial.simple_polynomial_addition_simple_polynomial(other_simple_polynomial);
//
//			auto& [this_binary_operator2, this_polynomial2] = *(this->calculated_polynomial_set_.end() - 2);
//			this_polynomial2.calculated_polynomial_set_.pop_back();
//		}
//	}
//	else {
//		this->calculated_polynomial_set_.emplace_back(BinaryOperator::addition, other);
//		if (other.has_addable_simple_polynomial()) {
//			auto& [this_binary_operator, this_polynomial] = this->calculated_polynomial_set_.back();
//			this_polynomial.calculated_polynomial_set_.pop_back();
//			const auto& [other_binary_operator, other_simple_polynomial] = other.calculated_polynomial_set_.back();
//			this->calculated_polynomial_set_.emplace_back(BinaryOperator::addition, other_simple_polynomial);
//		}
//	}
//	return *this;
//}
//
////void Polynomial::simple_polynomial_scalar_multiplication(const double scalar) {
////	if (scalar == 0)
////		this->be_zero();
////
////	for (auto& coefficient : this->coefficient_vector_)
////		coefficient *= scalar;
////}
//
//Polynomial& Polynomial::simple_polynomial_multiplication(const Polynomial& other) {
//	const auto num_this_term = this->coefficient_vector_.size();
//	const auto num_other_term = other.coefficient_vector_.size();
//
//	Polynomial result;
//	for (size_t i = 0; i < num_this_term; ++i) {
//		const auto& this_coefficient = this->coefficient_vector_[i];
//		const auto& this_monomial = this->monomial_vector_function_[i];
//		for (size_t j = 0; j < num_other_term; ++j) {
//			const auto& other_coefficient = other.coefficient_vector_[j];
//			const auto& other_monomial = other.monomial_vector_function_[j];
//
//			const auto result_coefficient = this_coefficient * other_coefficient;
//			const auto result_monomial = this_monomial * other_monomial;
//			result.simple_polynomial_addition_single_term(result_coefficient, result_monomial);
//		}
//	}
//	return *this = std::move(result);
//}
//
//bool Polynomial::is_zero(void) const {
//	return this->coefficient_vector_.empty() && this->monomial_vector_function_.empty() && this->is_simple_polynomial();
//}
//
//bool Polynomial::is_one(void) const {
//	if (!this->is_simple_polynomial())
//		return false;
//
//	if (this->coefficient_vector_.size() != 1 || this->monomial_vector_function_.size() != 1)
//		return false;
//
//	if (this->coefficient_vector_.front() == 1 && this->monomial_vector_function_.front() == Monomial())
//		return true;
//	else
//		return false;
//}
//
//bool Polynomial::is_operable(void) const {
//	return !this->absoulte_flag && this->power_index_ == 1.0;
//}
//
//bool Polynomial::is_simple_polynomial(void) const {
//	return this->calculated_polynomial_set_.empty();
//}
//
//bool Polynomial::is_single_term(void) const {
//	if (!this->is_simple_polynomial())
//		return false;
//	else
//		return this->coefficient_vector_.size() == 1;
//}
//
//bool Polynomial::is_scalar(void) const {
//	if (!this->is_simple_polynomial())
//		return false;
//
//	return this->monomial_vector_function_.size() == 1 && this->monomial_vector_function_.front().is_constant();
//}
//
//bool Polynomial::is_same_term(const Polynomial& other) const {
//	if (this->has_scalar_front_term()) {
//		if (!other.has_scalar_front_term())
//			return false;
//		auto tmp1 = *this;
//		tmp1.coefficient_vector_.front() = 1;
//		auto tmp2 = other;
//		tmp2.coefficient_vector_.front() = 1;
//
//		return tmp1 == tmp2;
//	}
//	else {
//		if (other.has_scalar_front_term())
//			return false;
//
//		return *this == other;	
//	}
//}
//
//bool Polynomial::has_addable_simple_polynomial(void) const {
//	const auto& [binary_operator, polynomial] = this->calculated_polynomial_set_.back();
//	return binary_operator == BinaryOperator::addition && polynomial.is_simple_polynomial();
//}
//
//bool Polynomial::has_scalar_front_term(void) const {
//	if (this->is_simple_polynomial())
//		return false;
//
//	return this->monomial_vector_function_.size() == 1 && this->monomial_vector_function_.front().is_constant();
//}
//
//std::string Polynomial::simple_polynomial_string(void) const {
//	if (this->is_zero())
//		return "0";
//
//	std::string str;
//
//	const auto num_term = this->coefficient_vector_.size();
//	std::map<Monomial, double> ordered_this_polynomial;
//	for (size_t i = 0; i < num_term; ++i)
//		ordered_this_polynomial.try_emplace(this->monomial_vector_function_[i], this->coefficient_vector_[i]);
//
//	for (auto iter = ordered_this_polynomial.rbegin(); iter != ordered_this_polynomial.rend(); ++iter) {
//		const auto& [monomial, coefficient] = *iter;
//		if (iter != ordered_this_polynomial.rbegin() && coefficient > 0)
//			str += "+";
//		if (monomial.is_constant())
//			str += ms::double_to_string(coefficient);
//		else if (coefficient == 1)
//			str += monomial.to_string();
//		else if (coefficient == -1)
//			str += "-" + monomial.to_string();
//		else
//			str += ms::double_to_string(coefficient) + monomial.to_string();
//	}
//
//	return str;
//}
//
//Polynomial operator+(const double scalar, const Polynomial& polynomial) {
//	return polynomial + scalar;
//}
//
//Polynomial operator+(const Monomial& monomial, const Polynomial& polynomial) {
//	return polynomial + monomial;
//}
//
//Polynomial operator-(const Monomial& monomial, const Polynomial& polynomial) {
//	return -1 * polynomial + monomial;
//}
//
//Polynomial operator*(const double scalar, const Polynomial& polynomial){
//	return polynomial * scalar;
//}
//
//std::ostream& operator<<(std::ostream& ostream, const Polynomial& monomial) {
//	return ostream << monomial.to_string();
//}
//
//size_t Polynomial::simple_polynomial_domain_dimension(void) const {
//	if (this->is_zero())
//		return 0;
//	
//	std::vector<size_t> monomial_domain_dimension_set;
//	for (const auto& monomial : this->monomial_vector_function_)
//		monomial_domain_dimension_set.push_back(monomial.domain_dimension());
//
//	return *std::max_element(monomial_domain_dimension_set.begin(), monomial_domain_dimension_set.end());
//}
//
//size_t Polynomial::simple_polynomial_order(void) const {
//	if (this->is_zero())
//		return 0;
//
//	std::vector<size_t> monomial_order_set;
//	for (const auto& monomial : this->monomial_vector_function_)
//		monomial_order_set.push_back(monomial.order());
//
//	return *std::max_element(monomial_order_set.begin(), monomial_order_set.end());
//}
//
//Polynomial Polynomial::to_simple_polynomial(void) const {
//	if (this->power_index_ != 1.0)
//		throw std::runtime_error("polynomial with power can not be simple polynomial in general.");
//
//	Polynomial result(this->coefficient_vector_,this->monomial_vector_function_);
//
//	for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
//		switch (binary_operator) {
//		case  BinaryOperator::addition:
//			result.simple_polynomial_addition_simple_polynomial(polynomial.to_simple_polynomial());
//			break;
//		case BinaryOperator::multiplication:
//			result.simple_polynomial_multiplication(polynomial.to_simple_polynomial());
//			break;
//		default:
//			throw std::runtime_error("it can not be a simple polynomial");
//		}
//	}
//	
//	return result;
//}
//
//Polynomial& Polynomial::simple_polynomial_addition_single_term(const double coefficient, const Monomial& monomial) {
//	if (coefficient == 0.0)
//		return *this;
//
//	const auto monomial_iter = std::find(this->monomial_vector_function_.begin(), this->monomial_vector_function_.end(), monomial);
//	if (monomial_iter == this->monomial_vector_function_.end()) {
//		this->monomial_vector_function_.push_back(monomial);
//		this->coefficient_vector_.push_back(coefficient);
//	}
//	else {
//		const auto coefficient_iter = coefficient_vector_.begin() + (monomial_iter - monomial_vector_function_.begin());
//		*coefficient_iter += coefficient;
//		if (*coefficient_iter == 0) {
//			this->monomial_vector_function_.erase(monomial_iter);
//			this->coefficient_vector_.erase(coefficient_iter);
//		}
//	}
//
//	return *this;
//}
//
//Polynomial& Polynomial::be_zero(void) {
//	this->coefficient_vector_.clear();
//	this->monomial_vector_function_.clear();
//	this->calculated_polynomial_set_.clear();
//	return *this;
//}
//
//
namespace ms {
	std::vector<MathVector> polynomial_compare_node_set(const size_t polynomial_order, const size_t domain_dimension) {
		const auto num_node = ms::combination_with_repetition(polynomial_order + 1, domain_dimension);

		std::vector<MathVector> compare_node_set;
		compare_node_set.reserve(num_node);

		MathVector compare_node(domain_dimension);
		if (domain_dimension == 0) {
			compare_node_set.push_back(compare_node);
			return compare_node_set;
		}

		while (true) {
			auto iter = std::find(compare_node.begin(), compare_node.end(), polynomial_order);
			if (iter != compare_node.end()) {
				compare_node_set.push_back(compare_node);

				if (iter == compare_node.begin())
					break;

				std::fill(compare_node.begin(), compare_node.end(), 0);
				(*(--iter))++;

				if (compare_node.front() == polynomial_order) {
					compare_node_set.push_back(compare_node);
					break;
				}

			}

			double component_sum = 0;
			for (const auto& val : compare_node)
				component_sum += val;

			if (component_sum == polynomial_order) {
				compare_node_set.push_back(compare_node);
				const auto is_zero = [](const double i) {return i == 0; };
				auto iter = std::find_if_not(compare_node.rbegin(), compare_node.rend(), is_zero);
				*iter = 0;
				(*(++iter))++;
				continue;
			}

			compare_node_set.push_back(compare_node);
			compare_node.back()++;
		}

		return compare_node_set;
	}

	size_t combination(const size_t n, const size_t k) {
		//calculate nCk
		//the combination of n things taken k at a time without repetition.
		if (n == k || k == 0)
			return 1;
		else
			return combination(n - 1, k - 1) + combination(n - 1, k);
	}

	size_t ms::combination_with_repetition(const size_t n, const size_t k) {
		//calculate nHk
		//the combination of n things taken k at a time with repetition.
		return combination(n + k - 1, k);
	}

	//Polynomial differentiate(const Polynomial& polynomial, const size_t variable_index) {
	//	auto result = polynomial;
	//	return result.differentiate(variable_index);
	//}

	//Polynomial sqrt(const Polynomial& polynomial) {
	//	return polynomial^(0.5);		
	//}

	bool is_positive_odd_number(const double val) {
		if (val < 0)
			return false;

		if (val - std::floor(val) == 0)
			return static_cast<size_t>(val) % 2 == 0;
		else
			return false;
	}
}