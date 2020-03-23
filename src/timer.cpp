#include "timer.h"

Timer::Timer() : started_(false) {}

Timer::~Timer() {}

void Timer::start()
{
    started_ = true;
    t1_ = std::chrono::high_resolution_clock::now();
}

double Timer::elapsed()
{
    if (!started_) {
        return 0.0;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    auto diff = t2 - t1_;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);

    return ms.count();
}
