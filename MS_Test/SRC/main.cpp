
//#include "../INC/TestFigure.h"
//#include "../INC/TestPolynomial.h"

int main(void)
{
	//TestCallOperator();


	//Test_Quad_Orthonormality3(6);



	//MathVector variable = { 1,2 };

	//Polynomial p1(1);
	//Polynomial p2(Monomial(0));
	//Polynomial p3(Monomial(1));

	//auto ib1 = p1 + p2 + p3;
	//auto ib2 = 3.0 * p2 * p3;

	//auto b1 = ib1 + ib2;
	//auto b2 = ib1 * ib2;
	//auto b3 = b1 + b2;
	//auto b4 = b1 * b2;
	//auto b5 = b1 * b1;

	////std::cout << ib1.to_String() << "\n";
	////std::cout << ib1.to_Extended_String() << "\n";
	////std::cout << ib2.to_String() << "\n";
	////std::cout << ib2.to_Extended_String() << "\n";
	////std::cout << b1.to_String() << "\n";
	////std::cout << b1.to_Extended_String() << "\n";
	//std::cout << b2.to_String() << "\n";
	//std::cout << b2.to_Extended_String() << "\n";
	//std::cout << b3.to_String() << "\n";
	//std::cout << b3.to_Extended_String() << "\n";
	//std::cout << b4.to_String() << "\n";
	//std::cout << b4.to_Extended_String() << "\n";
	//std::cout << b5.to_String() << "\n";
	//std::cout << b5.to_Extended_String() << "\n";





	//std::cout << ib1(variable) << "\n";
	//std::cout << ib2(variable) << "\n";


	//auto test = (ib1 + ib2) * ib1;
	//std::cout << "debug";

	//auto b1 = ib1 + ib2;
	//auto b2 = ib1 * ib2;
	//auto b3 = b1 + b2;
	//auto b5 = b1 * b1;
	//auto b7 = b3 * b3;


	//std::cout << b1(variable) << "\n";
	//std::cout << b2(variable) << "\n";
	//std::cout << b3(variable) << "\n";
	//std::cout << b5(variable) << "\n";
	//std::cout << b7(variable) << "\n";

	//auto test1 = ib1 - ib2;
	//auto test2 = b2 - b2;
	//auto test3 = b2 - 2 * b1;

	//std::cout << test1(variable) << "\n";
	//std::cout << test2(variable) << "\n";
	//std::cout << test3(variable) << "\n";



	//auto b1 = ib1;
	//b1 += ib2;

	//auto b2 = ib1;
	//b2 *= ib2;

	//auto b3 = b1;
	//b3 += b2;

	//auto b4 = b1;
	//b4 *= b2;

	//auto b5 = b1;
	//b5 *= b1;

	//auto b6 = b2;
	//b6 += b2;

	//auto b7 = b3;
	//b7 *= b3;

	//auto b8 = b4;
	//b8 += b4;

	//std::cout << b1(variable) << "\n";
	//std::cout << b2(variable) << "\n";
	//std::cout << b3(variable) << "\n";
	//std::cout << b4(variable) << "\n";
	//std::cout << b5(variable) << "\n";
	//std::cout << b6(variable) << "\n";
	//std::cout << b7(variable) << "\n";
	//std::cout << b8(variable) << "\n";



	//try MonoOply
	//MonoOply mo1;
	//mo1 += p1;
	//mo1 += p2;
	//mo1 += p3;

	//MonoOply mo2(p1);
	//mo2 += 3.0*p1;
	//mo2 += p2;
	//mo2 += p3;

	//PolynomialSet b1 = mo1 + mo2;
	//auto b2 = mo1 * mo2;
	//auto b3 = b1 + b2;

	////try funtion set 
	//FunctionSet<Polynomial> ib1;
	//ib1 += p1;
	//ib1 += p2;
	//ib1 += p3;

	//FunctionSet<Polynomial> ib2(p1);
	//ib2 *= 3.0 * p1;
	//ib2 *= p2;
	//ib2 *= p3;

	//std::cout << ib1(variable) << "\n";
	//std::cout << ib2(variable) << "\n";

	//FunctionSet<FunctionSet<Polynomial>> b1(ib1);
	//b1 += ib2;

	//FunctionSet<FunctionSet<Polynomial>> b2(ib1);
	//b2 *= ib2;

	//std::cout << b1(variable) << "\n";
	//std::cout << b2(variable) << "\n";


	//Rational r1;
	//r1 += p1;
	//r1 += p2;
	//r1 += p3;

	//Rational r2;
	//r2 += p1;
	//r2 += 2.0 * p2;
	//r2 += p3;

	//Rational r3 = r1;
	//r3 += r2;
	//std::cout << r3(variable) << "\n";


	//Rational debug = r3;
	//debug *= r3;

	//std::cout << debug(variable) << "\n";

	//Rational r4 = r1;
	//r4 *= r2;

	//Rational r5 = r3;
	//r5 *= r4;

	//Rational r7 = r2;
	//r7 *= r2;

	//Rational r6 = r5;	
	//r6 += r7;

	//MathVector variable = { 1,1 };
	//std::cout << r1(variable) << "\n";
	//std::cout << r2(variable) << "\n";
	//std::cout << r3(variable) << "\n";
	//std::cout << r4(variable) << "\n";
	//std::cout << r5(variable) << "\n";
	//std::cout << r6(variable) << "\n";

}
