#include "StateMachine.h"
#include "State.h"

bool StateMachine::update(float dt)
{
	// Make a copy of the stack to loop through in case any states are
	// popped during update. Otherwise the iteratos would be invalidated.
	auto tmpStates = m_states;
	bool isActiveState = true;	// If true, this state handles input
	bool isCovered = false;		// If true, the state will not be rendered

	for (auto it = tmpStates.rbegin(); it != tmpStates.rend(); ++it)
	{
		State* state = it->get();

		// Update the state's internal 'isActiveState' and 'isHidden' flag.
		state->metaUpdate(isActiveState, isCovered);

		if (state->isInitialized)
		{
			if (isActiveState)
			{
				state->handleInput();
				isActiveState = false;
			}

			state->update(dt);
		}
		else
		{
			state->init();
			isActiveState = false;
		}

		// Exit loop early if it's time to quit
		if (m_shouldExit)
			break;

		// If this state is not a popup, the remaining states are
		// considered convered and will not be rendered.
		if (!state->isPopup)
			isCovered = true;
	}

	return !m_shouldExit;
}	// The destructors of tmpState's elements are not being called with own SharedPtr implementation
	// Works as it should with std::shared_ptr

void StateMachine::render() const
{
	for (auto it = m_states.begin(); it != m_states.end(); ++it)
	{
		State* state = it->get();

		if (state->isHidden)
			continue;

		if (state->isInitialized)
			state->render();
		else
			m_renderLoadingScreen(state->initProgress);
	}
}

void StateMachine::cleanUp()
{
	m_states.clear();
}

void StateMachine::setLoadingScreen(LoadingScreen renderLoadingScreen)
{
	m_renderLoadingScreen = renderLoadingScreen;
}

void StateMachine::shouldExit(bool value)
{
	m_shouldExit = value;
}

void StateMachine::popState()
{
	m_states.back()->cleanUp();
	m_states.pop_back();

	// Alert the new top state
	if (!m_states.empty())
		m_states.back()->resume();
}