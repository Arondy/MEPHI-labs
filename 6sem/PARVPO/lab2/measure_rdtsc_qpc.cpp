#include <chrono>
#include <iostream>
#include <cmath>
#include <intrin.h>
#include <thread>
#include <vector>
#include <windows.h>

#define MAX_ITERATIONS (int) 1e8

using std::vector;

double get_tsc_frequency(){
	unsigned long long start_tsc = __rdtsc();
	auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	unsigned long long end_tsc = __rdtsc();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	return (end_tsc - start_tsc) / elapsed.count(); // TSC в Гц
}

// Относительно rdtsc
void measure_initialization_and_return(){
	double total_time11 = 0.0, total_time22 = 0.0, total_time12 = 0.0, total_time21 = 0.0;
	LARGE_INTEGER frequency, qpc_start_time, qpc_end_time;
	QueryPerformanceFrequency(&frequency);
	unsigned long long rdtsc_start_time, rdtsc_end_time;
	double tsc_freq = get_tsc_frequency();

	for (int i = 0; i < MAX_ITERATIONS; ++i){
		rdtsc_start_time = __rdtsc();
		rdtsc_end_time = __rdtsc();
		total_time11 += (rdtsc_end_time - rdtsc_start_time) / tsc_freq * 1e9;

		QueryPerformanceCounter(&qpc_start_time);
		QueryPerformanceCounter(&qpc_end_time);
		total_time22 += (qpc_end_time.QuadPart - qpc_start_time.QuadPart) * 1e9 / frequency.QuadPart;

		// rdtsc_start_time = __rdtsc();
		// QueryPerformanceCounter(&qpc_end_time);
		// total_time12 += qpc_end_time.QuadPart / frequency.QuadPart * 1e9 - rdtsc_start_time / tsc_freq * 1e9;
		//
		// QueryPerformanceCounter(&qpc_end_time);
		// rdtsc_start_time = __rdtsc();
		// total_time21 += rdtsc_start_time / tsc_freq * 1e9 - qpc_end_time.QuadPart / frequency.QuadPart * 1e9;
	}

	double avg_time11 = fabs(total_time11 / MAX_ITERATIONS);
	double avg_time22 = fabs(total_time22 / MAX_ITERATIONS);
	double avg_time12 = fabs(total_time12 / MAX_ITERATIONS);
	double avg_time21 = fabs(total_time21 / MAX_ITERATIONS);

	std::cout << "Clock __rdtsc() and QueryPerformanceCounter:\n";
	std::cout << "A1 + A2 = " << avg_time11 << " ns\n";
	std::cout << "B1 + B2 = " << avg_time22 << " ns\n";
	std::cout << "A1 + B2 = " << avg_time21 << " ns\n";
	std::cout << "B1 + A2 = " << avg_time12 << " ns\n";
}

int main(){
	measure_initialization_and_return();
	return 0;
}
