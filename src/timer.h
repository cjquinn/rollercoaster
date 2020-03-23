#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer 
{
public:
    Timer();
    ~Timer();

    void start();
    double elapsed();

private:
    std::chrono::high_resolution_clock::time_point t1_, t2_;
    bool started_;
};

#endif
