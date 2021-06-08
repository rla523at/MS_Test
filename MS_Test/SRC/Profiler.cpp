
#include "../INC/Profiler.h"

PROCESS_MEMORY_COUNTERS_EX Profiler::memory_recorder_;
std::vector<size_t> Profiler::memory_record_;
std::vector<std::chrono::steady_clock::time_point> Profiler::time_record_;


void Profiler::record_Consumed_Memory(void){		
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memory_recorder_, sizeof(memory_recorder_));

	memory_record_.push_back(memory_recorder_.PrivateUsage);
}

void Profiler::print_Consumed_Memory(void){
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memory_recorder_, sizeof(memory_recorder_));

	const auto consumed_memory = static_cast<size_t>((memory_recorder_.PrivateUsage - memory_record_.back()) / 1024.0 / 1024.0);
	std::cout << "\tconsummed_memory : " << consumed_memory << " MB\n";

	memory_record_.pop_back();
}

void Profiler::record_Consumed_Time(void){
	time_record_.push_back(std::chrono::steady_clock::now());
}

void Profiler::print_Consumed_Time(void){
	std::chrono::duration<double> consummed_time = std::chrono::steady_clock::now() - time_record_.back();
	std::cout << "\tconsummed time : " << consummed_time.count() << " s\n";

	time_record_.pop_back();
}

void Profiler::print_Consumed_Time_nano(void) {
	const auto nano_sec = std::chrono::steady_clock::now() - time_record_.back();
	//const auto consummed_time = std::chrono::duration_cast<std::chrono::milliseconds>(nano_sec);
	const auto consummed_time = std::chrono::duration<double>(nano_sec);
	std::cout << "\tconsummed time : " << consummed_time.count() << " s\n";

	time_record_.pop_back();
}

void Profiler::record_Consumed_Memory_And_Time(void){
	record_Consumed_Memory();
	record_Consumed_Time();
}

void Profiler::print_Consumed_Memory_And_Time(void) {
	if (memory_record_.size() == 0 || time_record_.size() == 0)
		throw std::runtime_error("there are no record to print!");

	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memory_recorder_, sizeof(memory_recorder_));

	const auto consummed_memory = static_cast<size_t>((memory_recorder_.PrivateUsage - memory_record_.back()) / 1024.0 / 1024.0);
	const std::chrono::duration<double> consummed_time = std::chrono::steady_clock::now() - time_record_.back();

	std::cout << "\tconsummed memory : " << consummed_memory << " MB" << "  \tconsummed time : " << consummed_time.count() << " s\n";

	memory_record_.pop_back();
	time_record_.pop_back();
}