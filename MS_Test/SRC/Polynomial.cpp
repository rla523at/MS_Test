#include "../INC/Polynomial.h"

Monomial Monomial::operator*(const Monomial& other) const {
	const auto num_variable_this = this->num_variable();
	const auto num_variable_other = other.num_variable();

	std::vector<size_t> exponent_set(std::max(num_variable_this, num_variable_other));
	for (size_t i = 0; i < num_variable_this; ++i)
		exponent_set[i] += this->exponent_set_[i];
	for (size_t i = 0; i < num_variable_other; ++i)
		exponent_set[i] += other.exponent_set_[i];

	return std::move(exponent_set);
}

double Monomial::operator()(const MathVector& variable_vector) const {
	double result = 1.0;
	for (size_t i = 0; i < this->num_variable(); ++i)
		result *= std::pow(variable_vector[i], this->exponent_set_[i]);

	return result;
}

bool Monomial::operator<(const Monomial& other) const {
	const auto this_order = this->order();
	const auto other_order = other.order();

	if (this_order == other_order) {
		auto min_num_variable = std::min(this->num_variable(), other.num_variable());
		for (size_t i = 0; i < min_num_variable; ++i) {
			if (this->exponent_set_[i] == other.exponent_set_[i])
				continue;

			return this->exponent_set_[i] < other.exponent_set_[i];
		}
		return false;
	}

	return this_order < other_order;
}

size_t Monomial::exponent(size_t variable_index) const {
	if (this->num_variable() <= variable_index)
		return 0;
	else
		return this->exponent_set_[variable_index];
}

bool Monomial::is_Constant(void) const {
	for (const auto exponent : this->exponent_set_) {
		if (exponent != 0)
			return false;
	}
	return true;
}

size_t Monomial::order(void) const {
	size_t order = 0;
	for (const auto exponent : this->exponent_set_)
		order += exponent;
	return order;
}

void Monomial::reduce_Order(const size_t variable_index) {
	if (this->exponent(variable_index) == 0)
		*this = Monomial();
	else
		this->exponent_set_[variable_index]--;
}

std::string Monomial::to_String(void) const {
	std::string str;
	if (this->is_Constant()) {
		str += "(1)";
		return str;
	}

	//str += "(";
	//for (size_t i = 0; i < this->num_variable(); ++i) {
	//	if (this->exponent_set_[i] == 0)
	//		continue;
	//	else
	//		str += " (x" + Editor::to_String(i) + ")^(" + Editor::to_String(this->exponent_set_[i]) + ") ";
	//}
	//str += ")";

	for (size_t i = 0; i < this->num_variable(); ++i) {
		if (this->exponent_set_[i] == 0)
			continue;
		else
			str += "(x" + Editor::to_String(i) + ")^(" + Editor::to_String(this->exponent_set_[i]) + ")";
	}

	return str;
}


Polynomial& Polynomial::operator*=(const double scalar) {
	if (this->is_Zero())
		return *this;

	if (scalar == 0.0) {
		*this = Polynomial();
		return *this;
	}

	for (auto& [monomial, coefficient] : monomial_to_coefficient_)
		coefficient *= scalar;

	return *this;
}

Polynomial Polynomial::operator*(const double scalar) const{
	if (scalar == 0)	
		return Polynomial();

	Polynomial result(*this);
	return result *= scalar;
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {
	if (this->is_Zero() || other.is_Zero())
		return *this = Polynomial();

	Polynomial result;
	for (const auto& [this_monomial, this_coefficient] : this->monomial_to_coefficient_)
		for (const auto& [other_monomial, other_coefficient] : other.monomial_to_coefficient_) {
			const auto result_coefficient = this_coefficient * other_coefficient;
			auto result_monomial = this_monomial * other_monomial;
			result.insert(result_coefficient, std::move(result_monomial));
		}
	return *this = std::move(result);
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
	Polynomial result(*this);
	return result *= other;
}

Polynomial& Polynomial::operator+=(const Polynomial& other){
	for (const auto& [other_monomial, other_coefficient] : other.monomial_to_coefficient_)
		this->insert(other_coefficient, other_monomial);

	return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other){
	for (const auto& [other_monomial, other_coefficient] : other.monomial_to_coefficient_)
		this->insert(-other_coefficient, other_monomial);

	return *this;
}

double Polynomial::operator()(const MathVector& variable_vector) const {
	double result = 0.0;
	for (const auto& [monomial, coefficient] : this->monomial_to_coefficient_)
		result += coefficient * monomial(variable_vector);

	return result;
}

Polynomial& Polynomial::differentiate(const size_t differential_variable) {
	Polynomial result;

	for (auto& [monomial, coefficient] : this->monomial_to_coefficient_)
		result += coefficient * Math::differentiate(monomial, differential_variable);

	*this = std::move(result);
	return *this;
}

Polynomial Polynomial::differentiate(const size_t differential_variable) const {
	Polynomial result;

	for (auto& [monomial, coefficient] : this->monomial_to_coefficient_) 
		result += coefficient * Math::differentiate(monomial, differential_variable);

	return result;
}


void Polynomial::insert(const double coefficient, const Monomial& monomial) {
	if (coefficient == 0) 
		return;

	auto iter = this->monomial_to_coefficient_.find(monomial);
	if (iter == this->monomial_to_coefficient_.end())
		this->monomial_to_coefficient_.emplace(monomial, coefficient);
	else{
		iter->second += coefficient;
		if (iter->second == 0)
			this->monomial_to_coefficient_.erase(iter);
	}
}

void Polynomial::insert(const double coefficient, Monomial&& monomial) {
	if (coefficient == 0)
		return;

	auto iter = this->monomial_to_coefficient_.find(monomial);
	if (iter == this->monomial_to_coefficient_.end())
		this->monomial_to_coefficient_.emplace(std::move(monomial), coefficient);
	else {
		iter->second += coefficient;
		if (iter->second == 0)
			this->monomial_to_coefficient_.erase(iter);
	}
}

bool Polynomial::is_Zero() const{
	if (this->monomial_to_coefficient_.size() == 0)
		return true;

	if (this->monomial_to_coefficient_.size() > 1)
		return false;

	const auto& [monomial, coefficient] = *this->monomial_to_coefficient_.begin();

	if (coefficient == 0)
		return true;
	else
		return false;
}

size_t Polynomial::order(void) const {
	const auto& [highest_order_monomial, coefficient] = *this->monomial_to_coefficient_.rbegin();
	return highest_order_monomial.order();
}

Polynomial& Polynomial::translate(const MathVector& translation_vector) {
	Polynomial result;
	for (const auto& [monomial, coefficient] : this->monomial_to_coefficient_)
		result += coefficient * Math::translate(monomial, translation_vector);
	*this = std::move(result);
	return *this;
}

std::string Polynomial::to_String(void) const {
	if (this->is_Zero())
		return "{ 0 }";

	std::string str;

	str += "{ ";
	const auto start_iter = this->monomial_to_coefficient_.rbegin();
	const auto end_iter = this->monomial_to_coefficient_.rend();
	for (auto iter = start_iter; iter != end_iter; ++iter) {
		const auto& [monomial, coefficient] = *iter;
		str += Editor::to_String(coefficient) + "\t";
	}
	StringEditor::erase_back(str, 1);
	str += " } { ";
	for (auto iter = start_iter; iter != end_iter; ++iter) {
		const auto& [monomial, coefficient] = *iter;
		str += Editor::to_String(monomial) + ", ";
	}
	StringEditor::erase_back(str, 2);
	str += " }";

	
	

	return str;
}


namespace Math {
	Polynomial translate(const Monomial& monomial, const MathVector& translation_vector) {
		if (monomial.is_Constant())
			return Polynomial(monomial);

		Polynomial result(1.0);
		for (size_t i = 0; i < monomial.num_variable(); ++i) {
			const auto exponent = monomial.exponent(i);

			if (exponent == 0)
				continue;

			std::array<double, 2> coefficient_set = { 1, -translation_vector[i] };
			std::array<Monomial, 2> monomial_set = { Monomial(i),Monomial() };
			Polynomial translated_Xi(coefficient_set, monomial_set);

			for (size_t i = 0; i < exponent; ++i)
				result *= translated_Xi;
		}

		return result;
	}

	Polynomial differentiate(const Monomial& monomial, const size_t differential_variable_index) {
		if (monomial.is_Constant())
			return Polynomial();

		auto result_monomial = monomial;

		const auto coefficient = static_cast<double>(monomial.exponent(differential_variable_index));
		result_monomial.reduce_Order(differential_variable_index);

		return Polynomial(coefficient, result_monomial);
	}
}





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