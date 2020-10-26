#include "Timer.h"

bool Timer::m_flagActivated = false;
std::chrono::duration <double> Timer::m_prevTime (0);

double Timer::GetDeltaTime ()
{
	auto currTime = std::chrono::steady_clock::now ().time_since_epoch ();
	
	if (m_flagActivated)
	{
		auto dt = currTime - m_prevTime;
		m_prevTime = currTime;
		
		return dt.count () * 0.000000001;
	}
	else
	{
		m_flagActivated = true;

		m_prevTime = currTime;

		return 0;
	}

	return 0.0l;
}