
#include "../INC/MathVector.h"
#include <iostream>
class A
{
public:
	A() { 
		std::cout<<"construct\n"; 
	};
	A(const A& other) {
		std::cout<<"copy\n"; 
	};
	A(A&& other) {
		std::cout<<"move\n"; 
	};
};

int main(void)
{
	std::vector<A> set;
	set.emplace_back();
	set.push_back(A());

	auto a = A();

	std::cout << set.size();
}
