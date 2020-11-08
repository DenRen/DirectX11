#pragma once

#include <chrono>

class Timer
{
public:

	static double GetDeltaTime ();

private:
	static std::chrono::duration <double> m_prevTime;
	static bool m_flagActivated;
};