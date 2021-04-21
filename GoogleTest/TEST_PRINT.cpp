#include <gtest/gtest.h>
#include <sstream>

GTEST_TEST(PRINT, PRECISION){
	{
		constexpr double val = 0.1;
		std::stringstream stream;
		stream << std::setprecision(15) << std::noshowpoint;
		stream << val;

		std::string str = stream.str();
		std::string ref = "0.1";

		EXPECT_EQ(str, ref);
	}
	{
		double val = 0.01234;
		std::stringstream stream;
		stream << std::setprecision(15) << std::noshowpoint;
		stream << val;

		std::string str = stream.str();
		std::string ref = "0.01234";
		
		EXPECT_EQ(str, ref);
	}
	{
		double val = 0.0001234;
		std::stringstream stream;
		stream << std::setprecision(15) << std::noshowpoint;
		stream << val;

		std::string str = stream.str();
		std::string ref = "0.0001234";

		EXPECT_EQ(str, ref);
	}
	{
		double val = 0.0000001234;
		std::stringstream stream;
		stream << std::setprecision(15) << std::noshowpoint;
		stream << std::fixed << val;

		std::string str = stream.str();
		std::string ref = "0.0000001234";

		EXPECT_EQ(str, ref);
	}
	{
		double val = 1.0E-17;
		std::stringstream stream;
		stream << std::setprecision(15) << val;

		std::string str = stream.str();
		std::string ref = "1e-17";

		EXPECT_EQ(str, ref);
	}
	{
		double val = 0.1;
		std::stringstream stream;
		stream << std::setprecision(15) << val;

		std::string str = stream.str();
		std::string ref = "0.1";

		EXPECT_EQ(str, ref);
	}
	{
		const size_t ref = 15;
		EXPECT_EQ(std::numeric_limits<double>::digits10, ref);
	}


	//double val = 123456789123456789.000001234;
	//std::stringstream stream;
	////stream << std::setprecision(15) << std::noshowpoint << std::fixed ;
	//stream << std::setprecision(15) << std::noshowpoint << std::fixed;
	//stream << val;

	////stream << std::setprecision(15) << std::noshowpoint << val;


	//std::string str = stream.str();
	//std::string ref = "0.000001234";
	//
	//EXPECT_EQ(str, ref);
}