#include <array>
#include <iostream>
#include <type_traits>

#include <algorithm>
#include <vector>

#include "../MS_Test/INC/Profiler.h"


class A
{
public:
	template <typename... Args>
	A(Args... args) { std::cout << "constuctor\n"; };
	A(const A& a) { std::cout << "copy\n"; }
};

int main(void) {

	RECORD_CONSUMED_TIME;
	for (size_t i = 0; i < 10000000; ++i) {
		std::array<double, 300> ar1 = { 1 + i * 1.0E-15,5,3 };
		std::sort(ar1.begin(), ar1.end());
	}
	PRINT_CONSUMED_TIME;

	std::array<double, 300> ar2 = { 1,5,3 };
	RECORD_CONSUMED_TIME;
	for (size_t i = 0; i < 10000000; ++i) {		
		if (!std::is_sorted(ar2.begin(),ar2.end()))
			std::sort(ar2.begin(), ar2.end());
	}
	PRINT_CONSUMED_TIME;
		
	RECORD_CONSUMED_TIME;
	for (size_t i = 0; i < 10000000; ++i) {
		std::array<double, 3> ar3 = { 1,5,3 };
		std::sort(ar3.begin(), ar3.end());
	}
	PRINT_CONSUMED_TIME;

	RECORD_CONSUMED_TIME;
	for (size_t i = 0; i < 10000000; ++i) {
		std::vector<double> ar4;
		std::sort(ar4.begin(), ar4.end());
	}
	PRINT_CONSUMED_TIME;

	RECORD_CONSUMED_TIME;
	for (size_t i = 0; i < 10000000; ++i) {
		std::vector<double> ar5;
		if (!ar5.empty())
			std::sort(ar5.begin(), ar5.end());
	}
	PRINT_CONSUMED_TIME;

}

//template <size_t DomainDim>
//class A {
//public:
//	A() {
//		val_.fill(DomainDim);
//		for (const auto& elem : val_)
//			std::cout << elem << "\n";
//	}
//
//private:
//	std::array<double, DomainDim> val_;
//};
//
//template <size_t DomainDim>
//class B {
//public:
//	template <char VariableIndex>
//	B() {
//		std::cout << VariableIndex << "\n";
//	}
//};
//
//
//int main(void) {
//	B<3> b('a');
//
//
//	constexpr char ar = '0';
//
//	constexpr size_t dd1 = 2;
//	constexpr size_t dd2 = 4;
//	constexpr auto dd_max = std::max(dd1, dd2);
//
//	A<dd_max> a;
//}


////using Test1 = int*;
////using Test2 = int&; // https://stackoverflow.com/questions/26631169/why-does-sizeof-a-reference-type-give-you-the-sizeof-the-type
////using Test3 = int;
//
//#define Static_Require static_assert
////
////class B {
////	B(int val = 0) {};
////};
////
////template<size_t N>
////class A {
////public:
////	template<typename... Args>
////	A(Args... args);
////
////	A();
////
////private:
////	std::array<double, N> ar_;
////	std::array<B, N> ar2_;
////};
////
////
////template<size_t N>
////A<N>::A() {}
////
////
////template<size_t N> template<typename... Args>
////A<N>::A(Args... args) {
////	Require(sizeof...(args) <= N, "It is too big");
////
////	ar_ = { static_cast<double>(args)... };
////}
//
//
//struct Func {
//	Func(int a) {};
//};
//
//template <size_t DomainDim, size_t RangeDim> class C {
//public:
//	template <typename... Args>
//	static C<DomainDim, RangeDim> make(const Args&... args) {
//		Static_Require(sizeof...(args) == RangeDim, "num arguments should be same with dimension of range");
//		Static_Require((... && std::is_same_v<Func, Args>), "Every argument should be function");
//		return { args... };
//	}
//
//	//template <typename... Args,
//	//	std::enable_if_t<(... && std::is_same_v<Func, Args>), bool> = true>
//	//	C(const Args&... args) : arr_{args...} {};
//
//private:
//	template <typename... Args>
//	C(Args&... args) : arr_{ args... } {};
//
//private:
//	std::array<Func, RangeDim> arr_;
//};
//
////make function
//
//
//
//
//
//
////template <typename... Args,
////	std::enable_if_t<(... && std::is_same_v<Func, Args>), bool> = true>
////	void only_func(const Args&... args) { std::cout << "only func\n"; };
//
//int main(void) {
//	Func f1(1), f2(1), f3(1), f4(1);
//	
//	auto c = C<2,4>::make(f1, f2, f3, 3);	
//	//C<2, 4> c2(f1, f2, f3, f4);
//}

//#include <iostream>
//#include <array>
//#include <initializer_list>
//#include <string>
//
//template <typename T, size_t dimension>
//class Vector
//{
//protected:
//	std::array<T, dimension> elements_;
//
//public:
//	template <typename... Args>
//	Vector(Args... args) : elements_{ static_cast<T>(args)... } {};
//
//	size_t dimension(void) { return elements_.size(); };
//};
//
//template <size_t dimension>
//class EuclideanVector : public Vector<double, dimension>
//{
//public:
//	template <typename... Args>
//	EuclideanVector(Args... args) : Vector<double, sizeof...(Args)>(args...) {};
//
//	std::string to_string(void) const {
//		std::string result;
//		for (const auto element : this->elements_)
//			result += std::to_string(element) + "\t";
//		return result;
//	}
//};
//
////user-defined deduction guides
//template <typename... Args>
//EuclideanVector(Args... args) -> EuclideanVector<sizeof...(Args)>; 
//
//int main(void)
//{
//	EuclideanVector node1 = { 1 };
//	EuclideanVector node3 = { 1, 2, 3.1 };
//	EuclideanVector node4 = { 1, 2, 3, 4 };
//
//	std::cout << node3.dimension() << "\t" << node3.to_string() << "\n";
//	std::cout << node4.dimension() << "\t" << node4.to_string() << "\n";
//}


//#include "../MS_Test/INC/Profiler.h"
//#include <iostream>
//#include <vector>
//#include <mkl.h>
//int main(void)
//{
//	constexpr size_t num_iter = 5;
//	constexpr size_t num_value = 10;
//	
//	////vdadd test
//	//for (size_t n = 0; n < num_iter; ++n) {
//	//	{
//	//		std::vector<double> v1(num_value, 1);
//	//		std::vector<double> v2(num_value, 1);
//	//		std::vector<double> v3(num_value);
//	//		RECORD_CONSUMED_TIME;
//	//		for (size_t i = 0; i < num_value; ++i)
//	//				v3[i] = v1[i] + v2[i];
//	//		PRINT_CONSUMED_TIME;
//	//		std::cout << v3[num_iter] << "그냥한거\n";
//	//	}
//	//	{
//	//		std::vector<double> v1(num_value, 1);
//	//		std::vector<double> v2(num_value, 1);
//	//		std::vector<double> v3(num_value);
//	//		RECORD_CONSUMED_TIME;
//	//		const MKL_INT n = static_cast<int>(v1.size());
//	//		vdAdd(n, v1.data(), v2.data(), v3.data());
//	//		PRINT_CONSUMED_TIME;
//	//		std::cout << v3[num_iter] << "vdAdd\n";
//	//	}
//	//}
//	//cdot test
//	for (size_t n = 0; n < num_iter; ++n) {
//		{
//			std::vector<double> v1(num_value, 1);
//			std::vector<double> v2(num_value, 1);
//			double result = 0;
//			RECORD_CONSUMED_TIME;
//			for (size_t i = 0; i < num_value; ++i)
//				result += v1[i] * v2[i];
//			PRINT_CONSUMED_TIME;
//			std::cout << result << "그냥한거\n";
//		}
//		{
//			std::vector<double> v1(num_value, 1);
//			std::vector<double> v2(num_value, 1);			
//			RECORD_CONSUMED_TIME;
//			const MKL_INT n = static_cast<int>(v1.size());
//			const MKL_INT incx = 1;
//			const MKL_INT incy = 1;
//			const auto result = cblas_ddot(n, v1.data(), incx, v2.data(), incy);
//			PRINT_CONSUMED_TIME;
//			std::cout << result << "cblas_ddot\n";
//		}
//	}
//}

//
//#include "../MS_Test/INC/Figure.h"
//#include "../MS_Test/INC/Profiler.h"
//#include <iostream>
//int main(void)
//{
//	const FigureType figure_type = FigureType::Quadrilateral;
//	const size_t figure_order = 1;
//
//	for (size_t j = 0; j < 1; ++j) {
//		RECORD_CONSUMED_TIME;
//		for (size_t i = 0; i < 10; ++i) {
//			const MathVector delta = { 1.0E-10 * i,1.0E-10 * i ,0 };
//
//			MathVector p1 = { 0.3635520579711813,		0.2973431147402148,		0 };
//			MathVector p2 = { 0.3512301560533574,		0.3184608229801218,		0 };
//			MathVector p3 = { 0.3309655464243111,		0.3010404355350647,		0 };
//			MathVector p4 = { 0.3359655464243111,		0.2910404355350647,		0 };
//
//			p1 += delta;
//			p2 += delta;
//			p3 += delta;
//			p4 += delta;
//
//			std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };
//
//			Figure figure(figure_type, figure_order, std::move(pv));
//			const size_t polynomial_order = 5;
//			const auto initial_basis = figure.initial_basis_vector(polynomial_order);
//			const auto orthonormal_basis = ms::Gram_Schmidt_Process(initial_basis, figure);
//
//			//for (const auto& basis1 : initial_basis)
//			//	std::cout << basis1 << "\n";
//
//			//for (const auto& basis2 : orthonormal_basis)
//			//	std::cout << basis2 << "\n";
//			//std::cout << basis2.added_poly_term_set_.size() << "\n";
//
//			double max_error = 0.0;
//			for (size_t i = 0; i < orthonormal_basis.size(); ++i) {
//				for (size_t j = 0; j <= i; ++j) {
//					const auto result = ms::inner_product(orthonormal_basis[i], orthonormal_basis[j], figure);
//
//					if (i == j)
//						max_error = max(max_error, std::abs(1 - result));
//					else
//						max_error = max(max_error, std::abs(result));
//				}
//			}
//
//			std::cout << max_error << "\n";
//		}
//		PRINT_CONSUMED_TIME_NANO;
//	}
//}
//
////#include <utility>
////
////std::pair<int, int> func(void) {
////	return { 1,1 };
////}
////
////void func2(const int i1, const int i2) {
////}
////
////int main(void) {
////	func2(func());
////}
//
////#include "../MS_Test/INC/Text.h"
////#include <iostream>
////
////
////
////int main(void) {
////	const size_t num_val = 10000;
////	
////	//std::vector<double> exact_val;
////	//exact_val.reserve(num_val);
////
////	//const double pi = 4.0 * std::atan(1.0);
////
////	//for (size_t i = 1; i <= 100; ++i) {
////	//	for (size_t j = 1; j <= 100; ++j) {
////	//		const double x = 0.01 * i - 0.005 + 1.0E-6;
////	//		const double y = 0.01 * j - 0.005 + 1.0E-7;
////
////	//		const double val = 1.0 + 0.2 * std::sin(2.0 * pi * (x - 0.5)) * std::sin(2.0 * pi * (y - 0.5));
////	//		exact_val.push_back(val);
////	//	}
////	//}
////
////	Text ref_text;
////	ref_text.reserve(num_val);
////	ref_text.read("DenbTest/Solution_REF.txt");
////	
////	std::vector<double> ref_val;
////	ref_val.reserve(num_val);
////
////	for (const auto& sentence : ref_text) {
////		auto parsed_sentece = ms::parse(sentence, ' ');
////		parsed_sentece[2].pop_back();
////		ref_val.push_back(ms::to_value<double>(parsed_sentece[2]));
////	}
////			
////	const size_t order_limit = 2;
////	const size_t num_cell_distance = 10;
////	
////	for (size_t order = 0; order <= order_limit; ++order) {
////		for (size_t i = 1; i <= 5; ++i) {
////			const auto num_cell = i * num_cell_distance;
////			const std::string file_path = "DenbTest/";
////			const std::string file_name = "Solution_P" + std::to_string(order) + "_" + std::to_string(num_cell) + "X" + std::to_string(num_cell) + ".txt";
////	
////			Text solution_text;
////			solution_text.reserve(num_val);
////			solution_text.read(file_path + file_name);
////
////			std::vector<double> sol_val;
////			sol_val.reserve(num_val);
////
////			for (const auto& sentence : solution_text) {
////				auto parsed_sentece = ms::parse(sentence, ' ');
////				parsed_sentece[2].pop_back();
////				sol_val.push_back(ms::to_value<double>(parsed_sentece[2]));
////			}
////
////			double error = 0.0;
////			for (size_t i = 0; i < num_val; ++i)
////				error += std::pow(ref_val[i] - sol_val[i], 2);
////			error *= 1.0E-4;
////			std::cout << "File Name : " << file_name << "\t" << "Error : " << std::sqrt(error) << "\n";
////		}
////	}	
////}
//
////#include <iostream>
////
////int main(void) {
////	int i = 0;
////	for (;; i++) {
////		std::cout << i << "\n";
////		if (i == 10)
////			break;
////	}
////}
//
////#include "../MS_Test/INC/Profiler.h"
////#include "../MS_Test/INC/Figure.h"
////
////int main(void) {
////
////	const FigureType figure_type = FigureType::Quadrilateral;
////	const size_t figure_order = 1;
////
////	const MathVector p1 = { 0.3635520579711813,		0.2973431147402148,		0 };
////	const MathVector p2 = { 0.3512301560533574,		0.3184608229801218,		0 };
////	const MathVector p3 = { 0.3309655464243111,		0.3010404355350647,		0 };
////	const MathVector p4 = { 0.3359655464243111,		0.2910404355350647,		0 };
////	std::vector<const MathVector*> pv = { &p1,&p2,&p3,&p4 };
////
////	Figure fig(figure_type, figure_order, std::move(pv));
////	const size_t polynomial_order = 5;
////	const size_t integrand_order = 2*polynomial_order;
////	constexpr size_t num_iter = 5;
////
////	std::cout << "polynomial order : " << polynomial_order << "\n";
////	std::cout << "\npre calculate quadrature rule\n";
////	for (size_t i=0; i<num_iter; ++i)
////	{		
////		RECORD_CONSUMED_TIME;
////		const auto quadrature_rule = fig.quadrature_rule(integrand_order);
////		const auto initial_basis = fig.initial_basis_vector(polynomial_order);
////		const auto orthonormal_basis = ms::Gram_Schmidt_Process(initial_basis, quadrature_rule);
////		PRINT_CONSUMED_TIME_NANO;
////	}
////	std::cout << "\nadaptive quadrature rule\n";
////	for (size_t i = 0; i < num_iter; ++i)
////	{		
////		RECORD_CONSUMED_TIME;
////		const auto initial_basis = fig.initial_basis_vector(polynomial_order);
////		const auto orthonormal_basis = ms::Gram_Schmidt_Process(initial_basis, fig);
////		PRINT_CONSUMED_TIME_NANO;
////	}
////	//for (size_t i = 0; i < 100; ++i)
////	//	p3.compare_v1(p4);
////	
////
////	//RECORD_CONSUMED_TIME;
////	//for (size_t i = 0; i < 100; ++i)
////	//	p3.compare_v2(p4);
////	//PRINT_CONSUMED_TIME_NANO;
////}
////
//// 
//// 
//// 
//// 
////#include "../MS_Test/INC/Polynomial.h"
////#include <iostream>
////
////int main(void) {
////	const size_t polynomial_order = 1;
////	const size_t domain_order = 2;
////
////	size_t num_basis = ms::combination_with_repetition(polynomial_order + 1, domain_order);
////
////	std::vector<MathVector> compare_node_set;
////	compare_node_set.reserve(num_basis);
////
////	MathVector compare_node(domain_order);
////	if (domain_order == 0) {
////		compare_node_set.push_back(compare_node);		
////		
////		for (const auto& node : compare_node_set)
////			std::cout << node << "\n";
////	}
////
////	while (true) {
////		auto iter = std::find(compare_node.begin(), compare_node.end(), polynomial_order);
////		if (iter != compare_node.end()) {
////			std::cout << compare_node << "\n";
////			compare_node_set.push_back(compare_node);
////
////			if (iter == compare_node.begin())
////				break;
////
////			std::fill(compare_node.begin(), compare_node.end(), 0);
////			(*(--iter))++;
////
////			if (compare_node.front() == polynomial_order) {
////				std::cout << compare_node << "\n";
////				compare_node_set.push_back(compare_node);
////				break;
////			}
////		}
////
////		double component_sum = 0;
////		for (const auto& val : compare_node)
////			component_sum += val;
////
////		if (component_sum == polynomial_order) {
////			std::cout << compare_node << "\n";
////			compare_node_set.push_back(compare_node);
////
////			const auto is_zero = [](const double i) {return i == 0; };
////			auto iter = std::find_if_not(compare_node.rbegin(), compare_node.rend(), is_zero);
////			*iter = 0;
////			(*(++iter))++;
////			continue;
////		}
////
////		std::cout << compare_node << "\n";
////		compare_node_set.push_back(compare_node);
////		compare_node.back()++;
////	}
////
////	//for (const auto& node : compare_node_set)
////	//	std::cout << node << "\n";	
////}
//
//
//
//
////#include <array>
////#include <vector>
////#include <iostream>
//
////int main(void) {
////	std::vector<std::array<size_t, 3>> vec = { {1,2,3},{4,5,6},{7,8,9} };
////
////	std::cout << "vec-array \n";
////	for (const auto& arr : vec)
////		for (const auto& val : arr)
////			std::cout << &val << "\n";
////
////	std::cout << "vec-vec \n";
////	std::vector<std::vector<size_t>> vvec = { {1,2,3},{4,5,6},{7,8,9} };
////	for (const auto& vec : vvec)
////		for (const auto& val : vec)
////			std::cout << &val << "\n";
////}
//
//
//
//
////#include <numeric>
////#include <iomanip>
////#include <iostream>
////
////bool compare_double(const double d1, const double d2, const size_t ULP_factor = 4) {
////	const auto lower_ULP = d1 - std::nextafter(d1, std::numeric_limits<double>::lowest());
////	const auto upper_ULP = std::nextafter(d1, std::numeric_limits<double>::max()) - d1;
////
////	return d1-ULP_factor*lower_ULP <= d2 && d2 <= d1+ULP_factor*upper_ULP;
////}
////
////int main(void) {
////	//different result start
////	//double d1 = 1.0;
////	//double d2 = 1.0000000000000009;//(16)
////
////	//different result end
////	//double d1 = 1.0;
////	//double d2 = 1.0000000000000004;//(16)
////
////	//same result again
////	//double d1 = 1.0;
////	//double d2 = 1.0000000000000003;//(16)
////
////	//same result
////	//double d1 = 1.0;
////	//double d2 = 1.000000000000001;//(15)
////	//double d2 = 1.0000000000000011102;
////
////	double d1 = 1.0000000000000015;
////	double d2 = 1.0000000000000011;
////
////	if (d1 == d2)	// 모든 비트가 동일해야 같다고 판단
////		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is same\n";
////	else
////		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is not same\n";
////
////	if (compare_double(d1, d2,1))
////		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is same\n";
////	else
////		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is not same\n";
////
////	if (compare_double(d1, d2,4))
////		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is same\n";
////	else
////		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is not same\n";
////
////}
//
////#include "../MS_Test/INC/Figure.h"
////size_t calculate_Num_Required_Point(const FigureType figure_type, const size_t required_order) {
////	switch (figure_type) {
////	case FigureType::Line:
////		return static_cast<size_t>((required_order + 1) * 0.5);
////	case FigureType::Triangle:
////		return static_cast<size_t>((required_order * 0.5 + 1) * (required_order * 0.5 + 1));
////	case FigureType::Quadrilateral:
////		return static_cast<size_t>((required_order + 1) * (required_order + 1) * 0.25);
////	default:
////		return NULL;
////	}
////}
////
////int main(void) {
////	FigureType figure_type = FigureType::Line;
////	const std::string path = "C:/Users/KimMinSeok/source/repos/MS_Test/GoogleTest/RSC/Quadrature/Standard/" + ms::to_string(figure_type)+ "/";
////	
////	for (size_t i = 0; i < 200; ++i) {
////		const size_t required_order = 2 * i + 1;
////		const size_t num_required_point = calculate_Num_Required_Point(figure_type, required_order);
////		const std::string read_file_path = path + "P" + std::to_string(required_order) + "_n" + std::to_string(num_required_point) + ".txt";
////
////		Text read_txt;
////		read_txt.read(read_file_path);
////
////		Text write_txt;
////		for (auto& sentence : read_txt) {
////			auto parsed_str_set = ms::parse(sentence, ' ');
////
////			const auto pos_iter = parsed_str_set.end() - 1;
////			parsed_str_set.insert(pos_iter, "0");
////
////			std::string output_sentence;
////			for (const auto& str : parsed_str_set)
////				output_sentence += str + " ";
////
////			write_txt.push_back(output_sentence);
////		}
////
////		const std::string write_file_path = path + "Test/" + "P" + std::to_string(required_order) + "_n" + std::to_string(num_required_point) + ".txt";
////		write_txt.write(write_file_path);
////	}
////}
//
//
//
//
//
////
////#include "../MS_Test/INC/Text.h"
////#include <iostream>
////
////int main(void) {
////	const std::string path = "C:/Users/KimMinSeok/source/repos/MS_Test/GoogleTest/RSC/Quadrature/Standard/Quadrilateral/";
////	const std::string read_file_path = path + "P5_n9.txt";
////
////	Text read_txt;
////	read_txt.read(read_file_path);
////		
////	std::string node_set_str = "{ ";
////	std::string weight_set_str = "{ ";
////	for (auto& sentence : read_txt) {
////		auto parsed_str_set = ms::parse(sentence, ' ');
////		weight_set_str += parsed_str_set.back() + ", ";
////		parsed_str_set.pop_back();
////
////		std::string node_str = "{ ";
////		for (const auto& parsed_str : parsed_str_set)
////			node_str += parsed_str + ",";
////		node_str.pop_back();
////		node_str += " }, ";
////
////		node_set_str += node_str;
////	}
////	node_set_str.pop_back();
////	node_set_str.pop_back();
////	node_set_str += " }";
////
////	weight_set_str.pop_back();
////	weight_set_str.pop_back();
////	weight_set_str += "}";
////
////	std::cout << "{" << node_set_str <<"," << weight_set_str << "}";
////}
//
//
//
//
//
//
//
//
////
////#include <iostream>
////#include <vector>
////class A
////{
////public:
////	A() { std::cout << "constructor\n"; };
////	A(const A& a) { std::cout << "copy constructor \n"; };
////	//A(A&& a) { std::cout << "move constructor \n"; };
////	A(const std::vector<double>& vec) { std::cout << "construct by vector \n"; };
////
////	//A& operator=(const A& a) { std::cout << "copy assignment \n"; return* this; };
////	//A& operator=(A&& a) { std::cout << "move assignment \n"; return*this; };
////};
////
////template <typename T>
////class B : public std::vector<T>
////{	
////public:
////	template <typename ... Vals>
////	explicit B(Vals&&... values) :std::vector<T>(std::forward<Vals>(values)...) {};
////	B(std::initializer_list<T> list) :std::vector<T>(list) {};
////};
////
////class C
////{
////	B<A> val;
////public:
////	C(void) {
////	}
////};
////
////A func(void) {
////	return A();
////}
////
////#include <array>
////int main(void){
////	A a;
////	A b;
////	a = std::move(b);
////}
////

