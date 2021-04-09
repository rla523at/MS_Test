#include "../INC/Polynomial.h"

Monomial::Monomial(const size_t variable_index) { 
	this->exponent_set_.resize(variable_index + 1);
	this->exponent_set_.back() = 1;
};

Monomial::Monomial(const std::initializer_list<size_t> list)
	: exponent_set_{ list } {
	if (this->check_constant())
		this->exponent_set_.clear();
};

Monomial::Monomial(std::vector<size_t>&& exponent_set)
	: exponent_set_(std::move(exponent_set)) {
	if (this->check_constant())
		this->exponent_set_.clear();
};

Monomial& Monomial::operator*=(const Monomial& other) {
	const auto this_size = this->exponent_set_.size();
	const auto other_size = other.exponent_set_.size();

	if (this_size <= other_size) 
		this->exponent_set_.resize(other_size);

	for (size_t i = 0; i < other_size; ++i)
		this->exponent_set_[i] += other.exponent_set_[i];

	return *this;
}

Monomial Monomial::operator*(const Monomial& other) const {
	Monomial result = *this;
	return result *= other;
}

double Monomial::operator()(void) const {
	if (!this->is_constant())
		throw "variable monomial need variable values";
	else
		return 1.0;
}

double Monomial::operator()(const MathVector& variable_value_vector) const {
	if (this->is_constant()) 
			throw "constnat monomial does not need variable values";

	if (this->exponent_set_.size() != variable_value_vector.size())
		throw "variables are not matched";

	double result = 1.0;
	for (size_t i = 0; i < this->exponent_set_.size(); ++i)
		result *= std::pow(variable_value_vector[i], this->exponent_set_[i]);

	return result;
}

double Monomial::call_operator1(const MathVector& variable_vector) const {
	if (this->exponent_set_.size() != variable_vector.size())
		throw "variables are not matched";

	double result = 1.0;
	for (size_t i = 0; i < this->exponent_set_.size(); ++i) {
		if (this->exponent_set_[i] == 0 || variable_vector[i] == 0)
			continue;
		else
			result *= std::pow(variable_vector[i], this->exponent_set_[i]);
	}
	return result;
}

double Monomial::call_operator2(const MathVector& variable_vector) const {
	if (this->exponent_set_.size() != variable_vector.size())
		throw "variables are not matched";

	double result = 1.0;
	for (size_t i = 0; i < this->exponent_set_.size(); ++i)
		result *= std::pow(variable_vector[i], this->exponent_set_[i]);

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
	if (this->exponent_set_ == other.exponent_set_)
		return true;
	else
		return false;
}

size_t Monomial::exponent(size_t variable_index) const {
	if (this->exponent_set_.size() <= variable_index)
		throw std::out_of_range("variable index exceed range");
	else if (this->is_constant())
		throw "constant monomial does not have exponent";

	return this->exponent_set_[variable_index];
}

bool Monomial::is_constant(void) const {
	return this->exponent_set_.empty();
}

//size_t Monomial::num_variable(void) const {
//	const size_t zero = 0;
//	const auto num_zero = std::count(this->exponent_set_.begin(), this->exponent_set_.end(), zero);
//
//	return this->exponent_set_.size() - num_zero;
//};


size_t Monomial::order(void) const {
	if (this->is_constant())
		return 0;

	size_t order = 0;
	for (const auto exponent : this->exponent_set_)
		order += exponent;
	return order;
}

Monomial& Monomial::reduce_order(const size_t variable_index) {
	if (this->is_constant())
		throw "constant can not reduce order";
	
	if (this->exponent_set_[variable_index] == 0)
		throw "exponent can not be negative";
	else
		this->exponent_set_[variable_index]--;

	if (this->check_constant())
		this->exponent_set_.clear();
	
	return *this;
}

Monomial Monomial::reduce_order(const size_t variable_index) const {
	Monomial result = *this;
	return result.reduce_order(variable_index);
}

std::string Monomial::to_string(void) const {	
	if (this->is_constant()) 
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

bool Monomial::check_constant(void) const {
	for (const auto& exponent : this->exponent_set_) {
		if (exponent != 0)
			return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& ostream, const Monomial& monomial) {
	return ostream << monomial.to_string();
}
//
//Polynomial& Polynomial::operator*=(const double scalar) {
//	if (this->is_Zero())
//		return *this;
//
//	if (scalar == 0.0) {
//		*this = Polynomial();
//		return *this;
//	}
//
//	for (auto& [monomial, coefficient] : monomial_to_coefficient_)
//		coefficient *= scalar;
//
//	return *this;
//}
//
//Polynomial Polynomial::operator*(const double scalar) const{
//	if (scalar == 0)	
//		return Polynomial();
//
//	Polynomial result(*this);
//	return result *= scalar;
//}
//
//Polynomial& Polynomial::operator*=(const Polynomial& other) {
//	if (this->is_Zero() || other.is_Zero())
//		return *this = Polynomial();
//
//	Polynomial result;
//	for (const auto& [this_monomial, this_coefficient] : this->monomial_to_coefficient_)
//		for (const auto& [other_monomial, other_coefficient] : other.monomial_to_coefficient_) {
//			const auto result_coefficient = this_coefficient * other_coefficient;
//			auto result_monomial = this_monomial * other_monomial;
//			result.insert(result_coefficient, std::move(result_monomial));
//		}
//	return *this = std::move(result);
//}
//
//Polynomial Polynomial::operator*(const Polynomial& other) const {
//	Polynomial result(*this);
//	return result *= other;
//}
//
//Polynomial& Polynomial::operator+=(const Polynomial& other){
//	for (const auto& [other_monomial, other_coefficient] : other.monomial_to_coefficient_)
//		this->insert(other_coefficient, other_monomial);
//
//	return *this;
//}
//
//Polynomial& Polynomial::operator-=(const Polynomial& other){
//	for (const auto& [other_monomial, other_coefficient] : other.monomial_to_coefficient_)
//		this->insert(-other_coefficient, other_monomial);
//
//	return *this;
//}
//
//double Polynomial::operator()(const MathVector& variable_vector) const {
//	double result = 0.0;
//	for (const auto& [monomial, coefficient] : this->monomial_to_coefficient_)
//		result += coefficient * monomial(variable_vector);
//
//	return result;
//}
//
//Polynomial& Polynomial::differentiate(const size_t differential_variable) {
//	Polynomial result;
//
//	for (auto& [monomial, coefficient] : this->monomial_to_coefficient_)
//		result += coefficient * Math::differentiate(monomial, differential_variable);
//
//	*this = std::move(result);
//	return *this;
//}
//
//Polynomial Polynomial::differentiate(const size_t differential_variable) const {
//	Polynomial result;
//
//	for (auto& [monomial, coefficient] : this->monomial_to_coefficient_) 
//		result += coefficient * Math::differentiate(monomial, differential_variable);
//
//	return result;
//}
//
//
//void Polynomial::insert(const double coefficient, const Monomial& monomial) {
//	if (coefficient == 0) 
//		return;
//
//	auto iter = this->monomial_to_coefficient_.find(monomial);
//	if (iter == this->monomial_to_coefficient_.end())
//		this->monomial_to_coefficient_.emplace(monomial, coefficient);
//	else{
//		iter->second += coefficient;
//		if (iter->second == 0)
//			this->monomial_to_coefficient_.erase(iter);
//	}
//}
//
//void Polynomial::insert(const double coefficient, Monomial&& monomial) {
//	if (coefficient == 0)
//		return;
//
//	auto iter = this->monomial_to_coefficient_.find(monomial);
//	if (iter == this->monomial_to_coefficient_.end())
//		this->monomial_to_coefficient_.emplace(std::move(monomial), coefficient);
//	else {
//		iter->second += coefficient;
//		if (iter->second == 0)
//			this->monomial_to_coefficient_.erase(iter);
//	}
//}
//
//bool Polynomial::is_Zero() const{
//	if (this->monomial_to_coefficient_.size() == 0)
//		return true;
//
//	if (this->monomial_to_coefficient_.size() > 1)
//		return false;
//
//	const auto& [monomial, coefficient] = *this->monomial_to_coefficient_.begin();
//
//	if (coefficient == 0)
//		return true;
//	else
//		return false;
//}
//
//size_t Polynomial::order(void) const {
//	const auto& [highest_order_monomial, coefficient] = *this->monomial_to_coefficient_.rbegin();
//	return highest_order_monomial.order();
//}
//
//Polynomial& Polynomial::translate(const MathVector& translation_vector) {
//	Polynomial result;
//	for (const auto& [monomial, coefficient] : this->monomial_to_coefficient_)
//		result += coefficient * Math::translate(monomial, translation_vector);
//	*this = std::move(result);
//	return *this;
//}
//
//std::string Polynomial::to_String(void) const {
//	if (this->is_Zero())
//		return "{ 0 }";
//
//	std::string str;
//
//	str += "{ ";
//	const auto start_iter = this->monomial_to_coefficient_.rbegin();
//	const auto end_iter = this->monomial_to_coefficient_.rend();
//	for (auto iter = start_iter; iter != end_iter; ++iter) {
//		const auto& [monomial, coefficient] = *iter;
//		str += std::to_string(coefficient) + "\t";
//	}
//	StringEditor::erase_back(str, 1);
//	str += " } { ";
//	for (auto iter = start_iter; iter != end_iter; ++iter) {
//		const auto& [monomial, coefficient] = *iter;
//		str += monomial.to_string() + ", ";
//	}
//	StringEditor::erase_back(str, 2);
//	str += " }";
//	
//	return str;
//}
//
//
//namespace Math {
//	Polynomial translate(const Monomial& monomial, const MathVector& translation_vector) {
//		if (monomial.is_constant())
//			return Polynomial(monomial);
//
//		Polynomial result(1.0);
//		for (size_t i = 0; i < monomial.num_variable(); ++i) {
//			const auto exponent = monomial.exponent(i);
//
//			if (exponent == 0)
//				continue;
//
//			std::array<double, 2> coefficient_set = { 1, -translation_vector[i] };
//			std::array<Monomial, 2> monomial_set = { Monomial(i),Monomial() };
//			Polynomial translated_Xi(coefficient_set, monomial_set);
//
//			for (size_t i = 0; i < exponent; ++i)
//				result *= translated_Xi;
//		}
//
//		return result;
//	}
//
//	Polynomial differentiate(const Monomial& monomial, const size_t differential_variable_index) {
//		if (monomial.is_constant())
//			return Polynomial();
//
//		auto result_monomial = monomial;
//
//		const auto coefficient = static_cast<double>(monomial.exponent(differential_variable_index));
//		result_monomial.reduce_order(differential_variable_index);
//
//		return Polynomial(coefficient, result_monomial);
//	}
//}





//
//double PolynomialSet::operator()(const MathVector& variable_vector) const {
//	double result = 0.0;
//	for (const auto& [op, polynomial] : this->polynomial_set_) {
//		switch (op) {
//		case  BinaryOperator::plus:
//			result += polynomial(variable_vector);
//			break;
//		case BinaryOperator::multiply:
//			result *= polynomial(variable_vector);
//			break;
//		}
//	}
//	return result;
//
//	//double result = 0.0;
//	//if (this->rational_set_.size() == 1){
//	//	for (const auto& [op, polynomial] : this->polynomial_set_) {
//	//		switch (op) {
//	//		case  BinaryOperator::plus:
//	//			result += polynomial(variable_vector);
//	//			break;
//	//		case BinaryOperator::multiply:
//	//			result *= polynomial(variable_vector);
//	//			break;
//	//		}
//	//	}
//	//}
//	//else {
//	//	for (const auto& [op, rational] : this->rational_set_) {
//	//		switch (op) {
//	//		case  BinaryOperator::plus:
//	//			result += rational(variable_vector);
//	//			break;
//	//		case BinaryOperator::multiply:
//	//			result *= rational(variable_vector);
//	//			break;
//	//		}
//	//	}
//	//}
//	//return result;
//
//
//	//if (this->junction_indicator_.empty()) {
//	//	const size_t start_index = 0;
//	//	const size_t end_index = this->polynomial_set_.size();
//	//	return this->calculate(variable_vector, start_index, end_index);
//	//}
//
//	//const auto num_junction = this->junction_indicator_.size();
//	//const auto num_intermediate_result = num_junction + 1;
//	//std::vector<double> result_set(num_intermediate_result);
//
//	//const auto num_index = num_intermediate_result + 1;
//	//std::vector<size_t> index_set;
//	//index_set.reserve(num_index);
//
//	//index_set.emplace_back(0);
//	//for (const auto& [junction_binary_operator, junction_index] : this->junction_indicator_)
//	//	index_set.emplace_back(junction_index);
//	//index_set.emplace_back(this->polynomial_set_.size());
//
//	//for (size_t i = 0; i < num_intermediate_result; ++i) {
//	//	const auto& start_index = index_set[i];
//	//	const auto& end_index = index_set[i + 1];
//
//	//	result_set[i] = this->calculate(variable_vector, start_index, end_index);
//	//}
//
//	//double result = 0.0;
//	//for (size_t i = 0; i < num_junction; ++i) {
//	//	const auto& [junction_binary_operator, junction_index] = this->junction_indicator_[i];
//	//	switch (junction_binary_operator) {
//	//	case  BinaryOperator::plus:
//	//		result += result_set[i] + result_set[i + 1];
//	//		break;
//	//	case BinaryOperator::multiply:
//	//		result *= result_set[i + 1];
//	//		break;
//	//	}
//	//}
//
//	//return result;
//}
//
////double Rational::calculate(const MathVector& variable_vector, const size_t start_index, const size_t end_index) const {
////	double result = 0.0;
////	for (size_t index = start_index; index < end_index; ++index) {
////		const auto& [binary_operator, polynomial] = this->polynomial_set_[index];
////		switch (binary_operator) {
////		case  BinaryOperator::plus:
////			result += polynomial(variable_vector);
////			break;
////		case BinaryOperator::multiply:
////			result *= polynomial(variable_vector);
////			break;
////		}
////	}
////	return result;
////}
//
////double Rational::calculate(const MathVector& variable_vector) const {
////	if (this->rational_set_.size() == 1)
////		return rational_set_.front().second(variable_vector);
////	else {
////		double result = 0.0;
////		for (const auto& [op, ra] : this->rational_set_) {
////			switch (op) {
////			case  BinaryOperator::plus:
////				result += ra.calculate(variable_vector);
////				break;
////			case BinaryOperator::multiply:
////				result *= ra.calculate(variable_vector);
////				break;
////			}
////		}
////	}	
////}