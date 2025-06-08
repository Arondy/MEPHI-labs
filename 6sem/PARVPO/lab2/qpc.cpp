#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h>

#define MAX_ITERATIONS (int) 1e8

void measure_qpc_resolution(){
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER previous_time, current_time;
	std::vector<double> intervals(MAX_ITERATIONS);
	int count = 0;

	QueryPerformanceCounter(&previous_time);

	for (int i = 0; i < MAX_ITERATIONS; ++i){
		QueryPerformanceCounter(&current_time);

		if (current_time.QuadPart != previous_time.QuadPart){
			double delta = static_cast<double>(current_time.QuadPart - previous_time.QuadPart);
			intervals[count++] = delta;
			previous_time = current_time;
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

	std::cout << "Resolution (average interval): " << mean / frequency.QuadPart * 1e9 << " ns\n";
	std::cout << "Mean Absolute Deviation: " << mean_absolute_deviation / frequency.QuadPart * 1e9 << " ns\n";
	std::cout << "Standard Deviation: " << standard_deviation / frequency.QuadPart * 1e9 << " ns\n";
}

int main(){
	measure_qpc_resolution();
	return 0;
}
