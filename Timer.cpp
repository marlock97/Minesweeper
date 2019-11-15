#include "Timer.h"

void Timer::Start()
{
    start = std::chrono::system_clock::now();
}

double Timer::Stop() const
{
    std::chrono::duration<double> diff = std::chrono::system_clock::now() - start;
    return diff.count();
}
