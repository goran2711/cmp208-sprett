#pragma once
#include "../State.h"
#include "../../../Util/Timer.h"
#include "../../../Common/DependencyPack.h"

#include <graphics/sprite.h>

/*
	The splash state simply displays a graphic for a few seconds
	before it pushes the menu state onto the stack.
*/

class SplashState : public State
{
public:
	SplashState(StateMachine& nss, const DependencyPack& dependencies, float splashTime = 3.f);

	void update(float dt) override;
	void handleInput() override;
	void render() const override;
	void cleanUp() override;

private:
	gef::Platform& m_platform;
	gef::SpriteRenderer* m_spriteRenderer	= nullptr;
	gef::Font* m_font						= nullptr;

	gef::Sprite m_background;

	// Will be passed on
	DependencyPack m_dependencies;

	Timer m_timer;
	bool m_skipped = false;
};