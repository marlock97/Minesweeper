#pragma once
#include <chrono>

#define TIME_POINT std::chrono::time_point<std::chrono::system_clock>

class Timer
{
public:
    void Start();
    double Stop() const;

private:
    TIME_POINT start;
};
