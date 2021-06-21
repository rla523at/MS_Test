#pragma once

#include <array>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include <type_traits>


template <size_t Dim> class EuclideanVector
{
public:
	template <typename... Args> EuclideanVector(Args... args) : elements_{ static_cast<double>(args)... } {};

	EuclideanVector operator+(const EuclideanVector& y) const;
	EuclideanVector operator-(const EuclideanVector& y) const;
	EuclideanVector operator*(const double scalar) const;
	bool operator==(const EuclideanVector& y) const;
	double operator[](const size_t position) const;

	constexpr size_t dimension(void) const;
	std::string to_string(void) const;

private:
	std::array<double, Dim> elements_;
};
template <typename... Args> EuclideanVector(Args... args)->EuclideanVector<sizeof...(Args)>;  //user-defined deduction guides
template <size_t Dim> std::ostream& operator<<(std::ostream& os, const EuclideanVector<Dim>& x);


template <typename T, size_t RangeDim> class VectorFunction
{
	using Function = T;

public:
	template <typename... Args> VectorFunction(Args... args) : elements_{ args... } {};

	const Function& operator[](const size_t position) const;

private:
	std::array<Function, RangeDim> elements_;
};
template <class First, class... Rest> struct Enforce_same_callable_type; 
template <class First, class... Rest> VectorFunction(First, Rest...)->VectorFunction<typename Enforce_same_callable_type<First, Rest...>::type, 1 + sizeof...(Rest)>; // user defined deduction guides

namespace ms {
	template <typename T> std::string to_string(const T& arg);
	template <typename T> struct is_callable;
	template <typename T> inline constexpr bool is_callable_v = is_callable<T>::value;
}


// Template Definition Part
template <size_t Dim> EuclideanVector<Dim> EuclideanVector<Dim>::operator+(const EuclideanVector& y) const {
	auto result = *this;
	for (size_t i = 0; i < this->dimension(); ++i)
		result.elements_[i] += y.elements_[i];
	return result;
}

template <size_t Dim> EuclideanVector<Dim> EuclideanVector<Dim>::operator-(const EuclideanVector& y) const {
	auto result = *this;
	for (size_t i = 0; i < this->dimension(); ++i)
		result.elements_[i] -= y.elements_[i];
	return result;
}

template <size_t Dim> EuclideanVector<Dim> EuclideanVector<Dim>::operator*(const double scalar) const {
	auto result = *this;
	for (auto& element : result.elements_)
		element *= scalar;
	return result;
}

template <size_t Dim> bool EuclideanVector<Dim>::operator==(const EuclideanVector& y) const {
	for (size_t i = 0; i < this->dimension(); ++i) {
		if (this->elements_[i] != y.elements_[i])
			return false;
	}
	return true;
}

template <size_t Dim> double EuclideanVector<Dim>::operator[](const size_t position) const {
	if (Dim <= position)
		throw std::out_of_range("Vector Function out of range");

	return this->elements_[position];
}

template <size_t Dim> constexpr size_t EuclideanVector<Dim>::dimension(void) const {
	return Dim;
};

template <size_t Dim> std::string EuclideanVector<Dim>::to_string(void) const {
	std::string result = "[";
	for (const auto& element : this->elements_)
		result += ms::to_string(element) + ",";
	result.pop_back();
	result += "]";
	return result;
}

template <size_t Dim> std::ostream& operator<<(std::ostream& os, const EuclideanVector<Dim>& x) {
	return os << x.to_string();
};

template <typename T, size_t RangeDim> const T& VectorFunction<T, RangeDim>::operator[](const size_t position) const {
	if (RangeDim <= position)
		throw std::out_of_range("Vector Function out of range");

	return this->elements_[position];
}

template<typename T> struct ms::is_callable {

//	//https://stackoverflow.com/questions/15393938/find-out-whether-a-c-object-is-callable
//	//https://en.wikibooks.org/wiki/More_C++_Idioms/Member_Detector
private:
	//typedef char(&yes)[1];
	//typedef char(&no)[2];
	using yes = char[1];
	using no = char[2];

	struct Fallback { void operator()(); };
	struct Derived : T, Fallback {};

	template<typename U, U> struct Check;

	template<typename>
	static yes& test(...);

	template<typename C>
	static no& test(Check<void (Fallback::*)(), &C::operator()>*);	//class 멤버 함수 포인터와 SFINAE idiom을 활용한 문법

	//using yes = char[1];
	//using no = char[2];

	//struct Fallback { int operator(); };
	//struct Derived : T, Fallback {};

	//template<typename U>
	//static yes& test(U*);

	//template<typename U>
	//static no& test(decltype(U::member)*);

public:
	static constexpr bool value = sizeof(test<Derived>(nullptr)) == sizeof(yes);

	////https://people.eecs.berkeley.edu/~brock/blog/detection_idiom.php
	//template <typename U>
	//static constexpr decltype(std::declval<U>()(), bool()) test(int) { return true; }

	//template <typename U>
	//static constexpr bool test(...) { return false; }

	//static constexpr bool value = test<T>(int());

};

template <class First, class... Rest> struct Enforce_same_callable_type {
	static_assert((std::is_same_v<First, Rest>&&...), "Vector Function constructor requires all the arguments should be same type.");
	static_assert((ms::is_callable_v<First>), "Vector Function constructor requires all the arguments should be callable.");
	using type = First;
};

namespace ms {
	template <typename T>
	std::string to_string(const T& arg) {
		if constexpr (std::is_class_v<T>)
			return arg.to_string();
		else if constexpr (std::is_floating_point_v<T>) {
			constexpr size_t precision = 15;
			std::stringstream stream;
			stream << std::setprecision(precision) << std::noshowpoint << arg;
			return stream.str();
		}
		else
			return std::to_string(arg);
	}
}




//template <size_t Dim> EuclideanVector<Dim> EuclideanVector<Dim>::operator+(const EuclideanVector& y) const {
//	return this->vector_ + y.vector_;
//}
//template <size_t Dim> EuclideanVector<Dim> EuclideanVector<Dim>::operator-(const EuclideanVector& y) const {
//	return this->vector_ - y.vector_;
//}
//template <size_t Dim> EuclideanVector<Dim> EuclideanVector<Dim>::operator*(const double scalar) const {
//	return this->vector_ * scalar;
//}
//template <size_t Dim> bool EuclideanVector<Dim>::operator==(const EuclideanVector& y) const {
//	return this->vector_ == y.vector_;
//}
//template <size_t Dim> constexpr size_t EuclideanVector<Dim>::dimension(void) const {
//	return Dim;
//}
//template <size_t Dim> std::string EuclideanVector<Dim>::to_string(void) const {
//	return this->vector_.to_string();
//}
//
//
//template <size_t Dim>
//std::ostream& operator<<(std::ostream& os, const EuclideanVector<Dim>& x) {
//	return os << x.to_string();
//};

//#include <algorithm>
//#include <fstream>
//#include <iomanip>	//set precision
//#include <mkl.h>
//#include <stdexcept>
//#include <string>
//#include <sstream>
//#include <vector>
//
////variable vector //real vector
//class MathVector : public std::vector<double>
//{
//public:
//	template <typename ... Vals>
//	explicit MathVector(Vals&&... values) :std::vector<double>(std::forward<Vals>(values)...) {};
//	MathVector(std::initializer_list<double> list) :std::vector<double>(list) {};
//	MathVector(const std::vector<double>& vec) :std::vector<double>(vec) {};
//	MathVector(const double) = delete;
//	
//	MathVector& operator+=(const MathVector& y);
//	MathVector& operator-=(const MathVector& y);
//	MathVector& operator*=(const double scalar);
	//MathVector operator+(const MathVector& y) const;
	//MathVector operator-(const MathVector& y) const;
	//MathVector operator*(const double scalar) const;
//
//	MathVector& abs(void);	
//	bool compare_with_finite_precision(const MathVector& other, const size_t ULP_precision = 4) const;
//	double inner_product(const MathVector& other) const;
//	double L2_Norm(void) const;
//	void merge(const MathVector& other);
//	void merge(MathVector&& other);
//	MathVector& normalize(void);
//	std::string to_string(void) const;
//};
//std::ostream& operator<<(std::ostream& os, const MathVector& x);
//MathVector operator*(const double scalar, const MathVector& x);
//
//
//namespace ms {
//	MathVector abs(const MathVector& x);
//	MathVector normalize(const MathVector& x);
//	std::string double_to_string(const double val, const size_t precision = 17);
//	bool compare_double(const double d1, const double d2, const size_t ULP_precision = 4);
//}
//
//
//
//
//
//template <typename T>
//class VectorFunction : public std::vector<T> // template 특수화를 통해 하나로 묶어보자
//{
//public:
//	template <typename ... Vals>
//	explicit VectorFunction(Vals&&... values) :std::vector<T>(std::forward<Vals>(values)...) {};
//	VectorFunction(std::initializer_list<T> list) :std::vector<T>(list) {};
//
//	MathVector operator()(const MathVector& variable_vector) const {
//		MathVector result;
//		result.reserve(this->size());
//		for (const auto& function : *this)
//			result.push_back(function(variable_vector));
//		return result;
//	}
//
//	std::vector<MathVector> operator()(const std::vector<MathVector>& variable_vector_set) const {
//		std::vector<MathVector> result;
//		result.reserve(variable_vector_set.size());
//		for (const auto& variable_vector : variable_vector_set)
//			result.push_back((*this)(variable_vector));
//		return result;
//	}
//
//	VectorFunction<T> cross_product(const VectorFunction<T>& other) const {
//		constexpr size_t range_dimension = 3;
//		if (this->size() != range_dimension || other.size() != range_dimension)
//			throw std::runtime_error("cross product is only defined on R^3 range");
//
//		VectorFunction<T> result(range_dimension, 0.0);
//		result[0] = (*this)[1] * other[2] - (*this)[2] * other[1];
//		result[1] = (*this)[2] * other[0] - (*this)[0] * other[2];
//		result[2] = (*this)[0] * other[1] - (*this)[1] * other[0];
//
//		return result;
//	}
//
//	VectorFunction<T>& be_derivative(const size_t variable_index) {
//		for (auto& func : *this)
//			func.be_derivative(variable_index);
//		return *this;
//	}
//
//	VectorFunction<T> differentiate(const size_t variable_index) const {
//		auto result = *this;
//		return result.be_derivative(variable_index);
//	}
//
//	size_t domain_dimension(void) const {
//		const size_t num_function = this->size();
//
//		std::vector<size_t> domain_dimension_set(num_function);
//		for (size_t i = 0; i < num_function; ++i)
//			domain_dimension_set[i] = (*this)[i].domain_dimension();
//
//		return *std::max_element(domain_dimension_set.begin(), domain_dimension_set.end());
//	}
//
//	//T L2_norm(void) const {
//	//	T result;
//	//	for (const auto& func : *this)
//	//		result += (func ^ 2);
//	//	return result.power(0.5);
//	//}
//
//	std::string to_string(void) const {
//		std::string result = "{ ";
//		for (const auto& func : *this)
//			result += func.to_string() + ", ";
//		result.erase(result.end() - 2, result.end());
//		result += " }";
//		return result;
//	}
//};

//template <typename T>
//std::ostream& operator<<(std::ostream& os, const VectorFunction<T>& x) {
//	return os << x.to_string();
//};

//template <size_t Dim>
//class EuclideanVector
//{
//public:
//	template <typename... Args> EuclideanVector(Args... args) : vector_(static_cast<double>(args)...) {};
//	EuclideanVector(const Vector<double, Dim>& vector) : vector_(vector) {};
//
//	EuclideanVector operator+(const EuclideanVector& y) const;
//	EuclideanVector operator-(const EuclideanVector& y) const;
//	EuclideanVector operator*(const double scalar) const;
//	bool operator==(const EuclideanVector& y) const;
//		
//	constexpr size_t dimension(void) const;
//	std::string to_string(void) const;
//
//private:
//	Vector<double, Dim> vector_;
//};
//template <typename... Args> EuclideanVector(Args... args)->EuclideanVector<sizeof...(Args)>; //user-defined deduction guides
//template <size_t Dim> std::ostream& operator<<(std::ostream& os, const EuclideanVector<Dim>& x);

//template <typename T, size_t Dim>
//class Vector
//{
//protected:
//	template <typename... Args>
//	Vector(Args... args) : elements_{ args... } {};
//
//public:
//	Vector operator+(const Vector& y) const;
//	Vector operator-(const Vector& y) const;
//	Vector operator*(const double scalar) const;
//	bool operator==(const Vector& y) const;
//
//	constexpr size_t dimension(void) const;
//	std::string to_string(void) const;
//
//protected:
//	std::array<T, Dim> elements_;
//};
//template <typename T, size_t Dim>
//std::ostream& operator<<(std::ostream& os, const Vector<T, Dim>& x);
//
//
//template <size_t Dim>
//class EuclideanVector : public Vector<double, Dim>
//{
//public:
//	template <typename... Args>
//	EuclideanVector(Args... args) : Vector<double, sizeof...(Args)>(static_cast<double>(args)...) {};
//
//};
//template <typename... Args> //user-defined deduction guides
//EuclideanVector(Args... args)->EuclideanVector<sizeof...(Args)>; 
//template <size_t Dim>
//std::ostream& operator<<(std::ostream& os, const EuclideanVector<Dim>& x);
//
//namespace ms {
//	template <typename T>
//	std::string to_string(const T& arg);
//}