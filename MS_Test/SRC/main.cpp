
#include "../INC/MathVector.h"

#include <iostream>
#include <array>
#include <vector>
class A
{
private:
	std::array<double, 3> val_;
	bool is_const_;
public:
	A() { std::cout << "construct\n"; };
	A(const A& other) { std::cout << "copy\n"; };
	A(A&& other) { std::cout << "move\n"; };

	void tell_address(void) const {
		for (size_t i = 0; i < val_.size(); ++i)
			std::cout << &val_[i] << "\n";

		std::cout << &is_const_ << "\n";
	}
};

class B
{
private:
	A a_;

public:
	template <typename T>
	B(T&& a)
		:a_(std::forward<T>(a)) {}
};

#include <string>
int main(void)
{
	std::string str = "abcdef";
	str.erase(str.end() - 3, str.end());
	std::cout << str;
}
