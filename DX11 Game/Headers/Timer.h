#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
	Timer();
	double GetMilliSecondsElapsed();
	void Restart();
	bool Stop();
	bool Start();
private:
	bool isRunning = false;
#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
#else
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> stop;
#endif
};

#endif