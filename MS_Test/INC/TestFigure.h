#pragma once

#include "Test.h"
#include "Figure.h"

void Test_Quad_QR(void) {
	const size_t polynomial_order = 4;

	const MathVector p0 = { 0.9481103358335534, 0.0453515835831667, 0 };
	const MathVector p1 = { 0.9702908506370765, 0.0536798988853734, 0 };
	const MathVector p2 = { 0.9719959994438482, 0.07742531579473901, 0 };
	const MathVector p3 = { 0.9504868794837469, 0.07036958152571295, 0 };

	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	std::vector<const MathVector*> point_set = { &p0,&p1,&p2,&p3 };
	Figure quadrilateral(figure_type, figure_order, std::move(point_set));

	const auto quadrature_rule = quadrilateral.calculate_Quadrature_Rule(polynomial_order);

	const auto num_QP = 16;
	std::cout << std::setprecision(20);
	for (size_t i = 0; i < num_QP; ++i)
		std::cout << quadrature_rule.quadrature_point_set[i] << quadrature_rule.quadrature_weight_set[i] << "\n";
}

void Test_Quad_QW(const size_t polynomial_order){
	Test_OBJ("quadrilateral QW of P" + Editor::to_String(polynomial_order));
	const MathVector p0 = { 0.3635520579711813,		0.2973431147402148,		0 };
	const MathVector p1 = { 0.3512301560533574,		0.3184608229801218,		0 };
	const MathVector p2 = { 0.3309655464243111,		0.3010404355350647,		0 };
	const MathVector p3 = { 0.3359655464243111,		0.2910404355350647,		0 };

	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	std::vector<const MathVector*> point_set = { &p0,&p1,&p2,&p3 };
	Figure quadrilateral(figure_type, figure_order, std::move(point_set));

	const auto quadrature_rule = quadrilateral.calculate_Quadrature_Rule(polynomial_order);

	double sum = 0.0;
	for (const auto& weight : quadrature_rule.quadrature_weight_set)
		sum += weight;
	
	constexpr double reference = 4.749864653523731e-04;
	const double epsilon = 1.0E-12;
	const auto result = std::abs(reference - sum);

	if (Test_less_then(result, epsilon)) {
		std::cout << "test success\n";
		std::cout << "area diff : " << result << "\n";
	}
	else {
		std::cout << "test fail!\n";
		std::cout << "area diff : " << result << "\n";
	}
}

void Test_Quad_QW2(const size_t polynomial_order) {
	Test_OBJ("quadrilateral QW of P" + Editor::to_String(polynomial_order));
	const MathVector p0 = { 0.9481103358335534, 0.0453515835831667, 0 };
	const MathVector p1 = { 0.9702908506370765, 0.0536798988853734, 0 };
	const MathVector p2 = { 0.9719959994438482, 0.07742531579473901, 0 };
	const MathVector p3 = { 0.9504868794837469, 0.07036958152571295, 0 };

	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	std::vector<const MathVector*> point_set = { &p0,&p1,&p2,&p3 };
	Figure quadrilateral(figure_type, figure_order, std::move(point_set));

	const auto quadrature_rule = quadrilateral.calculate_Quadrature_Rule(polynomial_order);

	double sum = 0.0;
	for (const auto& weight : quadrature_rule.quadrature_weight_set)
		sum += weight;

	constexpr double reference = 5.169157064036623e-04;
	const double epsilon = 1.0E-12;
	const auto result = std::abs(reference - sum);

	if (Test_less_then(result, epsilon)) {
		std::cout << "test success\n";
		std::cout << "area diff : " << result << "\n";
	}
	else {
		std::cout << "test fail!\n";
		std::cout << "area diff : " << result << "\n";
	}
}

void Test_Quad_Transformation_Function(void) {
	Test_OBJ("quadrilateral Transfomration Function");

	const MathVector p0 = { 0.3635520579711813,		0.2973431147402148,		0 };
	const MathVector p1 = { 0.3512301560533574,		0.3184608229801218,		0 };
	const MathVector p2 = { 0.3309655464243111,		0.3010404355350647,		0 };
	const MathVector p3 = { 0.3359655464243111,		0.2910404355350647,		0 };


	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	std::vector<const MathVector*> point_set = { &p0,&p1,&p2,&p3 };
	Figure quadrilateral(figure_type, figure_order, std::move(point_set));

	std::vector<MathVector> reference_point_set = { {-1,-1,0},{1,-1,0},{1,1,0},{-1,1,0} };

	const std::vector<MathVector> reference = { p0,p1,p2,p3 };
	const auto calculated = quadrilateral.calculate_Transformed_Point_Set(reference_point_set);

	std::cout << std::setprecision(16);
	for (size_t i = 0; i < 4; ++i) {
		std::cout << "Reference :\t";
		for (size_t j = 0; j < 3; ++j)
			std::cout << reference[i][j] << "\t";
		std::cout << "\n";

		std::cout << "calculated :\t";
		for (size_t j = 0; j < 3; ++j)
			std::cout << calculated[i][j] << "\t";
		std::cout << "\n\n";		

	}
}

void Test_Quad_Orthonormality(void) {
	Test_OBJ("calculate orthonormal basis on quadrilateral");

	const MathVector p0 = { 0.3635520579711813,		0.2973431147402148,		0 };
	const MathVector p1 = { 0.3512301560533574,		0.3184608229801218,		0 };
	const MathVector p2 = { 0.3309655464243111,		0.3010404355350647,		0 };
	const MathVector p3 = { 0.3359655464243111,		0.2910404355350647,		0 };

	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	std::vector<const MathVector*> point_set = { &p0,&p1,&p2,&p3 };
	Figure quadrilateral(figure_type, figure_order, std::move(point_set));
	const size_t solution_order = 2;

	const auto orthonormal_basis_function_set = Math::calculate_Orthonormal_Basis_Function_Set(quadrilateral, solution_order);

	const auto num_basis_function = orthonormal_basis_function_set.size();
	for (size_t i = 0; i < num_basis_function; ++i) {
		for (size_t j = 0; j < num_basis_function; ++j) {
			const auto& ibasis = orthonormal_basis_function_set[i];
			const auto& jbasis = orthonormal_basis_function_set[j];

			std::cout << i << "," << j << " basis inner product result : " << Math::inner_product(ibasis, jbasis, quadrilateral) << "\n";
		}
	}
}

void Test_Quad_Orthonormality2(void) {
	Test_OBJ("calculate orthonormal basis on quadrilateral");

	const MathVector p0 = { 0.9481103358335534, 0.0453515835831667, 0 };
	const MathVector p1 = { 0.9702908506370765, 0.0536798988853734, 0 };
	const MathVector p2 = { 0.9719959994438482, 0.07742531579473901, 0 };
	const MathVector p3 = { 0.9504868794837469, 0.07036958152571295, 0 };

	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	std::vector<const MathVector*> point_set = { &p0,&p1,&p2,&p3 };
	Figure quadrilateral(figure_type, figure_order, std::move(point_set));
	const size_t solution_order = 2;

	const auto orthonormal_basis_function_set = Math::calculate_Orthonormal_Basis_Function_Set(quadrilateral, solution_order);

	const auto num_basis_function = orthonormal_basis_function_set.size();
	for (size_t i = 0; i < num_basis_function; ++i) {
		for (size_t j = 0; j < num_basis_function; ++j) {
			const auto& ibasis = orthonormal_basis_function_set[i];
			const auto& jbasis = orthonormal_basis_function_set[j];
			
			std::cout << i << "," << j << " basis inner product result : " << Math::inner_product(ibasis, jbasis, quadrilateral) << "\n";
		}
	}
}

void Test_Quad_Orthonormality3(const size_t integrand_order) {
	Test_OBJ("calculate orthonormal basis on quadrilateral");

	//const MathVector p0 = { 0.9481103358335534, 0.0453515835831667, 0 };
	//const MathVector p1 = { 0.9702908506370765, 0.0536798988853734, 0 };
	//const MathVector p2 = { 0.9719959994438482, 0.07742531579473901, 0 };
	//const MathVector p3 = { 0.9504868794837469, 0.07036958152571295, 0 };

	MathVector p0 = { -1,-1,0 };
	MathVector p1 = { 1,-1,0 };
	MathVector p2 = { 1,1,0 };
	MathVector p3 = { -1,1,0 };

	// translate
	MathVector trans = { 100,100,0 };
	p0 += trans;
	p1 += trans;
	p2 += trans;
	p3 += trans;

	//// scaling
	//double scale_factor = 0.00001;
	//p0 *= scale_factor;
	//p1 *= scale_factor;
	//p2 *= scale_factor;
	//p3 *= scale_factor;

	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;
	std::vector<const MathVector*> point_set = { &p0,&p1,&p2,&p3 };
	Figure quadrilateral(figure_type, figure_order, std::move(point_set));
	const size_t solution_order = 2;

	const auto quadrature_rule = quadrilateral.calculate_Quadrature_Rule(integrand_order);
	
	////debug
	std::cout << std::setprecision(15);

	auto val = 0.009;
	auto exact = 1.0e-6;
	auto expand = val * val - 2 *1.0e-2 * val + 1.0e-4;
	auto compact = std::pow(val - 1.0e-2, 2);


	//auto val = 0.9;
	//auto exact = 0.01;
	//auto expand = val * val - 2.0 * val + 1.0;
	//auto compact = std::pow(val - 1, 2);

	//auto val = 9.9;
	//auto exact = 0.01;
	//auto expand = val * val - 20.0 * val + 100.0;
	//auto compact = std::pow(val - 10, 2);

	//auto val = 99.9;
	//auto exact = 0.01;
	//auto expand = val * val - 200 * val + 10000;
	//auto compact = std::pow(val - 100, 2);

	//auto val = 99999.9;
	//auto exact = 0.01;
	//auto expand = val * val - 2 * 1.0e5 * val + 1.0e10;
	//auto compact = std::pow(val - 1.0e5, 2);
	
	std::cout << expand<<"\n";
	std::cout << compact<<"\n";
	std::cout << expand - exact << "\n";
	std::cout << compact - exact << "\n";

	// 왜 전개한게 수치 오차가 클까 ?


	//const auto num_quad = quadrature_rule.quadrature_point_set.size();	
	//for (size_t i = 0; i < num_quad; ++i)
	//	std::cout << quadrature_rule.quadrature_point_set[i] << "\t" << quadrature_rule.quadrature_weight_set[i] << "\n";

	
	std::vector<double> coefficient_set = { 1.0,-100.0 };
	std::vector<Monomial> monomial_set = { Monomial{1},Monomial{0} };
	Polynomial poly(coefficient_set, monomial_set);

	auto poly2 = poly * poly;
	std::cout << poly2 << "\n";

	//MathVector qp1 = { 99.1388636884059, 99.1388636884059,0 };
	//auto val = qp1[0] - 100;
	//std::cout << qp1[0] * qp1[0] - 200 * qp1[0] + 10000 << "\n";
	//std::cout << std::pow(val, 2)<<"\n";
	//std::cout << poly2(qp1) << "\n";

	auto inner_prod = Math::integrate(poly2, quadrature_rule);
	auto l2_norm = std::sqrt(inner_prod);
	std::cout << inner_prod  << "\n";
	std::cout << l2_norm  << "\n";
	////debug

	const auto initial_basis_function_set = Math::calculate_Initial_Basis_Function_Set(quadrilateral, solution_order);

	//for (const auto& basis : initial_basis_function_set)
	//	std::cout << basis << "\n";


	const auto num_basis_function = initial_basis_function_set.size();
	const auto orthonormal_basis_function_set = Math::Gram_Schmidt_Process(initial_basis_function_set, quadrature_rule);
	

	std::cout << "check orthonormality! \n";
	std::cout << std::setprecision(20);
	for (size_t i = 0; i < num_basis_function; ++i) {
		for (size_t j = 0; j < num_basis_function; ++j) {
			const auto& ibasis = orthonormal_basis_function_set[i];
			const auto& jbasis = orthonormal_basis_function_set[j];

			std::cout << i << "," << j << " : " << Math::inner_product(ibasis, jbasis, quadrature_rule) << "\n";
		}
	}
}