#include "../INC/MathVector.h"

MathVector& MathVector::operator+=(const MathVector& y) {
	if (this->size() != y.size())
		throw std::length_error("two vector have different length");

	const MKL_INT n = static_cast<int>(this->size());
	vdAdd(n, this->data(), y.data(), this->data());

	return *this;
};

MathVector& MathVector::operator-=(const MathVector& y) {
	if (this->size() != y.size())
		throw std::length_error("two vector have different length");

	const MKL_INT n = static_cast<int>(this->size());
	vdSub(n, this->data(), y.data(), this->data());

	return *this;
};

MathVector& MathVector::operator*=(const double scalar) {	
	const MKL_INT n = static_cast<int>(this->size());
	const double a = scalar;
	const MKL_INT incx = 1;
	cblas_dscal(n, a, this->data(), incx);

	return *this;
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

MathVector& MathVector::abs(void) {
	const MKL_INT n = static_cast<int>(this->size());
	vdAbs(n, this->data(), this->data());
	return *this;
}

bool MathVector::compare_with_finite_precision(const MathVector& other, const size_t ULP_precision) const {
	if (this->size() != other.size())
		return false;

	for (size_t i = 0; i < this->size(); ++i) {
		if (!ms::compare_double((*this)[i], other[i], ULP_precision))
			return false;
	}

	return true;
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

std::string MathVector::to_string(void) const {
	std::string result;
	result += "{ ";
	for (const auto& value : *this)
		result += ms::double_to_string(value) + ", ";
	result.erase(result.end() - 2, result.end());
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
	MathVector abs(const MathVector& x) {
		auto result = x;
		return result.abs();
	}
	
	MathVector normalize(const MathVector& x) {
		auto result = x;
		return result.normalize();
	}

	std::string double_to_string(const double val, const size_t precision) {
		std::stringstream stream;
		stream << std::setprecision(precision) << std::noshowpoint << val;
		return stream.str();
	}

	bool compare_double(const double d1, const double d2, const size_t ULP_precision) {
		const auto lower_ULP = d1 - std::nextafter(d1, std::numeric_limits<double>::lowest());
		const auto upper_ULP = std::nextafter(d1, std::numeric_limits<double>::max()) - d1;

		return d1 - ULP_precision * lower_ULP <= d2 && d2 <= d1 + ULP_precision * upper_ULP;
	}
}