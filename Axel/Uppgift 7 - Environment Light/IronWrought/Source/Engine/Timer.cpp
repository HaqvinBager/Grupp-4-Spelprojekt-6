#include "stdafx.h"
#include "Timer.h"

CTimer* CTimer::ourInstance = nullptr;

//Total duration in seconds since start
float CTimer::Time()
{
	return ourInstance->TotalTime();
}

float CTimer::Dt()
{
	return ourInstance->Mark();
}

CTimer::CTimer()
{
	ourInstance = this;
	myLast = std::chrono::steady_clock::now();
	myFirst = myLast;
}

CTimer::~CTimer()
{
	ourInstance = nullptr;
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

float CTimer::TotalTime() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - myFirst).count();
}