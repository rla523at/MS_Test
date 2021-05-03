#pragma once

#include <chrono>
#include <iostream>
#include <windows.h>	//compile error is occured when windows.h is not includes before psapi.h
#include <psapi.h>
#include <vector>


class Profiler
{
private:
	static PROCESS_MEMORY_COUNTERS_EX memory_recorder_;

	static std::vector<size_t> memory_record_;

	static std::vector<std::chrono::steady_clock::time_point> time_record_;


public:
	static void record_Consumed_Memory(void);
	static void print_Consumed_Memory(void);
	static void record_Consumed_Time(void);
	static void print_Consumed_Time(void);
	static void print_Consumed_Time_nano(void);
	static void record_Consumed_Memory_And_Time(void);
	static void print_Consumed_Memory_And_Time(void);	
};


#define RECORD_CONSUMED_MEMORY Profiler::record_Consumed_Memory()
#define PRINT_CONSUMED_MEMORY Profiler::print_Consumed_Memory()
#define RECORD_CONSUMED_TIME Profiler::record_Consumed_Time()
#define PRINT_CONSUMED_TIME Profiler::print_Consumed_Time()
#define PRINT_CONSUMED_TIME_NANO Profiler::print_Consumed_Time_nano()
#define RECORD_CONSUMED_MEMORY_AND_TIME Profiler::record_Consumed_Memory_And_Time()
#define PRINT_CONSUMED_MEMORY_AND_TIME Profiler::print_Consumed_Memory_And_Time()
