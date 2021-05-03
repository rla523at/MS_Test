#include "../INC/Polynomial.h"

Monomial::Monomial(void) {
	this->is_constant_ = true;
}

Monomial::Monomial(const size_t variable_index) { 
	this->exponent_set_.resize(variable_index + 1);
	this->exponent_set_.back() = 1;
};

Monomial::Monomial(const std::initializer_list<size_t> list)
	: exponent_set_{ list } {
	this->remove_meaningless_zero();
	if (this->exponent_set_.empty())
		this->is_constant_ = true;
};

Monomial::Monomial(std::vector<size_t>&& exponent_set)
	: exponent_set_(std::move(exponent_set)) {
	this->remove_meaningless_zero();
	if (this->exponent_set_.empty())
		this->is_constant_ = true;
};

Monomial& Monomial::operator*=(const Monomial& other) {
	const auto this_size = this->exponent_set_.size();
	const auto other_size = other.exponent_set_.size();

	if (this_size <= other_size) 
		this->exponent_set_.resize(other_size);

	for (size_t i = 0; i < other_size; ++i)
		this->exponent_set_[i] += other.exponent_set_[i];

	this->remove_meaningless_zero();
	if (this->exponent_set_.empty())
		this->is_constant_ = true;

	return *this;
}

Monomial Monomial::operator*(const Monomial& other) const {
	Monomial result = *this;
	return result *= other;
}

double Monomial::operator()(void) const {
	if (!this->is_constant_)
		throw std::invalid_argument("variable monomial need variable values");
	else
		return 1.0;
}

double Monomial::operator()(const MathVector& variable_value_vector) const {
	if (this->exponent_set_.size() > variable_value_vector.size())
		throw std::invalid_argument("given variables are not enough");

	if (this->is_constant_)
		return 1.0;

	double result = 1.0;
	for (size_t i = 0; i < this->exponent_set_.size(); ++i)
		result *= std::pow(variable_value_vector[i], this->exponent_set_[i]);

	return result;
}

bool Monomial::operator<(const Monomial& other) const {
	const auto this_order = this->monomial_order();
	const auto other_order = other.monomial_order();

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
	if (this->exponent_set_ == other.exponent_set_)
		return true;
	else
		return false;
}

size_t Monomial::exponent(size_t variable_index) const {
	if (this->exponent_set_.size() <= variable_index || this->is_constant_)
		return 0;
	
	return this->exponent_set_[variable_index];
}

size_t Monomial::monomial_order(void) const {
	if (this->is_constant_)
		return 0;

	size_t order = 0;
	for (const auto exponent : this->exponent_set_)
		order += exponent;
	return order;
}

size_t Monomial::domain_order(void) const {
	return this->exponent_set_.size();
}

Monomial& Monomial::reduce_order(const size_t variable_index) {
	if (this->exponent(variable_index) == 0)
		throw "exponent can not be negative";
	else
		this->exponent_set_[variable_index]--;

	this->remove_meaningless_zero();
	if (this->exponent_set_.empty())
		this->is_constant_ = true;
	
	return *this;
}

std::string Monomial::to_string(void) const {	
	if (this->is_constant_) 
		return "(1)";

	std::string str;
	for (size_t i = 0; i < this->exponent_set_.size(); ++i) {
		if (this->exponent_set_[i] == 0)
			continue;
		else
			str += "(x" + std::to_string(i) + ")^" + std::to_string(this->exponent_set_[i]);
	}

	return str;
}

void Monomial::remove_meaningless_zero(void) {
	const auto is_zero = [](size_t exponent) {return exponent == 0; };
	const auto pos_iter = std::find_if_not(this->exponent_set_.rbegin(), this->exponent_set_.rend(), is_zero);
	const auto num_zero = pos_iter - this->exponent_set_.rbegin();

	this->exponent_set_.erase(this->exponent_set_.end()-num_zero, this->exponent_set_.end());
}

std::ostream& operator<<(std::ostream& ostream, const Monomial& monomial) {
	return ostream << monomial.to_string();
}



Polynomial::Polynomial(void) {
	this->coefficient_vector_.push_back(0.0);
	this->monomial_set_.emplace_back();
}

Polynomial::Polynomial(const double coefficient) {
	this->coefficient_vector_.push_back(coefficient);
	this->monomial_set_.emplace_back();
}

Polynomial::Polynomial(const Monomial& monomial) {
	this->coefficient_vector_.push_back(1.0);
	this->monomial_set_.push_back(monomial);
}

Polynomial::Polynomial(const double coefficient, const Monomial& monomial) {
	this->coefficient_vector_.push_back(coefficient);
	this->monomial_set_.push_back(monomial);
}

Polynomial::Polynomial(const std::vector<double>& coefficient_set, const std::vector<Monomial>& monomial_set) {
	if (coefficient_set.size() != monomial_set.size())
		throw "Size Error";

	this->coefficient_vector_ = coefficient_set;
	this->monomial_set_ = monomial_set;
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
	if (other.is_zero())
		return *this;

	if (this->is_simple_polynomial() && other.is_simple_polynomial())
		this->addition(other);
	else
		this->calculated_polynomial_set_.emplace_back(BinaryOperator::addition, other);
	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
	auto temp = other * -1.0;
	return *this += temp;
}

Polynomial& Polynomial::operator*=(const double scalar) {
	if (this->power_index_ == 1) {
		this->scalar_multiplication(scalar);
		for (auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
			if (binary_operator == BinaryOperator::addition)
				polynomial.scalar_multiplication(scalar);
		}
	}
	else
		*this *= Polynomial(scalar);

	return *this;
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {	
	if (this->is_zero() || other.is_zero())
		*this = Polynomial();

	if (other == Polynomial(1))
		return *this;

	if (*this == other)
		this->power_index_++;
	else
		this->calculated_polynomial_set_.emplace_back(BinaryOperator::multiplication, other);

	return *this;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
	Polynomial result = *this;
	return result += other;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
	Polynomial result = *this;
	return result -= other;
}

Polynomial Polynomial::operator*(const double scalar) const {
	Polynomial result(*this);
	return result *= scalar;
}


Polynomial Polynomial::operator*(const Polynomial& other) const {
	Polynomial result(*this);
	return result *= other;
}

double Polynomial::operator()(const MathVector& variable_vector) const {
	if (this->is_simple_polynomial())
		return this->calculate(variable_vector);
	else {
		auto result = this->calculate(variable_vector);
		for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
			switch (binary_operator) {
			case  BinaryOperator::addition:
				result += polynomial(variable_vector);
				break;
			case BinaryOperator::multiplication:
				result *= polynomial(variable_vector);
				break;
			}
		}
		return result;
	}
}

bool Polynomial::operator==(const Polynomial& other) const {
	if (this->polynomial_order() != other.polynomial_order() || this->domain_order() != other.domain_order())
		return false;
	
	auto compare_node_set = this->build_compare_node_set();

	for (const auto& compare_node : compare_node_set) {
		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
			return false;
	}

	return true;
}

bool Polynomial::operator!=(const Polynomial& other) const {
	return !((*this) == other);
}

Polynomial& Polynomial::differentiate(const size_t variable_index) {
	if (this->is_simple_polynomial())
		return this->differentiate_simple_poly(variable_index);
	else {
		auto [binary_operator, polynomial] = this->calculated_polynomial_set_.back();
		this->calculated_polynomial_set_.pop_back();

		switch (binary_operator) {
		case  BinaryOperator::addition: {
			this->differentiate(variable_index);
			polynomial.differentiate(variable_index);
			*this += polynomial;
			break;
		}
		case BinaryOperator::multiplication: {
			auto tmp_poly = *this;
			tmp_poly.differentiate(variable_index);
			tmp_poly *= polynomial;

			polynomial.differentiate(variable_index);
			*this *= polynomial;
			*this += tmp_poly;
			break;
		}
		}

		return *this;
	}
}

Polynomial& Polynomial::power(const size_t power_index) {
	this->power_index_ = power_index;
	return *this;
}

std::string Polynomial::to_string(void) const {
	if (this->is_simple_polynomial())
		return this->to_poly_string();
	else {
		auto str = this->to_poly_string();

		for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
			switch (binary_operator) {
			case  BinaryOperator::addition:
				str += " + ";
				break;
			case BinaryOperator::multiplication:
				str += " X ";
				break;
			}
			str += polynomial.to_string() + "]";
		}
		str.pop_back();
		return str;
	}
}

size_t Polynomial::domain_order(void) const {
	if (this->is_simple_polynomial())
		return this->simple_domain_order();
	else {
		auto result = this->simple_domain_order();
		for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) 
				result = std::max(result, polynomial.domain_order());
		return result;
	}
}

bool Polynomial::compare_v1(const Polynomial& other) const {
	const auto extended_this_poly = this->extend();
	const auto extended_other_poly = other.extend();

	const auto num_term = extended_this_poly.coefficient_vector_.size();
	if (num_term != extended_other_poly.coefficient_vector_.size())
		return false;

	std::map<Monomial, double> ordered_this_polynomial;
	std::map<Monomial, double> ordered_other_polynomial;
	for (size_t i = 0; i < num_term; ++i) {
		ordered_this_polynomial.try_emplace(extended_this_poly.monomial_set_[i], extended_this_poly.coefficient_vector_[i]);
		ordered_other_polynomial.try_emplace(extended_other_poly.monomial_set_[i], extended_other_poly.coefficient_vector_[i]);
	}

	return ordered_this_polynomial == ordered_other_polynomial;
}

bool Polynomial::compare_v2(const Polynomial& other) const {
	auto compare_node_set = this->build_compare_node_set();

	for (const auto& compare_node : compare_node_set) {
		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
			return false;
	}

	return true;
}

std::vector<MathVector> Polynomial::build_compare_node_set(void) const {
	const auto polynomial_order = this->polynomial_order();
	const auto domain_order = this->domain_order();	
	size_t num_basis = ms::combination_with_repetition(polynomial_order + 1, domain_order);
	
	std::vector<MathVector> compare_node_set;
	compare_node_set.reserve(num_basis);
	
	MathVector compare_node(domain_order);
	if (domain_order == 0) {
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


size_t Polynomial::polynomial_order(void) const {
	if (this->is_simple_polynomial())
		return this->simple_polynomial_order();
	else {
		auto result = this->simple_polynomial_order();
		for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
			switch (binary_operator) {
			case  BinaryOperator::addition:
				result = std::max(result,polynomial.polynomial_order());
				break;
			case BinaryOperator::multiplication:
				result += polynomial.polynomial_order();
				break;
			}
		}
		return result;
	}
}

double Polynomial::calculate(const MathVector& variable_vector) const {
	const auto num_term = this->coefficient_vector_.size();

	MathVector monomial_value_vector;
	monomial_value_vector.reserve(num_term);

	for (size_t i = 0; i < num_term; ++i)
		monomial_value_vector.push_back(this->monomial_set_[i](variable_vector));

	return std::pow(this->coefficient_vector_.inner_product(monomial_value_vector), this->power_index_);
}

Polynomial& Polynomial::differentiate_simple_poly(const size_t variable_index) {
	if (power_index_ == 1) {
		//differetiate term
		for (size_t i = 0; i < this->coefficient_vector_.size(); ++i) {
			const auto exponent = this->monomial_set_[i].exponent(variable_index);

			this->coefficient_vector_[i] *= exponent;
			if (coefficient_vector_[i] != 0)
				this->monomial_set_[i].reduce_order(variable_index);
		}

		//remove zero term
		for (;;) {
			constexpr double zero_coefficient = 0.0;
			auto iter = std::find(this->coefficient_vector_.begin(), this->coefficient_vector_.end(), zero_coefficient);

			if (iter == this->coefficient_vector_.end())
				break;

			const auto position = iter - this->coefficient_vector_.begin();
			this->coefficient_vector_.erase(this->coefficient_vector_.begin() + position);
			this->monomial_set_.erase(this->monomial_set_.begin() + position);
		}

		if (this->coefficient_vector_.empty())
			*this = Polynomial();
	}
	else {
		auto differentiate_polynomial = *this;
		differentiate_polynomial.power_index_ = 1;
		differentiate_polynomial.differentiate_simple_poly(variable_index);

		if (differentiate_polynomial == Polynomial())
			*this = Polynomial();
		else {
			differentiate_polynomial *= static_cast<double>(this->power_index_);
			this->power_index_--;

			*this *= differentiate_polynomial;
		}
	}	
	return *this;
}

void Polynomial::addition(const Polynomial& other) {
	const auto num_term = other.coefficient_vector_.size();
	for (size_t i = 0; i < num_term; ++i)
		this->insert(other.coefficient_vector_[i], other.monomial_set_[i]);
}

void Polynomial::scalar_multiplication(const double scalar) {
	if (scalar == 0)
		*this = Polynomial();

	for (auto& coefficient : this->coefficient_vector_)
		coefficient *= scalar;
}

void Polynomial::multiplication(const Polynomial& other) {
	const auto num_this_term = this->coefficient_vector_.size();
	const auto num_other_term = other.coefficient_vector_.size();

	Polynomial result;
	for (size_t i = 0; i < num_this_term; ++i) {
		const auto& this_coefficient = this->coefficient_vector_[i];
		const auto& this_monomial = this->monomial_set_[i];
		for (size_t j = 0; j < num_other_term; ++j) {
			const auto& other_coefficient = other.coefficient_vector_[j];
			const auto& other_monomial = other.monomial_set_[j];

			const auto result_coefficient = this_coefficient * other_coefficient;
			const auto result_monomial = this_monomial * other_monomial;
			result.insert(result_coefficient, result_monomial);
		}
	}
	*this = std::move(result);
}

bool Polynomial::is_zero(void) const {
	return this->coefficient_vector_.size() == 1 && this->coefficient_vector_.front() == 0.0;
}

bool Polynomial::is_simple_polynomial(void) const {
	return this->calculated_polynomial_set_.empty();
}

std::string Polynomial::to_poly_string(void) const {
	std::string str;
	str += "[";

	const auto num_term = this->coefficient_vector_.size();
	std::map<Monomial, double> ordered_this_polynomial;
	for (size_t i = 0; i < num_term; ++i)
		ordered_this_polynomial.try_emplace(this->monomial_set_[i], this->coefficient_vector_[i]);

	auto iter = ordered_this_polynomial.rbegin();
	for (; iter != ordered_this_polynomial.rend(); ++iter) {
		const auto& [monomial, coefficient] = *iter;
		str += ms::double_to_string(coefficient) + monomial.to_string() + " + ";
	}

	str.erase(str.end() - 3, str.end());
	str += "]";

	if (this->power_index_ > 1)
		str += "^(" + std::to_string(this->power_index_) + ")";

	return str;
}

std::ostream& operator<<(std::ostream& ostream, const Polynomial& monomial) {
	return ostream << monomial.to_string();
}

size_t Polynomial::simple_domain_order(void) const {
	std::vector<size_t> monomial_domain_order_set;
	for (const auto& monomial : this->monomial_set_)
		monomial_domain_order_set.push_back(monomial.domain_order());

	return *std::max_element(monomial_domain_order_set.begin(), monomial_domain_order_set.end());
}

size_t Polynomial::simple_polynomial_order(void) const {
	std::vector<size_t> monomial_order_set;
	for (const auto& monomial : this->monomial_set_)
		monomial_order_set.push_back(monomial.monomial_order());

	const auto max_monomial_order = *std::max_element(monomial_order_set.begin(), monomial_order_set.end());
	return  max_monomial_order * this->power_index_;
}

Polynomial Polynomial::extend(void) const {
	Polynomial result(this->coefficient_vector_,this->monomial_set_);

	for (const auto& [binary_operator, polynomial] : this->calculated_polynomial_set_) {
		if (binary_operator == BinaryOperator::addition)
			result.addition(polynomial.extend());
		else
			result.multiplication(polynomial.extend());
	}

	if (this->power_index_ > 1) {
		for (size_t i = 1; i < this->power_index_; ++i)
			result.multiplication(result);
	}

	return result;
}

void Polynomial::insert(const double coefficient, const Monomial& monomial) {
	if (coefficient == 0.0)
		return;

	const auto monomial_iter = std::find(this->monomial_set_.begin(), this->monomial_set_.end(), monomial);
	if (monomial_iter == this->monomial_set_.end()) {
		this->monomial_set_.push_back(monomial);
		this->coefficient_vector_.push_back(coefficient);
	}
	else {
		const auto coefficient_iter = coefficient_vector_.begin() + (monomial_iter - monomial_set_.begin());
		*coefficient_iter += coefficient;
		if (*coefficient_iter == 0) {
			if (this->coefficient_vector_.size() == 1) {
				*this = Polynomial();
				return;
			}
			this->monomial_set_.erase(monomial_iter);
			this->coefficient_vector_.erase(coefficient_iter);
		}
	}
}


namespace ms {
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
}