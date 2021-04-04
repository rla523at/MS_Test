#include "../INC/Text.h"

Text::Text(const std::string& file_path){
	std::ifstream read_file(file_path);
	if (!read_file.is_open())
		FATAL_ERROR("Fail to open" + file_path);

	std::string file_string;
	while (std::getline(read_file, file_string))
		this->sentence_set_.emplace_back(file_string);
}

Text::Text(std::ifstream& read_file_stream, const size_t num_line) {
	if (!read_file_stream.is_open() || num_line == 0)
		return;
	
	std::string sentence;

	size_t line_count = 1;
	while (std::getline(read_file_stream, sentence)) {
		this->sentence_set_.emplace_back(std::move(sentence));
		
		if (line_count++ == num_line)
			break;
	}
}

Text::Text(std::ifstream& read_file_stream, const std::streampos& start_position, const size_t num_line) {
	if (!read_file_stream.is_open() || num_line == 0)
		return;

	read_file_stream.seekg(start_position);

	std::string sentence;

	size_t line_count = 1;
	while (std::getline(read_file_stream, sentence)) {
		this->sentence_set_.emplace_back(std::move(sentence));

		if (line_count++ == num_line)
			break;
	}
}

void Text::add_Write(const std::string& file_path) const{
	std::ofstream outfile(file_path, std::ios::app);

	if (!outfile.is_open())
		FATAL_ERROR("Fail to open" + file_path);

	const auto num_sentence = this->sentence_set_.size();
	for (size_t i = 0; i < num_sentence - 1; ++i)
		outfile << this->sentence_set_[i] << "\n";
	outfile << this->sentence_set_[num_sentence - 1];

	outfile.close();
}

void Text::write(const std::string& file_path) const{
	std::ofstream outfile(file_path);

	if (!outfile.is_open())
		FATAL_ERROR("Fail to open" + file_path);

	const auto num_sentence = this->sentence_set_.size();
	for (size_t i = 0; i < num_sentence - 1; ++i)
		outfile << this->sentence_set_[i] << "\n";
	outfile << this->sentence_set_[num_sentence - 1];

	outfile.close();
}