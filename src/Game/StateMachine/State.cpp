#include "State.h"

void State::metaUpdate(bool isActive, bool isCovered)
{
	isActiveState = isActive;
	isHidden = isCovered;
}

void State::needsInitialization(bool val)
{
	isInitialized = !val;
}

void State::canSeeThrough(bool val)
{
	isPopup = val;
}
