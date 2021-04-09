
#include "../INC/MathVector.h"

int main(void)
{
	MathVector v1 = { 0.1 };
	MathVector v2 = { 0.1,0.4 };
	MathVector ref = { 0.2,0.4 };
	v1 += v2;
}
