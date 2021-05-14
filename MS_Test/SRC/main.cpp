
//#include "../INC/Figure.h"
//#include "../INC/MathVector.h"
#include <iostream>
#include <vector>

int main(void)
{
	std::vector<double> v1 = { 1,2,3,4 };

	//const FigureType figure_type = FigureType::Quadrilateral;
	//const size_t figure_order = 1;

	//MathVector p1 = { 1,2,0 };
	//MathVector p2 = { 3,1,0 };
	//MathVector p3 = { 4,1,0 };
	//MathVector p4 = { 1,3,0 };
	//std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };

	//const size_t polynomial_order = 4;
	//Figure fig(figure_type, figure_order, std::move(pv));
	//const auto orthonormal_basis = fig.calculate_orthonormal_basis_vector(polynomial_order);
	//const auto num_basis = orthonormal_basis.size();

	//for (size_t i = 0; i < num_basis; ++i) {
	//	for (size_t j = 0; j < num_basis; ++j) {
	//		const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], fig);
	//		std::cout << i << j << "\t" << result << "\n";
	//	}
	//}
}
