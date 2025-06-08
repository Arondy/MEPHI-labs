#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <cmath>
#include <vector>

#define MAX_ITERATIONS (int) 1e5

using std::vector;

void measure_rtc_resolution(){
	int fd = open("/dev/rtc", O_RDONLY);
	if (fd == -1){
		std::cerr << "Failed to open /dev/rtc: " << std::endl;
		return;
	}

	struct rtc_time previous_time, current_time;
	vector<double> intervals;


	if (ioctl(fd, RTC_RD_TIME, &previous_time) == -1){
		std::cerr << "Failed to read initial RTC time: " << std::endl;
		close(fd);
		return;
	}

	for (int i = 0; i < MAX_ITERATIONS; ++i){
		if (ioctl(fd, RTC_RD_TIME, &current_time) == -1){
			std::cerr << "Failed to read RTC time: " << std::endl;
			close(fd);
			return;
		}


		if (current_time.tm_sec != previous_time.tm_sec ||
		    current_time.tm_min != previous_time.tm_min ||
		    current_time.tm_hour != previous_time.tm_hour){
			double delta = (current_time.tm_hour - previous_time.tm_hour) * 3600 +
			               (current_time.tm_min - previous_time.tm_min) * 60 +
			               (current_time.tm_sec - previous_time.tm_sec);

			intervals.push_back(delta);
			previous_time = current_time;
		}
	}

	close(fd);


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


	std::cout << "Resolution (average interval): " << mean << " seconds\n";
	std::cout << "Mean Absolute Deviation: " << mean_absolute_deviation << " seconds\n";
	std::cout << "Standard Deviation: " << standard_deviation << " seconds\n";
}

int main(){
	measure_rtc_resolution();
	return 0;
}
