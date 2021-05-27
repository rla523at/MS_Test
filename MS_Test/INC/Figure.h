#pragma once

#include "Matrix.h"
#include "Polynomial.h"
#include "Text.h"


enum class FigureType{
	Point,
	Line,
	Triangle, Quadrilateral,
};


struct QuadratureRule{
	std::vector<MathVector> node_set;
	std::vector<double> weight_set;	
};


class ReferenceFigure
{
private:
	static std::map<std::pair<FigureType, size_t>, std::vector<MathVector>> key_to_transformation_node_set_;
	static std::map<std::pair<FigureType, size_t>, VectorFunction<Monomial>> key_to_transformation_monomial_vector_;
	static std::map<std::pair<FigureType, size_t>, RowMajorMatrix> key_to_inverse_transformation_monomial_matrix_;
	static std::map<std::pair<FigureType, size_t>, QuadratureRule> key_to_quadrature_rule_;
	static std::map<std::pair<FigureType, size_t>, std::vector<MathVector>> key_to_post_node_set_;
	static std::map<std::pair<FigureType, size_t>, std::vector<std::vector<size_t>>> key_to_connectivity_;

private:
	FigureType figure_type_;
	size_t figure_order_;

public:
	ReferenceFigure(const FigureType figure_type, const size_t figure_order);

	VectorFunction<CompactPolynomial> calculate_transformation_function(const std::vector<const MathVector*>& transformed_node_set) const;
	QuadratureRule calculate_quadrature_rule(const VectorFunction<CompactPolynomial>& trasnformation_function, const size_t integrand_order) const;

	MathVector center_node(void) const;
	std::vector<FigureType> face_figure_type_set(void) const;
	size_t figure_dimension(void) const;
	MathVector normal_vector(void) const;
	FigureType simplex_figure_type(void) const;

	//getter style 필요 없음!
	const std::vector<MathVector>& reference_post_node_set(const size_t post_order);
	const std::vector<std::vector<size_t>>& reference_connectivity(const size_t post_order);

	// node index set을 받아서 알아서 처리하게 그러면 Reference Figure말고 Indexed Figure에 넣어놓는게 맞지 않나 ?
	std::map<size_t, std::vector<size_t>> face_index_to_node_index_order_set(void) const;	
	std::vector<size_t> vertex_node_index_order_set(void) const;
	std::vector<std::vector<size_t>> vertex_simplex_node_index_order_family(void) const;

//private:
public: //for test
	size_t calculate_Required_Order(const size_t integrand_order) const;
	size_t calculate_Num_Required_Point(const size_t required_order) const;
	size_t support_element_order(void) const;

	CompactPolynomial calculate_trasnformation_scale_function(const VectorFunction<CompactPolynomial>& transformation_function) const;

	std::vector<MathVector> transformation_node_set(void) const;
	VectorFunction<Monomial> transformation_monomial_vector(void) const;
	RowMajorMatrix inverse_transformation_monomial_matrix(void) const;
	QuadratureRule reference_quadrature_rule(const size_t integrand_order) const;
};


namespace ms {
	std::string to_string(const FigureType figure_type);
}


class Figure
{
private:
	ReferenceFigure reference_figure_;
	std::vector<const MathVector*> node_set_;
	VectorFunction<CompactPolynomial> transformation_function_;
	//JacobianFunction<Polynomial> transformation_Jacobian_function_;

public:
	explicit Figure(const FigureType figure_type, const size_t figure_order, std::vector<const MathVector*>&& node_set);

	MathVector calculate_center_node(void) const;
	VectorFunction<CompactPolynomial> calculate_orthonormal_basis_vector(const size_t polynomial_order) const;
	QuadratureRule calculate_quadrature_rule(const size_t integrand_roder) const;
//private:
public: //for test
	VectorFunction<CompactPolynomial> calculate_initial_basis_vector(const size_t polynomial_order) const;
	
};


VectorFunction<CompactPolynomial> operator*(const RowMajorMatrix& m, const VectorFunction<Monomial> vector_function);


namespace ms {
	double integrate(const CompactPolynomial& integrand, const QuadratureRule& quadrature_rule);
	double integrate(const CompactPolynomial& integrand, const Figure& figure);
	double inner_product(const CompactPolynomial& f1, const CompactPolynomial& f2, const QuadratureRule& quadrature_rule);
	double inner_product(const CompactPolynomial& f1, const CompactPolynomial& f2, const Figure& figure);
	double L2_Norm(const CompactPolynomial& polynomial, const QuadratureRule& quadrature_rule);
	double L2_Norm(const CompactPolynomial& polynomial, const Figure& figure);
	std::vector<CompactPolynomial> Gram_Schmidt_Process(const std::vector<CompactPolynomial>& initial_polynomial_set, const QuadratureRule& quadrature_rule);
	std::vector<CompactPolynomial> Gram_Schmidt_Process(const VectorFunction<CompactPolynomial>& initial_polynomial_set, const Figure& figure);
}

//
//class Figure : public ReferenceFigure
//{
//protected:
//	FigureType figure_type_;
//	
//	size_t figure_order_;
//		
	//std::vector<const MathVector*> node_set_;

	//VectorFunction<Polynomial> transformation_function_;

	//JacobianMatrix<Polynomial> transformation_Jacobian_matrix_;
//
//	//debug
//	VectorFunction<Polynomial> linear_transformation_function_;
//
//public:
//	explicit Figure(const FigureType figure_type, const size_t figure_order, std::vector<const MathVector*>&& node_set);
//
//	explicit Figure(const std::string& figure_type_name, const size_t figure_order, std::vector<const MathVector*>&& node_set) {
//		*this = Figure(this->name_to_Figure_Type(figure_type_name), figure_order, std::move(node_set));
//	};
//
//	//debug
//	const VectorFunction<Polynomial>& linear_transformation_function(void) const {
//		return this->linear_transformation_function_;
//	}
//	//debug
//
//	std::vector<std::vector<double>> calculate_Connecitivity_Set(const size_t post_order, const size_t start_index) const;
//
//	std::vector<MathVector> calculate_Post_Point_Set(const size_t post_order) const;
//
//	std::vector<MathVector> calculate_Transformed_Point_Set(const std::vector<MathVector>& point_set) const {
//		return this->transformation_function_(point_set);
//	}
//
//	QuadratureRule calculate_Quadrature_Rule(const size_t integrand_order) const;
//
//	Figure build_Face_Figure(const size_t face_index) const;
//
//	MathVector calculate_Center_Node(void) const {
//		return this->transformation_function_(ReferenceFigure::calculate_Center_Node(this->figure_type_));
//	};
//
//	double calculate_Volume(void) const;
//
//	std::vector<double> calculate_Projection_Volume_Set(void) const;
//
//	size_t dimension(void) const;
//
//	size_t figure_order(void) const{
//		return this->figure_order_; 
//	};
//
//	FigureType figure_type(void) const {
//		return this->figure_type_;
//	};
//
//
//protected:
//	RowMajorMatrix calculate_Transformation_Normal_Matrix(const MathVector& point) const;
//
//private:
//	std::vector<const MathVector*> calculate_Face_Node_Set(const size_t face_index) const;
//
//	std::map<size_t, std::vector<const MathVector*>> calculate_Face_Index_to_Node_Set(void) const;
//
//	size_t calculate_Transformation_Scale_Function_Order(void) const;
//
//	//vector function return으로 바꾸기
//	std::vector<Monomial> calculate_Linear_Transformation_Monomial_Set(void) const;
//
//	std::vector<Monomial> calculate_Transformation_Monomial_Set(void) const;
//	//vector function return으로 바꾸기
//
//	double calculate_Transformation_Scale(const MathVector& point) const;
//	
//	bool is_Simplex(void) const;
//
//	FigureType name_to_Figure_Type(const std::string& figure_type_name) const;
//};
//
//
//enum class FigureFaceType
//{
//	InwardFace,
//	OutwardFace,
//	NotMyFace
//};
//
//
//struct IndexedFigureData
//{
//	std::string figure_type_name;
//	size_t figure_order;
//	std::vector<const MathVector*> node_set;
//	std::vector<size_t> node_index_set;
//};
//
//
//class IndexedFigure : public Figure
//{
//protected:
//	std::vector<size_t> node_index_set_;
//	
//public:
//	explicit IndexedFigure(const FigureType figure_type, const size_t figure_order, std::vector<const MathVector*>&& node_set, std::vector<size_t>&& node_index_set)
//		:Figure(figure_type, figure_order, std::move(node_set)), node_index_set_(std::move(node_index_set)) {};
//
//	explicit IndexedFigure(const std::string& figure_type_name, const size_t figure_order, std::vector<const MathVector*>&& node_set, std::vector<size_t>&& node_index_set)
//		:Figure(figure_type_name, figure_order, std::move(node_set)), node_index_set_(std::move(node_index_set)) {};
//
//	IndexedFigure(IndexedFigureData&& data)
//		:Figure(data.figure_type_name, data.figure_order, std::move(data.node_set)), node_index_set_(std::move(data.node_index_set)) {};
//
//
//	IndexedFigure build_Face_Figure(const size_t face_index) const;
//
//	std::map<size_t, std::vector<size_t>> calculate_Face_Index_to_Node_Index_Set(void) const;
//	
//	std::vector<size_t> calculate_Vertex_Node_Index_Set(void) const;
//
//	std::unordered_map<size_t, std::vector<size_t>> calculate_Vertex_Node_Index_To_Vertex_Simplex_Element_Consisting_Node_Index(void) const;
//
//	FigureFaceType examine_Figure_Face_Type(const IndexedFigure& face_figure) const;
//};
//
//
//namespace Math {
//	std::vector<Polynomial> calculate_Initial_Basis_Function_Set(const Figure& figure, const size_t polynomial_order);
//
//	std::vector<Polynomial> calculate_Orthonormal_Basis_Function_Set(const Figure& figure, const size_t polynomial_order);
//
//	double integrate(const Polynomial& integrand, const QuadratureRule& quadrature_rule);
//
//	double integrate(const Polynomial& integrand, const Figure& figure);
//
//	double inner_product(const Polynomial& f1, const Polynomial& f2, const QuadratureRule& quadrature_rule);
//
//	double inner_product(const Polynomial& f1, const Polynomial& f2, const Figure& figure);
//
//	double L2_Norm(const Polynomial& polynomial, const QuadratureRule& quadrature_rule);
//
//	double L2_Norm(const Polynomial& polynomial, const Figure& figure);
//
//	std::vector<Polynomial> Gram_Schmidt_Process(const std::vector<Polynomial>& initial_polynomial_set, const QuadratureRule& quadrature_rule);
//
//	std::vector<Polynomial> Gram_Schmidt_Process(const std::vector<Polynomial>& initial_polynomial_set, const Figure& figure);
//}
