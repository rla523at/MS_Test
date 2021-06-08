#include "../INC/Polynomial.h"

Polynomial& Polynomial::operator+=(const Polynomial& other) {
	this->simple_poly_term_ += other.simple_poly_term_;

	for (const auto& poly_term : other.added_poly_term_set_)
		this->add_assign_poly_term(poly_term);

	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
	return *this += (-1 * other);
}

Polynomial& Polynomial::operator*=(const double constant) {
	if (constant == 0.0)
		return *this = 0.0;

	for (auto& poly_term : this->added_poly_term_set_)
		poly_term *= constant;

	if (this->simple_poly_term_.is_constant())
		this->simple_poly_term_ = this->simple_poly_term_.be_constant() * constant;
	else {
		this->add_assign_poly_term(PolyTerm(this->simple_poly_term_) * constant);
		this->simple_poly_term_ = 0.0;
	}

	return *this;
}

Polynomial Polynomial::operator+(const Polynomial& other) const {
	Polynomial result(*this);
	return result += other;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
	Polynomial result(*this);
	return result += -1 * other;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
	Polynomial result = 0.0;

	const auto num_this_term = this->added_poly_term_set_.size();
	const auto num_other_term = other.added_poly_term_set_.size();
	for (size_t i = 0; i < num_this_term; ++i)
		for (size_t j = 0; j < num_other_term; ++j)
			result.add_assign_poly_term(this->added_poly_term_set_[i] * other.added_poly_term_set_[j]);

	if (this->simple_poly_term_ != 0.0)
		for (size_t j = 0; j < num_other_term; ++j)
			result.add_assign_poly_term(other.added_poly_term_set_[j] * this->simple_poly_term_);

	if (other.simple_poly_term_ != 0.0)
		for (size_t i = 0; i < num_this_term; ++i)
			result.add_assign_poly_term(this->added_poly_term_set_[i] * other.simple_poly_term_);

	if (this->simple_poly_term_ != 0.0 && other.simple_poly_term_ != 0.0) {
		const auto poly_term = PolyTerm(this->simple_poly_term_) * other.simple_poly_term_;
		if (poly_term.is_constant())
			result.simple_poly_term_ = poly_term.be_constant();
		else
			result.add_assign_poly_term(poly_term);
	}

	return result;
}

Polynomial Polynomial::operator*(const double constant) const {
	Polynomial result(*this);
	return result *= constant;
}

Polynomial Polynomial::operator^(const size_t power_index) const {
	if (power_index == 0)
		return 1;

	auto result = *this;
	for (size_t i = 1; i < power_index; ++i)
		result = std::move(result * *this);
	return result;
}

double Polynomial::operator()(const MathVector& variable_vector) const {
	auto result = this->simple_poly_term_(variable_vector);
	for (const auto& poly_term : this->added_poly_term_set_)
		result += poly_term(variable_vector);
	return result;
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
	size_t result = this->simple_poly_term_.domain_dimension();
	for (const auto& term : this->added_poly_term_set_)
		result = std::max(result, term.domain_dimension());
	return result;
}

Polynomial& Polynomial::be_derivative(const size_t variable_index) {
	auto result = this->differentiate(variable_index);
	return *this = std::move(result);
}

Polynomial Polynomial::differentiate(const size_t variable_index) const {
	Polynomial result = 0.0;
	result.simple_poly_term_ = this->simple_poly_term_.differentiate(variable_index);
	for (const auto& poly_term : this->added_poly_term_set_)
		result += poly_term.differentiate(variable_index);

	return result;
}

VectorFunction<Polynomial> Polynomial::gradient(void) const {
	const auto domain_dimension = this->domain_dimension();
	return this->gradient(domain_dimension);
}

VectorFunction<Polynomial> Polynomial::gradient(const size_t domain_dimension) const {
	VectorFunction<Polynomial> result;
	result.reserve(domain_dimension);
	for (size_t i = 0; i < domain_dimension; ++i)
		result.push_back(this->differentiate(i));

	return result;
}

IrrationalFunction Polynomial::root(const double root_index) const {
	return IrrationalFunction(*this, root_index);
}

size_t Polynomial::order(void) const {
	size_t result = this->simple_poly_term_.order();
	for (const auto& term : this->added_poly_term_set_)
		result = std::max(result, term.order());
	return result;
}

std::string Polynomial::to_string(void) const {
	if (this->added_poly_term_set_.empty())
		return this->simple_poly_term_.to_string();

	std::string str;
	for (const auto& poly_term : this->added_poly_term_set_)
		str += poly_term.to_string();

	if (!(this->simple_poly_term_ == 0))
		str += "+" + this->simple_poly_term_.to_string();

	if (str.front() == '+')
		str.erase(str.begin());

	return str;
}

void Polynomial::add_assign_poly_term(const PolyTerm& term) {
	for (auto iter = this->added_poly_term_set_.begin(); iter != this->added_poly_term_set_.end(); ++iter) {
		if (iter->has_same_form(term)) {
			iter->add_assign_with_same_form(term);
			if (iter->is_zero())
				this->added_poly_term_set_.erase(iter);
			return;
		}
	}
	this->added_poly_term_set_.push_back(term);
}

std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial) {
	return ostream << polynomial.to_string();
}

Polynomial operator+(const double constant, const Polynomial& polynomial) {
	return polynomial + constant;
}

Polynomial operator*(const double constant, const Polynomial& polynomial) {
	return polynomial * constant;
}


Polynomial::VariableTerm::VariableTerm(const std::string& variable) {
	if (variable.front() != 'x')
		throw std::invalid_argument("variable should start with 'x'");

	constexpr size_t position = 1;
	this->variable_index_ = std::stoull(variable.substr(position));
	this->is_constant_ = false;
}

void Polynomial::VariableTerm::add_assign_with_same_variable(const VariableTerm& other) {
	this->coefficient_ += other.coefficient_;
	if (this->coefficient_ == 0.0)
		*this = 0.0;
}

//Polynomial::VariableTerm& Polynomial::VariableTerm::operator*=(const double constant) {
//	if (constant == 0.0)
//		*this = 0.0;
//
//	this->coefficient_ *= constant;
//	return *this;
//}

double Polynomial::VariableTerm::operator()(const MathVector& value_vector) const {
	if (this->is_constant_)
		return this->coefficient_;

	if (value_vector.size() <= this->variable_index_)
		throw std::invalid_argument("too small domain dimension");

	return this->coefficient_ * value_vector[this->variable_index_];
}

bool Polynomial::VariableTerm::operator==(const VariableTerm& other) const {
	return this->coefficient_ == other.coefficient_
		&& this->variable_index_ == other.variable_index_
		&& this->is_constant_ == other.is_constant_;
}

bool Polynomial::VariableTerm::operator<(const VariableTerm& other) const {
	return this->variable_index_ < other.variable_index_;
}

bool Polynomial::VariableTerm::operator>(const VariableTerm& other) const {
	return this->variable_index_ > other.variable_index_;
}

double Polynomial::VariableTerm::be_constant(void) const {
	return this->coefficient_;
}

size_t Polynomial::VariableTerm::domain_dimension(void) const {
	return this->variable_index_ + 1;
}

Polynomial::VariableTerm& Polynomial::VariableTerm::be_derivative(const size_t variable_index) {
	if (this->is_constant_ || this->variable_index_ != variable_index)
		return *this = 0.0;

	this->variable_index_ = 0;
	this->is_constant_ = true;
	return *this;
}

bool Polynomial::VariableTerm::has_same_variable(const VariableTerm& other) const {
	return this->variable_index_ == other.variable_index_;
}

bool Polynomial::VariableTerm::is_constant(void) const {
	return this->is_constant_;
}

std::string Polynomial::VariableTerm::to_string(void) const {
	if (this->is_constant_)
		return ms::double_to_string(this->coefficient_);

	std::string str;
	if (this->coefficient_ == 1.0)
		str += '+';
	else if (this->coefficient_ == -1.0)
		str += '-';
	else if (this->coefficient_ > 0.0)
		str += '+' + ms::double_to_string(this->coefficient_);
	else
		str += ms::double_to_string(this->coefficient_);

	str += "(x" + std::to_string(this->variable_index_) + ")";

	return str;
}

Polynomial::SimplePolyTerm::SimplePolyTerm(const std::string& variable) {
	this->added_variable_term_set_.push_back(variable);
}

Polynomial::SimplePolyTerm& Polynomial::SimplePolyTerm::operator+=(const SimplePolyTerm& other) {
	this->constant_ += other.constant_;
	for (const auto& other_variable_term : other.added_variable_term_set_)
		this->add_assign_variable_term(other_variable_term);

	constexpr double zero = 0.0;
	for (;;) {
		const auto zero_iter = std::find(this->added_variable_term_set_.begin(), this->added_variable_term_set_.end(), zero);
		if (zero_iter == this->added_variable_term_set_.end())
			break;
		else
			this->added_variable_term_set_.erase(zero_iter);
	}

	return *this;
}

double Polynomial::SimplePolyTerm::operator()(const MathVector& value_vector) const {
	double result = this->constant_;
	for (const auto& term : this->added_variable_term_set_)
		result += term(value_vector);
	return result;
}

bool Polynomial::SimplePolyTerm::operator==(const SimplePolyTerm& other) const {
	return this->constant_ == other.constant_
		&& this->added_variable_term_set_ == other.added_variable_term_set_;
}

bool Polynomial::SimplePolyTerm::operator!=(const SimplePolyTerm& other) const {
	return !(*this == other);
}

double Polynomial::SimplePolyTerm::be_constant(void) const {
	return this->constant_;
}

Polynomial::SimplePolyTerm& Polynomial::SimplePolyTerm::be_derivative(const size_t variable_index) {
	this->constant_ = 0.0;
	for (auto& term : this->added_variable_term_set_)
		term.be_derivative(variable_index);

	const auto is_constant = [](const VariableTerm& variable_term) {return variable_term.is_constant(); };
	for (;;) {
		const auto constant_iter = std::find_if(this->added_variable_term_set_.begin(), this->added_variable_term_set_.end(), is_constant);
		if (constant_iter == this->added_variable_term_set_.end())
			break;

		this->constant_ += constant_iter->be_constant();
		this->added_variable_term_set_.erase(constant_iter);
	}

	std::sort(this->added_variable_term_set_.begin(), this->added_variable_term_set_.end(), std::greater<>());
	return *this;
}

Polynomial::SimplePolyTerm Polynomial::SimplePolyTerm::differentiate(const size_t variable_index) const {
	auto result = *this;
	return result.be_derivative(variable_index);
}

size_t Polynomial::SimplePolyTerm::domain_dimension(void) const {
	if (this->is_constant())
		return 0;

	return this->added_variable_term_set_.back().domain_dimension();
}

size_t Polynomial::SimplePolyTerm::order(void) const {
	if (this->is_constant())
		return 0;
	else
		return 1;
}

bool Polynomial::SimplePolyTerm::is_constant(void) const {
	return this->added_variable_term_set_.empty();
}

std::string Polynomial::SimplePolyTerm::to_string(void) const {
	if (this->is_constant())
		return +"[" + ms::double_to_string(this->constant_) + "]";

	std::string str = "[";
	for (const auto& variable_term : this->added_variable_term_set_)
		str += variable_term.to_string();

	constexpr size_t position = 1;
	constexpr size_t size = 1;
	if (str.at(position) == '+')
		str.erase(position, size);

	if (this->constant_ == 0)
		return str += "]";

	if (this->constant_ > 0)
		return str += '+' + ms::double_to_string(this->constant_) + "]";
	else
		return str += ms::double_to_string(this->constant_) + "]";
}

void Polynomial::SimplePolyTerm::add_assign_variable_term(const VariableTerm& variable_term) {
	const auto start_iter = this->added_variable_term_set_.begin();
	const auto end_iter = this->added_variable_term_set_.end();
	const auto iter = std::lower_bound(start_iter, end_iter, variable_term);

	if (iter != end_iter && iter->has_same_variable(variable_term))
		iter->add_assign_with_same_variable(variable_term);
	else
		this->added_variable_term_set_.push_back(variable_term);
}

void Polynomial::PoweredPolyTerm::multiply_assign_with_same_base(const PoweredPolyTerm& other) {
	this->exponent_ += other.exponent_;
}

double Polynomial::PoweredPolyTerm::operator()(const MathVector& variable_vector) const {
	return std::pow(this->base_(variable_vector), this->exponent_);
}

bool Polynomial::PoweredPolyTerm::operator==(const PoweredPolyTerm& other) const {
	return this->base_ == other.base_ && this->exponent_ == other.exponent_;
}

double Polynomial::PoweredPolyTerm::be_constant(void) const {
	return std::pow(this->base_.be_constant(), this->exponent_);
}

size_t Polynomial::PoweredPolyTerm::domain_dimension(void) const {
	return this->base_.domain_dimension();
}

Polynomial::PolyTerm Polynomial::PoweredPolyTerm::differentiate(const size_t variable_index) const {
	auto base_derivative = this->base_.differentiate(variable_index);
	if (base_derivative == 0.0)
		return 0.0;

	if (this->exponent_ == 1)
		return base_derivative;
	else {
		auto tmp = *this;	
		tmp.exponent_--;
		PolyTerm result = tmp;
		result *= static_cast<double>(this->exponent_);
		return result *= base_derivative;
		//return tmp * base_derivative * static_cast<double>(this->exponent_);
	}
}

bool Polynomial::PoweredPolyTerm::has_same_base(const PoweredPolyTerm& other) const {
	return this->base_ == other.base_;
}

bool Polynomial::PoweredPolyTerm::is_constant(void) const {
	return this->base_.is_constant();
}

size_t Polynomial::PoweredPolyTerm::order(void) const {
	if (this->is_constant())
		return 0;
	else
		return this->exponent_;
}

std::string Polynomial::PoweredPolyTerm::to_string(void) const {
	auto str = this->base_.to_string();
	if (this->exponent_ != 1)
		return  str + "^" + std::to_string(this->exponent_);
	else
		return str;
}

Polynomial::PolyTerm::PolyTerm(const SimplePolyTerm& simple_poly_term) {
	if (simple_poly_term.is_constant())
		this->coefficient_ = simple_poly_term.be_constant();
	else
		this->multiplied_power_poly_term_set_.push_back(simple_poly_term);
}
Polynomial::PolyTerm::PolyTerm(const PoweredPolyTerm& powered_poly_term) {
	if (powered_poly_term.is_constant())
		this->coefficient_ = powered_poly_term.be_constant();
	else
		this->multiplied_power_poly_term_set_.push_back(powered_poly_term);
}

void Polynomial::PolyTerm::add_assign_with_same_form(const PolyTerm& other) {
	this->coefficient_ += other.coefficient_;
	if (this->coefficient_ == 0.0)
		*this = 0;
}

Polynomial::PolyTerm& Polynomial::PolyTerm::operator*=(const PolyTerm& other) {
	this->coefficient_ *= other.coefficient_;
	if (this->coefficient_ == 0.0)
		return *this = 0.0;

	for (const auto& other_power_poly_term : other.multiplied_power_poly_term_set_)
		this->multiply_assign_powered_poly_term(other_power_poly_term);

	return *this;
}


Polynomial::PolyTerm Polynomial::PolyTerm::operator*(const PolyTerm& other) const {
	PolyTerm result(*this);
	return result *= other;
}

double Polynomial::PolyTerm::operator()(const MathVector& variable_vector) const {
	double result = this->coefficient_;
	for (const auto& power_poly_term : this->multiplied_power_poly_term_set_)
		result *= power_poly_term(variable_vector);
	return result;
}

bool Polynomial::PolyTerm::operator==(const PolyTerm& other) const {
	const auto max_order = std::max(this->order(), other.order());
	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);

	for (const auto& compare_node : compare_node_set) {
		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
			return false;
	}

	return true;
}

bool Polynomial::PolyTerm::operator!=(const PolyTerm& other) const {
	return !(*this == other);
}

double Polynomial::PolyTerm::be_constant(void) const {
	return this->coefficient_;
}

size_t Polynomial::PolyTerm::domain_dimension(void) const {
	size_t result = 0;
	for (const auto& term : this->multiplied_power_poly_term_set_)
		result = std::max(result, term.domain_dimension());
	return result;
}

Polynomial Polynomial::PolyTerm::differentiate(const size_t variable_index) const {
	Polynomial result = 0.0;
	const auto num_term = this->multiplied_power_poly_term_set_.size();
	for (size_t i = 0; i < num_term; ++i) {
		const auto diff_term = this->multiplied_power_poly_term_set_[i].differentiate(variable_index);
		if (diff_term != 0.0) {
			auto derivative = *this;
			derivative.multiplied_power_poly_term_set_.erase(derivative.multiplied_power_poly_term_set_.begin() + i);

			derivative *= diff_term;

			if (derivative.is_constant())
				result.simple_poly_term_ += derivative.be_constant();
			else
				result.add_assign_poly_term(derivative);
		}
	}

	return result;
}

size_t Polynomial::PolyTerm::order(void) const {
	size_t result = 0;
	for (const auto& term : this->multiplied_power_poly_term_set_)
		result += term.order();
	return result;
}

bool Polynomial::PolyTerm::has_same_form(const PolyTerm& other) const {
	if (this->multiplied_power_poly_term_set_.size() != other.multiplied_power_poly_term_set_.size())
		return false;

	const auto start_iter = this->multiplied_power_poly_term_set_.begin();
	const auto end_iter = this->multiplied_power_poly_term_set_.end();
	for (const auto& other_power_poly_term : other.multiplied_power_poly_term_set_) {
		if (std::find(start_iter, end_iter, other_power_poly_term) == end_iter)
			return false;
	}
	return true;
}

bool Polynomial::PolyTerm::is_constant(void) const {
	return this->multiplied_power_poly_term_set_.empty();
}

bool Polynomial::PolyTerm::is_zero(void) const {
	return this->is_constant() && this->coefficient_ == 0.0;
}

std::string Polynomial::PolyTerm::to_string(void) const {
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

	for (const auto& term : this->multiplied_power_poly_term_set_)
		str += term.to_string();

	return str;
}

void Polynomial::PolyTerm::multiply_assign_powered_poly_term(const PoweredPolyTerm& power_poly_term) {
	for (auto& this_term : this->multiplied_power_poly_term_set_) {
		if (this_term.has_same_base(power_poly_term)) {
			this_term.multiply_assign_with_same_base(power_poly_term);
			return;
		}
	}
	this->multiplied_power_poly_term_set_.push_back(power_poly_term);
}

IrrationalFunction::IrrationalFunction(const Polynomial& polynomial, const double root_index) {
	if (root_index == 1.0)
		this->polynomial_term_ = polynomial;
	else
		this->added_irrational_term_set_.push_back(PoweredPolynomial(polynomial, root_index));	
}

double IrrationalFunction::operator()(const MathVector& value_vector) const {
	double result = this->polynomial_term_(value_vector);
	for (const auto& term : this->added_irrational_term_set_)
		result += term(value_vector);
	return result;
}

bool IrrationalFunction::operator==(const IrrationalFunction& other) const {
	//temporary
	const auto max_order = std::max(this->order(), other.order());
	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);

	for (const auto& compare_node : compare_node_set) {
		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
			return false;
	}

	return true;
}

size_t IrrationalFunction::domain_dimension(void) const {
	size_t result = this->polynomial_term_.domain_dimension();
	for (const auto& term : this->added_irrational_term_set_)
		result = std::max(result, term.domain_dimension());
	return result;
}

size_t IrrationalFunction::order(void) const {
	size_t result = this->polynomial_term_.order();
	for (const auto& term : this->added_irrational_term_set_)
		result = std::max(result, term.order());
	return result;
}

std::string IrrationalFunction::to_string(void) const {
	if (this->added_irrational_term_set_.empty())
		return this->polynomial_term_.to_string();

	std::string str;
	for (const auto& irrational_term : this->added_irrational_term_set_)
		str += irrational_term.to_string();

	if (!(this->polynomial_term_ == 0))
		str += "+" + this->polynomial_term_.to_string();

	if (str.front() == '+')
		str.erase(str.begin());

	return str;
}

std::ostream& operator<<(std::ostream& ostream, const IrrationalFunction& irrational_function) {
	return ostream << irrational_function.to_string();
}

double IrrationalFunction::PoweredPolynomial::operator()(const MathVector& value_vector) const {
	return std::pow(this->base_(value_vector), this->exponent_);
}

size_t IrrationalFunction::PoweredPolynomial::domain_dimension(void) const {
	return this->base_.domain_dimension();
}

size_t IrrationalFunction::PoweredPolynomial::order(void) const {
	return this->base_.order() * static_cast<size_t>(std::ceil(this->exponent_));
}

std::string IrrationalFunction::PoweredPolynomial::to_string(void) const {
	auto str = this->base_.to_string();
	if (this->exponent_ != 1)
		return  str + "^" + ms::double_to_string(this->exponent_);
	else
		return str;
}

IrrationalFunction::Term::Term(const PoweredPolynomial& powered_polynomial) {
	this->multiplied_powered_polynomial_set_.push_back(powered_polynomial);
}

double IrrationalFunction::Term::operator()(const MathVector& value_vector) const {
	double result = this->coefficient_;
	for (const auto& powered_polynomial : this->multiplied_powered_polynomial_set_)
		result *= powered_polynomial(value_vector);
	return result;
}

size_t IrrationalFunction::Term::domain_dimension(void) const {
	size_t result = 0;
	for (const auto& term : this->multiplied_powered_polynomial_set_)
		result = std::max(result, term.domain_dimension());
	return result;
}

size_t IrrationalFunction::Term::order(void) const {
	size_t result = 0;
	for (const auto& term : this->multiplied_powered_polynomial_set_)
		result += term.order();
	return result;
}

std::string IrrationalFunction::Term::to_string(void) const {
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

	for (const auto& term : this->multiplied_powered_polynomial_set_)
		str += term.to_string();

	return str;
}


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

	bool is_positive_odd_number(const double val) {
		if (val < 0)
			return false;

		if (val - std::floor(val) == 0)
			return static_cast<size_t>(val) % 2 == 0;
		else
			return false;
	}

	bool is_natural_number(const double val) {
		if (val < 0)
			return false;

		if (val - std::floor(val) == 0)
			return true;
		else
			return false;
	}
}
















































//MonoTerm::MonoTerm(const std::string& variable) {
//	if (variable.front() != 'x')
//		throw std::invalid_argument("variable should start with 'x'");
//
//	constexpr size_t position = 1;
//	const auto variable_index = std::stoull(variable.substr(position));
//	this->monomial_exponents_.resize(variable_index + 1);
//	this->monomial_exponents_.back() = 1;
//}
//
//void MonoTerm::add_assign_with_same_monomial(const MonoTerm& other) {
//	this->coefficient_ += other.coefficient_;
//	if (this->coefficient_ == 0.0)
//		*this = 0.0;
//}
//
//MonoTerm& MonoTerm::operator*=(const MonoTerm& other) {
//	if (other.coefficient_ == 0.0)
//		*this = 0.0;
//
//	this->coefficient_ *= other.coefficient_;
//
//	if (this->domain_dimension() <= other.domain_dimension())
//		this->monomial_exponents_.resize(other.domain_dimension());
//	for (size_t i = 0; i < other.domain_dimension(); ++i)
//		this->monomial_exponents_[i] += other.monomial_exponents_[i];
//
//	const auto is_zero = [](const size_t exponent) {return exponent == 0; };
//	const auto iter = std::find_if_not(this->monomial_exponents_.rbegin(), this->monomial_exponents_.rend(), is_zero);
//	const auto num_zero = iter - this->monomial_exponents_.rbegin();
//	this->monomial_exponents_.erase(this->monomial_exponents_.end() - num_zero, this->monomial_exponents_.end());
//	
//	return *this;
//}
//
//SimplePolynomial MonoTerm::operator+(const double& constant) const {
//	SimplePolynomial result = *this;
//	return result += constant;
//}
//
//SimplePolynomial MonoTerm::operator+(const MonoTerm& mono_term) const {
//	SimplePolynomial result = *this;
//	return result += mono_term;
//}
//
//SimplePolynomial MonoTerm::operator+(const SimplePolynomial& simple_polynomial) const {
//	SimplePolynomial result = *this;
//	return result += simple_polynomial;
//}
//
//SimplePolynomial MonoTerm::operator-(const double& constant) const {
//	SimplePolynomial result = *this;
//	return result -= constant;
//}
//
//SimplePolynomial MonoTerm::operator-(const MonoTerm& mono_term) const {
//	SimplePolynomial result = *this;
//	return result -= mono_term;
//}
//
//SimplePolynomial MonoTerm::operator-(const SimplePolynomial& simple_polynomial) const {
//	SimplePolynomial result = *this;
//	return result -= simple_polynomial;
//}
//
//MonoTerm MonoTerm::operator*(const double constant) const {
//	auto result = *this;
//	return result *= constant;
//}
//
//MonoTerm MonoTerm::operator*(const MonoTerm& other) const {
//	MonoTerm result = *this;
//	return result *= other;
//}
//
//PolyTerm MonoTerm::operator*(const SimplePolynomial& simple_polynomial) const {
//	return simple_polynomial * *this;
//}
//
//MonoTerm MonoTerm::operator^(const size_t power_index) const {
//	MonoTerm result = *this;
//	return result.power(power_index);
//}
//
//double MonoTerm::operator()(const MathVector& variable_value_vector) const {
//	if (this->monomial_exponents_.size() > variable_value_vector.size())
//		throw std::invalid_argument("given variables are not enough");
//
//	double result = this->coefficient_;
//	for (size_t i = 0; i < this->monomial_exponents_.size(); ++i)
//		result *= std::pow(variable_value_vector[i], this->monomial_exponents_[i]);
//
//	return result;
//}
//
//bool MonoTerm::operator==(const MonoTerm& other) const {
//	return this->monomial_exponents_ == other.monomial_exponents_
//		&& this->coefficient_ == other.coefficient_;
//}
//
//bool MonoTerm::operator<(const MonoTerm& other) const {
//	const auto this_order = this->order();
//	const auto other_order = other.order();
//
//	if (this_order == other_order) {
//		const auto min_set_size = std::min(this->monomial_exponents_.size(), other.monomial_exponents_.size());
//		for (size_t i = 0; i < min_set_size; ++i) {
//			if (this->monomial_exponents_[i] == other.monomial_exponents_[i])
//				continue;
//
//			return this->monomial_exponents_[i] < other.monomial_exponents_[i];
//		}
//		return false;
//	}
//
//	return this_order < other_order;
//}
//
//double MonoTerm::be_constant(void) const {
//	return this->coefficient_;
//}
//
//size_t MonoTerm::domain_dimension(void) const {
//	return this->monomial_exponents_.size();
//}
//
//MonoTerm& MonoTerm::be_derivative(const size_t variable_index) {
//	if (this->domain_dimension() <= variable_index)
//		return *this = 0.0;
//
//	this->coefficient_ *= this->monomial_exponents_[variable_index];
//	if (this->coefficient_ == 0.0)
//		return *this = 0.0;
//
//	this->monomial_exponents_[variable_index]--;
//
//	const auto is_zero = [](size_t exponent) {return exponent == 0; };
//	const auto pos_iter = std::find_if_not(this->monomial_exponents_.rbegin(), this->monomial_exponents_.rend(), is_zero);
//	const auto num_meaningless_zero = pos_iter - this->monomial_exponents_.rbegin();
//	this->monomial_exponents_.erase(this->monomial_exponents_.end() - num_meaningless_zero, this->monomial_exponents_.end());
//
//	return *this;
//}
//
//MonoTerm MonoTerm::be_derivative(const size_t variable_index) const {
//	auto result = *this;
//	return result.be_derivative(variable_index);
//}
//
//bool MonoTerm::has_same_monomial(const MonoTerm& other) const {
//	return this->monomial_exponents_ == other.monomial_exponents_;
//}
//
//bool MonoTerm::is_constant(void) const {
//	return this->monomial_exponents_.empty();
//}
//
//size_t MonoTerm::order(void) const {
//	size_t order = 0;
//	for (const auto exponent : this->monomial_exponents_)
//		order += exponent;
//	return order;
//}
//
//MonoTerm& MonoTerm::power(const size_t power_index) {
//	if (power_index == 0)
//		return *this = 1.0;
//
//	this->coefficient_ = std::pow(this->coefficient_, power_index);
//	for (auto& exponent : this->monomial_exponents_)
//		exponent *= power_index;
//
//	return *this;
//}
//
//std::string MonoTerm::to_string(void) const {
//	if (this->is_constant())
//		return ms::double_to_string(this->coefficient_);
//
//	std::string str;
//	if (this->coefficient_ == 1.0)
//		str += '+';
//	else if (this->coefficient_ == -1.0)
//		str += '-';
//	else if (this->coefficient_ > 0.0)
//		str += '+' + ms::double_to_string(this->coefficient_);
//	else
//		str += ms::double_to_string(this->coefficient_);
//
//
//	for (size_t i = 0; i < this->monomial_exponents_.size(); ++i) {
//		if (this->monomial_exponents_[i] == 0)
//			continue;
//
//		if (this->monomial_exponents_[i] == 1)
//			str += "(x" + std::to_string(i) + ")";
//		else
//			str += "(x" + std::to_string(i) + ")^" + std::to_string(this->monomial_exponents_[i]);
//	}
//
//	return str;
//}
//
//std::ostream& operator<<(std::ostream& os, const MonoTerm& mono_term) {
//	return os << mono_term.to_string();
//}
//
//MonoTerm operator* (const double constant, const MonoTerm& mono_term) {
//	return mono_term * constant;
//}

//SimplePolynomial::SimplePolynomial(const MonoTerm& mono_term) {
//	this->added_mono_term_set_.push_back(mono_term);
//}
//
//SimplePolynomial& SimplePolynomial::operator+=(const SimplePolynomial& other) {
//	this->constant_ += other.constant_;
//	for (const auto& other_mono_term : other.added_mono_term_set_)
//		this->add_assign_mono_term(other_mono_term);
//	return *this;
//}
//
//SimplePolynomial& SimplePolynomial::operator-=(const SimplePolynomial& other) {
//	return *this += -1 * other;
//}
//
//SimplePolynomial& SimplePolynomial::operator*=(const double constant) {
//	this->constant_ *= constant;
//	for (auto& mono_term : this->added_mono_term_set_)
//		mono_term *= constant;
//	return *this;
//}
//
//SimplePolynomial SimplePolynomial::operator+(const SimplePolynomial& other) const {
//	auto result = *this;
//	return result += other;
//}
//
//SimplePolynomial SimplePolynomial::operator-(const SimplePolynomial& other) const {
//	auto result = *this;
//	return result -= other;
//}
//
//SimplePolynomial SimplePolynomial::operator*(const double constant) const {
//	auto result = *this;
//	return result *= constant;
//}
//
//PolyTerm SimplePolynomial::operator*(const SimplePolynomial& other) const {
//	PolyTerm result = *this;
//	return result *= other;
//}
//
//PolyTerm SimplePolynomial::operator^(const size_t power_index) const {
//	PolyTerm result = *this;
//	return result.power(power_index);
//}
//
//double SimplePolynomial::operator()(const MathVector& variable_vector) const {
//	double result = this->constant_;
//	for (const auto& term : this->added_mono_term_set_)
//		result += term(variable_vector);
//	return result;
//}
//
//bool SimplePolynomial::operator==(const SimplePolynomial& other) const {
//	const auto max_order = std::max(this->order(), other.order());
//	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
//	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);
//
//	for (const auto& compare_node : compare_node_set) {
//		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
//			return false;
//	}
//
//	return true;
//}
//
//bool SimplePolynomial::operator!=(const SimplePolynomial& other) const {
//	return !(*this == other);
//}
//
//double SimplePolynomial::be_constant(void) const {
//	return this->constant_;
//}
//
//SimplePolynomial& SimplePolynomial::be_derivative(const size_t variable_index) {
//	this->constant_ = 0.0;
//	for (auto& term : this->added_mono_term_set_)
//		term.be_derivative(variable_index);
//		
//	const auto is_constant = [](const MonoTerm& mono_term) {return mono_term.is_constant(); };
//	for (;;) {
//		const auto constant_iter = std::find_if(this->added_mono_term_set_.begin(), this->added_mono_term_set_.end(), is_constant);
//		if (constant_iter == this->added_mono_term_set_.end())
//			break;
//
//		this->constant_ += constant_iter->be_constant();
//		this->added_mono_term_set_.erase(constant_iter);
//	}
//	return *this;
//}
//
//SimplePolynomial SimplePolynomial::differentiate(const size_t variable_index) const {
//	auto result = *this;
//	return result.be_derivative(variable_index);
//}
//
//size_t SimplePolynomial::domain_dimension(void) const {
//	if (this->is_constant())
//		return 0;
//
//	std::vector<size_t> mono_term_domain_dimension_set;
//	mono_term_domain_dimension_set.reserve(this->added_mono_term_set_.size());
//	for (const auto& mono_term : this->added_mono_term_set_)
//		mono_term_domain_dimension_set.push_back(mono_term.domain_dimension());
//
//	return *std::max_element(mono_term_domain_dimension_set.begin(), mono_term_domain_dimension_set.end());
//}
//
//bool SimplePolynomial::is_constant(void) const {
//	return this->added_mono_term_set_.empty();
//}
//
//size_t SimplePolynomial::order(void) const {
//	if (this->is_constant())
//		return 0;
//
//	std::vector<size_t> mono_term_order_set;
//	mono_term_order_set.reserve(this->added_mono_term_set_.size());
//	for (const auto& mono_term : this->added_mono_term_set_)
//		mono_term_order_set.push_back(mono_term.order());
//
//	return *std::max_element(mono_term_order_set.begin(), mono_term_order_set.end());
//}
//
//std::string SimplePolynomial::to_string(void) const {
//	if (this->is_constant())
//		return +"[" + ms::double_to_string(this->constant_) + "]";
//
//	std::string str = "[";
//	auto sorted_set = this->added_mono_term_set_;
//	std::sort(sorted_set.begin(), sorted_set.end());
//	for (auto iter = sorted_set.rbegin(); iter != sorted_set.rend(); iter++)
//		str += iter->to_string();
//
//	constexpr size_t position = 1;
//	constexpr size_t size = 1;
//	if (str.at(position) == '+')
//		str.erase(position, size);
//
//	if (this->constant_ == 0)
//		return str += "]";
//
//	if (this->constant_ > 0)
//		return str += '+' + ms::double_to_string(this->constant_) + "]";
//	else
//		return str += ms::double_to_string(this->constant_) + "]";
//}
//
//void SimplePolynomial::add_assign_mono_term(const MonoTerm& mono_term) {
//	bool is_added = false;
//	for (auto& this_mono_term : this->added_mono_term_set_) {
//		if (this_mono_term.has_same_monomial(mono_term)) {
//			this_mono_term.add_assign_with_same_monomial(mono_term);
//			is_added = true;
//		}
//	}
//	if (!is_added)
//		this->added_mono_term_set_.push_back(mono_term);
//
//	constexpr double zero = 0.0;
//	for (;;) {
//		const auto zero_iter = std::find(this->added_mono_term_set_.begin(), this->added_mono_term_set_.end(), zero);
//		if (zero_iter == this->added_mono_term_set_.end())
//			break;
//		else
//			this->added_mono_term_set_.erase(zero_iter);
//	}
//
//	return *this;
//}
//
//std::ostream& operator<<(std::ostream& os, const SimplePolynomial& simple_polynomial) {
//	return os << simple_polynomial.to_string();
//}
//
//SimplePolynomial operator*(const double constant, const SimplePolynomial& simple_polynomial) {
//	return simple_polynomial * constant;
//}


//void PolyTerm::PoweredPolynomial::multiply_assign_with_same_base(const PoweredPolynomial& other) {
//	this->exponent_ += other.exponent_;
//}
//
//double PolyTerm::PoweredPolynomial::operator()(const MathVector& variable_vector) const {
//	return std::pow(this->base_(variable_vector), this->exponent_);
//}
//
//bool PolyTerm::PoweredPolynomial::operator==(const PoweredPolynomial& other) const {
//	return this->base_ == other.base_ && this->exponent_ == other.exponent_;
//}
//
//double PolyTerm::PoweredPolynomial::be_constant(void) const {
//	return std::pow(this->base_.be_constant(), this->exponent_);
//}
//
//size_t PolyTerm::PoweredPolynomial::domain_dimension(void) const {
//	return this->base_.domain_dimension();
//}
//
//PolyTerm PolyTerm::PoweredPolynomial::differentiate(const size_t variable_index) const {
//	auto base_derivative = this->base_.differentiate(variable_index);
//	if (base_derivative == 0.0)
//		return 0.0;
//
//	if (this->exponent_ == 1) 
//			return base_derivative;
//	else {
//		auto tmp = *this;	// tmp 이름 바꾸기
//		tmp.exponent_--;
//		return static_cast<double>(this->exponent_) * tmp * base_derivative;
//	}
//}
//
//bool PolyTerm::PoweredPolynomial::has_same_base(const PoweredPolynomial& other) const {
//	return this->base_ == other.base_;
//}
//
//bool PolyTerm::PoweredPolynomial::is_constant(void) const {
//	return this->base_.is_constant();
//}
//
//size_t PolyTerm::PoweredPolynomial::order(void) const {
//	return this->base_.order() * this->exponent_;
//}
//
//void PolyTerm::PoweredPolynomial::power(const size_t power_index) {
//	this->exponent_ *= power_index;
//}
//
//std::string PolyTerm::PoweredPolynomial::to_string(void) const {
//	auto str = this->base_.to_string();
//	if (this->exponent_ != 1)
//		return  str + "^" + std::to_string(this->exponent_);
//	else
//		return str;
//}


//PolyTerm::PolyTerm(const SimplePolynomial& simple_polynomial) {
//	if (simple_polynomial.is_constant())
//		this->coefficient_ = simple_polynomial.be_constant();
//	else
//		this->multiplied_power_poly_term_set_.push_back(simple_polynomial);
//}
//
//void PolyTerm::add_assign_with_same_form(const PolyTerm& other) {
//	this->coefficient_ += other.coefficient_;
//	if (this->coefficient_ == 0.0)
//		*this = 0;
//}
//
//PolyTerm& PolyTerm::operator*=(const PolyTerm& other) {
//	this->coefficient_ *= other.coefficient_;
//	if (this->coefficient_ == 0.0)		
//		return *this = 0.0;
//
//	for (const auto& other_power_poly_term : other.multiplied_power_poly_term_set_)
//		this->multiply_assign(other_power_poly_term);
//
//	return *this;
//}
//
//CompactPolynomial PolyTerm::operator+(const CompactPolynomial& compact_polynomial) const {
//	CompactPolynomial result(*this);
//	return result += compact_polynomial;
//}
//
//CompactPolynomial PolyTerm::operator-(const CompactPolynomial& compact_polynomial) const {
//	CompactPolynomial result(*this);
//	return result -= compact_polynomial;
//}
//
//PolyTerm PolyTerm::operator*(const PolyTerm& other) const {
//	PolyTerm result(*this);
//	return result *= other;
//}
//
//PolyTerm PolyTerm::operator^(const size_t power_index) const {
//	PolyTerm result(*this);
//	return result.power(power_index);
//}
//
//double PolyTerm::operator()(const MathVector& variable_vector) const {
//	double result = this->coefficient_;
//	for (const auto& power_poly_term : this->multiplied_power_poly_term_set_)
//		result *= power_poly_term(variable_vector);
//	return result;
//}
//
//bool PolyTerm::operator==(const PolyTerm& other) const {
//	const auto max_order = std::max(this->order(), other.order());
//	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
//	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);
//
//	for (const auto& compare_node : compare_node_set) {
//		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
//			return false;
//	}
//
//	return true;
//}
//
//bool PolyTerm::operator!=(const PolyTerm& other) const {
//	return !(*this == other);
//}
//
//size_t PolyTerm::domain_dimension(void) const {
//	size_t result = 0;
//	for (const auto& term : this->multiplied_power_poly_term_set_)
//		result = std::max(result, term.domain_dimension());
//	return result;
//}
//
//CompactPolynomial PolyTerm::differentiate(const size_t variable_index) const {
//	CompactPolynomial result;	
//	const auto num_term = this->multiplied_power_poly_term_set_.size();
//	for (size_t i = 0; i < num_term; ++i) {
//		auto tmp = *this;
//		tmp.multiplied_power_poly_term_set_.erase(tmp.multiplied_power_poly_term_set_.begin() + i);
//
//		auto diff_term = this->multiplied_power_poly_term_set_[i].differentiate(variable_index);
//		tmp *= diff_term;
//
//		result += tmp;
//	}
//
//	return result;
//}
//
//size_t PolyTerm::order(void) const {
//	size_t result = 0;
//	for (const auto& term : this->multiplied_power_poly_term_set_)
//		result += term.order();
//	return result;
//}
//
//bool PolyTerm::has_same_form(const PolyTerm& other) const {	
//	if (this->multiplied_power_poly_term_set_.size() != other.multiplied_power_poly_term_set_.size())
//		return false;
//
//	const auto start_iter = this->multiplied_power_poly_term_set_.begin();
//	const auto end_iter = this->multiplied_power_poly_term_set_.end();
//	for (const auto& other_power_poly_term : other.multiplied_power_poly_term_set_) {
//		if (std::find(start_iter, end_iter, other_power_poly_term) == end_iter)
//			return false;		
//	}
//	return true;
//}
//
//PolyTerm& PolyTerm::power(const size_t power_index) {
//	if (power_index == 0)
//		return *this = 1;
//
//	this->coefficient_ = std::pow(this->coefficient_, power_index);
//	for (auto& power_poly_term : this->multiplied_power_poly_term_set_)
//		power_poly_term.power(power_index);
//
//	return *this;
//}
//
//std::string PolyTerm::to_string(void) const {
//	std::string str;
//	if (std::abs(this->coefficient_) != 1.0) {
//		if (this->coefficient_ > 0)
//			str += "+";
//
//		str += ms::double_to_string(this->coefficient_);
//	}
//	else if (this->coefficient_ == 1.0)
//		str += "+";
//	else
//		str += "-";
//	
//	for (const auto& term : this->multiplied_power_poly_term_set_)
//		str += term.to_string();
//
//	return str;
//}
//
//PolyTerm::PolyTerm(const PoweredPolynomial& simple_polynomial) {
//	if (simple_polynomial.is_constant())
//		this->coefficient_ = simple_polynomial.be_constant();
//	else
//		this->multiplied_power_poly_term_set_.push_back(simple_polynomial);
//}
//
//void PolyTerm::multiply_assign(const PoweredPolynomial& power_poly_term) {
//	for (auto& this_term : this->multiplied_power_poly_term_set_) {
//		if (this_term.has_same_base(power_poly_term)) {
//			this_term.multiply_assign_with_same_base(power_poly_term);
//			return;
//		}
//	}
//	this->multiplied_power_poly_term_set_.push_back(power_poly_term);
//}
//
//std::ostream& operator<<(std::ostream& ostream, const PolyTerm& poly_term) {
//	return ostream << poly_term.to_string();
//}
//
//PolyTerm operator*(const double constant, const PolyTerm& other) {
//	return other * constant;
//}



//PolyTerm::PolyTerm(const PoweredPolynomial& power_poly_term) {
//	if (power_poly_term.is_constant())
//		this->coefficient_ = power_poly_term.be_constant();
//	else
//		this->multiplied_power_poly_term_set_.push_back(power_poly_term);
//}

//std::ostream& operator<<(std::ostream& ostream, const PolyTerm& term) {
//	return ostream << term.to_string();
//}
//
//Polynomial::PolyTerm operator*(const SimplePolyTerm& simple_poly1, const SimplePolyTerm& simple_poly2) {
//	Polynomial::PolyTerm result(simple_poly1);
//	return result *= simple_poly2;
//}
//Polynomial::PolyTerm& Polynomial::PolyTerm::operator*=(const double constant) {
//	if (constant == 0.0)
//		*this = 0;
//	
//	this->coefficient_ *= constant;
//	return *this;
//}
//Polynomial::PolyTerm::PolyTerm(const double constant, const PowerPolyTerm& power_poly_term) {
//	this->coefficient_ = constant;
//	if (power_poly_term.is_constant())
//		this->coefficient_ *= power_poly_term.be_constant();
//	else
//		this->multiplied_power_poly_term_set_.push_back(power_poly_term);
//}













//CompactPolynomial::CompactPolynomial(const PolyTerm& poly_term) {
//	this->added_poly_term_set_.push_back(poly_term);
//}
//
//CompactPolynomial& CompactPolynomial::operator+=(const CompactPolynomial& other) {
//	this->simple_poly_term_ += other.simple_poly_term_;
//
//	for (const auto& poly_term : other.added_poly_term_set_)
//		this->add_assign_poly_term(poly_term);
//
//	return *this;
//}
//
//CompactPolynomial& CompactPolynomial::operator-=(const CompactPolynomial& other) {
//	return *this += (other * -1);
//}
//
//CompactPolynomial& CompactPolynomial::operator*=(const double constant) {
//	for (auto& poly_term : this->added_poly_term_set_)
//		poly_term *= constant;
//
//	if (this->simple_poly_term_.is_constant())
//		this->simple_poly_term_ = this->simple_poly_term_.be_constant() * constant;
//	else {
//		this->add_assign_poly_term(constant * simple_poly_term_);
//		this->simple_poly_term_ = 0.0;
//	}
//
//	return *this;
//}
//
//CompactPolynomial CompactPolynomial::operator+(const CompactPolynomial& other) const {
//	CompactPolynomial result(*this);
//	return result += other;
//}
//
//CompactPolynomial CompactPolynomial::operator-(const CompactPolynomial& other) const {
//	CompactPolynomial result(*this);
//	return result -= other;
//}
//
//CompactPolynomial CompactPolynomial::operator*(const CompactPolynomial& other) const {
//	CompactPolynomial result;
//
//	const auto num_this_term = this->added_poly_term_set_.size();
//	const auto num_other_term = other.added_poly_term_set_.size();
//	for (size_t i = 0; i < num_this_term; ++i)
//		for (size_t j = 0; j < num_other_term; ++j)
//			result.add_assign_poly_term(this->added_poly_term_set_[i] * other.added_poly_term_set_[j]);
//
//	for (size_t i = 0; i < num_this_term; ++i)
//		result.add_assign_poly_term(this->added_poly_term_set_[i] * other.simple_poly_term_);
//
//	for (size_t j = 0; j < num_other_term; ++j)
//		result.add_assign_poly_term(other.added_poly_term_set_[j] * this->simple_poly_term_);
//
//	result.add_assign_poly_term(this->simple_poly_term_ * other.simple_poly_term_);
//
//	return result;
//}
//
//CompactPolynomial CompactPolynomial::operator*(const double constant) const {
//	CompactPolynomial result(*this);
//	return result *= constant;
//}
//
//double CompactPolynomial::operator()(const MathVector& variable_vector) const {
//	auto result = this->simple_poly_term_(variable_vector);
//	for (const auto& poly_term : this->added_poly_term_set_)
//		result += poly_term(variable_vector);
//	return result;
//}
//
//bool CompactPolynomial::operator==(const CompactPolynomial& other) const {
//	const auto max_order = std::max(this->order(), other.order());
//	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
//	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);
//
//	for (const auto& compare_node : compare_node_set) {
//		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
//			return false;
//	}
//
//	return true;
//}
//
//size_t CompactPolynomial::domain_dimension(void) const {
//	size_t result = this->simple_poly_term_.domain_dimension();
//	for (const auto& term : this->added_poly_term_set_)
//		result = std::max(result, term.domain_dimension());
//	return result;
//}
//
//CompactPolynomial CompactPolynomial::differentiate(const size_t variable_index) const {
//	CompactPolynomial result = this->simple_poly_term_.differentiate(variable_index);
//	for (const auto& poly_term : this->added_poly_term_set_)
//		result += poly_term.differentiate(variable_index);
//
//	return result;
//}
//
//VectorFunction<CompactPolynomial> CompactPolynomial::gradient(void) const {
//	const auto domain_dimension = this->domain_dimension();
//	return this->gradient(domain_dimension);
//}
//
//VectorFunction<CompactPolynomial> CompactPolynomial::gradient(const size_t domain_dimension) const {
//	VectorFunction<CompactPolynomial> result;
//	result.reserve(domain_dimension);
//	for (size_t i = 0; i < domain_dimension; ++i)
//		result.push_back(this->differentiate(i));
//
//	return result;
//}
//
//size_t CompactPolynomial::order(void) const {
//	size_t result = this->simple_poly_term_.order();
//	for (const auto& term : this->added_poly_term_set_)
//		result = std::max(result, term.order());
//	return result;
//}
//
//std::string CompactPolynomial::to_string(void) const {
//	std::string str;
//	for (const auto& poly_term : this->added_poly_term_set_)
//		str += poly_term.to_string();
//
//	if (this->simple_poly_term_ != 0)
//		str += "+" + this->simple_poly_term_.to_string();
//
//	if (str.empty())
//		return this->simple_poly_term_.to_string();
//
//	if (str.front() == '+')
//		str.erase(str.begin());
//
//	return str;
//}
//
//void CompactPolynomial::add_assign_poly_term(const PolyTerm& term) {
//	for (auto& this_poly_term : this->added_poly_term_set_) {
//		if (this_poly_term.has_same_form(term)) {
//			this_poly_term.add_assign_with_same_form(term);
//			return;
//		}
//	}
//	this->added_poly_term_set_.push_back(term);
//}
//
//std::ostream& operator<<(std::ostream& ostream, const CompactPolynomial& polynomial) {
//	return ostream << polynomial.to_string();
//}
//
//CompactPolynomial operator*(const double constant, const CompactPolynomial& compact_polynomial) {
//	return compact_polynomial * constant;
//}
//
//CompactPolynomial operator+(const PolyTerm& poly_term, const CompactPolynomial& polynomial) {
//	return polynomial + poly_term;
//}
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
////
//Polynomial operator-(const PolyTerm& poly_term, const Polynomial& polynomial) {
//	return -1.0 * polynomial += poly_term;
//}
//
//Polynomial operator*(const PolyTerm& poly_term, const Polynomial& polynomial) {
//	return polynomial * poly_term;
//}
//CompactPolynomial::Polynomial(const double scalar) {
//	this->simple_poly_term_ = scalar;
//}
//
//CompactPolynomial::Polynomial(const Monomial& monomial) {
//	this->simple_poly_term_ = monomial;
//}
//
//CompactPolynomial& CompactPolynomial::power(const double power_index) {
//	 if (ms::is_natural_number(power_index)) {
//		const auto tmp = *this;
//		for (size_t i = 1; i < power_index; ++i)
//			*this *= tmp;
//	}
//	else
//		throw std::runtime_error("complex polynomial can not be powered");
//
//	return *this;
//}
//CompactPolynomial CompactPolynomial::operator^(const double power_index) const {
//	CompactPolynomial result = *this;
//	return result.power(power_index);
//}























//Polynomial::Polynomial(const PolyTerm& poly_term) {
//	this->added_poly_term_set_.push_back(poly_term);
//}


































































//
//PolyTerm::PolyTerm(void) {
//	this->coefficient_vector_.push_back(0.0);
//	this->monomial_vector_function_.push_back(Monomial());
//}
//
//PolyTerm::PolyTerm(const double scalar) {
//	this->coefficient_vector_.push_back(scalar);
//	this->monomial_vector_function_.push_back(Monomial());
//}
//
//PolyTerm::PolyTerm(const Monomial& monomial) {
//	this->coefficient_vector_.push_back(1.0);
//	this->monomial_vector_function_.push_back(monomial);
//}
//
//PolyTerm::PolyTerm(const std::vector<double>& coefficient_vector, const std::vector<Monomial>& monomial_set) {
//	const auto num_term = coefficient_vector.size();
//	if (num_term != monomial_set.size())
//		throw std::length_error("Size Error");
//
//	for (size_t i = 0; i < num_term; ++i)
//		this->add_mono_term(coefficient_vector[i], monomial_set[i]);
//}
//
//PolyTerm& PolyTerm::operator*=(const PolyTerm& other) {
//	if (this->is_single_term() && other.is_single_term()) {
//		this->coefficient_vector_.front() *= other.coefficient_vector_.front();
//		this->monomial_vector_function_.front() *= other.monomial_vector_function_.front();
//		return *this;
//	}
//
//	if (!this->has_constant_front())
//		this->make_constant_front();
//
//	if (other.is_simple() && other.has_constant_front()) {
//		this->constant_front() *= other.constant_front();
//		return *this;
//	}
//	
//	if (other.is_simple())
//		this->simple_term_multiplication(other);
//	else {		
//		this->constant_front() *= other.constant_front();
//		for (const auto& other_term : other.multiplied_term_set_)
//			this->simple_term_multiplication(other_term);
//	}
//	return *this;
//}
//
//PolyTerm PolyTerm::operator*(const PolyTerm& other) const {
//	PolyTerm result(*this);
//	return result *= other;
//}
//
//PolyTerm PolyTerm::operator^(const double power_index) const {
//	PolyTerm result(*this);
//	return result.power(power_index);
//}
//
//double PolyTerm::operator()(const MathVector& variable_vector) const {
//	if (this->is_simple())
//		return this->simple_term_calculation(variable_vector);
//
//	double result = this->constant_front();
//	for (const auto& term : this->multiplied_term_set_) 
//		result *= term.simple_term_calculation(variable_vector);	
//	return result;
//}
//
//
//bool PolyTerm::operator==(const PolyTerm& other) const {
//	const auto max_order = std::max(this->order(), other.order());
//	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
//	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);
//
//	for (const auto& compare_node : compare_node_set) {
//		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
//			return false;
//	}
//
//	return true;
//}
//
//bool PolyTerm::operator!=(const PolyTerm& other) const {
//	return !(*this == other);
//}
//
//size_t PolyTerm::domain_dimension(void) const {
//	if (this->is_simple())
//		return this->simple_term_domain_dimension();
//
//	size_t result = 0;
//	for (const auto& term : this->multiplied_term_set_)
//		result = std::max(result, term.simple_term_domain_dimension());
//	return result;
//}
//
//size_t PolyTerm::order(void) const {
//	if (this->is_simple())
//		return this->simple_term_order();
//
//	size_t result = 0;
//	for (const auto& term : this->multiplied_term_set_)
//		result += term.simple_term_order();
//	return result;
//}
//
//std::string PolyTerm::to_string(void) const {
//	if (this->is_simple())
//		return this->simple_term_to_string();
//
//	std::string str;
//	if (this->coefficient_vector_.front() != 1.0)
//		str += ms::double_to_string(this->coefficient_vector_.front());
//
//	for (const auto& term : this->multiplied_term_set_)
//		str += "X" + term.simple_term_to_string();	
//
//	const auto pos = str.find('X');
//	const auto size = 1;
//	str.erase(pos,size);
//
//	return str;
//}
//
//void PolyTerm::add_mono_term(const double coefficient, const Monomial& monomial) {
//	if (coefficient == 0.0)
//		return;
//
//	if (this->has_constant_front() && this->constant_front() == 0.0) {
//		this->coefficient_vector_.front() = coefficient;
//		this->monomial_vector_function_.front() = monomial;
//		return;
//	}
//
//	const auto monomial_iter = std::find(this->monomial_vector_function_.begin(), this->monomial_vector_function_.end(), monomial);
//	if (monomial_iter == this->monomial_vector_function_.end()) {
//		this->monomial_vector_function_.push_back(monomial);
//		this->coefficient_vector_.push_back(coefficient);
//	}
//	else {
//		const auto coefficient_iter = coefficient_vector_.begin() + (monomial_iter - monomial_vector_function_.begin());
//		*coefficient_iter += coefficient;
//
//		if (*coefficient_iter == 0) {
//			this->monomial_vector_function_.erase(monomial_iter);
//			this->coefficient_vector_.erase(coefficient_iter);
//		}
//	}
//
//	if (this->coefficient_vector_.empty() && this->monomial_vector_function_.empty())
//		*this = 0.0;
//}
//
//PolyTerm PolyTerm::base(void) const {
//	PolyTerm result = *this;
//	result.power_index_ = 1.0;
//	return result;
//}
//
//void PolyTerm::be_simple(void) {
//	*this = this->multiplied_term_set_.front();
//}
//
//bool PolyTerm::can_be_simple(void) const {
//	return this->constant_front() == 1 && this->multiplied_term_set_.size() == 1 && this->multiplied_term_set_.front().is_simple();
//}
//
//double PolyTerm::constant_front(void) const {
//	return this->coefficient_vector_.front();
//}
//
//double& PolyTerm::constant_front(void) {
//	return this->coefficient_vector_.front();
//}
//
//void PolyTerm::make_constant_front(void) {
//	PolyTerm result = 1;
//	result.multiplied_term_set_.push_back(std::move(*this));
//	*this = std::move(result);
//}
//
//PolyTerm& PolyTerm::power(const double power_index) {
//	if (power_index == 0)
//		return *this = 1;
//
//	if (!this->has_constant_front())
//		this->make_constant_front();
//
//	this->constant_front() = std::pow(this->constant_front(), power_index);
//	for (auto& term : this->multiplied_term_set_)
//		term.power_index_ *= power_index;
//	return *this;
//}
//
//bool PolyTerm::has_constant_front(void) const {
//	return this->coefficient_vector_.size() == 1
//		&& this->monomial_vector_function_.size() == 1
//		&& this->monomial_vector_function_.front().is_one();
//}
//
//bool PolyTerm::is_simple(void) const {
//	return this->multiplied_term_set_.empty() && this->power_index_ == 1.0;
//}
//
//bool PolyTerm::is_single_term(void) const {
//	return this->is_simple()
//		&& this->coefficient_vector_.size() == 1
//		&& this->monomial_vector_function_.size() == 1;
//}
//
//PolyTerm& PolyTerm::simple_term_addition(const PolyTerm& other) {
//	const auto num_other_term = other.coefficient_vector_.size();
//	for (size_t i = 0; i < num_other_term; ++i)
//		this->add_mono_term(other.coefficient_vector_[i], other.monomial_vector_function_[i]);
//	return *this;
//}
//
//double PolyTerm::simple_term_calculation(const MathVector& variable_vector) const {
//	const auto monomial_value_vector = this->monomial_vector_function_(variable_vector);
//	const auto base_value = this->coefficient_vector_.inner_product(monomial_value_vector);
//	return std::pow(base_value, this->power_index_); 
//}
//
//PolyTerm& PolyTerm::simple_term_differentiate(const size_t variable_index) {
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
//	if (this->coefficient_vector_.empty() && this->monomial_vector_function_.empty())
//		*this = 0.0;
//
//	return *this;
//}
//
//PolyTerm& PolyTerm::simple_term_multiplication(const PolyTerm& other) {
//	for (auto& term : this->multiplied_term_set_) {
//		if (term.base() == other.base()) {
//			term.power_index_ += other.power_index_;
//			return *this;
//		}
//	}
//	this->multiplied_term_set_.push_back(other);
//	return *this;
//}
//
//size_t PolyTerm::simple_term_domain_dimension(void) const {
//	std::vector<size_t> monomial_domain_dimension_set;
//	for (const auto& monomial : this->monomial_vector_function_)
//		monomial_domain_dimension_set.push_back(monomial.domain_dimension());
//
//	return *std::max_element(monomial_domain_dimension_set.begin(), monomial_domain_dimension_set.end());
//}
//
//size_t PolyTerm::simple_term_order(void) const {
//	std::vector<size_t> monomial_order_set;
//	for (const auto& monomial : this->monomial_vector_function_)
//		monomial_order_set.push_back(monomial.order());
//
//	return *std::max_element(monomial_order_set.begin(), monomial_order_set.end());
//}
//
//std::string PolyTerm::simple_term_to_string(void) const {
//	std::string str = "[";
//
//	const auto num_term = this->coefficient_vector_.size();
//	std::map<Monomial, double> ordered_term;
//	for (size_t i = 0; i < num_term; ++i)
//		ordered_term.try_emplace(this->monomial_vector_function_[i], this->coefficient_vector_[i]);
//
//	for (auto iter = ordered_term.rbegin(); iter != ordered_term.rend(); ++iter) {
//		const auto& [monomial, coefficient] = *iter;
//		if (iter != ordered_term.rbegin() && coefficient > 0)
//			str += "+";
//		if (monomial.is_one())
//			str += ms::double_to_string(coefficient);
//		else if (coefficient == 1)
//			str += monomial.to_string();
//		else if (coefficient == -1)
//			str += "-" + monomial.to_string();
//		else
//			str += ms::double_to_string(coefficient) + monomial.to_string();
//	}
//
//	str += "]";
//
//	if (this->power_index_ != 1.0)
//		str += "^" + ms::double_to_string(this->power_index_);
//
//	return str;
//}
//
//std::ostream& operator<<(std::ostream& ostream, const PolyTerm& term) {
//	return ostream << term.to_string();
//}
//
//
//Polynomial::Polynomial(const double scalar) {
//	this->add_term(1.0, scalar);
//}
//
//Polynomial::Polynomial(const Monomial& monomial) {
//	this->add_term(1.0, monomial);
//}
//
//Polynomial::Polynomial(const PolyTerm& poly_term) {
//	this->add_term(1.0, poly_term);
//}
//
//Polynomial& Polynomial::operator+=(const Polynomial& other) {
//	this->simple_term_.simple_term_addition(other.simple_term_);
//
//	const auto num_other_term = other.coefficient_vector_.size();
//	for (size_t i = 0; i < num_other_term; ++i)
//		this->add_term(other.coefficient_vector_[i], other.poly_term_vector_function_[i]);
//	return *this;
//}
//
//Polynomial& Polynomial::operator-=(const Polynomial& other) {
//	return *this += (other * -1);
//}
//
//Polynomial& Polynomial::operator*=(const Polynomial& other) {
//	Polynomial result;
//
//	const auto num_this_term = this->coefficient_vector_.size();
//	const auto num_other_term = other.coefficient_vector_.size();
//	for (size_t i = 0; i < num_this_term; ++i)
//		for (size_t j = 0; j < num_other_term; ++j)
//			result.add_term(this->coefficient_vector_[i] * other.coefficient_vector_[j], this->poly_term_vector_function_[i] * other.poly_term_vector_function_[j]);
//
//	for (size_t i = 0; i < num_this_term; ++i)
//		result.add_term(this->coefficient_vector_[i], this->poly_term_vector_function_[i] * other.simple_term_);
//
//	for (size_t i = 0; i < num_other_term; ++i)
//		result.add_term(other.coefficient_vector_[i], other.poly_term_vector_function_[i] * this->simple_term_);
//
//	result.add_term(1.0, this->simple_term_ * other.simple_term_);
//
//	return *this = std::move(result);
//}
//
//Polynomial Polynomial::operator+(const Polynomial& other) const {
//	Polynomial result(*this);
//	return result += other;
//}
//
//Polynomial Polynomial::operator-(const Polynomial& other) const {
//	Polynomial result(*this);
//	return result -= other;
//}
//
//Polynomial Polynomial::operator*(const Polynomial& other) const {
//	Polynomial result(*this);
//	return result *= other;
//}
//
//Polynomial Polynomial::operator^(const double power_index) const {
//	Polynomial result = *this;
//	return result.power(power_index);
//}
//
//double Polynomial::operator()(const MathVector& variable_vector) const {
//	const auto poly_term_value_vector = this->poly_term_vector_function_(variable_vector);
//	return this->coefficient_vector_.inner_product(poly_term_value_vector) + this->simple_term_(variable_vector);
//}
//
//bool Polynomial::operator==(const Polynomial& other) const {
//	const auto max_order = std::max(this->order(), other.order());
//	const auto max_domain_dimension = std::max(this->domain_dimension(), other.domain_dimension());
//	const auto compare_node_set = ms::polynomial_compare_node_set(max_order, max_domain_dimension);
//
//	for (const auto& compare_node : compare_node_set) {
//		if (!ms::compare_double((*this)(compare_node), other(compare_node)))
//			return false;
//	}
//
//	return true;
//}
//
//size_t Polynomial::domain_dimension(void) const {
//	size_t result = this->simple_term_.domain_dimension();
//	for (const auto& term : this->poly_term_vector_function_)
//		result = std::max(result, term.domain_dimension());
//	return result;
//
//}
//
//Polynomial& Polynomial::differentiate(const size_t variable_index) {
//	Polynomial result = this->simple_term_.simple_term_differentiate(variable_index);
//
//	const auto num_poly_term = this->coefficient_vector_.size();
//	for (size_t i = 0; i < num_poly_term; ++i) 
//		result += this->coefficient_vector_[i] * this->poly_term_differentiate(this->poly_term_vector_function_[i], variable_index);
//
//	*this = std::move(result);
//	return *this;
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
//size_t Polynomial::order(void) const {
//	size_t result = this->simple_term_.order();
//	for (const auto& term : this->poly_term_vector_function_)
//		result = std::max(result, term.order());
//	return result;
//}
//
//Polynomial& Polynomial::power(const double power_index) {
//	if (this->coefficient_vector_.empty()) {
//		this->simple_term_.power(power_index);
//		this->coefficient_vector_.push_back(1.0);
//		this->poly_term_vector_function_.push_back(std::move(this->simple_term_));
//		this->simple_term_ = 0.0;
//	}
//	else if (this->coefficient_vector_.size() == 1 && this->simple_term_ == 0) {
//		this->coefficient_vector_.front() = std::pow(this->coefficient_vector_.front(), power_index);
//		this->poly_term_vector_function_.front().power(power_index);
//	}
//	else if (ms::is_natural_number(power_index)) {
//		const auto tmp = *this;
//		for (size_t i = 1; i < power_index; ++i)
//			*this *= tmp;
//	}
//	//else
//	//	throw std::runtime_error("complex polynomial can not be powered");
//
//	return *this;
//}
//
//std::string Polynomial::to_string(void) const {
//	std::string str;
//
//	const auto num_term = this->coefficient_vector_.size();
//	for (size_t i = 0; i < num_term; ++i) {
//		const auto coefficient = this->coefficient_vector_[i];
//		if (coefficient == 1.0)
//			str += "+" + this->poly_term_vector_function_[i].to_string();
//		else
//			str += "+" + ms::double_to_string(coefficient) + this->poly_term_vector_function_[i].to_string();
//	}
//
//	if (!this->simple_term_.has_constant_front() || this->simple_term_.constant_front() != 0.0)
//		str += "+" + this->simple_term_.to_string();
//
//	if (str.empty())
//		return this->simple_term_.to_string();
//
//	str.erase(str.begin());
//	return str;
//}
//
//Polynomial& Polynomial::add_term(const double coefficient, const PolyTerm& term) {
//	if (term.is_simple()) {
//		this->simple_term_.simple_term_addition(term);
//		return *this;
//	}
//
//	auto new_coefficient = coefficient * term.constant_front();
//	auto new_term = term;	
//	new_term.coefficient_vector_.front() = 1.0;
//	
//	if (new_coefficient == 0.0)
//		return *this;
//
//	const auto poly_term_iter = std::find(this->poly_term_vector_function_.begin(), this->poly_term_vector_function_.end(), new_term);
//	if (poly_term_iter == this->poly_term_vector_function_.end()) {
//		this->poly_term_vector_function_.push_back(std::move(new_term));
//		this->coefficient_vector_.push_back(new_coefficient);
//	}
//	else {
//		const auto position = poly_term_iter - this->poly_term_vector_function_.begin();
//		const auto coefficient_iter = this->coefficient_vector_.begin() + position;
//		*coefficient_iter += new_coefficient;
//
//		if (*coefficient_iter == 0) {
//			this->poly_term_vector_function_.erase(poly_term_iter);
//			this->coefficient_vector_.erase(coefficient_iter);
//		}
//	}
//	return *this;
//}
//
//Polynomial Polynomial::differentiate(const size_t variable_index) const {
//	auto simple_term = this->simple_term_;
//	Polynomial result = simple_term.simple_term_differentiate(variable_index);
//
//	const auto num_poly_term = this->coefficient_vector_.size();
//	for (size_t i = 0; i < num_poly_term; ++i)
//		result += this->coefficient_vector_[i] * this->poly_term_differentiate(this->poly_term_vector_function_[i], variable_index);
//
//	return result;
//}
//
//Polynomial Polynomial::poly_term_differentiate(const PolyTerm& poly_term, const size_t variable_index){
////Polynomial Polynomial::poly_term_differentiate(const PolyTerm& poly_term, const size_t variable_index) const {
//	Polynomial result;
//
//	const auto num_multiplied_term = poly_term.multiplied_power_poly_term_set_.size();
//	for (size_t i = 0; i < num_multiplied_term; ++i) {
//		auto tmp = poly_term;
//
//		if (tmp.multiplied_power_poly_term_set_[i].power_index_ == 1.0) {
//			tmp.multiplied_power_poly_term_set_[i].simple_term_differentiate(variable_index);
//
//			if (tmp.multiplied_power_poly_term_set_[i] == 0)
//				continue;
//
//			if (tmp.multiplied_power_poly_term_set_[i] == 1)
//				tmp.multiplied_power_poly_term_set_.erase(tmp.multiplied_power_poly_term_set_.begin() + i);
//
//			if (tmp.can_be_simple())
//				tmp.be_simple();
//		}
//		else {
//			auto diff_term = tmp.multiplied_power_poly_term_set_[i];
//			diff_term.power_index_ = 1.0;
//			diff_term.simple_term_differentiate(variable_index);
//
//			if (diff_term == 0)
//				continue;
//
//			tmp.constant_front() *= poly_term.multiplied_power_poly_term_set_[i].power_index_;
//			tmp.multiplied_power_poly_term_set_[i].power_index_ -= 1.0;
//
//			if (diff_term != 1)
//				tmp.multiplied_power_poly_term_set_.push_back(std::move(diff_term));
//		}
//		result += tmp;
//	}
//	return result;
//}
//
//std::ostream& operator<<(std::ostream& ostream, const Polynomial& polynomial) {
//	return ostream << polynomial.to_string();
//}
//
//Polynomial operator+(const PolyTerm& poly_term, const Polynomial& polynomial) {
//	return polynomial + poly_term;
//}
//
//Polynomial operator-(const PolyTerm& poly_term, const Polynomial& polynomial) {
//	return -1.0 * polynomial += poly_term;
//}
//
//Polynomial operator*(const PolyTerm& poly_term, const Polynomial& polynomial) {
//	return polynomial * poly_term;
//}
































































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
	//const auto domain_dimension = this->domain_dimension();
	//return this->gradient(domain_dimension);
//}
//
//VectorFunction<Polynomial> Polynomial::gradient(const size_t domain_dimension) const {
	//VectorFunction<Polynomial> result;
	//result.reserve(domain_dimension);

	//for (size_t i = 0; i < domain_dimension; ++i)
	//	result.push_back(ms::differentiate(*this, i));

	//return result;
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


