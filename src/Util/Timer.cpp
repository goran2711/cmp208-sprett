#include "Timer.h"

float Timer::tick(float dt)
{
	if (m_time > 0.f)
		m_time -= dt;

	return m_time;
}

void Timer::reset(float time)
{
	m_time = time;
}

float InvocationTimer::tick(float dt)
{
	if (Timer::tick(dt) <= 0.f && !m_isCalled)
	{
		m_action();
		m_isCalled = true;
	}

	return m_time;
}

void InvocationTimer::reset(float time, Action action)
{
	m_time = time;
	m_isCalled = false;
	m_action = action;
}
