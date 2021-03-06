#pragma once
#include "MathVector.h"


class RowMajorMatrix
{
private:	
	CBLAS_TRANSPOSE transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;
	size_t row_ = 0;
	size_t column_ = 0;
	MathVector value_set_;

public:
	explicit RowMajorMatrix(void) = default;
	explicit RowMajorMatrix(const size_t num_row, const size_t num_column);
	explicit RowMajorMatrix(const size_t matrix_order);
	RowMajorMatrix(const size_t num_row, const size_t num_column, const MathVector& value);
	RowMajorMatrix(const size_t num_row, const size_t num_column, MathVector&& value);

	RowMajorMatrix& operator+=(const RowMajorMatrix& other);
	RowMajorMatrix& operator*=(const RowMajorMatrix& other);

	double& at(const size_t row, const size_t column);
	double at(const size_t row, const size_t column) const;
	RowMajorMatrix& inverse(void);
	std::pair<size_t, size_t> size(void) const;
	RowMajorMatrix& transpose(void);
	std::string to_string(void) const;

private:
	MathVector multiply_value(const RowMajorMatrix& other) const;

	size_t leading_dimension(void) const;
	std::vector<int> PLU_decomposition(void);
	RowMajorMatrix& transpose_value(void);
	bool is_square_matrix(void) const;
	bool is_transposed(void) const;
	void inspect_range(const size_t irow, const size_t jcolumn) const;
	void inspect_value_size(void) const;	
};


std::ostream& operator<<(std::ostream& os, const RowMajorMatrix& x);
template <typename T>
VectorFunction<T> operator*(const RowMajorMatrix& m, const VectorFunction<T> vector_function);


namespace ms {
	RowMajorMatrix transpose(const RowMajorMatrix& A);
}


template <typename T>
class JacobianMatrix
{
private:
	std::vector<std::vector<T>> function_set_;

public:
	JacobianMatrix(void) = default;
	JacobianMatrix(const VectorFunction<T>& vector_function, const size_t num_variable);

	RowMajorMatrix operator()(const MathVector& variable_vector) const;

	T& at(const size_t i_index, const size_t j_index);
	const T& at(const size_t i_index, const size_t j_index) const;
	std::pair<size_t, size_t> size(void) const;
	std::string to_string(void) const;
};


template <typename T>
std::ostream& operator<<(std::ostream& os, const JacobianMatrix<T>& Jacobian_matrix);




//template definition part
template <typename T>
VectorFunction<T> operator*(const RowMajorMatrix& m, const VectorFunction<T> vector_function) {
	const auto [num_row, num_colum] = m.size();

	if (num_row != vector_function.size())
		throw std::length_error("length does not match");

	VectorFunction<T> result(num_row);
	for (size_t i = 0; i < num_row; ++i)
		for (size_t j = 0; j < num_column; ++j)
			result[i] += m.at(i, j) * vector_function[j];

	return result;
}


template <typename T>
JacobianMatrix<T>::JacobianMatrix(const VectorFunction<T>& vector_function, const size_t num_variable) {
	const auto num_function = vector_function.size();

	this->function_set_.resize(num_function);
	for (auto& set : this->function_set_)
		set.resize(num_variable);

	for (size_t i = 0; i < num_function; ++i)
		for (size_t j = 0; j < num_variable; ++j)
			this->function_set_[i][j] = ms::differentiate(vector_function.at(i), j);
}

template <typename T>
RowMajorMatrix JacobianMatrix<T>::operator()(const MathVector& variable_vector) const {
	const auto [num_row, num_column] = this->size();

	RowMajorMatrix value(num_row, num_column);
	for (size_t i = 0; i < num_row; ++i)
		for (size_t j = 0; j < num_column; ++j)
			value.at(i, j) = this->function_set_[i][j](variable_vector);

	return value;
}

template <typename T>
T& JacobianMatrix<T>::at(const size_t i_index, const size_t j_index) {
	return this->function_set_[i_index][j_index];
}

template <typename T>
const T& JacobianMatrix<T>::at(const size_t i_index, const size_t j_index) const {
	return this->function_set_[i_index][j_index];
}

template <typename T>
std::pair<size_t, size_t> JacobianMatrix<T>::size(void) const {
	return { this->function_set_.size(), this->function_set_.front().size() };
}

template <typename T>
std::string JacobianMatrix<T>::to_string(void) const {
	std::string str;

	const auto [num_row, num_column] = this->size();
	for (size_t i = 0; i < num_row; ++i)
		for (size_t j = 0; j < num_column; ++j)
			str += "[" + std::to_string(i) + "," + std::to_string(j) + "]  :  " + this->at(i, j).to_string() + "\n";

	return str;
}


template <typename T>
std::ostream& operator<<(std::ostream& os, const JacobianMatrix<T>& Jacobian_matrix) {
	return os << Jacobian_matrix.to_string();
}









//
//#include "MathVector.h"
//
//#include <array>
//#include <algorithm> //min max
//#include <stdexcept>
//
//
//enum class MatrixType
//{
//	Full,
//	FullUpperTriangle,
//	FullLowerTriangle,
//	PackedUpperTriangle,
//	PackedLowerTriangle
//};
//
//
//class RowMajorMatrix
//{
//private:
//	CBLAS_TRANSPOSE transpose_type_ = CBLAS_TRANSPOSE::CblasNoTrans;
//	mutable MatrixType matrix_type_ = MatrixType::Full;
	//size_t row_ = 0;
	//size_t column_ = 0;
	//MathVector value_set_;
//
//public:
	//explicit RowMajorMatrix(void) = default;
	//explicit RowMajorMatrix(const MatrixType matrix_type, const size_t num_row, const size_t num_column);
	//explicit RowMajorMatrix(const MatrixType matrix_type, const size_t matrix_order);
	//RowMajorMatrix(const MatrixType matrix_type, const size_t matrix_order, const MathVector& value);
	//RowMajorMatrix(const MatrixType matrix_type, const size_t matrix_order, MathVector&& value);
	//RowMajorMatrix(const MatrixType matrix_type, const size_t num_row, const size_t num_column, const MathVector& value);
	//RowMajorMatrix(const MatrixType matrix_type, const size_t num_row, const size_t num_column, MathVector&& value);
//
//
//	RowMajorMatrix& operator+=(const RowMajorMatrix& other);
//	RowMajorMatrix& operator-=(const RowMajorMatrix& B);
//	RowMajorMatrix& operator*=(const double scalar);
//	RowMajorMatrix& operator*=(const RowMajorMatrix& other);
//	RowMajorMatrix operator+(const RowMajorMatrix& other) const;
//	RowMajorMatrix operator-(const RowMajorMatrix& other) const;
//	RowMajorMatrix operator*(const double scalar) const;	
//	MathVector operator*(const MathVector& vec) const;
//	RowMajorMatrix operator*(const RowMajorMatrix& other) const;
//	bool operator==(const RowMajorMatrix& B) const;
//	bool operator!=(const RowMajorMatrix& B) const {
//		return !(*this == B);
//	};
//
//	
//	RowMajorMatrix& abs(void) {
//		Math::abs(this->value_set_);
//		return *this;
//	}
//
	//double& at(const size_t row, const size_t column);

	//double at(const size_t row, const size_t column) const;
//
//	RowMajorMatrix cofactor_matrix(void) const;
//
//	void change_Column(const size_t column_index, const MathVector& vector);
//
//	void change_Row(const size_t row_index, const MathVector& vector);
//
//	MathVector column(const size_t column_index) const;
//
//	RowMajorMatrix column(const size_t start_column, const size_t end_column) const;
//
//	double determinant(void);
//
//	double determinant(void) const {
//		RowMajorMatrix A = *this;
//		return A.determinant();
//	}
//
//	RowMajorMatrix& inverse(void);
//
//	RowMajorMatrix inverse(void) const {
//		RowMajorMatrix A = *this;
//		return A.inverse();
//	}
//
//	double L2_Norm(void) const {
//		return this->value_set_.L2_Norm();
//	}
//
//	double minor(const size_t deleting_row_index, const size_t deleting_column_index) const;
//
//	void Moore_Penrose_Inverse(void);
//
//	RowMajorMatrix part(const size_t start_row, const size_t end_row, const size_t start_column, const size_t end_column) const;
//
//	RowMajorMatrix part(const size_t part_matrix_order) const;
//
//	RowMajorMatrix part(const size_t deleting_row_index, const size_t deleting_column_index) const;
//
//	std::vector<int> PLU_decomposition(void);
//
//	MathVector row(const size_t row_index) const;
//
//	RowMajorMatrix row(const size_t start_row, const size_t end_row) const;
//
//	std::array<RowMajorMatrix, 3> singular_value_decomposition(void);
//
//	std::pair<size_t, size_t> size(void) const {
//		return { this->row_, this->column_ };
//	};
//
//	RowMajorMatrix& scale_Row(const size_t start_row, const size_t end_row, const double scale_factor);
//
//	RowMajorMatrix& scale_Column(const size_t start_column, const size_t end_column, const double scale_factor);
//
//	std::string to_String(void) const;
//
//	RowMajorMatrix& transpose(void);
//
//	RowMajorMatrix transpose(void) const;
//
//private:
//	CBLAS_DIAG CBALS_Diagonal_Type(void) const;
//
//	CBLAS_UPLO CBLAS_Triangle_Type(void) const;
//
//	double* data(void) {
//		return this->value_set_.data();
//	};
//
//	const double* data(void) const {
//		return this->value_set_.data();
//	};
//
//	double determinant_Full_Matrix(void);
//
//	double determinant_Full_Matrix(void) const;
//
//	double determinant_Triangle_Matrix(void) const;
//
//	void inspect_Range(const size_t irow, const size_t jcolumn) const;
//
//	void inspect_Size(void) const;
//
//	void inspect_Value(void) const;
//
//	RowMajorMatrix& inverse_Full_Matrix(void);
//
//	RowMajorMatrix& inverse_Full_Triangle_Matrix(void);
//
//	RowMajorMatrix& inverse_Packed_Triangle_Matrix(void);
//
//	bool is_Full_Storage_Type(void) const;
//
//	bool is_Full_Storage_Value(void) const;
//
//	bool is_Lower_Triangle_Value(void) const;
//
//	bool is_Lower_Part(const size_t irow, const size_t jcolumn) const;
//		
//	bool is_Packed_Storage_Value(void) const;
//
//	bool is_Square_Matrix(void) const;
//
//	bool is_transposed(void) const;
//
//	bool is_Upper_Triangle_Value(void) const;
//
//	bool is_Upper_Triangle_Type(void) const;
//
//	bool is_Unit_Diagonal(void) const;
//	
//	bool is_Upper_Part(const size_t irow, const size_t jcolumn) const;
//
//	char LAPACK_Diagonal_Type(void) const;
//
//	char LAPACK_Triangle_Type(void) const;
//
//	size_t Leading_Dimension(void) const;
//
//	RowMajorMatrix& multiply_Full_Matrix_Full_Matrix(const RowMajorMatrix& B);
//
//	RowMajorMatrix& multiply_Full_Matrix_Full_Triangle_Matrix(const RowMajorMatrix& B);
//
//	MathVector multiply_Full_Matrix_Vector(const MathVector& vec) const;
//	
//	MathVector multiply_Full_Triangle_Matrix_Vector(const MathVector& vec) const;
//
//	MathVector multiply_Packed_Triangle_Matrix_Vector(const MathVector& vec) const;
//
//	size_t order(void) const;
//
//	RowMajorMatrix& transpose_Value(void);
//
//	RowMajorMatrix transpose_Value(void) const;
//
//	RowMajorMatrix& to_Full_Matrix(void);
//
//	RowMajorMatrix to_Full_Matrix(void) const;
//
//	const RowMajorMatrix& update_Type(void) const;
//};
//
//
//inline std::ostream& operator<<(std::ostream& os, const RowMajorMatrix& A) {
//	return os << A.to_String();
//}
//
//
//inline RowMajorMatrix operator*(const double scalar, const RowMajorMatrix& A) {
//	return A * scalar;
//}
//
//
//template <typename T>
//class JacobianMatrix
//{
//private:
//	std::vector<std::vector<T>> function_set_;
//
//public:
//	JacobianMatrix(void) = default;
//
//	JacobianMatrix(const size_t num_function, const size_t num_variable) {
//		function_set_.resize(num_function);
//		for (auto& set : function_set_)
//			set.resize(num_variable);
//	}
//
//
//	RowMajorMatrix operator()(const MathVector& variable_vector) const {
//		const auto [num_row, num_column] = this->size();
//
//		RowMajorMatrix value(MatrixType::Full, num_row, num_column);
//		for (size_t i = 0; i < num_row; ++i)
//			for (size_t j = 0; j < num_column; ++j)
//				value.at(i, j) = function_set_[i][j](variable_vector);
//
//		return value;
//	}
//
//	T& at(const size_t i_index, const size_t j_index) {
//		return function_set_[i_index][j_index];
//	}
//
//	const T& at(const size_t i_index, const size_t j_index) const {
//		return function_set_[i_index][j_index];
//	}
//
//	std::pair<size_t, size_t> size(void) const {
//		return { this->function_set_.size(), this->function_set_.front().size() };
//	}
//
//	std::string to_String(void) const {
//		std::string str;
//
//		const auto [num_row, num_column] = this->size();
//		for (size_t i = 0; i < num_row; ++i)
//			for (size_t j = 0; j < num_column; ++j)
//				str += "[" + Editor::to_String(i) + "," + Editor::to_String(j) + "]  :  " + Editor::to_String(this->at(i, j)) + "\n";
//
//		return str;
//	}
//};
//
//
//template <typename T>
//inline std::ostream& operator<<(std::ostream& os, const JacobianMatrix<T>& Jacobian_matrix) {
//	return os << Jacobian_matrix.to_String();
//}
//
//namespace Math {
//	template <typename T>
	//JacobianMatrix<T> Jacobian(const VectorFunction<T>& vector_function, const size_t num_variable) {
	//	const auto num_function = vector_function.size();

	//	JacobianMatrix<T> jacobian_matrix(num_function, num_variable);
	//	for (size_t i = 0; i < num_function; ++i)
	//		for (size_t j = 0; j < num_variable; ++j)
	//			jacobian_matrix.at(i, j) = Math::differentiate(vector_function.at(i), j);

	//	return jacobian_matrix;
//	}
//}