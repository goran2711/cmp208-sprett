#pragma once

/*
	Every state has a reference to the state machine to allow them to push new states and pop themselves (and others)
*/

class StateMachine;

class State
{
	friend class StateMachine;
public:
	virtual ~State() = default;

protected:
	explicit State(StateMachine& stateStack_) : stateStack(stateStack_) {}
	virtual void init() {}

	// Called when a new state is pushed on top of this one
	virtual void suspend() {}
	// Called when the state above is popped and this becomes the current state
	virtual void resume() {}

	virtual void handleInput() = 0;
	virtual void update(float dt) {};
	virtual void render() const = 0;

	// CleanUp is only here as a compromise, because own SharedPtr implementation
	// isn't working correctly (destructors not being called). Otherwise would use RAII
	virtual void cleanUp() {}

	bool isInitialized = false;
	
	// If a state is active, it will handle input for this frame
	bool isActiveState = false;

	// Used by the loading screen
	float initProgress = 0.f;
    
	// Utility functions
	void needsInitialization(bool val);
	void canSeeThrough(bool val);

	StateMachine& stateStack;

private:
	// Used by the state machine to set isActiveState and isHidden appropriately
	void metaUpdate(bool isActive, bool isCovered);

	// If a state is a popup, states below it will still be rendered
	bool isPopup = false;
	// If a state is hidden, it will not be rendered
	bool isHidden = false;
};