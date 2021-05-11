#include "../INC/Figure.h"

std::map<std::pair<FigureType, size_t>, std::vector<MathVector>> ReferenceFigure::key_to_transformation_node_set_;
std::map<std::pair<FigureType, size_t>, VectorFunction<Monomial>> ReferenceFigure::key_to_transformation_monomial_vector_;
std::map<std::pair<FigureType, size_t>, RowMajorMatrix> ReferenceFigure::key_to_inverse_transformation_monomial_matrix_;

std::map<std::pair<FigureType, size_t>, QuadratureRule> ReferenceFigure::key_to_quadrature_rule_;
std::map<std::pair<FigureType, size_t>, std::vector<MathVector>> ReferenceFigure::key_to_post_node_set_;
std::map<std::pair<FigureType, size_t>, std::vector<std::vector<size_t>>> ReferenceFigure::key_to_connectivity_;

ReferenceFigure::ReferenceFigure(const FigureType figure_type, const size_t figure_order)
	: figure_type_(figure_type), figure_order_(figure_order) {
	if (figure_order == 0)
		throw std::invalid_argument("figure order should be greater than 1");

	auto key = std::make_pair(this->figure_type_, this->figure_order_);
	if (ReferenceFigure::key_to_transformation_node_set_.find(key) == ReferenceFigure::key_to_transformation_node_set_.end()) {
		ReferenceFigure::key_to_transformation_node_set_.emplace(key, this->transformation_node_set());
		ReferenceFigure::key_to_transformation_monomial_vector_.emplace(key, this->transformation_monomial_vector());
		ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.emplace(std::move(key), this->inverse_transformation_monomial_matrix());
	}
}


const std::vector<MathVector>& ReferenceFigure::reference_post_node_set(const size_t post_order) {
	auto key = std::make_pair(this->figure_type_, post_order);

	if (ReferenceFigure::key_to_post_node_set_.find(key) == ReferenceFigure::key_to_post_node_set_.end()) {
		std::vector<MathVector> node_set;
		switch (this->figure_type_) {

		case FigureType::Triangle: {
			const size_t num_node = static_cast<size_t>((post_order + 2) * (post_order + 3) * 0.5);
			node_set.reserve(num_node);

			const double delta = 2.0 / (post_order + 1);

			const MathVector start_node = { -1.0,-1.0 };
			for (size_t j = 0; j <= post_order + 1; ++j) {
				for (size_t i = 0; i <= post_order + 1 - j; ++i) {
					MathVector translate = {delta* i, delta* j};
					node_set.push_back(start_node+translate);
				}
			}
			break;
		}

		case FigureType::Quadrilateral: {
			const size_t num_node = (post_order + 2) * (post_order + 2);
			node_set.reserve(num_node);

			const double delta = 2.0 / (post_order + 1);

			const MathVector start_node = { -1.0,-1.0 };
			for (size_t j = 0; j <= post_order + 1; ++j) {
				for (size_t i = 0; i <= post_order + 1; ++i) {
					MathVector translate = { delta * i, delta * j };
					node_set.push_back(start_node + translate);
				}
			}
			break;
		}

		default:
			throw std::runtime_error("worng figure type");
			break;
		}
		ReferenceFigure::key_to_post_node_set_.emplace(std::move(key), std::move(node_set));
	}		

	return ReferenceFigure::key_to_post_node_set_.at(key);
}

const std::vector<std::vector<size_t>>& ReferenceFigure::reference_connectivity(const size_t post_order) {
	auto key = std::make_pair(this->figure_type_, post_order);
	if (ReferenceFigure::key_to_connectivity_.find(key) == ReferenceFigure::key_to_connectivity_.end()) {
		std::vector<std::vector<size_t>> connectivity;

		switch (this->figure_type_) {
		case FigureType::Triangle: {
			const size_t num_simplex = (post_order + 1) * (post_order + 1);
			connectivity.resize(num_simplex);

			constexpr size_t num_node = 3;

			size_t isimplex = 0;
			for (size_t j = 0; j <= post_order; j++) {
				for (size_t i = 0; i <= post_order - j; i++) {
					//    b  --------- b+1		// recurrence relation
					//    |            |		// a(0,j+1) = a(0,j) + (order + 2) - j
					//    |            |		// a(0,j)   = a(0,0) + (order + 2) * j - (j-1) * j * 0.5
					//    a  --------- a+1		// a(0,0)	= 1

					const size_t a_point_index = static_cast<size_t>(i + 1 + (post_order + 2) * j - (j - 1) * j * 0.5);
					const size_t b_point_index = static_cast<size_t>(i + 1 + (post_order + 2) * (j + 1) - j * (j + 1) * 0.5);

					connectivity[isimplex].resize(num_node);
					connectivity[isimplex][0] = a_point_index;
					connectivity[isimplex][1] = a_point_index + 1;
					connectivity[isimplex][2] = b_point_index;
					isimplex++;

					if (i == post_order - j)
						continue;

					connectivity[isimplex].resize(num_node);
					connectivity[isimplex][0] = a_point_index + 1;
					connectivity[isimplex][1] = b_point_index + 1;
					connectivity[isimplex][2] = b_point_index;
					isimplex++;
				}
			}
			break;
		}

		case FigureType::Quadrilateral: {
			const size_t num_simplex = 2 * (post_order + 1) * (post_order + 1);
			connectivity.resize(num_simplex);

			const size_t num_node = 3;
			size_t isimplex = 0;
			for (size_t j = 0; j <= post_order; j++) {
				for (size_t i = 0; i <= post_order; i++) {
					//    b  --------- b+1		// recurrence relation
					//    |            |		// a(0,j+1) = a(0,j) + order + 2
					//    |            |		// a(0,j)   = a(0,0) + (order + 2) * j
					//    a  --------- a+1		// a(0,0)	= 1

					const size_t a_point_index = i + 1 + (post_order + 2) * j;
					const size_t b_point_index = i + 1 + (post_order + 2) * (j + 1);

					connectivity[isimplex].resize(num_node);
					connectivity[isimplex][0] = a_point_index;
					connectivity[isimplex][1] = a_point_index + 1;
					connectivity[isimplex][2] = b_point_index;
					isimplex++;

					connectivity[isimplex].resize(num_node);
					connectivity[isimplex][0] = a_point_index + 1;
					connectivity[isimplex][1] = b_point_index + 1;
					connectivity[isimplex][2] = b_point_index;
					isimplex++;
				}
			}
			break;
		}

		default:
			throw std::runtime_error("worng figure type");
			break;
		}
		ReferenceFigure::key_to_connectivity_.emplace(std::move(key), std::move(connectivity));
	}
	return ReferenceFigure::key_to_connectivity_.at(key);
}

MathVector ReferenceFigure::center_node(void) const {
	switch (this->figure_type_) {
	case FigureType::Line:
		return { 0, 0, 0 };

	case FigureType::Triangle:
		return { -1.0 / 3.0, -1.0 / 3.0, 0 };

	case FigureType::Quadrilateral:
		return { 0, 0, 0 };

	default:
		throw std::runtime_error("wrong figure type");
		return MathVector();
	}
}

std::vector<FigureType> ReferenceFigure::face_figure_type_set(void) const {
	std::vector<FigureType> face_figure_type_set;
	switch (this->figure_type_) {

	case FigureType::Line: {
		// 0 ──── 1

		face_figure_type_set.push_back(FigureType::Point);
		face_figure_type_set.push_back(FigureType::Point);
		break;
	}

	case FigureType::Triangle: {
		//    / \
		//	 2   1
		//	/     \
		//  ───0───

		face_figure_type_set.push_back(FigureType::Line);
		face_figure_type_set.push_back(FigureType::Line);
		face_figure_type_set.push_back(FigureType::Line);
		break;
	}

	case FigureType::Quadrilateral: {
		//  ┌─ 2 ─┐
		//  3     1
		//  └─ 0 ─┘

		face_figure_type_set.push_back(FigureType::Line);
		face_figure_type_set.push_back(FigureType::Line);
		face_figure_type_set.push_back(FigureType::Line);
		face_figure_type_set.push_back(FigureType::Line);
		break;
	}
	default:
		throw std::runtime_error("wrong figure type");
	}

	return face_figure_type_set;
}

size_t ReferenceFigure::figure_dimension(void) const {
	switch (this->figure_type_) {
	case FigureType::Line:
		return 1;
	case FigureType::Triangle:
	case FigureType::Quadrilateral:
		return 2;
	default:
		throw std::runtime_error("wrong figure type");
		return NULL;
	}
}

MathVector ReferenceFigure::normal_vector(void) const {
	switch (this->figure_type_) {
	case FigureType::Line:
		return { 0,1,0 };

	case FigureType::Triangle:
	case FigureType::Quadrilateral:
		return { 0,0,1 };

	default:
		throw std::runtime_error("wrong figure type");
		return MathVector();
	}
}

FigureType ReferenceFigure::simplex_figure_type(void) const {
	switch (this->figure_type_) {
	case FigureType::Triangle:
	case FigureType::Quadrilateral:
		return FigureType::Triangle;

	default:
		throw std::runtime_error("wrong figure type");
		return FigureType::Triangle;
	}
}

VectorFunction<Polynomial> ReferenceFigure::calculate_transformation_function(const std::vector<const MathVector*>& transformed_node_set) const {
	const auto key = std::make_pair(this->figure_type_, this->figure_order_);	
	const auto& reference_transformation_node_set = ReferenceFigure::key_to_transformation_node_set_.at(key);
	
	const auto num_reference_node = reference_transformation_node_set.size();
	const auto num_transformed_node = transformed_node_set.size();
	if (num_reference_node != num_transformed_node)
		throw std::runtime_error("reference node and transformed node does not 1:1 match");

	//	T = CM
	//	T : transformed node matrix			
	//	C : transformation coefficient matrix	
	//	M : transformation monomial matrix	=> always same for same figure type
	constexpr size_t range_dimension = 3;
	RowMajorMatrix T(range_dimension, num_transformed_node);
	for (size_t i = 0; i < num_transformed_node; ++i)
		T.change_column(i, *transformed_node_set[i]);

	const auto& inv_M = ReferenceFigure::key_to_inverse_transformation_monomial_matrix_.at(key);
	const auto C = T * inv_M;

	const auto& transformation_monomial_vector = ReferenceFigure::key_to_transformation_monomial_vector_.at(key);
	return C * transformation_monomial_vector;
}

QuadratureRule ReferenceFigure::calculate_quadrature_rule(const VectorFunction<Polynomial>& transformation_function, const size_t physical_domain_integrand_order) const {
	const auto TS_function = this->calculate_trasnformation_scale_function(transformation_function);

	const auto reference_domain_integrand_order = physical_domain_integrand_order + TS_function.polynomial_order();	
	auto key = std::make_pair(this->figure_type_, reference_domain_integrand_order);
	if (ReferenceFigure::key_to_quadrature_rule_.find(key) == ReferenceFigure::key_to_quadrature_rule_.end())
		ReferenceFigure::key_to_quadrature_rule_.emplace(key, this->reference_quadrature_rule(reference_domain_integrand_order));
	
	const auto& reference_quadrature_rule = ReferenceFigure::key_to_quadrature_rule_.at(key);

	const auto transformed_QP = transformation_function(reference_quadrature_rule.node_set);
	
	const auto num_QP = transformed_QP.size();
	std::vector<double> transformed_QW(num_QP);
	for (size_t ipoint = 0; ipoint < num_QP; ++ipoint) {
		const auto& node = reference_quadrature_rule.node_set[ipoint];
		const auto& weight = reference_quadrature_rule.weight_set[ipoint];
		transformed_QW[ipoint] = TS_function(node) * weight;
	}

	return QuadratureRule{ transformed_QP, transformed_QW };
}

size_t ReferenceFigure::calculate_Required_Order(const size_t integrand_order) const {
	const auto remain = integrand_order % 2;

	switch (this->figure_type_) {
	case FigureType::Line:
	case FigureType::Quadrilateral: {
		if (remain == 0)
			return integrand_order + 1;
		else
			return integrand_order;
	}
	case FigureType::Triangle: {
		if (remain == 0)
			return integrand_order;
		else
			return integrand_order + 1;
	}
	default:
		return NULL;
	}
}

size_t ReferenceFigure::calculate_Num_Required_Point(const size_t required_order) const {
	switch (this->figure_type_) {
	case FigureType::Line:
		return static_cast<size_t>((required_order + 1) * 0.5);
	case FigureType::Triangle:
		return static_cast<size_t>((required_order * 0.5 + 1) * (required_order * 0.5 + 1));
	case FigureType::Quadrilateral:
		return static_cast<size_t>((required_order + 1) * (required_order + 1) * 0.25);
	default:
		return NULL;
	}
}

size_t ReferenceFigure::support_element_order(void) const {
	switch (this->figure_type_) {
	case FigureType::Line:
		return 6;
	case FigureType::Triangle:
		return 5;
	case FigureType::Quadrilateral:
		return 6;
	default:
		throw std::runtime_error("wrong type");
		return NULL;
	}
}

Polynomial ReferenceFigure::calculate_trasnformation_scale_function(const VectorFunction<Polynomial>& transformation_function) const {
	switch (this->figure_dimension())
	{
	case 1: {
		constexpr size_t r = 0;
		const auto T_r = ms::differentiate(transformation_function, r);
		return T_r.L2_norm();
	}
	case 2: {
		constexpr size_t r = 0;
		constexpr size_t s = 1;
		const auto T_r = ms::differentiate(transformation_function, r);
		const auto T_s = ms::differentiate(transformation_function, s);
		return T_r.cross_product(T_s).L2_norm();
	}
	default:
		throw std::runtime_error("not supported figure dimension");
		return Polynomial();
	}
}

std::vector<MathVector> ReferenceFigure::transformation_node_set(void) const {
	if (this->figure_order_ > this->support_element_order())
		throw std::runtime_error("exceed support element order");

	const std::string read_file_path = "RSC/ReferenceFigures/" + ms::to_string(this->figure_type_) + "_P" + std::to_string(this->figure_order_) + ".msh";

	Text transformation_node_set_text;
	transformation_node_set_text.read(read_file_path);
	transformation_node_set_text.remove_empty_line();

	const auto num_nodes = ms::to_value<size_t>(transformation_node_set_text.front());
	transformation_node_set_text.erase(transformation_node_set_text.begin());

	std::vector<MathVector> transformation_node_set;
	transformation_node_set.reserve(num_nodes);
	for (const auto& sentence : transformation_node_set_text) {
		const char delimiter = ' ';
		auto parsed_str = ms::parse(sentence, delimiter);
		auto point_value = ms::to_value_set<double>(parsed_str);

		point_value.erase(point_value.begin());
		transformation_node_set.emplace_back(std::move(point_value));
	}

	return transformation_node_set;
}

VectorFunction<Monomial> ReferenceFigure::transformation_monomial_vector(void) const {
	VectorFunction<Monomial> transformation_monomial_vector;
	switch (this->figure_type_) {
	case FigureType::Line: {
		const size_t num_monomial = this->figure_order_ + 1;
		transformation_monomial_vector.reserve(num_monomial);

		for (size_t a = 0; a <= this->figure_order_; ++a)
			transformation_monomial_vector.push_back(X ^ a);

		return transformation_monomial_vector;	// 1 r r^2 ...
	}
	case FigureType::Triangle: {
		const size_t num_monomial = static_cast<size_t>((this->figure_order_ + 2) * (this->figure_order_ + 1) * 0.5);
		transformation_monomial_vector.reserve(num_monomial);

		for (size_t a = 0; a <= this->figure_order_; ++a)
			for (size_t b = 0; b <= a; ++b)
				transformation_monomial_vector.push_back((X ^ (a - b)) * (Y ^ b));

		return transformation_monomial_vector;	// 1 r s r^2 rs s^2 ...
	}
	case FigureType::Quadrilateral: {
		const size_t num_monomial = static_cast<size_t>((this->figure_order_ + 1) * (this->figure_order_ + 1));
		transformation_monomial_vector.reserve(num_monomial);

		for (size_t a = 0; a <= this->figure_order_; ++a) {
			for (size_t b = 0; b <= a; ++b)
				transformation_monomial_vector.push_back((X ^ a) * (Y ^ b));

			if (a == 0)
				continue;

			size_t index = a;
			while (true) {
				transformation_monomial_vector.push_back((X ^ (--index)) * (Y ^ a));
				if (index == 0)
					break;
			}
		}
		return transformation_monomial_vector;	// 1 r rs s r^2 r^2s r^2s^2 rs^2 s^2...
	}
	default:
		throw std::runtime_error("wrong figure type");
		return transformation_monomial_vector;
	}
}

RowMajorMatrix ReferenceFigure::inverse_transformation_monomial_matrix(void) const {
	const auto key = std::make_pair(this->figure_type_, this->figure_order_);
	
	const auto transformation_node_set = ReferenceFigure::key_to_transformation_node_set_.at(key);
	const auto transformation_monomial_vector = ReferenceFigure::key_to_transformation_monomial_vector_.at(key);
	
	const auto num_column = transformation_monomial_vector.size();
	RowMajorMatrix transformation_monomial_matrix(num_column);
	for (size_t i = 0; i < num_column; ++i)
		transformation_monomial_matrix.change_column(i, transformation_monomial_vector(transformation_node_set[i]));

	return transformation_monomial_matrix.inverse();
}

QuadratureRule ReferenceFigure::reference_quadrature_rule(const size_t integrand_order) const {
	const auto required_order = this->calculate_Required_Order(integrand_order);
	const auto num_required_point = this->calculate_Num_Required_Point(required_order);

	const std::string read_file_path = "RSC/Quadrature/Standard/" + ms::to_string(this->figure_type_) + "/P" + std::to_string(required_order) + "_n" + std::to_string(num_required_point) + ".txt";

	Text reference_quadrature_text;
	reference_quadrature_text.read(read_file_path);
	reference_quadrature_text.remove_empty_line();

	const auto num_quadrature_point = reference_quadrature_text.size();
	std::vector<MathVector> node_set;
	std::vector<double> weight_set;
	node_set.reserve(num_quadrature_point);
	weight_set.reserve(num_quadrature_point);

	for (const auto& sentence : reference_quadrature_text) {
		const char denominator = ' ';
		const auto parsed_sentence = ms::parse(sentence, denominator);

		auto quadrature_value_set = ms::to_value_set<double>(parsed_sentence);

		weight_set.emplace_back(quadrature_value_set.back());
		quadrature_value_set.pop_back();
		node_set.emplace_back(std::move(quadrature_value_set));
	}

	return { node_set,weight_set };
}

std::vector<std::vector<size_t>> ReferenceFigure::vertex_simplex_node_index_order_family(void) const {
	//For hMLP_BD Limiter
	std::vector<std::vector<size_t>> vertex_simplex_space_node_index_order_family;
	switch (this->figure_type_) {
	case FigureType::Quadrilateral:
	{
		//  3─────2
		//  │     │
		//  0─────1

		constexpr size_t num_simplex = 4;

		const std::vector<size_t> first_simplex_node_index = { 0,1,3 };
		const std::vector<size_t> second_simplex_node_index = { 1,2,0 };
		const std::vector<size_t> third_simplex_node_index = { 2,3,1 };
		const std::vector<size_t> fourth_simplex_node_index = { 3,0,2 };

		vertex_simplex_space_node_index_order_family.reserve(num_simplex);
		vertex_simplex_space_node_index_order_family.emplace_back(first_simplex_node_index);
		vertex_simplex_space_node_index_order_family.emplace_back(second_simplex_node_index);
		vertex_simplex_space_node_index_order_family.emplace_back(third_simplex_node_index);
		vertex_simplex_space_node_index_order_family.emplace_back(fourth_simplex_node_index);

		break;
	}
	default:
		throw std::runtime_error("wrong figure type");
	}

	return vertex_simplex_space_node_index_order_family;
}

std::vector<size_t> ReferenceFigure::vertex_node_index_order_set(void) const {
	switch (this->figure_type_) {
	case FigureType::Line: {
		// 0 ──── 1

		return { 0,1 };
	}

	case FigureType::Triangle: {
		//  2
		//  │ \
		//	│  \
		//  0───1

		return { 0,1,2 };
	}

	case FigureType::Quadrilateral: {
		//  3─────2
		//  │     │
		//  0─────1

		return { 0,1,2,3 };
	}

	default:
		throw std::runtime_error("wrong figure type");
		return { 0 };
	}
}

std::map<size_t, std::vector<size_t>> ReferenceFigure::face_index_to_node_index_order_set(void) const {
	// it tells index order of i - th face consisting node at cell consisting node index

	std::map<size_t, std::vector<size_t>> face_index_to_node_index_order_set;
	switch (this->figure_type_) {

	case FigureType::Line: {
		// 0 ──── 1

		constexpr size_t num_face = 2;

		std::vector<size_t> first_face_node_index = { 0 };
		std::vector<size_t> second_face_node_index = { 1 };

		face_index_to_node_index_order_set.emplace(0, std::move(first_face_node_index));
		face_index_to_node_index_order_set.emplace(1, std::move(second_face_node_index));

		break;
	}

	case FigureType::Triangle: {

		//      2
		//  2  / \  1
		//	  /   \
		//   0─────1
		//      0
		constexpr size_t num_face = 3;

		std::vector<size_t> face0_node_index = { 0,1 };
		std::vector<size_t> face1_node_index = { 1,2 };
		std::vector<size_t> face2_node_index = { 2,0 };

		face_index_to_node_index_order_set.emplace(0, std::move(face0_node_index));
		face_index_to_node_index_order_set.emplace(1, std::move(face1_node_index));
		face_index_to_node_index_order_set.emplace(2, std::move(face2_node_index));

		//if (element_order > 1)
		//{
		//	const size_t num_additional_point = element_order - 1;
		//	
		//	size_t index = num_face;
		//	for (size_t iface = 0; iface < num_face; ++iface)
		//		for (size_t ipoint = 0; ipoint < num_additional_point; ++ipoint)
		//			face_node_index_order[iface].push_back(index++);
		//}

		break;
	}

	case FigureType::Quadrilateral: {
		//      2
		//   3─────2
		//3  │     │   1
		//   0─────1
		//      0
		const size_t num_face = 4;

		std::vector<size_t> face0_node_index = { 0,1 };
		std::vector<size_t> face1_node_index = { 1,2 };
		std::vector<size_t> face2_node_index = { 2,3 };
		std::vector<size_t> face3_node_index = { 3,0 };

		face_index_to_node_index_order_set.emplace(0, std::move(face0_node_index));
		face_index_to_node_index_order_set.emplace(1, std::move(face1_node_index));
		face_index_to_node_index_order_set.emplace(2, std::move(face2_node_index));
		face_index_to_node_index_order_set.emplace(3, std::move(face3_node_index));

		//if (element_order > 1)
		//{
		//	const size_t num_additional_point = element_order - 1;

		//	size_t index = num_face;
		//	for (size_t iface = 0; iface < num_face; ++iface)
		//		for (size_t ipoint = 0; ipoint<num_additional_point; ++ipoint)
		//			face_node_index_order[iface].push_back(index++);
		//}

		break;
	}
	default:
		throw std::runtime_error("wrong figure type");
	}

	return face_index_to_node_index_order_set;
}

namespace ms {
	std::string to_string(const FigureType figure_type) {
		switch (figure_type) {
		case FigureType::Point:
			return "Point";
		case FigureType::Line:
			return "Line";
		case FigureType::Triangle:
			return "Triangle";
		case FigureType::Quadrilateral:
			return "Quadrilateral";
		default:
			throw std::runtime_error("wrong type");
			return NULL;
		}
	}
}


Figure::Figure(const FigureType figure_type, const size_t figure_order, std::vector<const MathVector*>&& node_set)
	: reference_figure_(figure_type, figure_order), node_set_(std::move(node_set)) {	
	
	this->transformation_function_ = this->reference_figure_.calculate_transformation_function(this->node_set_);
	//this->transformation_Jacobian_function_ = JacobianFunction(transformation_function_);
	////linear transformation function
	//const auto linear_transformation_monomial_set = this->calculate_Linear_Transformation_Monomial_Set();
	//const auto num_linear_transformation_monomial = linear_transformation_monomial_set.size();

	//RowMajorMatrix M_linear(MatrixType::Full, num_linear_transformation_monomial, num_original_point);
	//for (size_t i = 0; i < num_linear_transformation_monomial; ++i)
	//	for (size_t j = 0; j < num_original_point; ++j)
	//		M_linear.at(i, j) = linear_transformation_monomial_set[i](*this->node_set_[j]);

	//// X_linear = C_linear * M_linear
	//// C_linear = X_linear * TRS(M_linear) * INV(M_linear * TRS(M_linear))
	//RowMajorMatrix X_linear(MatrixType::Full, num_coord, num_original_point);
	//for (size_t i = 0; i < num_original_point; ++i)
	//	X_linear.change_Column(i, reference_transformation_node_set[i]);

	//const auto tmp = M_linear;
	//const auto& trs_M_linear = M_linear.transpose();
	//const auto C_linear = X_linear * trs_M_linear * ((tmp * trs_M_linear).inverse());
	//
	//const auto first_coord_linear_trasnformation_coeffcient_set = C_linear.row(0);
	//const auto second_coord_linear_trasnformation_coeffcient_set = C_linear.row(1);
	//const auto third_coord_linear_trasnformation_coeffcient_set = C_linear.row(2);

	//this->linear_transformation_function_.emplace_back(Polynomial(first_coord_linear_trasnformation_coeffcient_set, linear_transformation_monomial_set));
	//this->linear_transformation_function_.emplace_back(Polynomial(second_coord_linear_trasnformation_coeffcient_set, linear_transformation_monomial_set));
	//this->linear_transformation_function_.emplace_back(Polynomial(third_coord_linear_trasnformation_coeffcient_set, linear_transformation_monomial_set));
};

MathVector Figure::calculate_center_node(void) const {
	return this->transformation_function_(this->reference_figure_.center_node());
}

VectorFunction<Polynomial> Figure::calculate_orthonormal_basis_vector(const size_t polynomial_order) const {
	//initial basis
	const auto figure_dimension = this->reference_figure_.figure_dimension();
	const auto num_basis = ms::combination_with_repetition(1 + figure_dimension, polynomial_order);

	VectorFunction<Polynomial> initial_basis_set;
	initial_basis_set.reserve(num_basis);

	if (figure_dimension == 2) {
		//const auto& linear_transformation_function = figure.linear_transformation_function();
		//const auto& x0 = linear_transformation_function[0];
		//const auto& x1 = linear_transformation_function[1];

		//for (size_t a = 0; a <= order; ++a) {
		//	for (size_t b = 0; b <= a; ++b) {
		//		Polynomial initial_basis(1);

		//		for (size_t count = 0; count < a - b; ++count)
		//			initial_basis *= x0;

		//		for (size_t count = 0; count < b; ++count)
		//			initial_basis *= x1;

		//		initial_basis_set.emplace_back(std::move(initial_basis));
		//	}
		//}
		
		//// 1 x y x^2 xy y^2
		//for (size_t a = 0; a <= polynomial_order; ++a)
		//	for (size_t b = 0; b <= a; ++b)
		//		initial_basis_set.push_back((X^(a-b))*(Y^b));

		//1 (x - x_c) (y - y_c)  ...
		const auto center_node = this->calculate_center_node();
		for (double a = 0; a <= polynomial_order; ++a)
			for (double b = 0; b <= a; ++b)
				initial_basis_set.push_back(((X-center_node[0])^(a-b))*((Y-center_node[1])^b));
	}
	else
		throw std::runtime_error("wrong figure type");

	return initial_basis_set;
}

QuadratureRule Figure::calculate_quadrature_rule(const size_t integrand_roder) const {
	return this->reference_figure_.calculate_quadrature_rule(this->transformation_function_, integrand_roder);
}


VectorFunction<Polynomial> operator*(const RowMajorMatrix& m, const VectorFunction<Monomial> vector_function) {
	const auto [num_row, num_column] = m.size();

	if (num_column != vector_function.size())
		throw std::length_error("length does not match");

	VectorFunction<Polynomial> result;
	result.reserve(num_row);
	for (size_t i = 0; i < num_row; ++i)
		result.push_back(Polynomial{ m.row(i) ,vector_function });

	return result;
}

namespace ms {
	std::vector<Polynomial> Gram_Schmidt_Process(const VectorFunction<Polynomial>& initial_polynomial_set, const Figure& figure) {
		auto normalized_polynomial_set = initial_polynomial_set;

		for (size_t i = 0; i < initial_polynomial_set.size(); ++i) {
			for (size_t j = 0; j < i; ++j)
				normalized_polynomial_set[i] -= ms::inner_product(normalized_polynomial_set[i], normalized_polynomial_set[j], figure) * normalized_polynomial_set[j];

			normalized_polynomial_set[i] *= 1.0 / ms::L2_Norm(normalized_polynomial_set[i], figure);
		}

		return normalized_polynomial_set;
	}

	double integrate(const Polynomial& integrand, const QuadratureRule& quadrature_rule) {
		const auto& QP_set = quadrature_rule.node_set;
		const auto& QW_set = quadrature_rule.weight_set;

		double result = 0.0;
		for (size_t i = 0; i < QP_set.size(); ++i) 
			result += integrand(QP_set[i]) * QW_set[i];

		return result;
	}

	double integrate(const Polynomial& integrand, const Figure& figure) {
		const auto quadrature_rule = figure.calculate_quadrature_rule(integrand.polynomial_order());
		return ms::integrate(integrand, quadrature_rule);
	}

	double inner_product(const Polynomial& f1, const Polynomial& f2, const QuadratureRule& quadrature_rule) {
		return ms::integrate(f1 * f2, quadrature_rule);
	}

	double inner_product(const Polynomial& f1, const Polynomial& f2, const Figure& figure) {
		return ms::integrate(f1 * f2, figure);
	}

	double L2_Norm(const Polynomial& polynomial, const QuadratureRule& quadrature_rule) {
		return std::sqrt(ms::inner_product(polynomial, polynomial, quadrature_rule));
	}

	double L2_Norm(const Polynomial& polynomial, const Figure& figure) {
		return std::sqrt(ms::inner_product(polynomial, polynomial, figure));
	}
}

//
//std::vector<std::vector<double>> Figure::calculate_Connecitivity_Set(const size_t post_order, const size_t start_index) const {
//	const auto& reference_connectivity = ReferenceFigure::ReferenceConnectivity::get(this->figure_type_, post_order);
//
//	auto connectivity = reference_connectivity;
//
//	for (auto& simplex_consisting_node_index_set : connectivity)
//		for (auto& node_index : simplex_consisting_node_index_set)
//			node_index += start_index;
//
//	return connectivity;
//}
//
//std::vector<MathVector> Figure::calculate_Post_Point_Set(const size_t post_order) const {
//	const auto& reference_post_point_set = ReferenceFigure::ReferencePostPointSet::get(this->figure_type_, post_order);
//	return this->transformation_function_(reference_post_point_set);
//}
//
//QuadratureRule Figure::calculate_Quadrature_Rule(const size_t polynomial_order) const {
	//const auto integrand_order = polynomial_order + this->calculate_Transformation_Scale_Function_Order();
	//const auto& reference_quadrature_rule = ReferenceFigure::ReferenceQuadratureRule::get(this->figure_type_, integrand_order);
	//const auto& reference_QP = reference_quadrature_rule.quadrature_point_set;
	//const auto& reference_QW = reference_quadrature_rule.quadrature_weight_set;

	//const auto transformed_QP = this->transformation_function_(reference_QP);

	//const auto num_QP = reference_QP.size();
	//std::vector<double> transformed_QW(num_QP);
	//for (size_t ipoint = 0; ipoint < num_QP; ++ipoint) {
	//	const auto& node = reference_QP[ipoint];
	//	const auto trasnformation_scale = this->calculate_Transformation_Scale(node);

	//	transformed_QW[ipoint] = reference_QW[ipoint] * trasnformation_scale;
	//}

	//return QuadratureRule{ transformed_QP, transformed_QW };
//}
//
//Figure Figure::build_Face_Figure(const size_t face_index) const {
//	const auto face_index_to_face_figure_type = ReferenceFigure::calculate_Face_index_to_Figure_Type_Set(this->figure_type_);
//
//	const auto face_figure_type = face_index_to_face_figure_type.at(face_index);
//	const auto face_figure_order = this->figure_order_;
//	auto face_node_set = this->calculate_Face_Node_Set(face_index);
//
//	return Figure(face_figure_type, face_figure_order, std::move(face_node_set));
//}
//
//std::vector<double> Figure::calculate_Projection_Volume_Set(void) const{
//	switch (this->dimension())
//	{
//	case 2:	{
//		double projection_volume_on_x0_axis = 0.0;
//		double projection_volume_on_x1_axis = 0.0;
//
//		for (const auto& [face_index, face_node_set] : this->calculate_Face_Index_to_Node_Set())
//		{
//			const auto& start_node = *face_node_set[0];
//			const auto& end_node = *face_node_set[1];
//			const auto node_to_node = end_node - start_node;
//
//			projection_volume_on_x0_axis += std::abs(node_to_node[0]);
//			projection_volume_on_x1_axis += std::abs(node_to_node[1]);
//		}
//
//		return { 0.5 * projection_volume_on_x0_axis, 0.5 * projection_volume_on_x1_axis };
//	}
//	default:
//		throw std::runtime_error("wrong figure type");
//		return std::vector<double>();
//	}
//}
//
//
//
//double Figure::calculate_Volume(void) const {
//	const size_t integrand_order = 0;	// int(1) = volume
//
//	const auto calculate_Quadrature_Rule = this->calculate_Quadrature_Rule(integrand_order);
//	const auto& QW_set = calculate_Quadrature_Rule.quadrature_weight_set;
//
//	double calculate_Volume = 0;
//	for (const double QW : QW_set)
//		calculate_Volume += QW;
//
//	return calculate_Volume;
//}
//
//size_t Figure::dimension(void) const {
//	switch (this->figure_type_) {
//	case FigureType::Line:				return 1;
//	case FigureType::Triangle:
//	case FigureType::Quadrilateral:		return 2;
//	default:
//		throw std::runtime_error("wrong figure type");
//		return NULL;
//	}
//}
//
//std::vector<const MathVector*> Figure::calculate_Face_Node_Set(const size_t face_index) const {
//	const auto face_index_to_node_index_order_set = ReferenceFigure::calculate_Face_index_to_Node_Index_Order_Set(this->figure_type_);
//	const auto& face_node_index_order_set = face_index_to_node_index_order_set.at(face_index);
//	const auto num_node = face_node_index_order_set.size();
//
//	std::vector<const MathVector*> face_node_set;
//	face_node_set.reserve(num_node);
//
//	for (const auto& face_node_index_order : face_node_index_order_set)
//		face_node_set.emplace_back(this->node_set_[face_node_index_order]);
//
//	return face_node_set;
//}
//
//std::map<size_t, std::vector<const MathVector*>> Figure::calculate_Face_Index_to_Node_Set(void) const {
//	const auto face_index_to_node_index_order_set = ReferenceFigure::calculate_Face_index_to_Node_Index_Order_Set(this->figure_type_);
//	const auto num_face = face_index_to_node_index_order_set.size();
//
//	std::map<size_t, std::vector<const MathVector*>> face_index_to_node_set;
//	for (const auto& [face_index, face_node_index_order_set] : face_index_to_node_index_order_set) {
//		const auto num_consisting_node = face_node_index_order_set.size();
//		std::vector<const MathVector*> face_node_set(num_consisting_node);
//		for (size_t i = 0; i < num_consisting_node; ++i)
//			face_node_set[i] = this->node_set_[face_node_index_order_set[i]];
//
//		face_index_to_node_set.emplace(face_index, std::move(face_node_set));
//	}
//
//	return face_index_to_node_set;
//}
//
//size_t Figure::calculate_Transformation_Scale_Function_Order(void) const {
//	switch (this->dimension()) {
//	case 1: {
//		// transformation scale function : ||J||_F
//		// high order mesh에서 1D -> 2D로 갈경우 transformation scale function이 polynomial이 아님 -> approximate max(J_00, J_10, J_20)
//		// where J_ij order of i,j component in Jacobian		
//
//		std::array<size_t, 3> order_set;
//		order_set.fill(0);
//				
//		for (size_t i = 0; i < 3; ++i)
//			order_set[i] = this->transformation_Jacobian_matrix_.at(i, 0).order();
//
//		return *std::max_element(order_set.begin(), order_set.end());
// 	}
//	case 2: {
//		// transformation scale function : ||Cof(J)||_F
//		// 사각형에서 2D -> 3D로 갈 경우 transformation scale function이 polynomial이 아님 -> approximate max(J_10*J_21, J_11*J_20, J_00*J_21, J_01*J_20, J_00*J_11, J_01*J_10)
//		// where J_ij order of i, j component in Jacobian
//
//		std::array<std::array<size_t, 3>, 3> Jacobian_order_set;
//		Jacobian_order_set.fill({ 0,0,0 });
//
//		for (size_t i = 0; i < 3; ++i)
//			for (size_t j = 0; j < 3; ++j)
//				Jacobian_order_set[i][j] = this->transformation_Jacobian_matrix_.at(i, j).order();
//
//		std::array<size_t, 6> order_set;
//		order_set.fill(0);
//
//		// 0과 곱해도 order가 보존되는 문제가 있음 
//		// 계수합이 0 이라서 상쇄되는 경우를 인식하지 못함
//		// 실제로 transfomration scale function 계산해야되나?
//		order_set[0] = Jacobian_order_set[1][0] + Jacobian_order_set[2][1];
//		order_set[1] = Jacobian_order_set[1][1] + Jacobian_order_set[2][0];
//		order_set[2] = Jacobian_order_set[0][0] + Jacobian_order_set[2][1];
//		order_set[3] = Jacobian_order_set[0][1] + Jacobian_order_set[2][0];
//		order_set[4] = Jacobian_order_set[0][0] + Jacobian_order_set[1][1];
//		order_set[5] = Jacobian_order_set[0][1] + Jacobian_order_set[1][0];
//
//		return *std::max_element(order_set.begin(), order_set.end());
//	}
//	case 3: {
//		// transformation scale function : |det(J)|
//		// max( J_00*J_11*J_22, J_00*J_12*J_21, J_01*J_10*J_22, J_01*J_12*J_20, J_02*J_10*J_21, J_02*J_11*J_20)
//		// where J_ij order of i, j component in Jacobian
//
//		std::array<std::array<size_t, 3>, 3> Jacobian_order_set;
//		Jacobian_order_set.fill({ 0,0,0 });
//
//		for (size_t i = 0; i < 3; ++i)
//			for (size_t j = 0; j < 3; ++j)
//				Jacobian_order_set[i][j] = this->transformation_Jacobian_matrix_.at(i, j).order();
//
//		std::array<size_t, 6> order_set;
//		order_set.fill(0);
//
//		order_set[0] = Jacobian_order_set[0][0] + Jacobian_order_set[1][1] + Jacobian_order_set[2][2];
//		order_set[1] = Jacobian_order_set[0][0] + Jacobian_order_set[1][2] + Jacobian_order_set[2][1];
//		order_set[2] = Jacobian_order_set[0][1] + Jacobian_order_set[1][0] + Jacobian_order_set[2][2];
//		order_set[3] = Jacobian_order_set[0][1] + Jacobian_order_set[1][2] + Jacobian_order_set[2][0];
//		order_set[4] = Jacobian_order_set[0][2] + Jacobian_order_set[1][2] + Jacobian_order_set[2][1];
//		order_set[5] = Jacobian_order_set[0][2] + Jacobian_order_set[1][1] + Jacobian_order_set[2][0];
//
//		return *std::max_element(order_set.begin(), order_set.end());
//	}
//	default:
//		FATAL_ERROR("dimension can't exceed 3");
//		return NULL;
//	}	
//}
//
//std::vector<Monomial> Figure::calculate_Linear_Transformation_Monomial_Set(void) const {
	//std::vector<Monomial> transformation_monomials;

	//switch (this->figure_type_) {
	//case FigureType::Line: {
	//	const size_t num_transformation_monomial = this->figure_order_ + 1;
	//	transformation_monomials.reserve(num_transformation_monomial);

	//	for (size_t a = 0; a <= this->figure_order_; ++a)
	//		transformation_monomials.emplace_back(Monomial{ a });

	//	return transformation_monomials;	// 1 r r^2 ...
	//}
	//case FigureType::Triangle: 
	//case FigureType::Quadrilateral: {
	//	const size_t num_transformation_monomial = static_cast<size_t>((this->figure_order_ + 2) * (this->figure_order_ + 1) * 0.5);
	//	transformation_monomials.reserve(num_transformation_monomial);

	//	for (size_t a = 0; a <= this->figure_order_; ++a)
	//		for (size_t b = 0; b <= a; ++b)
	//			transformation_monomials.emplace_back(a - b, b);

	//	return transformation_monomials;	// 1 r s r^2 rs s^2 ...
	//}
	//default:
	//	throw std::runtime_error("wrong figure type");
	//	return transformation_monomials;
	//}
//}
//
//std::vector<Monomial> Figure::calculate_Transformation_Monomial_Set(void) const {
//	std::vector<Monomial> transformation_monomials;
//
	//switch (this->figure_type_) {
	//case FigureType::Line: {
	//	const size_t num_transformation_monomial = this->figure_order_ + 1;
	//	transformation_monomials.reserve(num_transformation_monomial);

	//	for (size_t a = 0; a <= this->figure_order_; ++a)
	//		transformation_monomials.emplace_back(Monomial{ a });

	//	return transformation_monomials;	// 1 r r^2 ...
	//}
	//case FigureType::Triangle: {
	//	const size_t num_transformation_monomial = static_cast<size_t>((this->figure_order_ + 2) * (this->figure_order_ + 1) * 0.5);
	//	transformation_monomials.reserve(num_transformation_monomial);

	//	for (size_t a = 0; a <= this->figure_order_; ++a)
	//		for (size_t b = 0; b <= a; ++b)
	//			transformation_monomials.emplace_back(a - b, b);

	//	return transformation_monomials;	// 1 r s r^2 rs s^2 ...
	//}
	//case FigureType::Quadrilateral: {
	//	const size_t num_transformation_monomial = static_cast<size_t>((this->figure_order_ + 1) * (this->figure_order_ + 1));
	//	transformation_monomials.reserve(num_transformation_monomial);

	//	for (size_t a = 0; a <= this->figure_order_; ++a) {
	//		for (size_t b = 0; b <= a; ++b)
	//			transformation_monomials.emplace_back(a, b);

	//		if (a == 0)
	//			continue;

	//		size_t index = a;
	//		while (true) {
	//			transformation_monomials.emplace_back(--index, a);
	//			if (index == 0)
	//				break;
	//		}
	//	}

	//	return transformation_monomials;	// 1 r rs s r^2 r^2s r^2s^2 rs^2 s^2...
	//}
	//default:
	//	throw std::runtime_error("wrong figure type");
	//	return transformation_monomials;
	//}
//}
//
//double Figure::calculate_Transformation_Scale(const MathVector& position_vector) const {
//	switch (this->figure_type_) {
//	case FigureType::Line: {
//		const auto Jacobian_matrix = this->transformation_Jacobian_matrix_(position_vector);
//		return Math::L2_Norm(Jacobian_matrix);
//	}
//	case FigureType::Triangle:
//	case FigureType::Quadrilateral: {
//		const auto Jacobian_matrix = this->transformation_Jacobian_matrix_(position_vector);
//		const auto cofactor_matrix = Jacobian_matrix.cofactor_matrix();
//
//		//debug
//		//std::cout << position_vector << "\n";
//		////std::cout << "Jacobian Matrix : " << Jacobian_matrix << "\n";
//		//std::cout << "cofactor_matrix : " << cofactor_matrix << "\n";
//		////std::cout << "L2 norm of cofactor matrix : " << Editor::to_String(Math::L2_Norm(cofactor_matrix)) << "\n";
//		//debug
//
//		return Math::L2_Norm(cofactor_matrix);
//	}
//	default:
//		throw std::runtime_error("wrong figure type");
//		return NULL;
//	}
//}
//
//RowMajorMatrix Figure::calculate_Transformation_Normal_Matrix(const MathVector& position_vector) const {
//	switch (this->figure_type_) {
//	case FigureType::Line: {
//		constexpr size_t restricted_dimension = 2;
//		const auto Jacobian_matrix = this->transformation_Jacobian_matrix_(position_vector);
//		const auto Jacobian_sub_matrix = Jacobian_matrix.part(restricted_dimension);
//		return Jacobian_sub_matrix.cofactor_matrix();
//	}
//	case FigureType::Triangle:
//	case FigureType::Quadrilateral: {
//		const auto Jacobian_matrix = this->transformation_Jacobian_matrix_(position_vector);
//		return Jacobian_matrix.cofactor_matrix();
//	}
//	default:
//		throw std::runtime_error("wrong figure type");
//		return RowMajorMatrix();
//	}
//}
//
//bool Figure::is_Simplex(void) const {
//	if (this->figure_type_ == FigureType::Triangle) // (figure_type == ReferenceFigureType::Triangle ||figure_type == ReferenceFigureType::Tetrahedral)
//		return true;
//	else
//		return false;
//}
//
//FigureType Figure::name_to_Figure_Type(const std::string& figure_type_name) const {
//	if (ms::is_same_without_Case_Sensitivity(figure_type_name, "Point"))
//		return FigureType::Point;
//	else if (ms::is_same_without_Case_Sensitivity(figure_type_name, "Line"))
//		return FigureType::Line;
//	else if (ms::is_same_without_Case_Sensitivity(figure_type_name, "Triangle"))
//		return FigureType::Triangle;
//	else if (ms::is_same_without_Case_Sensitivity(figure_type_name, "Quadrilateral"))
//		return FigureType::Quadrilateral;
//	else {
//		throw std::runtime_error("wrong figure type");
//		return FigureType::NotInList;
//	}
//}
//
//IndexedFigure IndexedFigure::build_Face_Figure(const size_t face_index) const {
//	const auto face_index_to_figure_type = ReferenceFigure::calculate_Face_index_to_Figure_Type_Set(this->figure_type_);
//
//	const auto face_figure_type = face_index_to_figure_type.at(face_index);
//	const auto face_figure_order = this->figure_order_;
//	
//	const auto face_index_to_node_index_order_set = ReferenceFigure::calculate_Face_index_to_Node_Index_Order_Set(this->figure_type_);
//	const auto& face_node_index_order_set = face_index_to_node_index_order_set.at(face_index);
//	const auto num_node = face_node_index_order_set.size();
//
//	std::vector<const MathVector*> face_node_set;
//	face_node_set.reserve(num_node);
//
//	std::vector<size_t> face_node_index_set;
//	face_node_index_set.reserve(num_node);
//
//	for (const auto& node_index_order : face_node_index_order_set) {
//		face_node_set.emplace_back(this->node_set_[node_index_order]);
//		face_node_index_set.emplace_back(this->node_index_set_[node_index_order]);
//	}
//
//	return IndexedFigure(face_figure_type, face_figure_order, std::move(face_node_set), std::move(face_node_index_set));
//}
//
//std::map<size_t, std::vector<size_t>> IndexedFigure::calculate_Face_Index_to_Node_Index_Set(void) const {
//	const auto face_index_to_node_index_order_set = ReferenceFigure::calculate_Face_index_to_Node_Index_Order_Set(this->figure_type_);
//	const auto num_face = face_index_to_node_index_order_set.size();
//
//	std::map<size_t, std::vector<size_t>> face_index_to_node_index_set;
//	for (const auto& [face_index, face_node_index_order_set] : face_index_to_node_index_order_set) {
//		const auto num_node = face_node_index_order_set.size();
//		std::vector<size_t> face_node_index_set(num_node);
//		for (size_t i = 0; i < num_node; ++i)
//			face_node_index_set[i] = this->node_index_set_[face_node_index_order_set[i]];
//
//		face_index_to_node_index_set.emplace(face_index, std::move(face_node_index_set));
//	}
//
//	return face_index_to_node_index_set;
//}
//
//std::vector<size_t> IndexedFigure::calculate_Vertex_Node_Index_Set(void) const{
//	const auto vertex_node_index_order_set = ReferenceFigure::calculate_Vertex_Node_Index_Order_Set(this->figure_type_);
//	const auto num_vertex = vertex_node_index_order_set.size();
//
//	std::vector<size_t> vertex_node_index_set(num_vertex);
//	for (size_t i = 0; i < num_vertex; ++i)
//		vertex_node_index_set[i] = this->node_index_set_[vertex_node_index_order_set[i]];
//
//	return vertex_node_index_set;
//}
//
//std::unordered_map<size_t, std::vector<size_t>> IndexedFigure::calculate_Vertex_Node_Index_To_Vertex_Simplex_Element_Consisting_Node_Index(void) const{
//	const auto vertex_node_index_set = this->calculate_Vertex_Node_Index_Set();
//	const auto num_vertex = vertex_node_index_set.size();
//	
//	const auto vertex_simplex_element_consisting_node_index_order_family = ReferenceFigure::calculate_Vertex_Simplex_Element_Consisting_Node_Index_Order_Family(this->figure_type_);
//	
//	std::unordered_map<size_t, std::vector<size_t>> calculate_vertex_node_index_to_vertex_simplex_element_consisting_node_index;
//	calculate_vertex_node_index_to_vertex_simplex_element_consisting_node_index.reserve(num_vertex);
//	for (size_t i = 0; i < num_vertex; ++i)	{
//		const auto vertex_node_index = vertex_node_index_set[i];
//		const auto num_consisting_node = vertex_simplex_element_consisting_node_index_order_family[i].size();
//
//		std::vector<size_t> simplex_element_consisting_node_index(num_consisting_node);
//		for (size_t j = 0; j < num_consisting_node; ++j)
//			simplex_element_consisting_node_index[j] = vertex_node_index_set[vertex_simplex_element_consisting_node_index_order_family[i][j]];
//
//		calculate_vertex_node_index_to_vertex_simplex_element_consisting_node_index.emplace(vertex_node_index, std::move(simplex_element_consisting_node_index));
//	}
//
//	return calculate_vertex_node_index_to_vertex_simplex_element_consisting_node_index;
//}
//
//FigureFaceType IndexedFigure::examine_Figure_Face_Type(const IndexedFigure& face_figure) const {
//	const auto face_index_to_node_index_set = this->calculate_Face_Index_to_Node_Index_Set();
//	
//	for (const auto& [face_index, face_node_index_set] : face_index_to_node_index_set) {
//		if (!std::is_permutation(face_node_index_set.begin(), face_node_index_set.end(), face_figure.node_index_set_.begin(),face_figure.node_index_set_.end()))
//			continue;
//		else if (face_node_index_set == face_figure.node_index_set_)
//			return FigureFaceType::InwardFace;
//		else
//			return FigureFaceType::OutwardFace;
//	}
//
//	return FigureFaceType::NotMyFace;
//}
//
//
//namespace Math {
//	std::vector<Polynomial> calculate_Initial_Basis_Function_Set(const Figure& figure, const size_t order) {
		//const auto figure_dimension = figure.dimension();

		//const auto num_basis = Math::combination_with_repetition(1 + figure_dimension, order);
		//std::vector<Polynomial> initial_basis_set;
		//initial_basis_set.reserve(num_basis);

		//if (figure_dimension == 2) {
		//	const auto& linear_transformation_function = figure.linear_transformation_function();
		//	const auto& x0 = linear_transformation_function[0];
		//	const auto& x1 = linear_transformation_function[1];

		//	for (size_t a = 0; a <= order; ++a) {
		//		for (size_t b = 0; b <= a; ++b) {
		//			Polynomial initial_basis(1);

		//			for (size_t count = 0; count < a - b; ++count)
		//				initial_basis *= x0;

		//			for (size_t count = 0; count < b; ++count)
		//				initial_basis *= x1;

		//			initial_basis_set.emplace_back(std::move(initial_basis));
		//		}
		//	}
		//}

		//const auto figure_dimension = figure.dimension();

		//const auto num_basis = Math::combination_with_repetition(1 + figure_dimension, order);
		//std::vector<Polynomial> initial_basis_set;
		//initial_basis_set.reserve(num_basis);

		//if (figure_dimension == 2) {
		//	// 1 x y x^2 xy y^2
		//	for (size_t a = 0; a <= order; ++a)
		//		for (size_t b = 0; b <= a; ++b)
		//			initial_basis_set.emplace_back(Monomial{ a - b, b });
		//}


		//const auto figure_dimension = figure.dimension();
		//const auto center_node = figure.calculate_Center_Node();

		//const auto num_basis = Math::combination_with_repetition(1 + figure_dimension, order);
		//std::vector<Polynomial> initial_basis_set;
		//initial_basis_set.reserve(num_basis);

		//if (figure_dimension == 2) {
		//	 1 (x - x_c) ( y - y_c )  ...
		//	for (size_t a = 0; a <= order; ++a)
		//		for (size_t b = 0; b <= a; ++b)
		//			initial_basis_set.emplace_back(Monomial{ a - b, b });

		//	for (auto& basis : initial_basis_set) 
		//		Math::translate(basis, center_node);
		//	
		//}
		//else
		//	throw std::runtime_error("wrong figure type");

		//return initial_basis_set;
//	}
//
//	std::vector<Polynomial> calculate_Orthonormal_Basis_Function_Set(const Figure& figure, const size_t polynomial_order) {
//		const auto initial_basis_function_set = Math::calculate_Initial_Basis_Function_Set(figure, polynomial_order);
//		return Math::Gram_Schmidt_Process(initial_basis_function_set, figure);
//	}
//
	//double integrate(const Polynomial& integrand, const QuadratureRule& quadrature_rule) {
	//	const auto& QP_set = quadrature_rule.quadrature_point_set;
	//	const auto& QW_set = quadrature_rule.quadrature_weight_set;

	//	////debug
	//	//std::cout << std::setprecision(15);
	//	////debug

	//	double result = 0.0;		
	//	for (size_t i = 0; i < QP_set.size(); ++i) {


	//		result += integrand(QP_set[i]) * QW_set[i];
	//		////debug			
	//		//std::cout << "QP : " << QP_set[i] << "\n";
	//		//std::cout << "QW : " << QW_set[i] << "\n";
	//		//std::cout << "f(QP) : " << integrand(QP_set[i]) << "\n";
	//		//std::cout << "f(QP)*QW : " << integrand(QP_set[i]) * QW_set[i] << "\n";
	//		////debug
	//	}

	//	return result;
	//}

	//double integrate(const Polynomial& integrand, const Figure& figure) {
	//	const auto quadrature_rule = figure.calculate_Quadrature_Rule(integrand.order());
	//	return Math::integrate(integrand, quadrature_rule);
	//}

	//double inner_product(const Polynomial& f1, const Polynomial& f2, const QuadratureRule& quadrature_rule) {
	//	return Math::integrate(f1 * f2, quadrature_rule);
	//}

	//double inner_product(const Polynomial& f1, const Polynomial& f2, const Figure& geometry) {
	//	const auto quadrature_rule = geometry.calculate_Quadrature_Rule(f1.order() + f2.order());
	//	return Math::inner_product(f1, f2, quadrature_rule);
	//}

	//double L2_Norm(const Polynomial& function, const QuadratureRule& quadrature_rule) {
	//	return std::sqrt(Math::inner_product(function, function, quadrature_rule));
	//}

	//double L2_Norm(const Polynomial& polynomial, const Figure& geometry) {
	//	const auto quadrature_rule = geometry.calculate_Quadrature_Rule(polynomial.order() * 2);
	//	return Math::L2_Norm(polynomial, quadrature_rule);
	//}
//
//	std::vector<Polynomial> Gram_Schmidt_Process(const std::vector<Polynomial>& initial_polynomial_set, const QuadratureRule& quadrature_rule) {
//		auto normalized_polynomial_set = initial_polynomial_set;
//
//		//std::cout << "check orthogonality! \n";
//		std::cout << std::setprecision(15);
//		for (size_t i = 0; i < initial_polynomial_set.size(); ++i) {
//			for (size_t j = 0; j < i; ++j) {
//				////debug
//				//auto inner_product = Math::inner_product(normalized_polynomial_set[i], normalized_polynomial_set[j], quadrature_rule);
//				//std::cout << i << j << " inner product : " << inner_product << "\n";
//				////debug
//
//				normalized_polynomial_set[i] -= Math::inner_product(normalized_polynomial_set[i], normalized_polynomial_set[j], quadrature_rule) * normalized_polynomial_set[j];
//			}
//			//debug			
//			//for (size_t j = 0; j < i; ++j)
//			//	std::cout << i << "," << j << " : " << Math::inner_product(normalized_polynomial_set[i], normalized_polynomial_set[j], quadrature_rule) << "\n";
//			std::cout << i << " orthogonalized polynomial : " << normalized_polynomial_set[i] << "\n";
//
//
//			//std::cout << i << " basis L2_Norm Value\t1/L2 norm value : ";
//			//std::cout  << "\t" << 1.0 / Math::L2_Norm(normalized_polynomial_set[i], quadrature_rule) << "\n";
//
//			std::cout << i << " basis L2_Norm Value : " << Math::L2_Norm(normalized_polynomial_set[i], quadrature_rule) << "\n";
//			////debug
//
//			normalized_polynomial_set[i] *= 1.0 / Math::L2_Norm(normalized_polynomial_set[i], quadrature_rule);
//			////debug
//			//std::cout << i << " nomalzied polynomial : " << normalized_polynomial_set[i] << "\n";
//			////debug
//		}
//
//		//debug
//		for (size_t i = 0; i < initial_polynomial_set.size(); ++i)
//			std::cout << i << " nomalzied polynomial : " << normalized_polynomial_set[i] << "\n";
//
//		//std::cout << "check orthonormality! \n";
//		//for (size_t i = 0; i < normalized_polynomial_set.size(); ++i)
//		//	for (size_t j = 0; j < normalized_polynomial_set.size(); ++j)
//		//		std::cout << i << "," << j << " : " << Math::inner_product(normalized_polynomial_set[i], normalized_polynomial_set[j], quadrature_rule) << "\n";
//
//		//std::cout << "4 basis * 5 basis : " << normalized_polynomial_set[4] * normalized_polynomial_set[5] << "\n";
//		//std::cout << "5 basis * 4 basis : " << normalized_polynomial_set[5] * normalized_polynomial_set[4] << "\n";
//		//debug
//
//
//		return normalized_polynomial_set;
//	}
//
//	std::vector<Polynomial> Gram_Schmidt_Process(const std::vector<Polynomial>& initial_polynomial_set, const Figure& geometry) {
//		
	//	auto normalized_polynomial_set = initial_polynomial_set;

	//	for (size_t i = 0; i < initial_polynomial_set.size(); ++i) {
	//		//for (size_t j = 0; j < i; ++j)
	//		//	normalized_polynomial_set[i] -= Math::inner_product(normalized_polynomial_set[i], normalized_polynomial_set[j], geometry) * normalized_polynomial_set[j];

	//		//debug
	//		std::cout << std::setprecision(16);
	//		for (size_t j = 0; j < i; ++j) {
	//			auto inner_product = Math::inner_product(normalized_polynomial_set[i], normalized_polynomial_set[j], geometry);
	//			normalized_polynomial_set[i] -= inner_product * normalized_polynomial_set[j];
	//			std::cout << i << j << " inner product : " << inner_product << "\n";
	//			//std::cout << "orthogonalized basis : " << normalized_polynomial_set[i] <<"\n";
	//		}


	//		//std::cout << "\nbefore normalize orthogonality check\n";
	//		//for (size_t j = 0; j < i; ++j)
	//		//	std::cout << i << "," << j << " : " << Math::inner_product(normalized_polynomial_set[i], normalized_polynomial_set[j], geometry) << "\n";


	//		//auto debug = normalized_polynomial_set[i];
	//		//std::cout << "befor normalized " << i << "basis : " << normalized_polynomial_set[i] << "\n";
	//		
	//		//std::cout << "1/L2_Norm Value : " << 1.0 / Math::L2_Norm(normalized_polynomial_set[i], geometry) << "\n";
	//		//std::cout << "Normalized basis : " << (debug *= 1.0 / Math::L2_Norm(normalized_polynomial_set[i], geometry)) << "\n";

	//		//std::cout << "\nafter normalize orthogonality check\n";
	//		//for (size_t j = 0; j < i; ++j)
	//		//	std::cout << i << "," << j << " : " << Math::inner_product(debug, normalized_polynomial_set[j], geometry) << "\n";
	//		//debug

	//		normalized_polynomial_set[i] *= 1.0 / Math::L2_Norm(normalized_polynomial_set[i], geometry);
	//		//std::cout << i << " orthonormalized basis : " << normalized_polynomial_set[i] << "\n";
	//	}

	//	return normalized_polynomial_set;

	//	//std::vector<size_t> order_set;
	//	//order_set.reserve(initial_polynomial_set.size());

	//	//for (const auto& polynomial : initial_polynomial_set)
	//	//	order_set.emplace_back(polynomial.order());

	//	//const auto maximum_order = *std::max_element(order_set.begin(), order_set.end());
	//	//const auto integrand_order = maximum_order * 2;

	//	//const auto quadrature_rule = geometry.calculate_Quadrature_Rule(integrand_order);
	//	//return Math::Gram_Schmidt_Process(initial_polynomial_set, quadrature_rule);
	//}
//}
//
//
