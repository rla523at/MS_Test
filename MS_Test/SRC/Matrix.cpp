#include "../INC/Matrix.h"

RowMajorMatrix::RowMajorMatrix(const size_t num_row, const size_t num_column)
	:num_row_(num_row), num_column_(num_column) {
	this->value_vector_.resize(this->num_row_ * this->num_column_);
}

RowMajorMatrix::RowMajorMatrix(const size_t matrix_order)
	:num_row_(matrix_order), num_column_(matrix_order) {
	this->value_vector_.resize(this->num_row_ * this->num_column_);

	for (size_t i = 0; i < matrix_order; ++i)
		this->at(i, i) = 1;
}

RowMajorMatrix::RowMajorMatrix(const size_t num_row, const size_t num_column, const MathVector& value)
	: num_row_(num_row), num_column_(num_column), value_vector_(value) {
	this->inspect_value_size();
};

RowMajorMatrix::RowMajorMatrix(const size_t num_row, const size_t num_column, MathVector&& value)
	: num_row_(num_row), num_column_(num_column), value_vector_(std::move(value)) {
	this->inspect_value_size();
};

RowMajorMatrix& RowMajorMatrix::operator+=(const RowMajorMatrix& B) {
	if (this->size() != B.size())
		throw std::length_error("size of matrix is not same");

	if (this->transpose_type_ != B.transpose_type_) {
		this->transpose_value();
		if (this->is_transposed())
			this->transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;
		else
			this->transpose_type_ = CBLAS_TRANSPOSE::CblasTrans;
	}

	this->value_vector_ += B.value_vector_;

	return *this;
}

RowMajorMatrix& RowMajorMatrix::operator*=(const RowMajorMatrix& other) {
	this->value_vector_ = this->multiply_value(other);
	this->num_column_ = other.num_column_;
	this->transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;

	return *this;
}

MathVector RowMajorMatrix::operator*(const MathVector& x) const {
	if (this->num_column_ != x.size())
		throw std::length_error("matrix vector size is not match");

	const auto layout = CBLAS_LAYOUT::CblasRowMajor;
	const auto trans = this->transpose_type_;
	MKL_INT m;
	MKL_INT n;
	if (trans == CBLAS_TRANSPOSE::CblasNoTrans) {
		m = static_cast<int>(this->num_row_);
		n = static_cast<int>(this->num_column_);
	}
	else {
		m = static_cast<int>(this->num_column_);
		n = static_cast<int>(this->num_row_);
	}
	const double alpha = 1;
	const MKL_INT lda = n;
	const MKL_INT incx = 1;
	const MKL_INT beta = 0;
	const MKL_INT incy = 1;

	MathVector y(this->num_row_);
	cblas_dgemv(layout, trans, m, n, alpha, this->value_vector_.data(), lda, x.data(), incx, beta, y.data(), incy);

	return y;
}

RowMajorMatrix RowMajorMatrix::operator*(const RowMajorMatrix& other) const {
	return RowMajorMatrix(this->num_row_, other.num_column_, this->multiply_value(other));
}

bool RowMajorMatrix::operator==(const RowMajorMatrix& other) const {
	if (this->size() != other.size())
		return false;

	if (this->transpose_type_ != other.transpose_type_) {
		auto tmp = other;
		tmp.transpose_value();
		return this->value_vector_ == tmp.value_vector_;
	}
	
	return this->value_vector_ == other.value_vector_;
}

double& RowMajorMatrix::at(const size_t irow, const size_t jcolumn) {
	this->inspect_range(irow, jcolumn);
	if (this->is_transposed()) 
		return this->value_vector_[jcolumn * this->num_row_ + irow];
	else 
		return this->value_vector_[irow * this->num_column_ + jcolumn];
}

double RowMajorMatrix::at(const size_t irow, const size_t jcolumn) const {
	this->inspect_range(irow, jcolumn);
	if (this->is_transposed())
		return this->value_vector_[jcolumn * this->num_row_ + irow];
	else
		return this->value_vector_[irow * this->num_column_ + jcolumn];
}

RowMajorMatrix& RowMajorMatrix::be_inverse(void) {
	if (!this->is_square_matrix())
		throw std::runtime_error("can not inverse non square matrix");

	const auto ipiv = this->PLU_decomposition();

	const int matrix_layout = LAPACK_ROW_MAJOR;
	const lapack_int n = static_cast<int>(this->num_row_);
	const lapack_int lda = n;
	const lapack_int info = LAPACKE_dgetri(matrix_layout, n, this->value_vector_.data(), lda, ipiv.data());

	if (info > 0)
		throw std::runtime_error("U is singular matrix in L-U decomposition");
	else if (info < 0)
		throw std::runtime_error("fail to inverse the matrix");

	return *this;
}

RowMajorMatrix& RowMajorMatrix::be_transpose(void) {
	std::swap(this->num_row_, this->num_column_);

	if (this->is_transposed())
		this->transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;
	else
		this->transpose_type_ = CBLAS_TRANSPOSE::CblasTrans;

	return *this;
}

bool RowMajorMatrix::compare_with_finitie_precision(const RowMajorMatrix& other, const size_t ULP_precision) const {
	if (this->size() != other.size())
		return false;

	if (this->transpose_type_ != other.transpose_type_) {
		auto tmp = other;
		tmp.transpose_value();
		return this->value_vector_.compare_with_finite_precision(tmp.value_vector_,ULP_precision);
	}

	return this->value_vector_.compare_with_finite_precision(other.value_vector_,ULP_precision);
}

RowMajorMatrix& RowMajorMatrix::change_column(const size_t column_index, const MathVector& value) {
	if (this->num_column_ <= column_index)
		throw std::out_of_range("column index exceed range");

	if (this->num_row_ != value.size())
		throw std::length_error("vector size error");

	for (size_t i = 0; i < this->num_row_; ++i) 
		this->at(i, column_index) = value[i];

	return *this;
}

RowMajorMatrix RowMajorMatrix::inverse(void) const {
	auto result = *this;
	return result.be_inverse();
}

RowMajorMatrix RowMajorMatrix::part(const size_t row_start_index, const size_t row_end_index, const size_t column_start_index, const size_t column_end_index) const {
	const auto num_row = row_end_index - row_start_index + 1;
	const auto num_colmn = column_end_index - column_start_index + 1;

	RowMajorMatrix result(num_row, num_colmn);
	for (size_t i = 0; i < num_row; ++i)
		for (size_t j = 0; j < num_row; ++j)
			result.at(i, j) = this->at(row_start_index + i, column_start_index + j);

	return result;
}

MathVector RowMajorMatrix::row(const size_t row_index) const {
	if (this->num_row_ <= row_index)
		throw std::out_of_range("row index exceed range");
	
	MathVector result(this->num_column_);
	for (size_t i = 0; i < this->num_column_; ++i)
		result[i] = this->at(row_index, i);

	return result;
}

std::pair<size_t, size_t> RowMajorMatrix::size(void) const {
	return { this->num_row_,this->num_column_ };
}

std::string RowMajorMatrix::to_string(void) const {
	std::string str;
	str += "\n";
	for (size_t i = 0; i < this->num_row_; ++i) {
		for (size_t j = 0; j < this->num_column_; ++j)
			str += ms::double_to_string(this->at(i, j)) + "\t";
		str += "\n";
	}
	return str;
}

RowMajorMatrix RowMajorMatrix::transpose(void) const {
	auto result = *this;
	return result.be_transpose();
}

MathVector RowMajorMatrix::multiply_value(const RowMajorMatrix& other) const {
	if (this->num_column_ != other.num_row_)
		throw std::length_error("length is not matched");

	const CBLAS_LAYOUT layout = CBLAS_LAYOUT::CblasRowMajor;
	const CBLAS_TRANSPOSE transA = this->transpose_type_;
	const CBLAS_TRANSPOSE transB = other.transpose_type_;
	const MKL_INT m = static_cast<int>(this->num_row_);
	const MKL_INT n = static_cast<int>(other.num_column_);
	const MKL_INT k = static_cast<int>(this->num_column_);
	const double alpha = 1;
	const MKL_INT lda = static_cast<int>(this->leading_dimension());
	const MKL_INT ldb = static_cast<int>(other.leading_dimension());
	const double beta = 0;
	const MKL_INT ldc = n;

	MathVector new_value(this->num_row_ * other.num_column_);
	cblas_dgemm(layout, transA, transB, m, n, k, alpha, this->value_vector_.data(), lda, other.value_vector_.data(), ldb, beta, new_value.data(), ldc);
	
	return new_value;
}

size_t RowMajorMatrix::leading_dimension(void) const {
	if (this->is_transposed())
		return this->num_row_;
	else
		return this->num_column_;
}

std::vector<int> RowMajorMatrix::PLU_decomposition(void) {
	if (this->is_transposed()) {
		this->transpose_value();
		this->transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;
	}

	const int matrix_layout = LAPACK_ROW_MAJOR;
	const lapack_int m = static_cast<int>(this->num_row_);
	const lapack_int n = static_cast<int>(this->num_column_);
	const lapack_int lda = n;
	std::vector<int> ipiv(std::min(m, n));
	lapack_int info = LAPACKE_dgetrf(matrix_layout, m, n, this->value_vector_.data(), lda, ipiv.data());

	if (info < 0)
		throw std::runtime_error("Fail to PLU decompose");

	return ipiv;
}

RowMajorMatrix& RowMajorMatrix::transpose_value(void) {
	auto [rows, cols] = this->size();

	if (this->is_transposed()) 
		std::swap(rows,cols);
		
	const char odering = 'R';
	const char trans = 'T';	
	const double alpha = 1;
	const size_t lda = cols;
	const size_t ldb = rows;

	mkl_dimatcopy(odering, trans, rows, cols, alpha, this->value_vector_.data(), lda, ldb);
	return *this;
}

bool RowMajorMatrix::is_square_matrix(void) const {
	return this->num_row_ == this->num_column_;
}

bool RowMajorMatrix::is_transposed(void) const {
	return this->transpose_type_ != CBLAS_TRANSPOSE::CblasNoTrans;
}

void RowMajorMatrix::inspect_range(const size_t irow, const size_t jcolumn) const {
	if (this->num_row_ <= irow || this->num_column_ <= jcolumn)
		throw std::out_of_range("matrix indexes are out of range");
}

void RowMajorMatrix::inspect_value_size(void) const {
	if (this->value_vector_.size() != this->num_row_ * this->num_column_)
		throw std::length_error("matrix value size is not matched");
}

std::ostream& operator<<(std::ostream& os, const RowMajorMatrix& x) {
	return os << x.to_string();
}

































//bool RowMajorMatrix::operator==(const RowMajorMatrix& other) const {
//	if (this->size() != other.size())
//		return false;
//
//	if (this->transpose_type_ != other.transpose_type_) {
//		auto tmp = *this;
//		return tmp.transpose_value().value_set_ == other.value_set_;
//	}
//	else
//		return this->value_set_ == other.value_set_;
//}


//const double* RowMajorMatrix::data(void) const {
//	return this->value_set_.data();
//}
//
//double* RowMajorMatrix::data(void) {
//	return this->value_set_.data();
//}

//RowMajorMatrix::RowMajorMatrix(const MatrixType matrix_type, const size_t num_row, const size_t num_column)
//	:matrix_type_(matrix_type), row_(num_row), column_(num_column) {
//	size_t num_value = 0;
//	if (this->is_Full_Storage_Type())
//		this->value_set_.resize(this->row_ * this->column_);
//	else 
//		this->value_set_.resize(static_cast<size_t>((this->row_) * (this->row_ + 1) * 0.5));
//}


//
//RowMajorMatrix::RowMajorMatrix(const MatrixType matrix_type, const size_t matrix_order)
//	: matrix_type_(matrix_type), row_(matrix_order), column_(matrix_order) {
//	size_t num_value = 0;
//	if (this->is_Full_Storage_Type())
//		num_value = this->row_ * this->column_;
//	else 
//		num_value = static_cast<size_t>((this->row_) * (this->row_ + 1) * 0.5);
//
//	this->value_set_.resize(num_value);
//
//	for (size_t i = 0; i < matrix_order; ++i)
//		this->at(i, i) = 1;
//}
//
//RowMajorMatrix::RowMajorMatrix(const MatrixType matrix_type, const size_t num_row, const size_t num_column, const MathVector& value)
	//: matrix_type_(matrix_type), row_(num_row), column_(num_column), value_set_(value) {
	//this->inspect_Size();
	//this->inspect_Value();
//}
//
//RowMajorMatrix::RowMajorMatrix(const MatrixType matrix_type, const size_t num_row, const size_t num_column, MathVector&& value)
//	: matrix_type_(matrix_type), row_(num_row), column_(num_column), value_set_(std::move(value)) {
//	this->inspect_Size();
//	this->inspect_Value();
//}
//
//RowMajorMatrix::RowMajorMatrix(const MatrixType matrix_type, const size_t matrix_order, const MathVector& value)
//	: matrix_type_(matrix_type), row_(matrix_order), column_(matrix_order), value_set_(value) {
//	this->inspect_Size();
//	this->inspect_Value();
//}
//
//RowMajorMatrix::RowMajorMatrix(const MatrixType matrix_type, const size_t matrix_order, MathVector&& value)
//	: matrix_type_(matrix_type), row_(matrix_order), column_(matrix_order), value_set_(std::move(value)) {
//	this->inspect_Size();
//	this->inspect_Value();
//}
//
//RowMajorMatrix& RowMajorMatrix::operator+=(const RowMajorMatrix& B) {
//	RowMajorMatrix& A = *this;
//
//	if (A.size() != B.size())
//		FATAL_SIZE_ERROR;
//
//	if (!A.is_Full_Storage_Type())
//		A.to_Full_Matrix();
//
//	if (A.is_transposed())
//		A.transpose_Value();
//
//	if (!B.is_Full_Storage_Type()) {
//		auto full_B = B.to_Full_Matrix();
//
//		if (full_B.is_transposed())
//			full_B.transpose_Value();
//		
//		A.value_set_ += full_B.value_set_;
//
//		A.update_Type();
//		return A;
//	}
//	else {
//		if (B.is_transposed())
//			A.value_set_ += B.transpose_Value().value_set_;
//		else
//			A.value_set_ += B.value_set_;
//
//		A.update_Type();
//		return A;
//	}
//}
//
//RowMajorMatrix RowMajorMatrix::operator+(const RowMajorMatrix& B) const{
//	RowMajorMatrix A = *this;
//	return A += B;
//}
//
//RowMajorMatrix& RowMajorMatrix::operator-=(const RowMajorMatrix& B){
//	RowMajorMatrix& A = *this;
//
//	if (A.size() != B.size())
//		FATAL_SIZE_ERROR;
//
//	if (!A.is_Full_Storage_Type())
//		A.to_Full_Matrix();
//
//	if (A.is_transposed())
//		A.transpose_Value();
//
//	if (!B.is_Full_Storage_Type()) {
//		auto full_B = B.to_Full_Matrix();
//
//		if (full_B.is_transposed())
//			full_B.transpose_Value();
//
//		A.value_set_ -= full_B.value_set_;
//
//		A.update_Type();
//		return A;
//	}
//	else {
//		if (B.is_transposed())
//			A.value_set_ -= B.transpose_Value().value_set_;
//		else
//			A.value_set_ -= B.value_set_;
//
//		A.update_Type();
//		return A;
//	}
//}
//
//RowMajorMatrix RowMajorMatrix::operator-(const RowMajorMatrix& B) const{
//	RowMajorMatrix A = *this;
//	return A -= B;
//}
//
//RowMajorMatrix& RowMajorMatrix::operator*=(const double scalar){
//	this->value_set_ *= scalar;
//	return *this;
//}
//
//RowMajorMatrix RowMajorMatrix::operator*(const double scalar) const {
//	RowMajorMatrix A = *this;
//	return A *= scalar;
//}
//
//MathVector RowMajorMatrix::operator*(const MathVector& x) const {
//	switch (this->matrix_type_) {
//	case MatrixType::Full:
//		return this->multiply_Full_Matrix_Vector(x);
//	case MatrixType::FullLowerTriangle:
//	case MatrixType::FullUpperTriangle:
//		return this->multiply_Full_Triangle_Matrix_Vector(x);
//	case MatrixType::PackedLowerTriangle:
//	case MatrixType::PackedUpperTriangle:
//		return this->multiply_Packed_Triangle_Matrix_Vector(x);
//	default:
//		FATAL_TYPE_ERROR;
//		return MathVector();
//	}
//}
//
//RowMajorMatrix& RowMajorMatrix::operator*=(const RowMajorMatrix& B) {
//	if (this->is_Full_Storage_Type()) {
//		switch (B.matrix_type_) {
//		case MatrixType::Full:
//			return this->multiply_Full_Matrix_Full_Matrix(B);
//		case MatrixType::FullLowerTriangle:
//		case MatrixType::FullUpperTriangle:
//			return this->multiply_Full_Matrix_Full_Triangle_Matrix(B);
//		case MatrixType::PackedLowerTriangle:
//		case MatrixType::PackedUpperTriangle:
//			return this->multiply_Full_Matrix_Full_Triangle_Matrix(B.to_Full_Matrix());
//		default:
//			FATAL_TYPE_ERROR;
//			return *this;
//		}
//	}
//	else {
//		this->to_Full_Matrix();
//		switch (B.matrix_type_) {
//		case MatrixType::Full:
//			return this->multiply_Full_Matrix_Full_Matrix(B);
//		case MatrixType::FullLowerTriangle:
//		case MatrixType::FullUpperTriangle:
//			return this->multiply_Full_Matrix_Full_Triangle_Matrix(B);
//		case MatrixType::PackedLowerTriangle:
//		case MatrixType::PackedUpperTriangle:
//			return this->multiply_Full_Matrix_Full_Triangle_Matrix(B.to_Full_Matrix());
//		default:
//			FATAL_TYPE_ERROR;
//			return *this;
//		}
//	}
//}
//
//RowMajorMatrix RowMajorMatrix::operator*(const RowMajorMatrix& B) const {
//	RowMajorMatrix A = *this;
//	return A *= B;
//}
//
//bool RowMajorMatrix::operator==(const RowMajorMatrix& B) const {
//	if (this->transpose_type_ != B.transpose_type_ ||
//		this->matrix_type_ != B.matrix_type_ ||
//		this->row_ != B.row_ ||
//		this->column_ != B.column_ ||
//		this->value_set_ != B.value_set_)
//		return false;
//	else
//		return true;
//}
//
//double& RowMajorMatrix::at(const size_t irow, const size_t jcolumn){
//	this->inspect_Range(irow, jcolumn);
//	if (this->is_transposed()) {
//		switch (this->matrix_type_) {
//		case MatrixType::Full:
//		case MatrixType::FullLowerTriangle:
//		case MatrixType::FullUpperTriangle:			
//			return this->value_set_[jcolumn * this->row_ + irow];
//		case MatrixType::PackedLowerTriangle: {
//			if (this->is_Upper_Part(irow, jcolumn))
//				FATAL_ERROR("In packed lower triangle matrix, value reference of upper part doesn't supported");
//
//			size_t position = 0;
//			for (size_t i = 0; i <= jcolumn; ++i)
//				position += this->row_ - i;
//
//			position += irow - jcolumn;
//
//			return this->value_set_[position];
//		}
//		case MatrixType::PackedUpperTriangle: {
//			if (this->is_Lower_Part(irow, jcolumn))
//				FATAL_ERROR("In packed upper triangle matrix, value reference of lower part doesn't supported");
//
//			size_t position = 0;
//			for (size_t i = 0; i <= jcolumn; ++i)
//				position += i;
//
//			position += irow;
//
//			return this->value_set_[position];
//		}
//		default:
//			FATAL_TYPE_ERROR;
//			return *this->value_set_.data();
//		}
//	}
//	else {
//		switch (this->matrix_type_) {
//		case MatrixType::Full:
//		case MatrixType::FullLowerTriangle:
//		case MatrixType::FullUpperTriangle:
//			return this->value_set_[irow * this->column_ + jcolumn];
//		case MatrixType::PackedLowerTriangle: {
//			if (this->is_Upper_Part(irow, jcolumn))
//				FATAL_ERROR("In packed lower triangle matrix, value reference of upper part deosn't supported");
//
//			size_t position = 0;
//			position += static_cast<size_t>(irow * (irow + 1) * 0.5);
//
//			position += jcolumn;
//
//			return this->value_set_[position];
//		}
//		case MatrixType::PackedUpperTriangle: {
//			if (this->is_Lower_Part(irow, jcolumn))
//				FATAL_ERROR("In packed upper triangle matrix, value reference of lower part deosn't supported");
//
//			size_t position = 0;
//			for (size_t i = 0; i < irow; ++i)
//				position += this->row_ - i;
//
//			position += jcolumn - irow;
//
//			return this->value_set_[position];
//		}
//		default:
//			FATAL_TYPE_ERROR;
//			return *this->value_set_.data();
//		}
//	}
//}
//
//double RowMajorMatrix::at(const size_t irow, const size_t jcolumn) const {
//	this->inspect_Range(irow, jcolumn);
//	if (this->is_transposed()) {
//		switch (this->matrix_type_) {
//		case MatrixType::Full:
//		case MatrixType::FullLowerTriangle:
//		case MatrixType::FullUpperTriangle:
//			return this->value_set_[jcolumn * this->row_ + irow];
//		case MatrixType::PackedLowerTriangle: {
//			if (this->is_Upper_Part(irow, jcolumn))
//				return NULL;
//
//			size_t position = 0;
//			for (size_t i = 0; i <= jcolumn; ++i)
//				position += this->row_ - i;
//
//			position += irow - jcolumn;
//
//			return this->value_set_[position];
//		}
//		case MatrixType::PackedUpperTriangle: {
//			if (this->is_Lower_Part(irow, jcolumn))
//				return NULL;
//
//			size_t position = 0;
//			for (size_t i = 0; i <= jcolumn; ++i)
//				position += i;
//
//			position += irow;
//
//			return this->value_set_[position];
//		}
//		default:
//			FATAL_TYPE_ERROR;
//			return *this->value_set_.data();
//		}
//	}
//	else {
//		switch (this->matrix_type_) {
//		case MatrixType::Full:
//		case MatrixType::FullLowerTriangle:
//		case MatrixType::FullUpperTriangle:
//			return this->value_set_[irow * this->column_ + jcolumn];
//		case MatrixType::PackedLowerTriangle: {
//			if (this->is_Upper_Part(irow, jcolumn))
//				return NULL;
//
//			size_t position = 0;
//			position += static_cast<size_t>(irow * (irow + 1) * 0.5);
//
//			position += jcolumn;
//
//			return this->value_set_[position];
//		}
//		case MatrixType::PackedUpperTriangle: {
//			if (this->is_Lower_Part(irow, jcolumn))
//				return NULL;
//
//			size_t position = 0;
//			for (size_t i = 0; i <= irow; ++i)
//				position += this->row_ - i;
//
//			position += jcolumn - irow;
//
//			return this->value_set_[position];
//		}
//		default:
//			FATAL_TYPE_ERROR;
//			return *this->value_set_.data();
//		}
//	}
//}
//
//RowMajorMatrix RowMajorMatrix::cofactor_matrix(void) const{
//	//	C_i,j = (-1)^(i+j) * (M_i,j)
//	const auto [row, column] = this->size();
//	RowMajorMatrix cofactor_matrix(MatrixType::Full, row, column);
//	for (size_t i = 0; i < row; ++i)
//		for (size_t j = 0; j < column; ++j)
//			cofactor_matrix.at(i, j) = pow(-1.0, i + j) * this->minor(i, j);
//
//	return cofactor_matrix;
//}
//
//void RowMajorMatrix::change_Column(const size_t column_index, const MathVector& vector) {
//	if (this->row_ != vector.size())
//		FATAL_SIZE_ERROR;
//
//	for (size_t i = 0; i < this->row_; ++i)
//		this->at(i, column_index) = vector[i];
//}
//
//void RowMajorMatrix::change_Row(const size_t row_index, const MathVector& vector) {
//	if (this->column_ != vector.size())
//		FATAL_SIZE_ERROR;
//
//	for (size_t i = 0; i < this->column_; ++i)
//		this->at(row_index, i) = vector[i];
//}
//
//MathVector RowMajorMatrix::column(const size_t column_index) const {
//	this->inspect_Range(this->row_ - 1, column_index);
//
//	MathVector column_vector(this->row_);
//	for (size_t i = 0; i < this->row_; ++i)
//		column_vector[i] = this->at(i, column_index);
//
//	return column_vector;
//}
//
//RowMajorMatrix RowMajorMatrix::column(const size_t start_column, const size_t end_column) const {
//	if (start_column > end_column)
//		FATAL_SIZE_ERROR;
//
//	const size_t start_row = 0;
//	const size_t end_row = this->row_;
//	return this->part(start_row, end_row, start_column, end_column);
//}
//
//double RowMajorMatrix::determinant(void) {
//	if (!this->is_Square_Matrix())
//		FATAL_TYPE_ERROR;
//
//	this->update_Type();
//	if (this->matrix_type_ == MatrixType::Full)
//		return this->determinant_Full_Matrix();
//	else
//		return this->determinant_Triangle_Matrix();
//}
//
//RowMajorMatrix& RowMajorMatrix::inverse(void) {
//	if (this->matrix_type_ == MatrixType::Full)
//		return this->inverse_Full_Matrix();
//	else if (this->matrix_type_ == MatrixType::FullLowerTriangle || this->matrix_type_ == MatrixType::FullUpperTriangle)
//		return this->inverse_Full_Triangle_Matrix();
//	else
//		return this->inverse_Packed_Triangle_Matrix();
//}
//
//double RowMajorMatrix::minor(const size_t deleting_row_index, const size_t deleting_column_index) const {
//	//	M_i,j := Minor => determinant of eliminate i row j column
//	auto minor_matrix = this->part(deleting_row_index, deleting_column_index);
//	return minor_matrix.determinant();
//}
//
//void RowMajorMatrix::Moore_Penrose_Inverse(void) {
//	//https://software.intel.com/content/www/us/en/develop/articles/implement-pseudoinverse-of-a-matrix-by-intel-mkl.html
//	//https://software.intel.com/sites/products/documentation/doclib/mkl_sa/11/mkl_lapack_examples/lapacke_dgesvd_row.c.htm
//	//https://bskyvision.com/256
//
//	// {U , S , tr(V)}
//	auto decomposed_matrix_set = this->singular_value_decomposition();
//
//	auto& trU = decomposed_matrix_set[0].transpose();
//	auto& S = decomposed_matrix_set[1];
//
//	const auto [S_row, S_column] = S.size();
//	const auto num_diagonal = std::min(S_row, S_column);
//	for (size_t i = 0; i < num_diagonal; ++i) {
//		const auto singular_value = S.at(i, i);
//		if (singular_value > 1.0E-9)
//			S.at(i, i) = 1.0 / singular_value;
//	}
//
//	auto& PinvS = S.transpose();
//	auto& V = decomposed_matrix_set[2].transpose();
//
//	//	Pinv(A) = V * Pinv(S) * tr(U)
//	*this = V * PinvS * trU;
//}
//
//RowMajorMatrix RowMajorMatrix::part(const size_t start_row, const size_t end_row, const size_t start_column, const size_t end_column) const {
//	const auto maximum_row_index = end_row - 1;
//	const auto maximum_column_index = end_column - 1;
//	this->inspect_Range(maximum_row_index, maximum_column_index);
//
//	const auto num_part_row = end_row - start_row;
//	const auto num_part_column = end_column - start_column;
//	RowMajorMatrix part_matrix(MatrixType::Full, num_part_row, num_part_column);
//	for (size_t i = 0; i < num_part_row; ++i)
//		for (size_t j = 0; j < num_part_column; ++j)
//			part_matrix.at(i, j) = this->at(start_row + i, start_column + j);
//
//	return part_matrix;
//}
//
//RowMajorMatrix RowMajorMatrix::part(const size_t part_matrix_order) const {
//	const size_t start_row = 0;
//	const size_t end_row = part_matrix_order;
//	const size_t start_column = 0;
//	const size_t end_column = part_matrix_order;
//
//	return this->part(start_row, end_row, start_column, end_column);
//}
//
//RowMajorMatrix RowMajorMatrix::part(const size_t deleting_row_index, const size_t deleting_column_index) const {
//	this->inspect_Range(deleting_row_index, deleting_column_index);
//
//	const size_t num_part_row = this->row_ - 1;
//	const size_t num_part_column = this->column_ - 1;
//	MathVector value(num_part_row * num_part_column);
//
//	size_t index = 0;
//	for (size_t i = 0; i < this->row_; ++i) {
//		if (i == deleting_row_index)
//			continue;
//
//		for (size_t j = 0; j < this->column_; ++j) {
//			if (j == deleting_column_index)
//				continue;
//
//			value[index++] = this->at(i, j);
//		}
//	}
//
//	RowMajorMatrix part_matrix(MatrixType::Full, num_part_row, num_part_column, std::move(value));
//	return part_matrix;
//}
//
//std::vector<int> RowMajorMatrix::PLU_decomposition(void) { // A=PLU, A is overwriiten by L + U. The unit diagonal element of L are not stored		
	//if (this->matrix_type_ != MatrixType::Full)
	//	FATAL_TYPE_ERROR;

	//if (this->is_transposed())
	//	this->transpose_Value();

	//const int matrix_layout = LAPACK_ROW_MAJOR;
	//const lapack_int m = static_cast<int>(this->row_);
	//const lapack_int n = static_cast<int>(this->column_);
	//const lapack_int lda = n;
	//std::vector<int> ipiv(std::min(m, n));
	//lapack_int info = LAPACKE_dgetrf(matrix_layout, m, n, this->data(), lda, ipiv.data());

	//if (info < 0)
	//	FATAL_ERROR("Fail to decompose");

	//return ipiv;
//}
//
//MathVector RowMajorMatrix::row(const size_t row_index) const {
//	this->inspect_Range(row_index, this->column_ - 1);
//
//	MathVector row_vector(this->column_);
//	for (size_t j = 0; j < this->column_; ++j)
//		row_vector[j] = this->at(row_index, j);
//
//	return row_vector;
//}
//
//RowMajorMatrix RowMajorMatrix::row(const size_t start_row, const size_t end_row) const{
//	if (start_row > end_row)
//		FATAL_SIZE_ERROR;
//	
//	const size_t start_column = 0;
//	const size_t end_column = this->column_;
//	return this->part(start_row, end_row, start_column, end_column);
//}
//
//std::array<RowMajorMatrix, 3> RowMajorMatrix::singular_value_decomposition(void) {
//	//	singular value decomposition
//	//	A = U * S * tr(V)	
//	//	A := m * n matrix
//	//	U := m * m orthogonal matrix
//	//	S := m * n matrix which is zero excep for its min(m,n) diagonal elements
//	//	tr(V) := transpoed n * n orthogonal matrix which is zero except for its min(m,n) diagonal elements
//	//	return {U,S,trV}
//
//	if (!this->is_Full_Storage_Type())
//		this->to_Full_Matrix();
//
//	if (this->is_transposed())
//		this->transpose_Value();
//
//	const auto [row, column] = this->size();
//
//	const int matrix_layout = LAPACK_ROW_MAJOR;
//	const char jobu = 'A';
//	const char jobvt = 'A';
//	const lapack_int m = static_cast<int>(row);
//	const lapack_int n = static_cast<int>(column);
//	const lapack_int lda = static_cast<int>(this->Leading_Dimension());
//	const lapack_int ldu = m;
//	const lapack_int ldvt = n;
//
//	const auto num_singular_value = std::min(m, n);
//	const auto maximum_num_unconverged_diagonal = num_singular_value - 1;
//
//	RowMajorMatrix U(MatrixType::Full, m, m);
//	RowMajorMatrix S(MatrixType::Full, m, n);
//	RowMajorMatrix trV(MatrixType::Full, n, n);
//	std::vector<double> singular_value(num_singular_value);
//	std::vector<double> superb(maximum_num_unconverged_diagonal);
//
//	const auto info = LAPACKE_dgesvd(matrix_layout, jobu, jobvt, m, n, this->data(), lda, singular_value.data(), U.data(), ldu, trV.data(), ldvt, superb.data());
//
//	for (size_t i = 0; i < num_singular_value; ++i)
//		S.at(i, i) = singular_value[i];
//
//	if (info < 0)
//		FATAL_ERROR(Editor::to_String(info) + "th input of LAPACKE_dgesvd had an illegal value");
//
//	return { U,S,trV };
//}
//
//
//
//RowMajorMatrix& RowMajorMatrix::scale_Row(const size_t start_row, const size_t end_row, const double scale_factor){
//	if (start_row > end_row)
//		FATAL_SIZE_ERROR;
//
//	this->inspect_Range(end_row - 1, this->column_ - 1);
//
//	for (size_t i = start_row; i < end_row; ++i)
//		for (size_t j = 0; j < this->column_; ++j)
//			this->at(i, j) *= scale_factor;
//
//	return *this;
//}
//
//RowMajorMatrix& RowMajorMatrix::scale_Column(const size_t start_column, const size_t end_column, const double scale_factor){
//	if (start_column > end_column)
//		FATAL_SIZE_ERROR;
//
//	this->inspect_Range(this->row_ - 1, end_column - 1);
//
//	for (size_t i = 0; i < this->row_; ++i)
//		for (size_t j = start_column; j < end_column; ++j)
//			this->at(i, j) *= scale_factor;
//
//	return *this;
//}
//
//std::string RowMajorMatrix::to_String(void) const {
	//std::string str;
	//switch (this->matrix_type_) {
	//case MatrixType::Full: {
	//	str += "[Full Matrix]\n";
	//	for (size_t i = 0; i < this->row_; ++i) {
	//		for (size_t j = 0; j < this->column_; ++j)
	//			str += Editor::to_String(this->at(i, j)) + "\t";
	//		str += "\n\n";
	//	}
	//	return str;
	//}
	//case MatrixType::FullLowerTriangle:
	//case MatrixType::PackedLowerTriangle: {
	//	str += "[Lower Triangle Matrix]\n";
	//	for (size_t i = 0; i < this->row_; ++i) {
	//		for (size_t j = 0; j <= i; ++j)
	//			str += Editor::to_String(this->at(i, j)) + "\t";
	//		str += "\n\n";
	//	}
	//	return str;
	//}
	//case MatrixType::FullUpperTriangle:
	//case MatrixType::PackedUpperTriangle: {
	//	str += "[Upper Triangle Matrix]\n";
	//	for (size_t i = 0; i < this->row_; ++i) {
	//		for (size_t j = 0; j < i; ++j)
	//			str += "\t";
	//		for (size_t j = i; j < this->column_; ++j)
	//			str += Editor::to_String(this->at(i, j)) + "\t";
	//		str += "\n\n";
	//	}
	//	return str;
	//}
	//default:
	//	FATAL_TYPE_ERROR;
	//	return str;
	//}
//};
//
//RowMajorMatrix& RowMajorMatrix::transpose(void) {
//	std::swap(this->row_, this->column_);
//
//	if (this->is_transposed())
//		this->transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;
//	else
//		this->transpose_type_ = CBLAS_TRANSPOSE::CblasTrans;
//
//	switch (this->matrix_type_) {
//	case MatrixType::FullLowerTriangle:
//		this->matrix_type_ = MatrixType::FullUpperTriangle;
//		return *this;
//	case MatrixType::PackedLowerTriangle:
//		this->matrix_type_ = MatrixType::PackedUpperTriangle;
//		return *this;
//	case MatrixType::FullUpperTriangle:
//		this->matrix_type_ = MatrixType::FullLowerTriangle;
//		return *this;
//	case MatrixType::PackedUpperTriangle:
//		this->matrix_type_ = MatrixType::PackedLowerTriangle;
//		return *this;
//	default:
//		return *this;
//	}
//}
//
//RowMajorMatrix RowMajorMatrix::transpose(void) const {
//	RowMajorMatrix A = *this;
//	return A.transpose();
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
//CBLAS_DIAG RowMajorMatrix::CBALS_Diagonal_Type(void) const {
//	if (this->is_Unit_Diagonal())
//		return CBLAS_DIAG::CblasUnit;
//	else
//		return CBLAS_DIAG::CblasNonUnit;
//}
//
//CBLAS_UPLO RowMajorMatrix::CBLAS_Triangle_Type(void) const {
//	if (this->is_transposed()) {
//		// MKL routine needs original triangle matrix type to calculate tranposed matrix multiplication
//		if(this->is_Upper_Triangle_Type())
//			return CBLAS_UPLO::CblasLower;
//		else
//			return CBLAS_UPLO::CblasUpper;
//	}
//	else {
//		if (this->is_Upper_Triangle_Type())
//			return CBLAS_UPLO::CblasUpper;
//		else
//			return CBLAS_UPLO::CblasLower;
//	}
//}
//
//double RowMajorMatrix::determinant_Full_Matrix(void) {//https://en.wikipedia.org/wiki/LU_decomposition
//	const auto pivot_indices = this->PLU_decomposition();
//
//	double determinantU = 1.0;
//	for (size_t i = 0; i < this->order(); ++i)
//		determinantU *= this->at(i, i);
//
//	double determinantP = 1;
//	for (size_t indx = 0; indx < pivot_indices.size(); ++indx) {
//		if (indx + 1 != pivot_indices[indx])		// it means row exchange is occured !
//			determinantP *= -1;
//	}
//
//	return determinantP * determinantU;		//determinantL = 1
//}
//
//double RowMajorMatrix::determinant_Full_Matrix(void) const {
//	RowMajorMatrix tmp = *this;
//	return tmp.determinant_Full_Matrix();
//}
//
//double RowMajorMatrix::determinant_Triangle_Matrix(void) const {
//	double determinant = 1.0;
//	for (size_t i = 0; i < this->order(); ++i)
//		determinant *= this->at(i, i);
//
//	return determinant;
//}
//
//void RowMajorMatrix::inspect_Range(const size_t irow, const size_t jcolumn) const {
//	if (this->row_ <= irow || this->column_ <= jcolumn)
//		FATAL_ERROR("out of range");
//}
//
//void RowMajorMatrix::inspect_Size(void) const {
//	switch (this->matrix_type_) {
//	case MatrixType::Full: {
//		if (!this->is_Full_Storage_Value())
//			FATAL_SIZE_ERROR;
//		return;
//	}
//	case MatrixType::FullLowerTriangle:
//	case MatrixType::FullUpperTriangle: {
//		if (!this->is_Full_Storage_Value() || !this->is_Square_Matrix())
//			FATAL_SIZE_ERROR;
//		return;
//	}
//	case MatrixType::PackedLowerTriangle:
//	case MatrixType::PackedUpperTriangle: {
//		if (!this->is_Packed_Storage_Value() || !this->is_Square_Matrix())
//			FATAL_SIZE_ERROR;
//		return;
//	}
//	default:
//		FATAL_TYPE_ERROR;
//		return;
//	}
//}
//
//void RowMajorMatrix::inspect_Value(void) const {
//	switch (this->matrix_type_)
//	{
//	case MatrixType::Full:
//	case MatrixType::PackedLowerTriangle:
//	case MatrixType::PackedUpperTriangle:
//		return;
//	case MatrixType::FullLowerTriangle: {
//		if (!this->is_Lower_Triangle_Value())
//			FATAL_ERROR("This matrix have non zero component in upper triangle part");
//		return;
//	}
//	case MatrixType::FullUpperTriangle: {
//		if (!this->is_Upper_Triangle_Value())
//			FATAL_ERROR("This matrix have non zero component in lower triangle part");
//		return;
//	}
//	default:
//		FATAL_TYPE_ERROR;
//		return;
//	}
//}
//
//RowMajorMatrix& RowMajorMatrix::inverse_Full_Matrix(void) {
	//const std::vector<int> ipiv = this->PLU_decomposition();

	//const int matrix_layout = LAPACK_ROW_MAJOR;
	//const lapack_int n = static_cast<int>(this->order());
	//const lapack_int lda = n;
	//const lapack_int info = LAPACKE_dgetri(matrix_layout, n, this->data(), lda, ipiv.data());

	//if (info > 0)
	//	FATAL_ERROR("U is singular matrix in L-U decomposition");
	//else if (info < 0)
	//	FATAL_ERROR("fail to inverse the matrix");

	//this->update_Type();
	//return *this;
//}
//
//RowMajorMatrix& RowMajorMatrix::inverse_Full_Triangle_Matrix(void) {
//	if (this->is_transposed())
//		this->transpose_Value();
//
//	const int matrix_layout = LAPACK_ROW_MAJOR;
//	const char uplo = this->LAPACK_Triangle_Type();
//	const char diag = this->LAPACK_Diagonal_Type();
//	const lapack_int n = static_cast<int>(this->order());
//	const lapack_int lda = n;
//		
//	auto info = LAPACKE_dtrtri(matrix_layout, uplo, diag, n, this->data(), lda);
//
//	if (info > 0)
//		FATAL_ERROR("U is singular matrix in L-U decomposition");
//	else if (info < 0)
//		FATAL_ERROR("fail to inverse the matrix");
//
//	this->update_Type();
//	return *this;
//}
//
//RowMajorMatrix& RowMajorMatrix::inverse_Packed_Triangle_Matrix(void) {
//	if (this->is_transposed())
//		this->transpose_Value();
//
//	const int matrix_layout = LAPACK_ROW_MAJOR;
//	const char uplo = this->LAPACK_Triangle_Type();
//	const char diag = this->LAPACK_Diagonal_Type();
//	const lapack_int n = static_cast<int>(this->order());
//
//	auto info = LAPACKE_dtptri(matrix_layout, uplo, diag, n, this->data());
//
//	if (info > 0)
//		FATAL_ERROR("U is singular matrix in L-U decomposition");
//	else if (info < 0)
//		FATAL_ERROR("fail to inverse the matrix");
//
//	this->update_Type();
//	return *this;
//}
//
//bool RowMajorMatrix::is_Full_Storage_Type(void) const {
//	switch (this->matrix_type_) {
//	case MatrixType::Full:
//	case MatrixType::FullLowerTriangle:
//	case MatrixType::FullUpperTriangle:
//		return true;
//	case MatrixType::PackedLowerTriangle:
//	case MatrixType::PackedUpperTriangle:
//		return false;
//	default:
//		FATAL_TYPE_ERROR;
//		return false;
//	}
//}
//
//bool RowMajorMatrix::is_Full_Storage_Value(void) const {
//	if (this->row_ * this->column_ != this->value_set_.size())
//		return false;
//	else
//		return true;
//}
//
//bool RowMajorMatrix::is_Upper_Triangle_Type(void) const {
//	switch (this->matrix_type_) {
//	case MatrixType::FullUpperTriangle:
//	case MatrixType::PackedUpperTriangle:
//		return true;
//	case MatrixType::Full:
//	case MatrixType::FullLowerTriangle:
//	case MatrixType::PackedLowerTriangle:
//		return false;
//	default:
//		FATAL_TYPE_ERROR;
//		return false;
//	}
//}
//
//bool RowMajorMatrix::is_Square_Matrix(void) const {
//	if (this->row_ == this->column_)
//		return true;
//	else
//		return false;
//}
//
//bool RowMajorMatrix::is_transposed(void) const {
//	if (this->transpose_type_ == CBLAS_TRANSPOSE::CblasNoTrans)
//		return false;
//	else
//		return true;
//}
//
//bool RowMajorMatrix::is_Upper_Triangle_Value(void) const {
//	for (size_t i = 1; i < this->row_; ++i)
//		for (size_t j = 0; j < i; ++j) {
//			if (this->at(i, j) != 0)
//				return false;
//		}
//	return true;
//}
//
//bool RowMajorMatrix::is_Lower_Triangle_Value(void) const {
//	for (size_t i = 0; i < this->row_; ++i)
//		for (size_t j = i + 1; j < this->column_; ++j) {
//			if (this->at(i, j) != 0)
//				return false;
//		}
//	return true;
//}
//
//bool RowMajorMatrix::is_Packed_Storage_Value(void) const {
//	const size_t required_num_value = static_cast<size_t>((this->row_) * (this->row_ + 1) * 0.5);
//	if (this->value_set_.size() == required_num_value)
//		return true;
//	else
//		return false;
//}
//
//bool RowMajorMatrix::is_Unit_Diagonal(void) const {
//	if (!this->is_Square_Matrix())
//		return false;
//
//	for (size_t i = 0; i < this->row_; ++i) {
//		if (this->at(i, i) != 1)
//			return false;
//	}
//
//	return true;
//}
//
//bool RowMajorMatrix::is_Lower_Part(const size_t irow, const size_t jcolumn) const {
//	if (!this->is_Square_Matrix())
//		FATAL_ERROR("Only Square matrix can have lower part of matrix");
//
//	if (irow > jcolumn)
//		return true;
//	else
//		return false;
//}
//
//bool RowMajorMatrix::is_Upper_Part(const size_t irow, const size_t jcolumn) const {
//	if (!this->is_Square_Matrix())
//		FATAL_ERROR("Only Square matrix can have upper part of matrix");
//
//	if (irow < jcolumn)
//		return true;
//	else
//		return false;
//}
//
//char RowMajorMatrix::LAPACK_Diagonal_Type(void) const {
//	if (this->is_Unit_Diagonal())
//		return 'U';
//	else
//		return 'N';
//}
//
//char RowMajorMatrix::LAPACK_Triangle_Type(void) const {
//	if (this->is_Upper_Triangle_Type())
//		return 'U';
//	else
//		return 'L';
//}
//
//size_t RowMajorMatrix::Leading_Dimension(void) const {
	//// use when need original leading dimension
	//if (this->is_transposed())
	//	return this->row_;
	//else
	//	return this->column_;
//}
//
//RowMajorMatrix& RowMajorMatrix::multiply_Full_Matrix_Full_Matrix(const RowMajorMatrix& B) { // A = alpha * A * B + beta * A	=>	A *= B
	//RowMajorMatrix& A = *this;

	//if (A.column_ != B.row_)
	//	FATAL_SIZE_ERROR;

	//const CBLAS_LAYOUT layout = CBLAS_LAYOUT::CblasRowMajor;
	//const CBLAS_TRANSPOSE transA = A.transpose_type_;
	//const CBLAS_TRANSPOSE transB = B.transpose_type_;
	//const MKL_INT m = static_cast<int>(A.row_);
	//const MKL_INT n = static_cast<int>(B.column_);
	//const MKL_INT k = static_cast<int>(A.column_);
	//const double alpha = 1;
	//const MKL_INT lda = static_cast<int>(A.Leading_Dimension());
	//const MKL_INT ldb = static_cast<int>(B.Leading_Dimension());
	//const double beta = 0;
	//const MKL_INT ldc = n;

	//std::vector<double> new_value(A.row_ * B.column_);
	//cblas_dgemm(layout, transA, transB, m, n, k, alpha, A.data(), lda, B.data(), ldb, beta, new_value.data(), ldc);
	//
	//A.column_ = B.column_;
	//A.transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;
	//A.value_set_ = std::move(new_value);
	//A.update_Type();

	//return A;
//}
//
//RowMajorMatrix& RowMajorMatrix::multiply_Full_Matrix_Full_Triangle_Matrix(const RowMajorMatrix& B) { // A = alpha * A * B => A *= B
//	// A : Full matrix
//	// B : Full triangle matrix
//	RowMajorMatrix& A = *this;
//
//	if (A.column_ != B.row_)
//		FATAL_SIZE_ERROR;
//
//	if (A.is_transposed())
//		A.transpose_Value();
//
//	const CBLAS_LAYOUT layout = CBLAS_LAYOUT::CblasRowMajor;
//	const CBLAS_SIDE side = CBLAS_SIDE::CblasRight;
//	const CBLAS_UPLO uplo = B.CBLAS_Triangle_Type();
//	const CBLAS_TRANSPOSE transB = B.transpose_type_;
//	const CBLAS_DIAG diag = B.CBALS_Diagonal_Type();
//	const MKL_INT m = static_cast<int>(A.row_);
//	const MKL_INT n = static_cast<int>(A.column_);
//	const double alpha = 1;
//	const MKL_INT lda = n;
//	const MKL_INT ldb = n;
//
//	//std::vector<double> result_value_set(A.row_ * A.column_);
//	cblas_dtrmm(layout, side, uplo, transB, diag, m, n, alpha, B.data(), lda, A.data(), ldb);
//
//	//A.value_set_ = std::move(result_value_set);
//	A.update_Type();
//	return A;
//}
//
//MathVector RowMajorMatrix::multiply_Full_Matrix_Vector(const MathVector& x) const {// y = alpha Ax + beta y	
	//if (this->matrix_type_ != MatrixType::Full)
	//	FATAL_TYPE_ERROR;
	//if (this->column_ != x.size())
	//	FATAL_SIZE_ERROR;

	//const auto layout = CBLAS_LAYOUT::CblasRowMajor;
	//const auto trans = this->transpose_type_;
	//MKL_INT m;
	//MKL_INT n;
	//if (trans == CBLAS_TRANSPOSE::CblasNoTrans) {
	//	m = static_cast<int>(this->row_);
	//	n = static_cast<int>(this->column_);
	//}
	//else {
	//	m = static_cast<int>(this->column_);
	//	n = static_cast<int>(this->row_);
	//}
	//const double alpha = 1;
	//const MKL_INT lda = n;
	//const MKL_INT incx = 1;
	//const MKL_INT beta = 0;
	//const MKL_INT incy = 1;

	//MathVector y(this->row_);
	//cblas_dgemv(layout, trans, m, n, alpha, this->data(), lda, x.data(), incx, beta, y.data(), incy);

	//return y;
//}
//
//MathVector RowMajorMatrix::multiply_Full_Triangle_Matrix_Vector(const MathVector& vec) const {//x = Ax 
//	if (this->matrix_type_ != MatrixType::FullLowerTriangle && this->matrix_type_ != MatrixType::FullUpperTriangle)
//		FATAL_TYPE_ERROR;
//	if (this->column_ != vec.size())
//		FATAL_SIZE_ERROR;
//
//	const CBLAS_LAYOUT layout = CBLAS_LAYOUT::CblasRowMajor;
//	const CBLAS_UPLO uplo = this->CBLAS_Triangle_Type();
//	const CBLAS_TRANSPOSE trans = this->transpose_type_;
//	const CBLAS_DIAG diag = this->CBALS_Diagonal_Type();
//	const MKL_INT n = static_cast<int>(this->row_);
//	const MKL_INT lda = n;
//	const MKL_INT incx = 1;
//
//	MathVector x(vec.size());
//	cblas_dtrmv(layout, uplo, trans, diag, n, this->data(), lda, x.data(), incx);
//
//	return x;
//}
//
//MathVector RowMajorMatrix::multiply_Packed_Triangle_Matrix_Vector(const MathVector& vec) const {//x = Ax 
//	if (this->is_Full_Storage_Type())
//		FATAL_TYPE_ERROR;
//	if (this->column_ != vec.size())
//		FATAL_SIZE_ERROR;
//
//	const CBLAS_LAYOUT layout = CBLAS_LAYOUT::CblasRowMajor;
//	const CBLAS_UPLO uplo = this->CBLAS_Triangle_Type();
//	const CBLAS_TRANSPOSE trans = this->transpose_type_;
//	const CBLAS_DIAG diag = this->CBALS_Diagonal_Type();
//	const MKL_INT n = static_cast<int>(this->row_);
//	const MKL_INT incx = 1;
//
//	MathVector x(vec.size());
//	cblas_dtpmv(layout, uplo, trans, diag, n, this->data(), x.data(), incx);
//
//	return x;
//}
//
//size_t RowMajorMatrix::order(void) const {
//	if (this->is_Square_Matrix())
//		return this->row_;
//	else {
//		FATAL_ERROR("It is not Sqaure Matrix");
//		return NULL;
//	}
//}
//
//RowMajorMatrix& RowMajorMatrix::transpose_Value(void) {
//	if (!this->is_Full_Storage_Type())
//		this->to_Full_Matrix();
//
//	const char odering = 'R';
//	const char trans = 'T';
//	const size_t rows = this->column_;
//	const size_t cols = this->row_;
//	const double alpha = 1;
//	const size_t lda = cols;
//	const size_t ldb = rows;
//
//	mkl_dimatcopy(odering, trans, rows, cols, alpha, this->data(), lda, ldb);
//	this->transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;
//	return *this;
//}
//
//RowMajorMatrix RowMajorMatrix::transpose_Value(void) const {
//	RowMajorMatrix result(*this);
//	return result.transpose_Value();
//}
//
//RowMajorMatrix& RowMajorMatrix::to_Full_Matrix(void) {
//	switch (this->matrix_type_) {
//	case MatrixType::PackedLowerTriangle: {
//		RowMajorMatrix full_matrix(MatrixType::FullLowerTriangle, this->order());
//
//		for (size_t i = 0; i < this->row_; ++i)
//			for (size_t j = 0; j <= i; ++j)
//				full_matrix.at(i, j) = this->at(i, j);
//
//		return *this = std::move(full_matrix);
//	}
//	case MatrixType::PackedUpperTriangle: {
//		RowMajorMatrix full_matrix(MatrixType::FullUpperTriangle, this->order());
//
//		for (size_t i = 0; i < this->row_; ++i)
//			for (size_t j = i; j < this->column_; ++j)
//				full_matrix.at(i, j) = this->at(i, j);
//
//		return *this = std::move(full_matrix);
//	}
//	default:
//		FATAL_TYPE_ERROR;
//		break;
//	}
//
//	return *this;
//}
//
//RowMajorMatrix RowMajorMatrix::to_Full_Matrix(void) const {
//	RowMajorMatrix result = *this;
//	return result.to_Full_Matrix();
//}
//
//const RowMajorMatrix& RowMajorMatrix::update_Type(void) const {
//	if (!this->is_Full_Storage_Type())
//		return *this;
//
//	if (this->is_Square_Matrix()) {
//		if (this->is_Lower_Triangle_Value())
//			this->matrix_type_ = MatrixType::FullLowerTriangle;
//		else if (this->is_Upper_Triangle_Value())
//			this->matrix_type_ = MatrixType::FullUpperTriangle;
//		else
//			this->matrix_type_ = MatrixType::Full;
//		return *this;
//	}
//
//	this->matrix_type_ = MatrixType::Full;
//	return *this;
//}