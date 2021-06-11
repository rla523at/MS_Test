#pragma once

#include "Matrix.h"
#include "Polynomial.h"
#include "Text.h"

#include <map>
#include <optional>

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
public:
	ReferenceFigure(const FigureType figure_type, const size_t figure_order);
	
	MathVector center_node(void) const;
	std::vector<FigureType> face_figure_type_set(void) const;
	size_t figure_dimension(void) const;
	VectorFunction<Polynomial> mapping_function(const std::vector<const MathVector*>& physical_node_set) const;
	QuadratureRule Cartesian_quadrature_rule(const VectorFunction<Polynomial>& trasnformation_function, const size_t physical_domain_integrand_order) const;
	
	//MathVector normal_vector(void) const;
	//FigureType simplex_figure_type(void) const;
	VectorFunction<Polynomial> affine_trasnformation_function(const std::vector<const MathVector*>& transformed_node_set) const;


//private: //for test
	RowMajorMatrix inverse_mapping_monomial_matrix(void) const;
	VectorFunction<Polynomial> mapping_monomial_vector(void) const;
	std::vector<MathVector> node_set(void) const;
	QuadratureRule reference_quadrature_rule(const size_t integrand_order) const;
	size_t required_quadrature_rule_order(const size_t integrand_order) const;
	size_t required_quadarature_rule_num_point(const size_t required_order) const;
	std::optional<IrrationalFunction> scale_function(const VectorFunction<Polynomial>& transformation_function) const;	
	size_t support_element_order(void) const;

private:
	static std::map<std::pair<FigureType, size_t>, std::vector<MathVector>> key_to_node_set_;
	static std::map<std::pair<FigureType, size_t>, VectorFunction<Polynomial>> key_to_mapping_monomial_vector_;
	static std::map<std::pair<FigureType, size_t>, RowMajorMatrix> key_to_inverse_mapping_monomial_matrix_;
	static std::map<std::pair<FigureType, size_t>, QuadratureRule> key_to_quadrature_rule_;
	static std::map<std::pair<FigureType, size_t>, std::vector<MathVector>> key_to_post_node_set_;
	static std::map<std::pair<FigureType, size_t>, std::vector<std::vector<size_t>>> key_to_connectivity_;

	FigureType figure_type_;
	size_t figure_order_;
};

////getter style 필요 없음!
//const std::vector<MathVector>& reference_post_node_set(const size_t post_order);
//const std::vector<std::vector<size_t>>& reference_connectivity(const size_t post_order);

//// node index set을 받아서 알아서 처리하게 그러면 Reference Figure말고 Indexed Figure에 넣어놓는게 맞지 않나 ?
//std::map<size_t, std::vector<size_t>> face_index_to_node_index_order_set(void) const;	
//std::vector<size_t> vertex_node_index_order_set(void) const;
//std::vector<std::vector<size_t>> vertex_simplex_node_index_order_family(void) const;


class Figure
{
public:
	explicit Figure(const FigureType figure_type, const size_t figure_order, std::vector<const MathVector*>&& node_set);

	MathVector center_node(void) const;
	VectorFunction<Polynomial> orthonormal_basis_vector(const size_t polynomial_order) const;
	QuadratureRule quadrature_rule(const size_t integrand_roder) const;

//private: //for test
	VectorFunction<Polynomial> initial_basis_vector(const size_t polynomial_order) const;
	VectorFunction<Polynomial> initial_basis_vector1(const size_t polynomial_order) const;
	VectorFunction<Polynomial> initial_basis_vector2(const size_t polynomial_order) const;
	VectorFunction<Polynomial> initial_basis_vector3(const size_t polynomial_order) const;

//private: //for test
	ReferenceFigure reference_figure_;
	std::vector<const MathVector*> node_set_;
	VectorFunction<Polynomial> mapping_function_;
	mutable std::map<size_t, QuadratureRule> integrand_order_to_quadrature_rule_;
	//JacobianFunction<Polynomial> transformation_Jacobian_function_;
};


VectorFunction<Polynomial> operator*(const RowMajorMatrix& m, const VectorFunction<Polynomial> vector_function);


namespace ms {
	std::string figure_type_to_string(const FigureType figure_type);
	double integrate(const Polynomial& integrand, const QuadratureRule& quadrature_rule);
	double integrate(const Polynomial& integrand, const Figure& figure);
	double inner_product(const Polynomial& f1, const Polynomial& f2, const QuadratureRule& quadrature_rule);
	double inner_product(const Polynomial& f1, const Polynomial& f2, const Figure& figure);
	double L2_Norm(const Polynomial& polynomial, const QuadratureRule& quadrature_rule);
	double L2_Norm(const Polynomial& polynomial, const Figure& figure);
	VectorFunction<Polynomial> Gram_Schmidt_Process(const VectorFunction<Polynomial>& initial_polynomial_set, const QuadratureRule& quadrature_rule);
	VectorFunction<Polynomial> Gram_Schmidt_Process(const VectorFunction<Polynomial>& initial_polynomial_set, const Figure& figure);
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
