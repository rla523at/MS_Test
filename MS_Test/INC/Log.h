#pragma once

#include "Text.h"

#include <time.h>

#define LOG Log::getInstance()
class Log
{
private:
	Log(void) = default;

public:
	static Log& getInstance(void){
		static Log instance;
		return instance;
	}

	void record_Error_Calculation_Log(const std::string& case_description, const std::string& error_calculation_result) const;

private:
	std::string Date_Description(void) const;
};
