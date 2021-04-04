#include "../INC/Log.h"

void Log::record_Error_Calculation_Log(const std::string& case_description, const std::string& error_calculation_sentence) const {
	const auto date_description = this->Date_Description();

	Text error_calculation_log;
	error_calculation_log << date_description << case_description << error_calculation_sentence;

	const std::string file_path = "Log/Error_Calculation.txt"; 
	error_calculation_log.add_Write(file_path);
}

std::string Log::Date_Description(void) const {
	time_t date_calculator = time(nullptr);
	tm date;
	localtime_s(&date, &date_calculator);

	std::string date_description;
	date_description << "==================================================\n";
	date_description << "log date : " << date.tm_year + 1900 << "." << date.tm_mon + 1 << "." << date.tm_mday << "\t" << date.tm_hour << ":" << date.tm_min;

	return date_description;
}