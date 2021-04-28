#include "../INC/Text.h"

Text& Text::read(const std::string& file_path) {
	std::ifstream ifs(file_path);
	if (!ifs.is_open())
		throw std::runtime_error("Fail to open file" + file_path);

	std::string file_string;
	while (std::getline(ifs, file_string))
		this->emplace_back(file_string);

	ifs.close();
	return *this;
}

Text& Text::remove_empty_line(void) {
	this->erase(std::remove(this->begin(), this->end(), ""), this->end());
	return *this;
}

namespace StringEditor {
	std::vector<std::string> parse(const std::string& str, const char delimiter) {
		if (str.empty())
			return std::vector<std::string>();

		std::vector<std::string> parsed_string_set;
		for (auto iter1 = str.begin();;) {
			auto iter2 = std::find(iter1, str.end(), delimiter);

			if (iter1 != iter2)
				parsed_string_set.emplace_back(iter1, iter2);

			if (iter2 == str.end())
				return parsed_string_set;

			iter1 = iter2 + 1;
		}
	};
}


//	std::vector<std::string> parse(const std::string& str, const std::vector<char>& delimiter_set) {
//		if (str.empty())
//			return std::vector<std::string>();
//		if (delimiter_set.empty())
//			return std::vector<std::string>(1, str);
//
//		auto temporary_string = str;
//
//		const auto& reference_delimiter = delimiter_set.front();
//		for (const auto& other_delimiter : delimiter_set)
//		{
//			if (other_delimiter != reference_delimiter)
//				Editor::replace(temporary_string, other_delimiter, reference_delimiter);
//		}
//
//		return StringEditor::parse(temporary_string, reference_delimiter);
//	};
//
//	std::string& remove_comment(std::string& str, const std::string& comment) {
//		const auto position = Tool::find_First_Position(str, comment);
//
//		if (position == std::string::npos)
//			return str;
//		else
//			return str.erase(position, std::string::npos);
//	}
//
//	std::string& UpperCase(std::string& str) {
//		std::transform(str.begin(), str.end(), str.begin(), std::toupper);
//		return str;
//	};
//
//	std::string UpperCase(const std::string& str) {
//		auto result = str;
//		return StringEditor::UpperCase(result);
//	};
//
//	std::string UpperCase(std::string&& str) {
//		auto result = std::move(str);
//		return StringEditor::UpperCase(result);
//	};
//}
//
//
//Text::Text(std::ifstream& read_file_stream, const size_t num_line) {
//	if (!read_file_stream.is_open() || num_line == 0)
//		return;
//	
//	std::string sentence;
//
//	size_t line_count = 1;
//	while (std::getline(read_file_stream, sentence)) {
//		this->sentence_set_.emplace_back(std::move(sentence));
//		
//		if (line_count++ == num_line)
//			break;
//	}
//}
//
//Text::Text(std::ifstream& read_file_stream, const std::streampos& start_position, const size_t num_line) {
//	if (!read_file_stream.is_open() || num_line == 0)
//		return;
//
//	read_file_stream.seekg(start_position);
//
//	std::string sentence;
//
//	size_t line_count = 1;
//	while (std::getline(read_file_stream, sentence)) {
//		this->sentence_set_.emplace_back(std::move(sentence));
//
//		if (line_count++ == num_line)
//			break;
//	}
//}
//
//void Text::add_Write(const std::string& file_path) const{
//	std::ofstream outfile(file_path, std::ios::app);
//
//	if (!outfile.is_open())
//		FATAL_ERROR("Fail to open" + file_path);
//
//	const auto num_sentence = this->sentence_set_.size();
//	for (size_t i = 0; i < num_sentence - 1; ++i)
//		outfile << this->sentence_set_[i] << "\n";
//	outfile << this->sentence_set_[num_sentence - 1];
//
//	outfile.close();
//}
//
//void Text::write(const std::string& file_path) const{
	//std::ofstream outfile(file_path);

	//if (!outfile.is_open())
	//	FATAL_ERROR("Fail to open" + file_path);

	//const auto num_sentence = this->sentence_set_.size();
	//for (size_t i = 0; i < num_sentence - 1; ++i)
	//	outfile << this->sentence_set_[i] << "\n";
	//outfile << this->sentence_set_[num_sentence - 1];

	//outfile.close();
//}