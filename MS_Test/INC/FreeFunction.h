#pragma once

#include <algorithm>		//std::transform
#include <filesystem>
#include <fstream>			//std::ifstream
#include <iomanip>			//std::setprecision & std::noshowpoint
#include <iostream>
#include <sstream>			//std::ostringstream
#include <string>
#include <stdio.h>			//std::rename
#include <unordered_map>
#include <vector>


namespace Editor {
	// inline https://stackoverflow.com/questions/6424911/link-error-with-really-simple-functions-c-on-h-file

	template<typename C>
	std::string to_String(const C& obj) {
		return obj.to_String();
	};

	inline std::string to_String(const char arg) {
		std::ostringstream out;
		out << arg;
		return out.str();
	}

	inline std::string to_String(const char* arg) {
		return std::string(arg);
	}

	inline std::string to_String(const int i) {
		return std::to_string(i);
	}

	inline std::string to_String(const size_t arg) {
		return std::to_string(arg);
	};

	inline std::string to_String(const double arg) {
		std::ostringstream out;
		out << std::setprecision(15) << std::noshowpoint << arg;
		return out.str();
	};

	inline std::string to_String(const std::string& arg) {
		return arg;
	};

	template<typename C>
	void merge(C& obj, const C& obj2) {
		obj.merge(obj2);
	};

	template<typename C>
	void merge(C& obj1, C&& obj2) {
		obj1.merge(std::move(obj2));
	};

	template<typename T1>
	void merge(std::vector<T1>& obj1, const std::vector<T1>& obj2) {
		obj1.simple_polynomial_addition_simple_polynomial(obj1.end(), obj2.begin(), obj2.end());
	}

	template<typename T1>
	void merge(std::vector<T1>& obj1, std::vector<T1>&& obj2) {
		obj1.simple_polynomial_addition_simple_polynomial(obj1.end(), std::make_move_iterator(obj2.begin()), std::make_move_iterator(obj2.end()));
	}

	template<typename T1, typename T2>
	void merge(T1& obj1, T2&& obj2) {
		obj1.simple_polynomial_addition_simple_polynomial(obj1.end(), std::make_move_iterator(obj2.begin()), std::make_move_iterator(obj2.end()));
	}

	template <typename C, typename T>
	std::string& remove(C& obj, const T& value_to_remove) {
		obj.remove(value_to_remove);
	}

	template <typename T>
	std::string& remove(std::string& str, const T& str_to_remove);

	template <typename T>
	std::string& remove(std::string& str, const std::vector<T>& str_to_remove);

	template <typename T>
	void remove(std::vector<T>& vec, const T& value_to_remove) {
		vec.erase(std::remove(vec.begin(), vec.end(), value_to_remove), vec.end());
	}

	template <typename T1, typename T2>
	void replace(std::string& str, const T1& old_value, const T2& new_value);

	template <typename T1, typename T2>
	void replace(std::string& str, const std::vector<T1>& old_value_set, const T2& new_value);
}


namespace StringEditor {
	void erase_back(std::string& str, const size_t length);

	bool is_same_without_Case_Sensitivity(const std::string& str1, const std::string& str2);

	std::vector<std::string> parse(const std::string& str, const size_t position);

	std::vector<std::string> parse(const std::string& str, const char delimiter);

	std::vector<std::string> parse(const std::string& str, const std::vector<char>& delimiters);

	std::string& remove_comment(std::string& str, const std::string& comment);

	template<typename ValueType>
	ValueType toValue(const std::string& str) {
		std::istringstream iss(str);
		ValueType value;
		iss >> value;
		return value;
	};

	template<typename ValueType>
	std::vector<ValueType> toValue(const std::vector<std::string>& str_set) {
		std::vector<ValueType> value_set;
		value_set.reserve(str_set.size());

		for (const auto& sentence : str_set)
			value_set.push_back(StringEditor::toValue<ValueType>(sentence));

		return value_set;
	};

	std::string& UpperCase(std::string& str);

	std::string UpperCase(const std::string& str);

	std::string UpperCase(std::string&& str);
}


inline std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& str_set) {
	for (const auto& str : str_set)
		os << str << "\n";
	return os;
}

template <typename T>
std::string& operator<<(std::string& str, const T& value) {
	return str += Editor::to_String(value);
};


template <typename T>
std::string& operator<<(std::string&& str, const T& value) {
	return str += Editor::to_String(value);
}


namespace FileEditor {
	std::ifstream& move_Line(std::ifstream& file_stream, const size_t num_move_line);

	void rename(const std::string& file_path, const std::string& old_name, const std::string& new_name);

	std::vector<std::string> read_Name(const std::string& path);
}


namespace Tool {
	template <typename Index, typename T>
	std::vector<const T*> extract_Set_by_Index(const std::unordered_map<Index, T>& index_to_value, const std::vector<Index>& index_set) {
		std::vector<const T*> set;
		set.reserve(index_set.size());

		for (const auto index : index_set)
			set.emplace_back(&index_to_value.at(index));

		return set;
	}

	//find first poisition
	template<typename T>
	size_t find_First_Position(const std::string& str, const T& target, const size_t start_position = 0);

	template<typename T>
	std::streampos find_First_Position(std::ifstream& read_file, const T& target);

	template<typename T>
	std::vector<size_t> find_Position_Set(const std::string& str, const T& target);

	template <typename T>
	size_t total_size(const std::vector<std::vector<T>>& vvec) {
		size_t total_size = 0;
		for (const auto& vec : vvec)
			total_size += vec.size();
		return total_size;
	}
}


namespace Math {// for consistent expression, some mathmetical operation can't be member function
	template <typename C>
	C& abs(C& obj) {
		return obj.abs();
	}

	template <typename C>
	C abs(const C& obj) {
		C obj2 = obj;
		return obj2.abs();
	}

	template <typename C>
	C abs(C&& obj) {
		C obj2 = std::move(obj);
		return obj2.abs();
	}

	size_t combination(const size_t n, const size_t k);

	size_t combination_with_repetition(const size_t n, const size_t k);

	template <typename C>
	C& differentiate(C& obj, const size_t differential_variable) {
		return obj.differentiate(differential_variable);
	};

	template <typename C>
	C differentiate(const C& obj, const size_t differential_variable) {
		C obj2 = obj;
		return obj2.differentiate(differential_variable);
	};

	template <typename C>
	C differentiate(C&& obj, const size_t differential_variable) {
		C obj2 = std::move(obj);
		return obj2.differentiate(differential_variable);
	};

	template <typename C>
	double L2_Norm(const C& obj) {
		return obj.L2_Norm();
	}

	template <typename C>
	C& normalize(C& obj) {
		return obj.normalize();
	}

	template <typename C>
	C normalize(const C& obj) {
		C obj2 = obj;
		return obj2.normalize();
	}

	template <typename C>
	C normalize(C&& obj) {
		C obj2 = std::move(obj);
		return obj2.normalize();
	}

	template <typename C>
	C& sqrt(C& obj) {
		return obj.sqrt();
	}

	template <typename C>
	C sqrt(const C& obj) {
		C obj2 = obj;
		return obj2.sqrt();
	}

	template <typename C>
	C sqrt(C&& obj) {
		C obj2 = std::move(obj);
		return obj2.sqrt();
	}

	template <typename C, typename T>
	C& translate(C& obj, const T& translation_value) {
		return obj.translate(translation_value);
	};

	template <typename C, typename T>
	C translate(const C& obj, const T& translation_value) {
		C obj2 = obj;
		return obj2.translate(translation_value);
	};

	template <typename C, typename T>
	C translate(C&& obj, const T& translation_value) {
		C obj2 = std::move(obj);
		return obj2.translate(translation_value);
	};

}

namespace Editor {
	template <typename T>
	std::string& remove(std::string& str, const T& target) {
		const auto target_str = Editor::to_String(target);

		if (str.empty() || target_str.empty())
			return str;

		while (true) {
			const auto position = Tool::find_First_Position(str, target_str);

			if (position == std::string::npos)
				return str;

			str.erase(position, target_str.size());
		}
	}

	template <typename T>
	std::string& remove(std::string& str, const std::vector<T>& targets) {
		for (const auto& target : targets)
			Editor::remove(str, target);
		return str;
	}

	template <typename T1, typename T2>
	void replace(std::string& str, const T1& old_value, const T2& new_value) {
		if (old_value == new_value)
			return;

		const auto old_string = Editor::to_String(old_value);
		const auto new_string = Editor::to_String(new_value);

		if (old_string.empty())
			return;

		size_t position = 0;
		while (true) {
			position = Tool::find_First_Position(str, old_string, position);

			if (position == std::string::npos)
				return;

			str.replace(position, old_string.size(), new_string);
			position += new_string.size();
		}
	};

	template <typename T1, typename T2>
	void replace(std::string& str, const std::vector<T1>& old_value_set, const T2& new_value) {
		const auto old_string_set = Editor::to_String(old_value_set);
		const auto new_string = Editor::to_String(new_value);

		const auto& reference_old_string = old_string_set.front();
		for (const auto& old_string : old_string_set)
			Editor::replace(str, old_string, reference_old_string);
	};
}

namespace Tool {
	template<typename T>
	size_t find_First_Position(const std::string& str, const T& target, const size_t start_position) {
		const auto target_str = Editor::to_String(target); 
		const auto target_size = target_str.size();

		if (target_size == 0 || str.size() < target_size)
			return std::string::npos;

		for (auto iter = str.begin() + start_position; iter != str.end() - target_size + 1; ++iter) {
			if (std::string(iter, iter + target_size) == target_str)
				return (iter - str.begin());
		}

		return std::string::npos;
	};

	template<typename T>
	std::streampos find_First_Position(std::ifstream& read_file, const T& target) {
		const auto target_str = Editor::to_String(target); 
		const auto target_size = target_str.size();

		if (!read_file.is_open() || target_size == 0)
			return std::string::npos;

		std::size_t position = 0;
		std::string str;
		while (std::getline(read_file, str)) {
			if (str.find(target_str) != std::string::npos)
				return position + Tool::find_First_Position(str, target_str);
		
			position = read_file.tellg();
		}

		return std::string::npos;
	}

	template<typename T>
	std::vector<size_t> find_Position_Set(const std::string& str, const T& target) {
		const auto target_str = Editor::to_String(target);
		const auto target_size = target_str.size();

		if (target_size == 0 || str.size() < target_size)
			return std::vector<size_t>();

		std::vector<size_t> position_set;
		for (auto iter = str.begin(); iter != str.end() - target_size + 1; iter++) {
			if (std::string(iter, iter + target_size) == target_str)
				position_set.emplace_back(iter - str.begin());
		}

		return position_set;
	};
}