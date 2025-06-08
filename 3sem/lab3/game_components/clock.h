#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <thread>
#include <memory>

namespace Components {
    using namespace std::chrono;
    using std::pair, std::mutex, std::thread, std::shared_ptr, std::cout, std::endl, std::make_shared, std::lock_guard;

    static mutex clockResMutex;
    static mutex clockTimeMutex;
    class Time {
    public:
        int seconds;
        int minutes;
        int hours;
        Time() = default;
        explicit Time(int t);
        [[nodiscard]] int getTimeInSeconds() const;
        void formTime();
        friend std::ostream &operator<<(std::ostream &os, const Time &time1);
        bool operator==(int value) const;
    };

    class Clock {
    private:
        Time time1;
        Time time2;
        int increment;
        bool flag = true;
        bool stopFlag = false;
        shared_ptr<thread> clockThread;
        void decreaseTime(int &result);
        void stopThread();
    public:
        Clock() = default;
        Clock(int t1, int t2, int inc): time1{t1}, time2{t2}, increment{inc}{};
        Clock(int t1, int t2, int inc, bool flag): time1{t1}, time2{t2}, increment{inc}, flag{flag}{};
        [[nodiscard]] pair<Time, Time> getTime() const{ return {time1, time2}; };
        [[nodiscard]] int getInc() const{ return increment; };
        void increaseTime1();
        void increaseTime2();
        void clockFlagFlip();
        void startThread(int &result);
        ~Clock(){ stopThread(); };
    };
}

#endif