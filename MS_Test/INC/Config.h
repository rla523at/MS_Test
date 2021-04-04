#pragma once

#include "Text.h"

#include <map>


struct SolverOption
{
	size_t dimension;

	std::string governing_equation;
	std::string numerical_flux_name;

	std::string time_scheme_name;

	std::string time_step_type_name;
	double time_step_constant;

	std::string stop_condition_type_name;
	double stop_condition_constant;

	std::string post_condition_type_name;
	double post_condition_constant;
};


struct PostOption
{
	std::string post_file_name;
	size_t post_order;
	std::string post_solution_type;
	size_t dimension;
};


class Config
{
private:
	std::map<std::string, std::string> name_to_value;

public:
	Config(const std::string& file_name) {
		const auto config_text = this->read_File(file_name);
		this->set_Value(config_text);
	};


	template<typename ValueType>
	ValueType get(const std::string& config_name) const {
		if (name_to_value.find(StringEditor::UpperCase(config_name)) == name_to_value.end())
			FATAL_ERROR(config_name + " is not exist in config file");
				
		return StringEditor::toValue<ValueType>(name_to_value.at(StringEditor::UpperCase(config_name)));
	};

	SolverOption read_Solver_Option(void) const;

	PostOption read_Post_Option(void) const;

	std::string Case_Description(void) const;

private:
	Text read_File(const std::string& file_name);

	void set_Value(const Text& config_text);
};


