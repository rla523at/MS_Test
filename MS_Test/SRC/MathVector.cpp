#include "../INC/MathVector.h"

MathVector& MathVector::operator+=(const MathVector& y) { //x = x + y
	MathVector& x = *this;

	if (x.size() != y.size())
		throw "two vector have different length";

	const MKL_INT n = static_cast<int>(x.size());
	vdAdd(n, x.data(), y.data(), x.data());

	return x;
};

MathVector& MathVector::operator-=(const MathVector& y) { //x = x - y
	MathVector& x = *this;

	if (x.size() != y.size())
		throw "two vector have different length";

	const MKL_INT n = static_cast<int>(x.size());
	vdSub(n, x.data(), y.data(), x.data());

	return x;
};

MathVector& MathVector::operator*=(const double scalar) { //x = a * x	
	MathVector& x = *this;

	const MKL_INT n = static_cast<int>(x.size());
	const double a = scalar;
	const MKL_INT incx = 1;
	cblas_dscal(n, a, x.data(), incx);

	return x;
};

MathVector& MathVector::operator*=(const MathVector& y) { //x_i *= y_i value by value
	MathVector& x = *this;

	if (x.size() != y.size())
		throw "two vector have different length";

	const MKL_INT n = static_cast<int>(x.size());
	vdMul(n, x.data(), y.data(), x.data());

	return x;
};

MathVector MathVector::operator+(const MathVector& y) const {
	MathVector x = *this;
	return x += y;
};

MathVector MathVector::operator-(const MathVector& y) const {
	MathVector x = *this;
	return x -= y;
};

MathVector MathVector::operator*(const double scalar) const {
	MathVector x = *this;
	return x *= scalar;
};

MathVector MathVector::operator*(const MathVector& y) const {
	MathVector x = *this;
	return x *= y;
};


MathVector& MathVector::abs(void) {
	const MKL_INT n = static_cast<int>(this->size());
	vdAbs(n, this->data(), this->data());
	return *this;
}

MathVector MathVector::abs(void) const {
	MathVector x = *this;
	return x.abs();
}

double MathVector::inner_product(const MathVector& other) const {
	if (this->size() != other.size())
		throw "two vector have different length";

	const MKL_INT n = static_cast<int>(this->size());
	const MKL_INT incx = 1;
	const MKL_INT incy = 1;
	return cblas_ddot(n, this->data(), incx, other.data(), incy);
}

double MathVector::L2_Norm(void) const {
	const MKL_INT n = static_cast<int>(this->size());
	const MKL_INT incx = 1;
	return cblas_dnrm2(n, this->data(), incx);
}

void MathVector::merge(const MathVector& other) {
	this->insert(this->end(), other.begin(), other.end());
}

void MathVector::merge(MathVector&& other) {
	this->insert(this->end(), std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()));
}

MathVector& MathVector::normalize(void) {
	return *this *= 1.0 / this->L2_Norm();
}

MathVector MathVector::normalize(void) const {
	MathVector result = *this;
	return result.normalize();
}

MathVector& MathVector::sqrt(void) {
	const MKL_INT n = static_cast<int>(this->size());
	vdSqrt(n, this->data(), this->data());
	return *this;
}

MathVector MathVector::sqrt(void) const {
	MathVector x = *this;
	return x.sqrt();
}

std::string MathVector::to_string(void) const {
	std::string result;
	result += "{ ";
	for (const auto& value : *this)
		result += std::to_string(value) + ", ";
	result += " }";
	return result;
}

std::ostream& operator<<(std::ostream& os, const MathVector& x) {
	return os << x.to_string();
}

MathVector operator*(const double scalar, const MathVector& x) {
	return x * scalar;
}


namespace ms {
	std::string double_to_string(const double val, const size_t precision) {
		std::stringstream stream;
		stream << std::setprecision(precision) << std::noshowpoint << val;
		return stream.str();
	}

	double inner_product(const MathVector& v1, const MathVector& v2) {
		return v1.inner_product(v2);
	}
}