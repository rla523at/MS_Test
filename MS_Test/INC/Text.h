#pragma once

#include <fstream>	//file stream
#include <stdexcept>
#include <string>
#include <vector>

class Text : public std::vector<std::string>
{
public:
	template <typename ... Vals>
	Text(Vals&&... values) : std::vector<std::string>(std::forward<Vals>(values)...) {};
	Text(std::initializer_list<std::string> list) : std::vector<std::string>( list ) {};

	Text& read(const std::string& read_file_path);
	Text& remove_empty_line(void);
};


//#include "FatalError.h"
//#include "FreeFunction.h"

//
//
//class Text
//{
//	using Iter = std::vector<std::string>::iterator;
//	using CIter = std::vector<std::string>::const_iterator;
//
//private:
//	std::vector<std::string> sentence_set_;
//
//public:
//	Text(void) = default;
//	
//	Text(const std::vector<std::string>& other_sentence_set)
//		: sentence_set_(other_sentence_set) {};
//
	//Text(std::initializer_list<std::string> list)
	//	: sentence_set_{ list } {};
//
//	Text(const std::string& read_file);
//
//	Text(std::ifstream& read_file_stream, const size_t num_line = -1);
//
//	Text(std::ifstream& read_file_stream, const std::streampos& start_position, const size_t num_line = -1);
//
//
//
//	template <typename T>
//	Text& operator<<(const T& value) {
//		this->sentence_set_.emplace_back(Editor::to_String(value));
//		return *this;
//	};
//
//	Text& operator<<(std::string&& sentence) {
//		this->sentence_set_.emplace_back(std::move(sentence));
//		return *this;
//	};
//
//	std::string& operator[](const size_t index) {
//		return this->sentence_set_[index];
//	};
//
//	const std::string& operator[](const size_t index) const {
//		return this->sentence_set_[index];
//	};
//
//
//	std::string& back(void) {
//		return this->sentence_set_.back();
//	};
//
//	const std::string& back(void) const {
//		return this->sentence_set_.back();
//	};
//
//	Iter begin(void) {
//		return this->sentence_set_.begin(); 
//	};
//
//	CIter begin(void) const {
//		return this->sentence_set_.begin();
//	};
//
//	Iter end(void) {
//		return this->sentence_set_.end();
//	};
//
//	CIter end(void) const {
//		return this->sentence_set_.end();
//	};
//
//	Iter erase(const Iter& iter) {
//		return this->sentence_set_.erase(iter);
//	};
//
//	std::string& front(void) {
//		return this->sentence_set_.front();
//	};
//
//	const std::string& front(void) const {
//		return this->sentence_set_.front();
//	};
//
//	void pop_back(void) {
//		this->sentence_set_.pop_back();
//	};
//
//	void reserve(const size_t required_capacity) {
//		this->sentence_set_.reserve(required_capacity);
//	};
//
//	size_t size(void) const {
//		return this->sentence_set_.size();
//	};
//
//
//	void add_Write(const std::string& file_path) const;
//
//	void merge(const Text& other) {
//		this->sentence_set_.insert(this->end(), other.begin(), other.end());
//	};
//
//	void merge(Text&& other) {
//		this->sentence_set_.insert(this->end(), std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()));
//	};
//
//	template <typename T>
//	void remove(const T& target) {
//		this->sentence_set_.erase(std::remove(this->begin(), this->sentence_set_.end(), Editor::to_String(target)), this->sentence_set_.end());
//	};
//
//	void remove_empty(void) {
		//this->sentence_set_.erase(std::remove_if(this->begin(), this->end(), [](const std::string& str) {return str.empty(); }), this->end());
//	};
//
//	void replace(const std::string& old_str, const std::string& new_str) {
//		for (std::string& sentence : this->sentence_set_)
//			Editor::replace(sentence, old_str, new_str);
//	};
//
//	void write(const std::string& file_path) const;
//};
//
//
//inline std::ostream& operator<<(std::ostream& ostream, const Text& text) {
//	for (const auto& sentence : text)
//		ostream << sentence << "\n";
//	return ostream;
//}
