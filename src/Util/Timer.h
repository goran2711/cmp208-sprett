#pragma once
#include "Function.h"

class Timer
{
public:
	Timer() = default;
	Timer(float time) : m_time(time) {}
	virtual ~Timer() = default;

	virtual float tick(float dt);
	void reset(float time);

	void setTime(float time) { m_time = time; }
	float getTime() const { return m_time; }

	bool isFinished() const { return m_time <= 0.f; }

protected:
	float m_time = 0.f;
};

// The InvocationTimer calls a function once after a set amount of time
class InvocationTimer : public Timer
{
public:
	using Action = Function<void(void)>;
	InvocationTimer() = default;
	InvocationTimer(float time, Action action) : Timer(time), m_action(action) {}

	float tick(float dt) override;

	void reset(float time, Action action);

private:
	bool m_isCalled = false;
	Action m_action = nullptr;
};