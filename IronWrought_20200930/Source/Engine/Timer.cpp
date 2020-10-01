#include "stdafx.h"
#include "Timer.h"

CTimer::CTimer()
{
    myLast = std::chrono::steady_clock::now();
}

float CTimer::Mark()
{
    const auto old = myLast;
    myLast = std::chrono::steady_clock::now();
    const std::chrono::duration<float> dt = myLast - old; 
    return dt.count();
}

float CTimer::Peek() const
{
    return std::chrono::duration<float>(std::chrono::steady_clock::now() - myLast).count();
}