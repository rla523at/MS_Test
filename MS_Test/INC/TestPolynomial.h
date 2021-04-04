#pragma once

#include "Test.h"
#include "Polynomial.h"

void TestCallOperator(void) {
	
	Test_OBJ("Test Polynomial Function Call Operator");

	std::vector<double> coefficient_set = { 1,1 };
	std::vector<Monomial> monomial_set = { Monomial{0},Monomial{1} };

	Polynomial p1(coefficient_set, monomial_set);

	if (!Test_is_same(p1({ 0 }), 1.0))
		std::cout << "test fail!";
	else
		std::cout << "test success";
}