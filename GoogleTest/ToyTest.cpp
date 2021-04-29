#include <numeric>
#include <iomanip>
#include <iostream>

bool compare_double(const double d1, const double d2, const size_t ULP_factor = 4) {
	const auto lower_ULP = d1 - std::nextafter(d1, std::numeric_limits<double>::lowest());
	const auto upper_ULP = std::nextafter(d1, std::numeric_limits<double>::max()) - d1;

	return d1-ULP_factor*lower_ULP <= d2 && d2 <= d1+ULP_factor*upper_ULP;
}

int main(void) {

	//same result
	//double d1 = 1.0;
	//double d2 = 1.000000000000001;//(15)

	//different result start
	//double d1 = 1.0;
	//double d2 = 1.0000000000000009;//(16)

	//different result end
	//double d1 = 1.0;
	//double d2 = 1.0000000000000004;//(16)

	//same result again
	double d1 = 1.0;
	double d2 = 1.0000000000000003;//(16)


	if (compare_double(d1, d2))
		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is same\n";
	else
		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is not same\n";

	if (compare_double(d1, d2,1))
		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is same\n";
	else
		std::cout << std::setprecision(20) << d1 << "\t" << d2 << "\t is not same\n";

}

//#include "../MS_Test/INC/Figure.h"
//size_t calculate_Num_Required_Point(const FigureType figure_type, const size_t required_order) {
//	switch (figure_type) {
//	case FigureType::Line:
//		return static_cast<size_t>((required_order + 1) * 0.5);
//	case FigureType::Triangle:
//		return static_cast<size_t>((required_order * 0.5 + 1) * (required_order * 0.5 + 1));
//	case FigureType::Quadrilateral:
//		return static_cast<size_t>((required_order + 1) * (required_order + 1) * 0.25);
//	default:
//		return NULL;
//	}
//}
//
//int main(void) {
//	FigureType figure_type = FigureType::Line;
//	const std::string path = "C:/Users/KimMinSeok/source/repos/MS_Test/GoogleTest/RSC/Quadrature/Standard/" + ms::to_string(figure_type)+ "/";
//	
//	for (size_t i = 0; i < 200; ++i) {
//		const size_t required_order = 2 * i + 1;
//		const size_t num_required_point = calculate_Num_Required_Point(figure_type, required_order);
//		const std::string read_file_path = path + "P" + std::to_string(required_order) + "_n" + std::to_string(num_required_point) + ".txt";
//
//		Text read_txt;
//		read_txt.read(read_file_path);
//
//		Text write_txt;
//		for (auto& sentence : read_txt) {
//			auto parsed_str_set = ms::parse(sentence, ' ');
//
//			const auto pos_iter = parsed_str_set.end() - 1;
//			parsed_str_set.insert(pos_iter, "0");
//
//			std::string output_sentence;
//			for (const auto& str : parsed_str_set)
//				output_sentence += str + " ";
//
//			write_txt.push_back(output_sentence);
//		}
//
//		const std::string write_file_path = path + "Test/" + "P" + std::to_string(required_order) + "_n" + std::to_string(num_required_point) + ".txt";
//		write_txt.write(write_file_path);
//	}
//}

//
//#include "../MS_Test/INC/Text.h"
//#include <iostream>
//
//int main(void) {
//	const std::string path = "C:/Users/KimMinSeok/source/repos/MS_Test/GoogleTest/RSC/Quadrature/Standard/Quadrilateral/";
//	const std::string read_file_path = path + "P5_n9.txt";
//
//	Text read_txt;
//	read_txt.read(read_file_path);
//		
//	std::string node_set_str = "{ ";
//	std::string weight_set_str = "{ ";
//	for (auto& sentence : read_txt) {
//		auto parsed_str_set = ms::parse(sentence, ' ');
//		weight_set_str += parsed_str_set.back() + ", ";
//		parsed_str_set.pop_back();
//
//		std::string node_str = "{ ";
//		for (const auto& parsed_str : parsed_str_set)
//			node_str += parsed_str + ",";
//		node_str.pop_back();
//		node_str += " }, ";
//
//		node_set_str += node_str;
//	}
//	node_set_str.pop_back();
//	node_set_str.pop_back();
//	node_set_str += " }";
//
//	weight_set_str.pop_back();
//	weight_set_str.pop_back();
//	weight_set_str += "}";
//
//	std::cout << "{" << node_set_str <<"," << weight_set_str << "}";
//}









//#include <iostream>
//class A
//{
//public:
//	A() { std::cout << "constructor\n"; };
//	A(const A& a) { std::cout << "copy constructor \n"; };
//
//	A& operator=(const A& a) { std::cout << "copy assignment \n"; return* this; };
//	A& operator=(A&& a) { std::cout << "move assignment \n"; return*this; };
//};
//
//A func(void) {
//	return A();
//}
//
//#include <map>
//
//int main(void){
//	std::map<int, int> m;
//
//	//m.emplace(1, 1);
//	//m.emplace(2, 2);
//	//m.emplace(3, 3);
//
//	m[1] = 1;
//	m[2] = 2;
//
//	for (const auto& [key, value] : m)
//		std::cout << key << " " << value << "\n";
//
//}