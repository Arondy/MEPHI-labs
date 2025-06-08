#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cmath>

#define MAX_ITERATIONS (int) 2e8

using namespace std::chrono;
using std::vector;

vector<double> measure_time(auto clock_func){
	vector<double> intervals(MAX_ITERATIONS);
	int count = 0;
	auto previous_time = clock_func();

	for (int i = 0; i < MAX_ITERATIONS; ++i){
		auto current_time = clock_func();

		if (current_time != previous_time){
			auto delta = duration_cast<nanoseconds>(current_time - previous_time).count();
			intervals[count++] = static_cast<double>(delta);
			previous_time = current_time;
		}
	}
	intervals.resize(count);
	return intervals;
}

void measure_deviations(const vector<double> &intervals){
	double sum = 0.0, mean = 0.0, variance = 0.0, mean_absolute_deviation = 0.0;

	// std::cout << *std::ranges::max_element(intervals) << std::endl;

	for (double interval: intervals){
		sum += interval;
	}
	mean = sum / intervals.size();

	for (double interval: intervals){
		mean_absolute_deviation += abs(interval - mean);
	}
	mean_absolute_deviation /= intervals.size();

	for (double interval: intervals){
		variance += pow(interval - mean, 2);
	}
	variance /= intervals.size();
	double standart_deviation = std::sqrt(variance);

	std::cout << "Resolution: " << mean << " ns\n";
	std::cout << "mean_absolute_deviation: " << mean_absolute_deviation << " ns\n";
	std::cout << "standart_deviation: " << standart_deviation << " ns\n";
}

int main(){
	vector<double> intervals = measure_time(system_clock::now);
	measure_deviations(intervals);

	intervals = measure_time(steady_clock::now);
	measure_deviations(intervals);

	intervals = measure_time(high_resolution_clock::now);
	measure_deviations(intervals);
	return 0;
}
