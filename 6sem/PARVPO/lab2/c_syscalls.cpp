#include <algorithm>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <vector>

#define MAX_ITERATIONS (int) 1e7

using std::vector;

vector<double> measure_time(const int &clock){
	struct timespec previous_time, current_time;
	vector<double> intervals(MAX_ITERATIONS);
	int count = 0;

	clock_gettime(clock, &previous_time);

	for (int i = 0; i < MAX_ITERATIONS; ++i){
		clock_gettime(clock, &current_time);

		if (current_time.tv_sec != previous_time.tv_sec || current_time.tv_nsec != previous_time.tv_nsec){
			double delta = (current_time.tv_sec - previous_time.tv_sec) * 1e9 +
			               (current_time.tv_nsec - previous_time.tv_nsec);
			intervals[count++] = delta;
			previous_time = current_time;
		}
	}
	intervals.resize(count);
	return intervals;
}

void measure_deviations(const vector<double> &intervals){
	double sum = 0.0, mean = 0.0, variance = 0.0, mean_absolute_deviation = 0.0;

	for (const double &interval: intervals){
		sum += interval;
	}
	mean = sum / intervals.size();

	for (const double &interval: intervals){
		mean_absolute_deviation += fabs(interval - mean);
	}
	mean_absolute_deviation /= intervals.size();

	for (const double &interval: intervals){
		variance += pow(interval - mean, 2);
	}
	variance /= intervals.size();
	double standard_deviation = sqrt(variance);

	printf("Resolution: %.2f ns\n", mean);
	printf("Mean Absolute Deviation: %.2f ns\n", mean_absolute_deviation);
	printf("Standard Deviation: %.2f ns\n", standard_deviation);
}

void get_clock_resolution(const int &clock){
	struct timespec resolution;

	if (clock_getres(clock, &resolution) == 0){
		std::cout << "Resolution of " << clock << ": "
				<< resolution.tv_sec << " seconds, "
				<< resolution.tv_nsec << " nanoseconds\n";
	}
}

void measure_initialization_and_return(const int &clock1, const int &clock2){
	struct timespec start_time, end_time;
	double total_time11 = 0.0, total_time22 = 0.0, total_time12 = 0.0, total_time21 = 0.0;
	double base_diff = 0.0;

	if (clock1 == 0 and clock2 == 1){
		struct timespec clock1_base, clock2_base;
		clock_gettime(clock1, &clock1_base);
		clock_gettime(clock2, &clock2_base);
		base_diff = (clock2_base.tv_sec - clock1_base.tv_sec) * 1e9 + (clock2_base.tv_nsec - clock1_base.tv_nsec);
	}

	for (int i = 0; i < MAX_ITERATIONS; ++i){
		clock_gettime(clock1, &start_time);
		clock_gettime(clock1, &end_time);
		total_time11 += (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);

		clock_gettime(clock2, &start_time);
		clock_gettime(clock2, &end_time);
		total_time22 += (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);

		clock_gettime(clock1, &start_time);
		clock_gettime(clock2, &end_time);
		total_time12 += (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec) -
				base_diff;

		clock_gettime(clock2, &start_time);
		clock_gettime(clock1, &end_time);
		total_time21 += (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec) +
				base_diff;
	}

	double avg_time11 = fabs(total_time11 / MAX_ITERATIONS);
	double avg_time22 = fabs(total_time22 / MAX_ITERATIONS);
	double avg_time12 = fabs(total_time12 / MAX_ITERATIONS);
	double avg_time21 = fabs(total_time21 / MAX_ITERATIONS);

	std::cout << "Clock " << clock1 << " and " << clock2 << ":\n";

	std::cout << "A1 + A2 = " << avg_time11 << " ns\n";
	std::cout << "B1 + B2 = " << avg_time22 << " ns\n";
	std::cout << "A1 + B2 = " << avg_time21 << " ns\n";
	std::cout << "B1 + A2 = " << avg_time12 << " ns\n";
}

void measure_all_clocks_res_and_deviations(){
	for (int clock: {
		     CLOCK_REALTIME, CLOCK_MONOTONIC, CLOCK_THREAD_CPUTIME_ID, CLOCK_PROCESS_CPUTIME_ID, CLOCK_REALTIME_COARSE
	     }){
		const vector<double> intervals = measure_time(clock);
		measure_deviations(intervals);
		get_clock_resolution(clock);
	}
}

void measure_all_clocks_initialization_and_return(){
	int CLOCKS_EVEN_NUMBER = 6;
	int clocks[CLOCKS_EVEN_NUMBER] = {
		CLOCK_REALTIME, CLOCK_MONOTONIC, CLOCK_THREAD_CPUTIME_ID, CLOCK_PROCESS_CPUTIME_ID, CLOCK_REALTIME_COARSE,
		CLOCK_REALTIME
	};

	for (int i = 0; i < CLOCKS_EVEN_NUMBER; i += 2){
		measure_initialization_and_return(clocks[i], clocks[i + 1]);
	}
}

int main(){
	// measure_all_clocks_res_and_deviations();
	measure_all_clocks_initialization_and_return();
	return 0;
}
