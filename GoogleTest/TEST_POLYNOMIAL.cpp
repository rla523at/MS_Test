#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Polynomial.h"

#define X MonoTerm("x0")
#define Y MonoTerm("x1")
#define Z MonoTerm("x2")

GTEST_TEST(MonoTerm, operator_multiply_1) {
	MonoTerm m1 = 3 * X * Y;
	MonoTerm m2 = (2 * X) ^ 2;
	const auto result = m1 * m2;

	MonoTerm ref = 12 * (X ^ 3) * Y;
	EXPECT_EQ(result, ref);
}

GTEST_TEST(MonoTerm, operator_power_1) {
	const auto result = X ^ 3;

	MonoTerm ref = { 3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, operator_power_2) {
	const auto result = (X * (Y ^ 2) * (Z ^ 3)) ^ 2;

	MonoTerm ref = { 2,4,6 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MonoTerm, operator_call_1) {
	const MonoTerm m = 1;
	const MathVector values = { 1,2,3 };
	const auto result = m(values);

	constexpr double ref = 1.0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, operator_call_2) {
	const MonoTerm m = 1;
	const MathVector values;
	const auto result = m(values);

	constexpr double ref = 1.0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, operator_call_3) {
	const MonoTerm m = 3 * X * (Y ^ 2) * (Z ^ 3);
	const MathVector values = { 1,2,3 };
	const auto result = m(values);

	constexpr double ref2 = 324;
	EXPECT_EQ(result, ref2);
}
GTEST_TEST(MonoTerm, operator_call_4) {
	const MonoTerm m = X * (Y ^ 2) * (Z ^ 3);
	const MathVector values = { 1.84,2.789,3.487946 };
	const auto result = m(values);

	constexpr double ref3 = 6.073291260986822e+02;
	EXPECT_DOUBLE_EQ(result, ref3);
}


GTEST_TEST(MonoTerm, complex_operation_1) {
	MonoTerm m1 = X * (Y ^ 2) * (Z ^ 3);
	MonoTerm m2 = X * (Y ^ 2);
	const auto result = m1 * m2;

	MonoTerm ref = { 2,4,3 };
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, complex_operation_2) {
	MonoTerm m1 = X * (Y ^ 4);
	MonoTerm m2 = X * (Z ^ 3);
	const auto result = m1 * m2;

	MonoTerm ref = { 2,4,3 };
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MonoTerm, order_1) {
	MonoTerm m = 0;
	const auto result = m.order();

	constexpr size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, order_2) {
	MonoTerm m = X;
	const auto result = m.order();

	constexpr size_t ref = 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, order_3) {
	MonoTerm m = (X * Y)^2;
	const auto result = m.order();

	constexpr size_t ref = 4;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(MonoTerm, domain_dimension_1) {
	MonoTerm m = 0;
	const auto result = m.domain_dimension();

	constexpr size_t ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, domain_dimension_2) {
	MonoTerm m = X;
	const auto result = m.domain_dimension();

	constexpr size_t ref = 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, domain_dimension_3) {
	MonoTerm m = (X * Y) ^ 2;
	const auto result = m.domain_dimension();

	constexpr size_t ref = 2;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(MonoTerm, domain_dimension_4) {
	MonoTerm m = "x123";
	const auto result = m.domain_dimension();

	constexpr size_t ref = 124;
	EXPECT_EQ(result, ref);
}




GTEST_TEST(SimplePolynomial, operator_addition_1) {
	const auto p1 = 3 * X + Y;
	const auto p2 = 5 * (X ^ 2);
	const auto p3 = Y ^ 2;
	const auto p4 = -7;
	const auto result = p1 + p2 + p3 + p4;

	SimplePolynomial ref = 5 * (X ^ 2) + (Y ^ 2) + 3 * X + Y - 7;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_addition_2) {
	auto p1 = X + 1;
	auto p2 = X + Y + 1;
	const auto result = p1 + p2;

	auto ref = 2 * X + Y + 2;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_addition_3) {
	auto p1 = (X ^ 2) + X;
	auto p2 = X + Y + 1;
	const auto result = p1 + p2;

	auto ref = (X ^ 2) + 2 * X + Y + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_addition_4) {
	SimplePolynomial p1 = 0.0;
	SimplePolynomial p2 = 0.0;
	const auto result = p1 + p2;

	SimplePolynomial ref = 0.0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_addition_5) {
	auto p1 = -12 * (X ^ 2) * Y + 45 * X;
	auto p2 = 12 * (X ^ 2) * Y - 45 * X;
	const auto result = p1 + p2;

	SimplePolynomial ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_addition_6) {
	auto p1 = (X ^ 2) + X;
	auto p2 = X + 1;
	const auto result = p1 + p2;

	const auto ref = (X ^ 2) + 2 * X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_addition_7) {
	SimplePolynomial p1 = 0.0;
	const auto p2 = X + 1;
	const auto result = p1 + p2;

	const auto ref = X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_addition_8) {
	SimplePolynomial p1 = 0.0;
	const auto p2 = X + 1;
	const auto result = p2 + p1;

	const auto ref = X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_addition_9) {
	SimplePolynomial p1 = 3;
	const auto p2 = X + 1;
	const auto result = p1 + p2;

	const auto ref = X + 4;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(SimplePolynomial, operator_substraction_1) {
	auto p1 = X + Y;
	auto p2 = X + 3 * Y;
	const auto result = p1 - p2;

	auto ref = -2 * Y;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_substraction_2) {
	auto p1 = X * Y;
	auto p2 = X + 3 * Y;
	const auto result = p1 - p2;

	auto ref = X * Y - X - 3 * Y;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_substraction_3) {
	auto p1 = X * Y;
	auto p2 = X;
	const auto result = p1 - p2;

	auto ref = X * Y - X;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_substraction_4) {
	SimplePolynomial p1 = 0;
	auto p2 = X;
	const auto result = p1 - p2;

	auto ref = -1 * X;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_substraction_5) {
	SimplePolynomial p1 = 0;
	const auto p2 = 3 * (X * Y) - 3 * ((X ^ 2) * Z);
	const auto result = p1 - p2;

	auto ref = -3 * (X * Y) + 3 * ((X ^ 2) * Z);
	EXPECT_EQ(result, ref);
}

GTEST_TEST(SimplePolynomial, operator_multiplication_1) {
	const auto p1 = X + 1;
	const auto p2 = X - 2;
	const auto result = p1 * p2;

	const auto ref = (X ^ 2) - X - 2;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_2) {
	const auto p1 = (X ^ 2) - 1;
	const auto p2 = (X ^ 2) + 2 * X + 1;
	const auto result = p1 * p2;

	const auto ref = (X ^ 4) + 2 * (X ^ 3) - 2 * X - 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_3) {
	const auto p1 = (X ^ 2) + 2 * X + 1;
	const auto p2 = (X ^ 2) - 2 * X + 1;
	const auto result = p1 * p2;

	const auto ref = (X ^ 4) - 2 * (X ^ 2) + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_4) {
	SimplePolynomial p1 = 3;
	SimplePolynomial p2 = 2;
	const auto result = p1 * p2;

	SimplePolynomial ref = 6;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_5) {
	SimplePolynomial p1 = 3;
	SimplePolynomial p2 = X - 1;
	const auto result = p1 * p2;

	SimplePolynomial ref = 3 * X - 3;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_6) {
	auto p1 = X;
	auto p2 = X + Y + 1;
	const auto result = p1 * p2;

	auto ref = (X ^ 2) + X * Y + X;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_7) {
	auto p1 = X + Y + 1;
	auto p2 = X;
	const auto result = p1 * p2;

	auto ref = (X ^ 2) + X * Y + X;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_8) {
	auto p1 = X + Y;
	auto p2 = Y + X;
	const auto result = p1 * p2;

	auto ref = (X ^ 2) + 2 * X * Y + (Y ^ 2);
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_9) {
	const auto p1 = X + Y;
	const auto result = -1 * p1;

	auto ref = -1 * X - 1 * Y;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_10) {
	auto p1 = X + 1;
	auto p2 = X + 1;
	const auto result = p1 * p2;

	auto ref = (X ^ 2) + 2 * X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_11) {
	SimplePolynomial p1 = 0;
	SimplePolynomial p2 = 0;
	const auto result = p1 * p2;

	SimplePolynomial ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_12) {
	auto p1 = 1;
	auto p2 = X + 1;
	const auto result = p1 * p2;

	auto ref = X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_13) {
	auto p1 = X + 1;
	SimplePolynomial p2 = 0;
	const auto result = p1 * p2;

	SimplePolynomial ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_multiplication_14) {
	const auto p1 = (X ^ 2) + X + 1;
	const auto result = 5 * p1;

	auto ref = 5 * (X ^ 2) + 5 * X + 5;
	EXPECT_EQ(result, ref);
}


GTEST_TEST(SimplePolynomial, operator_power_1) {
	const auto p1 = (X ^ 2) - 1;
	auto result = p1 ^ 2;

	SimplePolynomial ref = (X ^ 4) - 2 * (X ^ 2) + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_power_2) {
	auto p1 = X + 1;
	const auto result = p1 ^ 2;

	auto ref = (X ^ 2) + 2 * X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_power_3) {
	SimplePolynomial p = 1.5;
	auto result = p ^ 2;

	auto ref = 2.25;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, operator_power_4) {
	SimplePolynomial p = 0;
	auto result = p ^ 2;

	SimplePolynomial ref = 0;
	EXPECT_EQ(result, ref);
}

GTEST_TEST(SimplePolynomial, differentiate_1) {
	const auto  p = X + 1;
	
	const size_t variable_index = 0;
	const auto result = p.differentiate(variable_index);

	SimplePolynomial ref = 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, differentiate_2) {
	SimplePolynomial p = (X ^ 2) - 1;

	const size_t variable_index = 0;
	const auto result = p.differentiate(variable_index);

	const auto ref = 2 * X;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, differentiate_3) {
	auto p1 = (X ^ 2) + X + 1;

	size_t variable_index = 0;
	const auto result = p1.differentiate(variable_index);

	auto ref = 2 * X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(SimplePolynomial, differentiate_4) {
	auto p1 = (X ^ 2) + X + 1;

	size_t variable_index = 1;
	const auto result = p1.differentiate(variable_index);

	const auto ref = 0;
	EXPECT_EQ(result, ref);
}

GTEST_TEST(CompactPolynomial, operator_substraction_1) {
	const auto p1 = (X - 1) * (Y - 1);
	const SimplePolynomial p2 = 1;
	const auto result = p1 - p2;

	const auto ref = X * Y - X - Y;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, operator_substraction_2) {
	const auto p1 = (X - 1) * (Y - 1);
	const auto p2 = X + 1;
	const auto result = p1 - p2;

	const auto ref = X * Y - 2 * X - Y;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, operator_substraction_3) {
	const auto p1 = (X - 1) * (Y - 1);
	const auto p2 = 2 * ((X - 1) ^ 2) + X + Y - 1;
	const auto result = p1 - p2;

	const auto ref = -2 * (X ^ 2) + X * Y + 2 * X - 2 * Y;
	EXPECT_EQ(result, ref);
}

GTEST_TEST(CompactPolynomial, operator_multiplication_1) {
	const auto p1 = X * (X + Y);
	const auto p2 = (Y + X) * X;
	const auto result = p1 * p2;

	const auto ref = (X ^ 4) + 2 * (X^3) * Y + ((X*Y) ^ 2);
	EXPECT_EQ(result, ref);
}

GTEST_TEST(CompactPolynomial, differentiate_1) {
	const auto p = (X + 1) * (X - 1) + 2;

	const size_t variable_index = 0;
	const auto result = p.differentiate(variable_index);

	const auto ref = 2 * X;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_2) {
	const auto p = (X - 1) ^ 2;

	const size_t variable_index = 0;
	const auto result = p.differentiate(variable_index);

	const auto ref = 2 * X - 2;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_3) {
	const auto p = (X - 1) ^ 2;

	const size_t variable_index = 1;
	const auto result = p.differentiate(variable_index);

	const auto ref = 0;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_4) {
	const auto p1 = (X + 1) ^ 2;

	size_t variable_index = 0;
	const auto result = p1.differentiate(variable_index);

	auto ref = 2 * X + 2;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_5) {
	const auto p1 = (X + Y + 2) * (X + 1);

	size_t variable_index = 0;
	const auto result = p1.differentiate(variable_index);

	auto ref = 2 * X + Y + 3;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_6) {
	const auto p1 = (X + Y + 2) * (X + 1);

	size_t variable_index = 1;
	const auto result = p1.differentiate(variable_index);

	auto ref = X + 1;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_7) {
	auto p1 = (X + 1) * (Y + 1) + X + 1;

	size_t variable_index = 0;
	const auto result = p1.differentiate(variable_index);

	auto ref = Y + 2;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_8) {
	const auto p1 = ((X + 1) ^ 2) * (X + 2);

	constexpr size_t variable_index = 0;
	const auto result = p1.differentiate(variable_index);

	auto ref = 3 * (X ^ 2) + 8 * X + 5;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_9) {
	auto p = (2 * X + 3) ^ 2;

	constexpr size_t variable_index = 0;
	const auto result = p.differentiate(variable_index);

	auto ref = 8 * X + 12;
	EXPECT_EQ(result, ref);
}
GTEST_TEST(CompactPolynomial, differentiate_10) {
	auto p = ((2 * X + 3) ^ 2) * X;

	constexpr size_t variable_index = 0;
	const auto result = p.differentiate(variable_index);

	auto ref = 12 * (X ^ 2) + 24 * X + 9;
	EXPECT_EQ(result, ref);
}



//GTEST_TEST(POLYNOMIAL, OPERATOR_POWER_4) {
//	auto p1 = (X ^ 2) + 2 * X + 1;
//	const auto p2 = p1 ^ 2;
//	const auto result = ms::sqrt(p2);
//
//	auto ref = (X ^ 2) + 2 * X + 1;
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, OPERATOR_MULTIPLICATION_10) {
//	auto p1 = X + 1;
//	const auto p2 = p1^(0.5);
//	const auto result = p2 * p2;
//
//	auto ref = X + 1;
//	EXPECT_EQ(result, ref);
//}

GTEST_TEST(POLYNOMIAL, COMPLEX_OPERATION_1) {
	const auto p1 = X + 1;
	const auto p2 = X + 2;
	const auto p3 = X + 3;
	const auto p4 = p1 * p2 + p3;
	const auto p5 = p4 * p4;
	const auto result = 2 * (p5 + p1);

	auto ref = 2 * (X ^ 4) + 16 * (X ^ 3) + 52 * (X ^ 2) + 82 * X + 52;
	EXPECT_EQ(result, ref);
}
//GTEST_TEST(POLYNOMIAL, COMPLEX_OPERATION_2) {
//	auto p1 = (X ^ 2) + X + 1;
//	const auto p2 = p1 ^ 0.7;
//	const auto p3 = 5 * p2;
//
//	for (size_t i = 0; i < 10; ++i) {
//		const double val = 0.31 * i;
//		const MathVector val_vec = { val };
//		const double result = p3(val_vec);
//		const double ref = 5 * (std::pow(val * val + val + 1, 0.7));
//		EXPECT_DOUBLE_EQ(result, ref);
//	}
//}
GTEST_TEST(POLYNOMIAL, COMPLEX_OPERATION_3) {
	auto p1 = X + 1;
	const auto p2 = p1 ^ 2;
	const auto p3 = p2 + X + 2;
	const auto p4 = p3 + 2 * X + 3;
	const auto result = 5 * p4;

	const auto ref = 5 * (X ^ 2) + 25 * X + 30;
	EXPECT_EQ(result, ref);
}
//GTEST_TEST(POLYNOMIAL, COMPLEX_OPERATION_4) {
//	auto p1 = ms::sqrt(X + 1) * X + X + 1;
//	auto p2 = ms::sqrt(X + 2) * X;
//	const auto result = p1 * p2;
//
//	for (size_t i = 0; i < 10; ++i) {
//		const double val = 0.31 * i;
//		const MathVector v = { val };
//
//		const double ref = (std::sqrt(val + 1) * val + val + 1) * (std::sqrt(val + 2) * val);
//		EXPECT_DOUBLE_EQ(result(v), ref);
//	}
//}
GTEST_TEST(CompactPolynomial, complex_operation_5) {
	auto p1 = X + Y;
	auto p2 = Y + X;
	auto p3 = X;
	const auto result = (p1 * p3) * (p2 * p3);

	auto ref = (X ^ 4) + 2 * (X ^ 3) * Y + ((X * Y) ^ 2);
	EXPECT_EQ(result, ref);
}





//
//
//GTEST_TEST(POLYNOMIAL, GRADIENT_1) {
//	auto p = X + Y + 1;
//	auto result = p.gradient();
//
//	VectorFunction<auto> ref = { 1, 1 };
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, GRADIENT2) {
//	auto p = (X ^ 2) + X * Y + 1;
//	auto result = p.gradient();
//
//	VectorFunction<auto> ref = { 2*X+Y, X };
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, GRADIENT3) {
//	auto p = (X ^ 2) + X * Y + 1;
//	const size_t domain_dimension = 3;
//	auto result = p.gradient(domain_dimension);
//
//	VectorFunction<auto> ref = { 2 * X + Y, X, 0 };
//	EXPECT_EQ(result, ref);
//}
//GTEST_TEST(POLYNOMIAL, GRADIENT4) {
//	const auto p = X*Y*Z;
//	const auto result = p.gradient();
//
//	const VectorFunction<auto> ref = { Y * Z,  X * Z, X * Y };
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(VECTORFUNCTION, OPERATOR_CALL1) {
//	auto p1 = (X ^ 2) + 3 * (X ^ 2) * Y + (Y ^ 3) + (Z ^ 2) - 6;
//	auto p2 = X + Y + Z - 3;
//	auto p3 = (Y ^ 2) * Z + X * Z - 2;
//	VectorFunction<auto> f = { p1,p2,p3 };
//	MathVector node = { 1,1,1 };
//	const auto result = f(node);
//
//	MathVector ref = { 0,0,0 };
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE1) {
//	auto p1 = X + 1;
//	auto p2 = X + Y + Z - 3;
//	auto p3 = Y + Z;
//	VectorFunction<auto> f = { p1,p2,p3 };
//
//	constexpr size_t variable_index = 0;
//	f.differentiate(variable_index);
//
//	VectorFunction<auto> ref = { 1,1,0 };
//	EXPECT_EQ(f, ref);
//}
//GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE2) {
//	auto p1 = X * Y + 1;
//	auto p2 = X + Y * Z + Z - 3;
//	auto p3 = Y + Z;
//	VectorFunction<auto> f = { p1,p2,p3 };
//
//	constexpr size_t variable_index = 0;
//	f.differentiate(variable_index);
//
//	VectorFunction<auto> ref = { Y,1,0 };
//	EXPECT_EQ(f, ref);
//}
//GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE3) {
//	auto p1 = X * Y + 1;
//	auto p2 = X + Y * Z + Z - 3;
//	auto p3 = Y + Z;
//	VectorFunction<auto> f = { p1,p2,p3 };
//
//	constexpr size_t variable_index = 1;
//	f.differentiate(variable_index);
//
//	VectorFunction<auto> ref = { X,Z,1 };
//	EXPECT_EQ(f, ref);
//}
//GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE4) {
//	auto p1 = 1.5 * X + 0.5 * Y + 3;
//	auto p2 = Y + 3;
//	auto p3 = 0;
//	VectorFunction<auto> f = { p1,p2,p3 };
//
//	constexpr size_t variable_index = 0;
//	f.differentiate(variable_index);
//
//	VectorFunction<auto> ref = { 1.5,0,0 };
//	EXPECT_EQ(f, ref);
//}
//GTEST_TEST(VECTORFUNCTION, DIFFERENTIATE5) {
//	auto p1 = 1.5 * X + 0.5 * Y + 3;
//	auto p2 = Y + 3;
//	auto p3 = 0;
//	VectorFunction<auto> f = { p1,p2,p3 };
//
//	constexpr size_t variable_index = 1;
//	f.differentiate(variable_index);
//
//	VectorFunction<auto> ref = { 0.5,1,0 };
//	EXPECT_EQ(f, ref);
//}
//
//
//GTEST_TEST(VECTORFUNCTION, CROSS_PRODUCT1) {
//	VectorFunction<auto> vf1 = { 1.5,0,0 };
//	VectorFunction<auto> vf2 = { 0.5,1,0 };
//	const auto result = vf1.cross_product(vf2);
//
//	VectorFunction<auto> ref = { 0,0,1.5 };
//	EXPECT_EQ(result, ref);
//}
//
//
//GTEST_TEST(VECTORFUNCTION, L2_NORM1) {
//	VectorFunction<auto> vf = { 0,0,1.5 };
//	const auto result = vf.L2_norm();
//
//	auto ref = 1.5;
//	EXPECT_EQ(result, ref);
//}
