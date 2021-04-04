#include "../INC/Config.h"

void Config::set_Value(const Text& config_text){
	const char delimiter = ':';

	for (auto& sentence : config_text){		
		if (sentence.empty())	
			continue;

		auto parsed_sentence_set = StringEditor::parse(sentence, delimiter);

		auto name_sentence = StringEditor::UpperCase(parsed_sentence_set.front());
		auto& value_sentence = parsed_sentence_set.back();
		
		this->name_to_value.emplace(std::move(name_sentence), std::move(value_sentence));
	}
}

Text Config::read_File(const std::string& file_name){
	std::string file_path = "RSC/" + file_name;	
	Text config_text(file_path);

	const std::vector<char> trim_character_set = { ' ', ',', '=', '\t', '\n', '\r' };
	const std::string comment_tag = "//";

	for (auto& sentence : config_text) {
		StringEditor::remove_comment(sentence, comment_tag);
		Editor::remove(sentence, trim_character_set);
	}

	return config_text;	
};

SolverOption Config::read_Solver_Option(void) const {
	const auto dimension = this->get<std::size_t>("dimension");

	const auto governing_equation = this->get<std::string>("governingEquation");
	const auto numerical_flux_name = this->get<std::string>("numericalFlux");

	const auto time_scheme_name = this->get<std::string>("TimeScheme");

	const auto time_step_type_name = this->get<std::string>("TimeStepType");
	const auto time_step_constant = this->get<double>("TimeStepConstant");

	const auto stop_condition_type_name = this->get<std::string>("StopConditionType");
	const auto stop_condition_constant = this->get<double>("StopConditionConstant");

	const auto post_condition_type_name = this->get<std::string>("PostConditionType");
	const auto post_condition_constant = this->get<double>("PostConditionConstant");

	return { dimension, governing_equation, numerical_flux_name, time_scheme_name, time_step_type_name, time_step_constant, stop_condition_type_name, stop_condition_constant, post_condition_type_name, post_condition_constant };
}

PostOption Config::read_Post_Option(void) const {
	const auto post_file_name = this->get<std::string>("PostFileName");
	const auto post_order = this->get<size_t>("PostOrder");
	const auto post_solution_type = this->get<std::string>("PostSolutionType");
	const auto dimension = this->get<size_t>("dimension");

	return { post_file_name,post_order,post_solution_type,dimension };
}

std::string Config::Case_Description(void) const {
	const auto grid_file_name = this->get<std::string>("gridFileName");

	const auto governing_equation_name = this->get<std::string>("GoverningEquation");
	const auto dimension = this->get<size_t>("Dimension");
	const auto initial_condition_name = this->get<std::string>("InitialCondition");

	const auto solution_order = this->get<size_t>("SolutionOrder");
	const auto numerical_flux_scheme_name = this->get<std::string>("numericalflux");
	const auto time_scheme_name = this->get<std::string>("TimeScheme");
	const auto end_time = this->get<double>("StopConditionConstant");
	const auto limiter_type_name = this->get<std::string>("LimiterType");
	const auto limiting_variable_name = this->get<std::string>("LimitingVariable");

	std::string case_description;
	case_description << "=================== case description ===================\n";
	case_description << "Problem : " << governing_equation_name << " " << dimension << "D " << initial_condition_name << "\n";
	case_description << "Grid file : " << grid_file_name << "\n";
	case_description << "Solution order : P" << solution_order << "\n";
	case_description << "Numerical flux : " << numerical_flux_scheme_name << "\n";
	case_description << "Time scheme : " << time_scheme_name << "\n";
	case_description << "End time : " << end_time << "\n";
	case_description << "Limiter : " << limiter_type_name << "\n";
	case_description << "Limiting variable : " << limiting_variable_name;

	return case_description;
}