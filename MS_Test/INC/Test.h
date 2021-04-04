#pragma once

#include <iostream>
#include <string>


inline void Test_OBJ(const std::string& title) {
	std::cout << "\n========================================================================\n";
	std::cout << "Test : " << title;
	std::cout << "\n========================================================================\n";
}

template <typename T>
bool Test_is_same(const T& A, const T& B) {
	if (A == B)
		return true;
	else 
		return false;	
}

template <typename T>
bool Test_less_then(const T& A, const T& B) {
	if (A <= B)
		return true;
	else 
		return false;	
}