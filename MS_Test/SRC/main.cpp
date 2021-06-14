#include "../INC/Figure.h"
#include "../INC/Profiler.h"
#include <iostream>
int main(void)
{
	const FigureType figure_type = FigureType::Quadrilateral;
	const size_t figure_order = 1;

	for (size_t j = 0; j < 1; ++j) {
		RECORD_CONSUMED_TIME;
		for (size_t i = 0; i < 1; ++i) {
			const MathVector delta = { 1.0E-10 * i,1.0E-10 * i ,0 };

			MathVector p1 = { 0.3635520579711813,		0.2973431147402148,		0 };
			MathVector p2 = { 0.3512301560533574,		0.3184608229801218,		0 };
			MathVector p3 = { 0.3309655464243111,		0.3010404355350647,		0 };
			MathVector p4 = { 0.3359655464243111,		0.2910404355350647,		0 };

			p1 += delta;
			p2 += delta;
			p3 += delta;
			p4 += delta;

			std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

			Figure figure(figure_type, figure_order, std::move(pv));
			const auto scale_function = figure.reference_figure_.scale_function(figure.mapping_function_).value();
			std::cout << scale_function << "\n";

			const size_t polynomial_order = 5;
			const auto initial_basis = figure.initial_basis_vector(polynomial_order);
			const auto orthonormal_basis = ms::Gram_Schmidt_Process(initial_basis, figure);



			size_t num_term = 0;
			for (size_t i = 0; i < orthonormal_basis.size(); ++i)
				for (size_t j = 0; j <= i; ++j)
					num_term += (orthonormal_basis[i] * orthonormal_basis[j]).added_poly_term_set_.size();

			std::cout << num_term << "\n";

			//for (const auto& basis1 : initial_basis)
			//	std::cout << basis1 << "\n";

			//for (const auto& basis2 : orthonormal_basis)
			//	std::cout << basis2 << "\n";
			//std::cout << basis2.added_poly_term_set_.size() << "\n";

			//double max_error = 0.0;
			//for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
			//	for (size_t j = 0; j <= i; ++j) {
			//		const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], figure);

			//		if (i == j)
			//			max_error = max(max_error, std::abs(1 - result));
			//		else
			//			max_error = max(max_error, std::abs(result));
			//	}
			//}

			//std::cout << max_error << "\n";
		}
		PRINT_CONSUMED_TIME_NANO;
	}
}
