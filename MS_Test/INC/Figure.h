#pragma once

#include "Matrix.h"
#include "Polynomial.h"
#include "Text.h"


struct QuadratureRule
{
	std::vector<MathVector> quadrature_point_set;
	std::vector<double> quadrature_weight_set;
};


enum class FigureType
{
	Point,
	Line,
	Triangle, Quadrilateral,
	NotInList
};


namespace Editor {
	inline std::string to_String(const FigureType figure_type) {
		switch (figure_type) {
		case FigureType::Line:
			return "Line";
		case FigureType::Triangle:
			return "Triangle";
		case FigureType::Quadrilateral:
			return "Quadrilateral";
		default:
			FATAL_TYPE_ERROR;
			return "NotInList";
		}
	}
}


class ReferenceFigure
{
protected:
	class ReferenceTransfomrationPointSet
	{
	private:
		static std::map<std::pair<FigureType, size_t>, std::vector<MathVector>> key_to_value_;

	public:
		static const std::vector<MathVector>& get(const FigureType figure_type, const size_t transformation_order);

	private:
		static std::vector<MathVector> add(const FigureType figure_type, const size_t transformation_order);

		static size_t support_element_order(const FigureType figure_type);

		static std::vector<MathVector> Text_To_Point_Set(Text& reference_transformation_point_set_text);
	};


	class ReferenceQuadratureRule
	{
	private:
		static std::map<std::pair<FigureType, size_t>, QuadratureRule> key_to_value_;

	public:
		static const QuadratureRule& get(const FigureType figure_type, const size_t transformation_order);

	private:
		static QuadratureRule add(const FigureType figure_type, const size_t transformation_order);

		static size_t calculate_Num_Required_Point(const FigureType figure_type, const size_t required_order);

		static size_t calculate_Required_Order(const FigureType figure_type, const size_t integrand_order);

		static QuadratureRule Text_To_Quadrature_Rule(const Text& quadrature_text);
	};


	class ReferencePostPointSet
	{
	private:
		static std::map<std::pair<FigureType, size_t>, std::vector<MathVector>> key_to_value_;

	public:
		static const std::vector<MathVector>& get(const FigureType figure_type, const size_t post_order);

	private:
		static std::vector<MathVector> add(const FigureType figure_type, const size_t post_order);
	};


	class ReferenceConnectivity
	{
	private:
		static std::map<std::pair<FigureType, size_t>, std::vector<std::vector<double>>> key_to_value_;

	public:
		static const std::vector<std::vector<double>>& get(const FigureType figure_type, const size_t post_order);

	private:
		static std::vector<std::vector<double>> add(const FigureType figure_type, const size_t post_order);
	};


	static MathVector calculate_Center_Node(const FigureType figure_type);

	static std::map<size_t, FigureType> calculate_Face_index_to_Figure_Type_Set(const FigureType figure_type);

	static std::map<size_t, std::vector<size_t>> calculate_Face_index_to_Node_Index_Order_Set(const FigureType figure_type);

	static MathVector calculate_Normal_Vector(const FigureType figure_type);

	static FigureType calculate_Simplex_Figure_Type(const FigureType figure_type);

	static std::vector<size_t> calculate_Vertex_Node_Index_Order_Set(const FigureType figure_type);

	static std::vector<std::vector<size_t>> calculate_Vertex_Simplex_Element_Consisting_Node_Index_Order_Family(const FigureType figure_type);
};


class Figure : public ReferenceFigure
{
protected:
	FigureType figure_type_;
	
	size_t figure_order_;
		
	std::vector<const MathVector*> node_set_;

	VectorFunction<Polynomial> transformation_function_;

	JacobianMatrix<Polynomial> transformation_Jacobian_matrix_;

	//debug
	VectorFunction<Polynomial> linear_transformation_function_;

public:
	explicit Figure(const FigureType figure_type, const size_t figure_order, std::vector<const MathVector*>&& node_set);

	explicit Figure(const std::string& figure_type_name, const size_t figure_order, std::vector<const MathVector*>&& node_set) {
		*this = Figure(this->name_to_Figure_Type(figure_type_name), figure_order, std::move(node_set));
	};

	//debug
	const VectorFunction<Polynomial>& linear_transformation_function(void) const {
		return this->linear_transformation_function_;
	}
	//debug

	std::vector<std::vector<double>> calculate_Connecitivity_Set(const size_t post_order, const size_t start_index) const;

	std::vector<MathVector> calculate_Post_Point_Set(const size_t post_order) const;

	std::vector<MathVector> calculate_Transformed_Point_Set(const std::vector<MathVector>& point_set) const {
		return this->transformation_function_(point_set);
	}

	QuadratureRule calculate_Quadrature_Rule(const size_t integrand_order) const;

	Figure build_Face_Figure(const size_t face_index) const;

	MathVector calculate_Center_Node(void) const {
		return this->transformation_function_(ReferenceFigure::calculate_Center_Node(this->figure_type_));
	};

	double calculate_Volume(void) const;

	std::vector<double> calculate_Projection_Volume_Set(void) const;

	size_t dimension(void) const;

	size_t figure_order(void) const{
		return this->figure_order_; 
	};

	FigureType figure_type(void) const {
		return this->figure_type_;
	};


protected:
	RowMajorMatrix calculate_Transformation_Normal_Matrix(const MathVector& point) const;

private:
	std::vector<const MathVector*> calculate_Face_Node_Set(const size_t face_index) const;

	std::map<size_t, std::vector<const MathVector*>> calculate_Face_Index_to_Node_Set(void) const;

	size_t calculate_Transformation_Scale_Function_Order(void) const;

	//vector function return으로 바꾸기
	std::vector<Monomial> calculate_Linear_Transformation_Monomial_Set(void) const;

	std::vector<Monomial> calculate_Transformation_Monomial_Set(void) const;
	//vector function return으로 바꾸기

	double calculate_Transformation_Scale(const MathVector& point) const;
	
	bool is_Simplex(void) const;

	FigureType name_to_Figure_Type(const std::string& figure_type_name) const;
};


enum class FigureFaceType
{
	InwardFace,
	OutwardFace,
	NotMyFace
};


struct IndexedFigureData
{
	std::string figure_type_name;
	size_t figure_order;
	std::vector<const MathVector*> node_set;
	std::vector<size_t> node_index_set;
};


class IndexedFigure : public Figure
{
protected:
	std::vector<size_t> node_index_set_;
	
public:
	explicit IndexedFigure(const FigureType figure_type, const size_t figure_order, std::vector<const MathVector*>&& node_set, std::vector<size_t>&& node_index_set)
		:Figure(figure_type, figure_order, std::move(node_set)), node_index_set_(std::move(node_index_set)) {};

	explicit IndexedFigure(const std::string& figure_type_name, const size_t figure_order, std::vector<const MathVector*>&& node_set, std::vector<size_t>&& node_index_set)
		:Figure(figure_type_name, figure_order, std::move(node_set)), node_index_set_(std::move(node_index_set)) {};

	IndexedFigure(IndexedFigureData&& data)
		:Figure(data.figure_type_name, data.figure_order, std::move(data.node_set)), node_index_set_(std::move(data.node_index_set)) {};


	IndexedFigure build_Face_Figure(const size_t face_index) const;

	std::map<size_t, std::vector<size_t>> calculate_Face_Index_to_Node_Index_Set(void) const;
	
	std::vector<size_t> calculate_Vertex_Node_Index_Set(void) const;

	std::unordered_map<size_t, std::vector<size_t>> calculate_Vertex_Node_Index_To_Vertex_Simplex_Element_Consisting_Node_Index(void) const;

	FigureFaceType examine_Figure_Face_Type(const IndexedFigure& face_figure) const;
};


namespace Math {
	std::vector<Polynomial> calculate_Initial_Basis_Function_Set(const Figure& figure, const size_t polynomial_order);

	std::vector<Polynomial> calculate_Orthonormal_Basis_Function_Set(const Figure& figure, const size_t polynomial_order);

	double integrate(const Polynomial& integrand, const QuadratureRule& quadrature_rule);

	double integrate(const Polynomial& integrand, const Figure& figure);

	double inner_product(const Polynomial& f1, const Polynomial& f2, const QuadratureRule& quadrature_rule);

	double inner_product(const Polynomial& f1, const Polynomial& f2, const Figure& figure);

	double L2_Norm(const Polynomial& polynomial, const QuadratureRule& quadrature_rule);

	double L2_Norm(const Polynomial& polynomial, const Figure& figure);

	std::vector<Polynomial> Gram_Schmidt_Process(const std::vector<Polynomial>& initial_polynomial_set, const QuadratureRule& quadrature_rule);

	std::vector<Polynomial> Gram_Schmidt_Process(const std::vector<Polynomial>& initial_polynomial_set, const Figure& figure);
}
