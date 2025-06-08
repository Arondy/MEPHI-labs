#include <iostream>
#include <cmath>
#include <fcntl.h>
#include <unistd.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <x86intrin.h>

#define MAX_ITERATIONS (int) 1e8

void measure_initialization_and_return() {
	double total_time11 = 0.0, total_time22 = 0.0, total_time12 = 0.0, total_time21 = 0.0;

	int fd = open("/dev/rtc", O_RDONLY);
	struct rtc_time rtc_start_time, rtc_end_time;

	for (int i = 0; i < MAX_ITERATIONS; ++i) {
		ioctl(fd, RTC_RD_TIME, &rtc_start_time);
		ioctl(fd, RTC_RD_TIME, &rtc_end_time);
		close(fd);
		total_time22 += (rtc_end_time.tm_sec - rtc_start_time.tm_sec) * 1e9;
	}

	double avg_time11 = fabs(total_time11 / MAX_ITERATIONS);
	double avg_time22 = fabs(total_time22 / MAX_ITERATIONS);
	double avg_time12 = fabs(total_time12 / MAX_ITERATIONS);
	double avg_time21 = fabs(total_time21 / MAX_ITERATIONS);

	std::cout << "Clock __rdtsc() and RTC:\n";
	std::cout << "A1 + A2 = " << avg_time11 << " ticks\n";
	std::cout << "B1 + B2 = " << avg_time22 << " ns\n";
	std::cout << "A1 + B2 = " << avg_time12 << " ns\n";
	std::cout << "B1 + A2 = " << avg_time21 << " ns\n";
}

int main() {
	measure_initialization_and_return();
	return 0;
}