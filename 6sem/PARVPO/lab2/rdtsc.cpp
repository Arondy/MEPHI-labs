#include <iostream>
#include <vector>
#include <cmath>
#include <intrin.h>
#include <thread>
#include <chrono>

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

void measure_rdtsc_resolution(){
	vector<double> intervals(MAX_ITERATIONS);
	int count = 0;
	unsigned long long previous_tsc = __rdtsc();

	for (int i = 0; i < MAX_ITERATIONS; ++i){
		unsigned long long current_tsc = __rdtsc();

		if (current_tsc != previous_tsc){
			double delta = static_cast<double>(current_tsc - previous_tsc);
			intervals[count++] = delta;
			previous_tsc = current_tsc;
		}
	}

	intervals.resize(count);

	double sum = 0.0, mean = 0.0, variance = 0.0, mean_absolute_deviation = 0.0;

	for (double interval: intervals){
		sum += interval;
	}
	mean = sum / intervals.size();

	for (double interval: intervals){
		mean_absolute_deviation += std::fabs(interval - mean);
		variance += std::pow(interval - mean, 2);
	}
	mean_absolute_deviation /= intervals.size();
	variance /= intervals.size();
	double standard_deviation = std::sqrt(variance);
	double tsc_frequency = get_tsc_frequency();

	std::cout << "Resolution (average interval): " << mean / tsc_frequency * 1e9 << " ns\n";
	std::cout << "Mean Absolute Deviation: " << mean_absolute_deviation / tsc_frequency * 1e9 << " ns\n";
	std::cout << "Standard Deviation: " << standard_deviation / tsc_frequency * 1e9 << " ns\n";
	std::cout << "tsc_frequency: " << tsc_frequency / 1e9 << " GHz\n";
}

int main(){
	measure_rdtsc_resolution();
	return 0;
}
