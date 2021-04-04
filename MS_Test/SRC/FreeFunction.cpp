#include "../INC/FreeFunction.h"

namespace FileEditor {
	std::ifstream& move_Line(std::ifstream& file_stream, const size_t num_move_line) {
		size_t line_count = 0;
		std::string str;
		while (std::getline(file_stream, str)) {
			if (++line_count == num_move_line)
				break;
		}

		return file_stream;
	}

	void rename(const std::string& file_path, const std::string& old_name, const std::string& new_name) {
		const std::string old_path = file_path + "\\" + old_name;
		const std::string new_path = file_path + "\\" + new_name;

		int result = std::rename(old_path.data(), new_path.data());			
	}

	std::vector<std::string> read_Name(const std::string& path) {
		std::filesystem::path p(path);

		std::vector<std::string> name_set;
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(p)) {
			std::string file_path = entry.path().string();
			Editor::remove(file_path, path + "\\");
			name_set.emplace_back(std::move(file_path));
		}

		return name_set;
	}
}


namespace StringEditor {
	void erase_back(std::string& str, const size_t length) {
		if (str.size() <= length) {
			str.clear();
			return;
		}
		else
			str.erase(str.size() - length, length);
	};

	bool is_same_without_Case_Sensitivity(const std::string& str1, const std::string& str2) {
		if (StringEditor::UpperCase(str1) == StringEditor::UpperCase(str2))
			return true;
		else
			return false;
	};

	std::vector<std::string> parse(const std::string& str, const size_t position) {
		if (str.empty())
			return std::vector<std::string>();

		std::vector<std::string> parsed_string_set;

		const auto iter1 = str.begin();
		const auto iter2 = iter1 + position;

		parsed_string_set.emplace_back(iter1, iter2);
		parsed_string_set.emplace_back(iter2, str.end());

		return parsed_string_set;
	};

	std::vector<std::string> parse(const std::string& str, const char delimiter) {
		std::vector<std::string> parsed_string_set;

		if (str.empty())
			return parsed_string_set;

		const auto position_set = Tool::find_Position_Set(str, delimiter);

		auto iter1 = str.begin();
		for (const auto& position : position_set) {
			auto iter2 = str.begin() + position;

			if (iter2 == iter1) {
				iter1 = iter2 + 1;
				continue;
			}

			parsed_string_set.emplace_back(iter1, iter2);

			iter1 = iter2 + 1;
		}

		if (iter1 != str.end())
			parsed_string_set.emplace_back(iter1, str.end());

		return parsed_string_set;
	};

	std::vector<std::string> parse(const std::string& str, const std::vector<char>& delimiter_set) {
		if (str.empty())
			return std::vector<std::string>();
		if (delimiter_set.empty())
			return std::vector<std::string>(1, str);

		auto temporary_string = str;

		const auto& reference_delimiter = delimiter_set.front();
		for (const auto& other_delimiter : delimiter_set)
		{
			if (other_delimiter != reference_delimiter)
				Editor::replace(temporary_string, other_delimiter, reference_delimiter);
		}

		return StringEditor::parse(temporary_string, reference_delimiter);
	};

	std::string& remove_comment(std::string& str, const std::string& comment) {
		const auto position = Tool::find_First_Position(str, comment);

		if (position == std::string::npos)
			return str;
		else
			return str.erase(position, std::string::npos);
	}

	std::string& UpperCase(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), std::toupper);
		return str;
	};

	std::string UpperCase(const std::string& str) {
		auto result = str;
		return StringEditor::UpperCase(result);
	};

	std::string UpperCase(std::string&& str) {
		auto result = std::move(str);
		return StringEditor::UpperCase(result);
	};
}

size_t Math::combination(const size_t n, const size_t k) { 
	//calculate nCk
	if (n == k || k == 0)
		return 1;
	else
		return combination(n - 1, k - 1) + combination(n - 1, k);
}

size_t Math::combination_with_repetition(const size_t n, const size_t k) { 
	//calculate nHk			
	return Math::combination(n + k - 1, k);
}