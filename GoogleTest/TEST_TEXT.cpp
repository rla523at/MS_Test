#pragma once
#include "gtest/gtest.h"
#include "../MS_Test/INC/Text.h"

GTEST_TEST(TEXT, CONSTRUCTOR1) {
	Text text1 = { "abc", "bde", "cwe" };

	std::vector<std::string> str_set = { "abc", "bde", "cwe" };
	Text text2 = str_set;

	EXPECT_EQ(text1, text2);
}
GTEST_TEST(TEXT, CONSTRUCTOR2) {
	Text text1 = { "abc", "abc", "abc" };
	Text text2(3, "abc");

	EXPECT_EQ(text1, text2);
}
//GTEST_TEST(TEXT, CONSTRUCTOR3) { //모호한 생성으로 compile time에 error 잡아내줌
//	EXPECT_ANY_THROW(Text("ABC"));
//}


GTEST_TEST(TEXT, READ1) {
	Text txt;
	
	std::string read_file_path = "abc";
	EXPECT_ANY_THROW(txt.read(read_file_path));
}
GTEST_TEST(TEXT, READ2) {		
	std::string file_name = "test.txt";
	std::ofstream ofs(file_name);
	ofs << "abc";
	ofs.close();

	Text txt;
	txt.read(file_name);

	std::string ref = "abc";
	EXPECT_EQ(txt.front(),ref);
}
GTEST_TEST(TEXT, READ3) {
	std::string file_name = "test.txt";
	std::ofstream ofs(file_name);
	ofs << "abc" << "def";
	ofs.close();

	Text txt;
	txt.read(file_name);

	std::string ref = "abcdef";
	EXPECT_EQ(txt.front(), ref);
}
GTEST_TEST(TEXT, READ4) {
	std::string file_name = "test.txt";
	std::ofstream ofs(file_name);
	ofs << "abc";
	ofs.close();

	Text txt;
	txt.read(file_name);

	Text txt2 = { "abc" };
	EXPECT_EQ(txt, txt2);
}
GTEST_TEST(TEXT, READ5) {
	std::string file_name = "test.txt";
	std::ofstream ofs(file_name);
	ofs << "abc" <<"\n\n\n" <<"bcd";
	ofs.close();

	Text txt;
	txt.read(file_name);

	const size_t ref = 4;
	EXPECT_EQ(txt.size(), ref);
}


GTEST_TEST(TEXT, REMOVE_EMPTY_LINE1) {
	std::string file_name = "test.txt";
	std::ofstream ofs(file_name);
	ofs << "abc" << "\n\n\n" << "bcd";
	ofs.close();

	Text txt;
	txt.read(file_name);
	txt.remove_empty_line();

	Text txt2 = { "abc","bcd" };
	EXPECT_EQ(txt, txt2);
}


GTEST_TEST(STRINGEDITOR, PARSE1) {
	std::string str = "a,b,c,f,d,e";
	const char delimiter = ',';
	const auto pared_str_set = StringEditor::parse(str, delimiter);

	Text ref = { "a","b","c","f","d","e" };
	EXPECT_EQ(pared_str_set, ref);
}
GTEST_TEST(STRINGEDITOR, PARSE2) {
	std::string str = ",,,,,,a,,,b,c,f,,,d,e,,,,";
	const char delimiter = ',';
	const auto pared_str_set = StringEditor::parse(str, delimiter);

	Text ref = { "a","b","c","f","d","e" };
	EXPECT_EQ(pared_str_set, ref);
}
GTEST_TEST(STRINGEDITOR, PARSE3) {
	std::string str = ",,,,,,abbc,,,bcf,,,d,e,,,,";
	const char delimiter = ',';
	const auto pared_str_set = StringEditor::parse(str, delimiter);

	Text ref = { "abbc","bcf","d","e" };
	EXPECT_EQ(pared_str_set, ref);
}
