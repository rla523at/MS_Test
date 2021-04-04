#pragma once

#include "Matrix.h"
#include "Test.h"

void Test1(void) {
	//Test_OBJ("full matrix *= full matrix");
	//MathVector v1 = { 1,0,0,1,0,0 };
	//MathVector v2 = { 0,1,0,0 };
	//const RowMajorMatrix A(MatrixType::Full, 2, 3, v1);
	//const RowMajorMatrix B(MatrixType::Full, 2, 2, v2);
	//const auto trsA = A.transpose();
	//const auto trsB = B.transpose();

	//MathVector refv1 = { 0,1,0,0,0,0 };
	//MathVector refv2 = { 0,0,0,1,0,0 };
	//const RowMajorMatrix refm1(MatrixType::Full, 3, 2, refv1);
	//const RowMajorMatrix refm2(MatrixType::Full, 2, 3, refv2);

	//std::cout << "A matrix\n" << A;
	//std::cout << "B matrix\n" << B;

	//Test_OBJ("trs(A)*=B");
	//auto test1 = trsA;
	//test1 *= B;
	//Test_is_same(refm1, test1);

	//Test_OBJ("B*=trs(A)");
	//auto test2 = trsB;
	//test2 *= A;
	//Test_is_same(refm2, test2);

	//MathVector v3 = { 1,0,1,3 };
	//MathVector v4 = { 0,1,2,1 };
	//const RowMajorMatrix C(MatrixType::Full, 2, 2, v3);
	//const RowMajorMatrix D(MatrixType::Full, 2, 2, v4);
	//const auto trsC = C.transpose();
	//const auto trsD = D.transpose();

	//MathVector refv3 = { 0,1,6,4 };
	//MathVector refv4 = { 0,2,3,5 };
	//MathVector refv5 = { 2,2,6,3 };
	//MathVector refv6 = { 1,3,3,3 };
	//const RowMajorMatrix refm3(MatrixType::Full, 2, 2, refv3);
	//const RowMajorMatrix refm4(MatrixType::Full, 2, 2, refv4);
	//const RowMajorMatrix refm5(MatrixType::Full, 2, 2, refv5);
	//const RowMajorMatrix refm6(MatrixType::Full, 2, 2, refv6);
	//
	//std::cout << "C matrix\n" << C;
	//std::cout << "D matrix\n" << D;

	//Test_OBJ("C*=D");
	//auto test3 = C;
	//test3 *= D;
	//Test_is_same(refm3, test3);

	//Test_OBJ("C*=trs(D)");
	//auto test4 = C;
	//test4 *= trsD;
	//Test_is_same(refm4, test4);

	//Test_OBJ("trs(C)*=D");
	//auto test5 = trsC;
	//test5 *= D;
	//Test_is_same(refm5, test5);

	//Test_OBJ("trs(C)*=trs(D)");
	//auto test6 = trsC;
	//test6 *= trsD;
	//Test_is_same(refm6, test6);

	//const MathVector v5 = { 1,2,3,4,5,6,7,8,0,0,0,0 };
	//const MathVector v6 = std::vector<double>(32,1);
	//const RowMajorMatrix m5(MatrixType::Full, 3, 4, v5);
	//const RowMajorMatrix m6(MatrixType::Full, 4, 8, v6);

	//std::cout << "m5 matrix\n" << m5;
	//std::cout << "m6 matrix\n" << m6;

	//{
	//	Test_OBJ("m5*=m6");
	//	const MathVector refv = { 10,10,10,10,10,10,10,10,26,26,26,26,26,26,26,26,0,0,0,0,0,0,0,0 };
	//	const RowMajorMatrix refm(MatrixType::Full, 3, 8, refv);
	//	auto test = m5;
	//	test *= m6;
	//	Test_is_same(refm, test);
	//}
	{
		Test_OBJ("m1*=m2");
		RowMajorMatrix m1(MatrixType::Full, 2, 3);
		m1.change_Column(0, { 1,1 });
		std::cout <<"m1 matrix\n"<< m1;

		RowMajorMatrix m2(MatrixType::Full, 3, 9);
		m2.change_Column(8, { 1,1,1 });
		std::cout << "m2 matrix\n" << m2;
				
		RowMajorMatrix refm(MatrixType::Full, 2, 9);
		refm.change_Column(8, { 1,1 });

		auto test = m1;
		test *= m2;
		Test_is_same(refm, test);
	}
}

void Test2(void) {
}

void Test3(void) {
	std::cout << "[test operator += ] \n\n";
	MathVector v = { 1,0,3,4 };
	MathVector v2 = { 1,3,4 };

	const RowMajorMatrix m1(MatrixType::Full, 2, 2, v);
	const RowMajorMatrix m2(MatrixType::FullLowerTriangle, 2, 2, v);
	const RowMajorMatrix m3(MatrixType::PackedLowerTriangle, 2, 2, v2);

	const auto m1t = m1.transpose();
	const auto m2t = m2.transpose();
	const auto m3t = m3.transpose();


	auto ref1 = m1 + m1;
	auto ref2 = m1t + m1;
	auto ref3 = m1t + m1t;
	auto ref4 = m1 + m1t;

	std::cout << std::boolalpha;
	std::cout << "\nfull- full\n"; {
		auto f1 = m1;
		auto f2 = m1t;
		auto f3 = m1t;
		auto f4 = m1;

		std::cout << (ref1 == (f1 += m1)) << "\n";
		std::cout << (ref2 == (f2 += m1)) << "\n";
		std::cout << (ref3 == (f3 += m1t)) << "\n";
		std::cout << (ref4 == (f4 += m1t)) << "\n";
	}
	std::cout << "\nfull - full triangle\n"; {
		auto f1 = m1;
		auto f2 = m1t;
		auto f3 = m1t;
		auto f4 = m1;

		std::cout << (ref1 == (f1 += m2)) << "\n";
		std::cout << (ref2 == (f2 += m2)) << "\n";
		std::cout << (ref3 == (f3 += m2t)) << "\n";
		std::cout << (ref4 == (f4 += m2t)) << "\n";
	}
	std::cout << "\nfull - pack triangle\n"; {
		auto f1 = m1;
		auto f2 = m1t;
		auto f3 = m1t;
		auto f4 = m1;

		std::cout << (ref1 == (f1 += m3)) << "\n";
		std::cout << (ref2 == (f2 += m3)) << "\n";
		std::cout << (ref3 == (f3 += m3t)) << "\n";
		std::cout << (ref4 == (f4 += m3t)) << "\n";
	}


	std::cout << "\nfull triangle - full\n"; {
		auto f1 = m2;
		auto f2 = m2t;
		auto f3 = m2t;
		auto f4 = m2;

		std::cout << (ref1 == (f1 += m1)) << "\n";
		std::cout << (ref2 == (f2 += m1)) << "\n";
		std::cout << (ref3 == (f3 += m1t)) << "\n";
		std::cout << (ref4 == (f4 += m1t)) << "\n";
	}
	std::cout << "\nfull triangle - full triangle\n"; {
		auto f1 = m2;
		auto f2 = m2t;
		auto f3 = m2t;
		auto f4 = m2;

		std::cout << (ref1 == (f1 += m2)) << "\n";
		std::cout << (ref2 == (f2 += m2)) << "\n";
		std::cout << (ref3 == (f3 += m2t)) << "\n";
		std::cout << (ref4 == (f4 += m2t)) << "\n";
	}
	std::cout << "\nfull triangle - pack triangle\n"; {
		auto f1 = m2;
		auto f2 = m2t;
		auto f3 = m2t;
		auto f4 = m2;

		std::cout << (ref1 == (f1 += m3)) << "\n";
		std::cout << (ref2 == (f2 += m3)) << "\n";
		std::cout << (ref3 == (f3 += m3t)) << "\n";
		std::cout << (ref4 == (f4 += m3t)) << "\n";
	}


	std::cout << "\npack triangle - full\n"; {
		auto f1 = m3;
		auto f2 = m3t;
		auto f3 = m3t;
		auto f4 = m3;

		std::cout << (ref1 == (f1 += m1)) << "\n";
		std::cout << (ref2 == (f2 += m1)) << "\n";
		std::cout << (ref3 == (f3 += m1t)) << "\n";
		std::cout << (ref4 == (f4 += m1t)) << "\n";
	}
	std::cout << "\npack triangle - full triangle\n"; {
		auto f1 = m3;
		auto f2 = m3t;
		auto f3 = m3t;
		auto f4 = m3;

		std::cout << (ref1 == (f1 += m2)) << "\n";
		std::cout << (ref2 == (f2 += m2)) << "\n";
		std::cout << (ref3 == (f3 += m2t)) << "\n";
		std::cout << (ref4 == (f4 += m2t)) << "\n";
	}
	std::cout << "\npack triangle - pack triangle\n"; {
		auto f1 = m3;
		auto f2 = m3t;
		auto f3 = m3t;
		auto f4 = m3;

		std::cout << (ref1 == (f1 += m3)) << "\n";
		std::cout << (ref2 == (f2 += m3)) << "\n";
		std::cout << (ref3 == (f3 += m3t)) << "\n";
		std::cout << (ref4 == (f4 += m3t)) << "\n";
	}
}

void Test4(void) {
	std::cout << "[test operator + ] \n\n" ;
	MathVector v = { 1,0,3,4 };
	MathVector v2 = { 1,3,4 };

	const RowMajorMatrix m1(MatrixType::Full, 2, 2, v);
	const RowMajorMatrix m2(MatrixType::FullLowerTriangle, 2, 2, v);
	const RowMajorMatrix m3(MatrixType::PackedLowerTriangle, 2, 2, v2);

	const auto m1t = m1.transpose();
	const auto m2t = m2.transpose();
	const auto m3t = m3.transpose();

	auto ref1 = m1 + m1;
	auto ref2 = m1t + m1;
	auto ref3 = m1t + m1t;
	auto ref4 = m1 + m1t;

	std::cout << std::boolalpha;

	std::cout << "\nfull- full\n";
	std::cout << (ref1 == m1 + m1) << "\n";
	std::cout << (ref2 == m1t + m1) << "\n";
	std::cout << (ref3 == m1t + m1t) << "\n";
	std::cout << (ref4 == m1 + m1t) << "\n";

	std::cout << "\nfull- full triangle\n";
	std::cout << (ref1 == m1 + m2) << "\n";
	std::cout << (ref2 == m1t + m2) << "\n";
	std::cout << (ref3 == m1t + m2t) << "\n";
	std::cout << (ref4 == m1 + m2t) << "\n";

	std::cout << "\nfull- packed triangle\n";
	std::cout << (ref1 == m1 + m3) << "\n";
	std::cout << (ref2 == m1t + m3) << "\n";
	std::cout << (ref3 == m1t + m3t) << "\n";
	std::cout << (ref4 == m1 + m3t) << "\n";


	std::cout << "\nfull triangle- full\n";
	std::cout << (ref1 == m2 + m1) << "\n";
	std::cout << (ref2 == m2t + m1) << "\n";
	std::cout << (ref3 == m2t + m1t) << "\n";
	std::cout << (ref4 == m2 + m1t) << "\n";

	std::cout << "\nfull triangle - full triangle\n";
	std::cout << (ref1 == m2 + m2) << "\n";
	std::cout << (ref2 == m2t + m2) << "\n";
	std::cout << (ref3 == m2t + m2t) << "\n";
	std::cout << (ref4 == m2 + m2t) << "\n";

	std::cout << "\nfull triangle- packed triangle\n";
	std::cout << (ref1 == m2 + m3) << "\n";
	std::cout << (ref2 == m2t + m3) << "\n";
	std::cout << (ref3 == m2t + m3t) << "\n";
	std::cout << (ref4 == m2 + m3t) << "\n";


	std::cout << "\npack triangle- full\n";
	std::cout << (ref1 == m3 + m1) << "\n";
	std::cout << (ref2 == m3t + m1) << "\n";
	std::cout << (ref3 == m3t + m1t) << "\n";
	std::cout << (ref4 == m3 + m1t) << "\n";

	std::cout << "\npack triangle - full triangle\n";
	std::cout << (ref1 == m3 + m2) << "\n";
	std::cout << (ref2 == m3t + m2) << "\n";
	std::cout << (ref3 == m3t + m2t) << "\n";
	std::cout << (ref4 == m3 + m2t) << "\n";

	std::cout << "\npack triangle- packed triangle\n";
	std::cout << (ref1 == m3 + m3) << "\n";
	std::cout << (ref2 == m3t + m3) << "\n";
	std::cout << (ref3 == m3t + m3t) << "\n";
	std::cout << (ref4 == m3 + m3t) << "\n";
}

void Test5(void) {
	std::cout << "[test operator -= ] \n\n" ;
	MathVector v = { 1,0,3,4 };
	MathVector v2 = { 1,3,4 };

	const RowMajorMatrix m1(MatrixType::Full, 2, 2, v);
	const RowMajorMatrix m2(MatrixType::FullLowerTriangle, 2, 2, v);
	const RowMajorMatrix m3(MatrixType::PackedLowerTriangle, 2, 2, v2);

	const auto m1t = m1.transpose();
	const auto m2t = m2.transpose();
	const auto m3t = m3.transpose();

	auto ref1 = m1 - m1;
	auto ref2 = m1t - m1;
	auto ref3 = m1t - m1t;
	auto ref4 = m1 - m1t;

	std::cout << std::boolalpha;
	std::cout << "\nfull- full\n"; {
		auto f1 = m1;
		auto f2 = m1t;
		auto f3 = m1t;
		auto f4 = m1;

		std::cout << (ref1 == (f1 -= m1)) << "\n";
		std::cout << (ref2 == (f2 -= m1)) << "\n";
		std::cout << (ref3 == (f3 -= m1t)) << "\n";
		std::cout << (ref4 == (f4 -= m1t)) << "\n";
	}
	std::cout << "\nfull - full triangle\n"; {
		auto f1 = m1;
		auto f2 = m1t;
		auto f3 = m1t;
		auto f4 = m1;

		std::cout << (ref1 == (f1 -= m2)) << "\n";
		std::cout << (ref2 == (f2 -= m2)) << "\n";
		std::cout << (ref3 == (f3 -= m2t)) << "\n";
		std::cout << (ref4 == (f4 -= m2t)) << "\n";
	}
	std::cout << "\nfull - pack triangle\n"; {
		auto f1 = m1;
		auto f2 = m1t;
		auto f3 = m1t;
		auto f4 = m1;

		std::cout << (ref1 == (f1 -= m3)) << "\n";
		std::cout << (ref2 == (f2 -= m3)) << "\n";
		std::cout << (ref3 == (f3 -= m3t)) << "\n";
		std::cout << (ref4 == (f4 -= m3t)) << "\n";
	}


	std::cout << "\nfull triangle - full\n"; {
		auto f1 = m2;
		auto f2 = m2t;
		auto f3 = m2t;
		auto f4 = m2;

		std::cout << (ref1 == (f1 -= m1)) << "\n";
		std::cout << (ref2 == (f2 -= m1)) << "\n";
		std::cout << (ref3 == (f3 -= m1t)) << "\n";
		std::cout << (ref4 == (f4 -= m1t)) << "\n";
	}
	std::cout << "\nfull triangle - full triangle\n"; {
		auto f1 = m2;
		auto f2 = m2t;
		auto f3 = m2t;
		auto f4 = m2;

		std::cout << (ref1 == (f1 -= m2)) << "\n";
		std::cout << (ref2 == (f2 -= m2)) << "\n";
		std::cout << (ref3 == (f3 -= m2t)) << "\n";
		std::cout << (ref4 == (f4 -= m2t)) << "\n";
	}
	std::cout << "\nfull triangle - pack triangle\n"; {
		auto f1 = m2;
		auto f2 = m2t;
		auto f3 = m2t;
		auto f4 = m2;

		std::cout << (ref1 == (f1 -= m3)) << "\n";
		std::cout << (ref2 == (f2 -= m3)) << "\n";
		std::cout << (ref3 == (f3 -= m3t)) << "\n";
		std::cout << (ref4 == (f4 -= m3t)) << "\n";
	}


	std::cout << "\npack triangle - full\n"; {
		auto f1 = m3;
		auto f2 = m3t;
		auto f3 = m3t;
		auto f4 = m3;

		std::cout << (ref1 == (f1 -= m1)) << "\n";
		std::cout << (ref2 == (f2 -= m1)) << "\n";
		std::cout << (ref3 == (f3 -= m1t)) << "\n";
		std::cout << (ref4 == (f4 -= m1t)) << "\n";
	}
	std::cout << "\npack triangle - full triangle\n"; {
		auto f1 = m3;
		auto f2 = m3t;
		auto f3 = m3t;
		auto f4 = m3;

		std::cout << (ref1 == (f1 -= m2)) << "\n";
		std::cout << (ref2 == (f2 -= m2)) << "\n";
		std::cout << (ref3 == (f3 -= m2t)) << "\n";
		std::cout << (ref4 == (f4 -= m2t)) << "\n";
	}
	std::cout << "\npack triangle - pack triangle\n"; {
		auto f1 = m3;
		auto f2 = m3t;
		auto f3 = m3t;
		auto f4 = m3;

		std::cout << (ref1 == (f1 -= m3)) << "\n";
		std::cout << (ref2 == (f2 -= m3)) << "\n";
		std::cout << (ref3 == (f3 -= m3t)) << "\n";
		std::cout << (ref4 == (f4 -= m3t)) << "\n";
	}
}

void Test6(void){
	std::cout << "[test operator - ] \n\n" ;
	MathVector v = { 1,0,3,4 };
	MathVector v2 = { 1,3,4 };

	const RowMajorMatrix m1(MatrixType::Full, 2, 2, v);
	const RowMajorMatrix m2(MatrixType::FullLowerTriangle, 2, 2, v);
	const RowMajorMatrix m3(MatrixType::PackedLowerTriangle, 2, 2, v2);

	const auto m1t = m1.transpose();
	const auto m2t = m2.transpose();
	const auto m3t = m3.transpose();

	auto ref1 = m1 - m1;
	auto ref2 = m1t - m1;
	auto ref3 = m1t - m1t;
	auto ref4 = m1 - m1t;

	std::cout << std::boolalpha;

	std::cout << "\nfull- full\n";
	std::cout << (ref1 == m1 - m1) << "\n";
	std::cout << (ref2 == m1t - m1) << "\n";
	std::cout << (ref3 == m1t - m1t) << "\n";
	std::cout << (ref4 == m1 - m1t) << "\n";

	std::cout << "\nfull- full triangle\n";
	std::cout << (ref1 == m1 - m2) << "\n";
	std::cout << (ref2 == m1t - m2) << "\n";
	std::cout << (ref3 == m1t - m2t) << "\n";
	std::cout << (ref4 == m1 - m2t) << "\n";

	std::cout << "\nfull- packed triangle\n";
	std::cout << (ref1 == m1 - m3) << "\n";
	std::cout << (ref2 == m1t - m3) << "\n";
	std::cout << (ref3 == m1t - m3t) << "\n";
	std::cout << (ref4 == m1 - m3t) << "\n";


	std::cout << "\nfull triangle- full\n";
	std::cout << (ref1 == m2 - m1) << "\n";
	std::cout << (ref2 == m2t - m1) << "\n";
	std::cout << (ref3 == m2t - m1t) << "\n";
	std::cout << (ref4 == m2 - m1t) << "\n";

	std::cout << "\nfull triangle - full triangle\n";
	std::cout << (ref1 == m2 - m2) << "\n";
	std::cout << (ref2 == m2t - m2) << "\n";
	std::cout << (ref3 == m2t - m2t) << "\n";
	std::cout << (ref4 == m2 - m2t) << "\n";

	std::cout << "\nfull triangle- packed triangle\n";
	std::cout << (ref1 == m2 - m3) << "\n";
	std::cout << (ref2 == m2t - m3) << "\n";
	std::cout << (ref3 == m2t - m3t) << "\n";
	std::cout << (ref4 == m2 - m3t) << "\n";


	std::cout << "\npack triangle- full\n";
	std::cout << (ref1 == m3 - m1) << "\n";
	std::cout << (ref2 == m3t - m1) << "\n";
	std::cout << (ref3 == m3t - m1t) << "\n";
	std::cout << (ref4 == m3 - m1t) << "\n";

	std::cout << "\npack triangle - full triangle\n";
	std::cout << (ref1 == m3 - m2) << "\n";
	std::cout << (ref2 == m3t - m2) << "\n";
	std::cout << (ref3 == m3t - m2t) << "\n";
	std::cout << (ref4 == m3 - m2t) << "\n";

	std::cout << "\npack triangle- packed triangle\n";
	std::cout << (ref1 == m3 - m3) << "\n";
	std::cout << (ref2 == m3t - m3) << "\n";
	std::cout << (ref3 == m3t - m3t) << "\n";
	std::cout << (ref4 == m3 - m3t) << "\n";
}

void Test7(void) {
	std::cout << "[ test determinant ] \n\n" ;
	MathVector v = { 1,0,3,4 };
	MathVector v2 = { 1,3,4 };

	const RowMajorMatrix m1(MatrixType::Full, 2, 2, v);
	const RowMajorMatrix m2(MatrixType::FullLowerTriangle, 2, 2, v);
	const RowMajorMatrix m3(MatrixType::PackedLowerTriangle, 2, 2, v2);

	const auto m1t = m1.transpose();
	const auto m2t = m2.transpose();
	const auto m3t = m3.transpose();

	auto ref1 = m1.determinant();
	auto ref2 = m1t.determinant();

	std::cout << std::boolalpha;
	std::cout << (ref1 == m1.determinant()) << "\n";
	std::cout << (ref1 == m2.determinant()) << "\n";
	std::cout << (ref1 == m3.determinant()) << "\n";
	std::cout << (ref2 == m1t.determinant()) << "\n";
	std::cout << (ref2 == m2t.determinant()) << "\n";
	std::cout << (ref2 == m3t.determinant()) << "\n";
}

void Test8(void) {
	std::cout << "[ test inverse ] \n\n" ;
	MathVector v = { 1,0,3,4 };
	MathVector v2 = { 1,3,4 };

	const RowMajorMatrix m1(MatrixType::Full, 2, 2, v);
	const RowMajorMatrix m2(MatrixType::FullLowerTriangle, 2, 2, v);
	const RowMajorMatrix m3(MatrixType::PackedLowerTriangle, 2, 2, v2);

	const auto m1t = m1.transpose();
	const auto m2t = m2.transpose();
	const auto m3t = m3.transpose();

	auto ref1 = m1.inverse();
	auto ref2 = m1t.inverse();

	std::cout << std::boolalpha;
	std::cout << (ref1 == m1.inverse()) << "\n";
	std::cout << (ref1 == m2.inverse()) << "\n";
	std::cout << (ref1 == m3.inverse()) << "\n";	//compare Full and Pack Isuue
	std::cout << (ref2 == m1t.inverse()) << "\n";
	std::cout << (ref2 == m2t.inverse()) << "\n";
	std::cout << (ref2 == m3t.inverse()) << "\n";	//compare Full and Pack Isuue
}

void Test9(void) {
	std::cout << "test part \n\n";
	MathVector v1 = { 1,0,0,2,3,0,4,5,6 };
	MathVector v2 = { 1,2,3,4,5,6 };

	const RowMajorMatrix m1(MatrixType::Full, 3, v1);
	const RowMajorMatrix m2(MatrixType::FullLowerTriangle, 3, v1);
	const RowMajorMatrix m3(MatrixType::PackedLowerTriangle, 3, v2);

	auto m1t = m1.transpose();
	auto m2t = m2.transpose();
	auto m3t = m3.transpose();

	std::cout << std::boolalpha;

	std::cout << "Full - Full Triangle \n";
	for (size_t row_start = 0; row_start <= 3; ++row_start)
		for (size_t column_start = 0; column_start <= 3; ++column_start)
			for (size_t row_end = row_start + 1; row_end <= 3; ++row_end)
				for (size_t column_end = column_start + 1; column_end <= 3; ++column_end) {
					std::cout << "part [" << row_start << "," << row_end << "," << column_start << "," << column_end << "]\n";
					std::cout << "row [" << row_start << "," << row_end << "), column [" << column_start << "," << column_end << ")\n";

					auto ref1 = m1.part(row_start, row_end, column_start, column_end);
					auto ref2 = m1t.part(row_start, row_end, column_start, column_end);

					std::cout << (ref1 == m2.part(row_start, row_end, column_start, column_end)) << "\n";
					std::cout << (ref2 == m2t.part(row_start, row_end, column_start, column_end)) << "\n";
					std::cout << "\n";
				}

	for (size_t row_deleting = 0; row_deleting < 3; ++row_deleting)
		for (size_t column_deleting = 0; column_deleting < 3; ++column_deleting) {
			std::cout << "deleting (" << row_deleting << "," << column_deleting << ") \n";

			auto ref1 = m1.part(row_deleting, column_deleting);
			auto ref2 = m1t.part(row_deleting, column_deleting);

			std::cout << (ref1 == m2.part(row_deleting, column_deleting)) << "\n";
			std::cout << (ref2 == m2t.part(row_deleting, column_deleting)) << "\n";
			std::cout << "\n";
		}


	std::cout << "Full - Pack Triangle \n";
	for (size_t row_start = 0; row_start <= 3; ++row_start)
		for (size_t column_start = 0; column_start <= 3; ++column_start)
			for (size_t row_end = row_start + 1; row_end <= 3; ++row_end)
				for (size_t column_end = column_start + 1; column_end <= 3; ++column_end) {
					std::cout << "part [" << row_start << "," << row_end << "," << column_start << "," << column_end << "]\n";
					std::cout << "row [" << row_start << "," << row_end << "), column [" << column_start << "," << column_end << ")\n";

					auto ref1 = m1.part(row_start, row_end, column_start, column_end);
					auto ref2 = m1t.part(row_start, row_end, column_start, column_end);

					std::cout << (ref1 == m3.part(row_start, row_end, column_start, column_end)) << "\n";
					std::cout << (ref2 == m3t.part(row_start, row_end, column_start, column_end)) << "\n";
					std::cout << "\n";
				}

	for (size_t row_deleting = 0; row_deleting < 3; ++row_deleting)
		for (size_t column_deleting = 0; column_deleting < 3; ++column_deleting) {
			std::cout << "deleting (" << row_deleting << "," << column_deleting << ") \n";

			auto ref1 = m1.part(row_deleting, column_deleting);
			auto ref2 = m1t.part(row_deleting, column_deleting);

			std::cout << (ref1 == m3.part(row_deleting, column_deleting)) << "\n";
			std::cout << (ref2 == m3t.part(row_deleting, column_deleting)) << "\n";
			std::cout << "\n";
		}


	std::cout << "Full Triangle - Pack Triangle \n";
	for (size_t row_start = 0; row_start <= 3; ++row_start)
		for (size_t column_start = 0; column_start <= 3; ++column_start)
			for (size_t row_end = row_start + 1; row_end <= 3; ++row_end)
				for (size_t column_end = column_start + 1; column_end <= 3; ++column_end) {
					std::cout << "part [" << row_start << "," << row_end << "," << column_start << "," << column_end << "]\n";
					std::cout << "row [" << row_start << "," << row_end << "), column [" << column_start << "," << column_end << ")\n";

					auto ref1 = m2.part(row_start, row_end, column_start, column_end);
					auto ref2 = m2t.part(row_start, row_end, column_start, column_end);

					std::cout << (ref1 == m3.part(row_start, row_end, column_start, column_end)) << "\n";
					std::cout << (ref2 == m3t.part(row_start, row_end, column_start, column_end)) << "\n";
					std::cout << "\n";
				}

	for (size_t row_deleting = 0; row_deleting < 3; ++row_deleting)
		for (size_t column_deleting = 0; column_deleting < 3; ++column_deleting) {
			std::cout << "deleting (" << row_deleting << "," << column_deleting << ") \n";

			auto ref1 = m2.part(row_deleting, column_deleting);
			auto ref2 = m2t.part(row_deleting, column_deleting);

			std::cout << (ref1 == m3.part(row_deleting, column_deleting)) << "\n";
			std::cout << (ref2 == m3t.part(row_deleting, column_deleting)) << "\n";
			std::cout << "\n";
		}
}