#pragma once
#include "../../Util/Function.h"
#include "../../Util/SharedPtr.h"

#include <vector>

/*
	The state machine manages which states should be updated, rendered
	and handle input.
*/

class State;

class StateMachine
{
	using LoadingScreen = Function<void(float)>;
	using Container = std::vector<SharedPtr<State>>;
public:
	bool update(float dt);
	void render() const;
	void cleanUp();

	void setLoadingScreen(LoadingScreen renderLoadingScreen);
	void shouldExit(bool value);

	template <typename T, typename ... Args>
	void pushState(Args&& ... args)
	{
		static_assert(std::is_base_of<State, T>::value, "T must derive from State");
		
		// Alert the to-be old top state
		if (!m_states.empty())
			m_states.back()->suspend();

		m_states.emplace_back(new T(*this, std::forward<Args>(args)...));
	}

    void popState();

private:
	LoadingScreen m_renderLoadingScreen;

	Container m_states;
	bool m_shouldExit = false;
};

