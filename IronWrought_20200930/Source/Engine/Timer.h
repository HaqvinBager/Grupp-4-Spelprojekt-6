#pragma once
#include <chrono>

class CTimer {
public:
	CTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point myLast;
};