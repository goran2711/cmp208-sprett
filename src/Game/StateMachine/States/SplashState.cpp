#include "SplashState.h"
#include "MenuState.h"
#include "../StateMachine.h"

#include "../../../Common/Common.h"
#include "../../../Common/Input.h"
#include "../../../Common/Audio.h"
#include "../../../Common/TextureManager.h"

#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

SplashState::SplashState(StateMachine& nss, const DependencyPack& dependencies, float splashTime)
	:	State(nss)
	,	m_platform(dependencies.platform)
	,	m_spriteRenderer(dependencies.spriteRenderer)
	,	m_font(dependencies.font)
	,	m_dependencies(dependencies)
	,	m_timer(splashTime)
{
	// Setup the background sprite
	m_background.set_width(WINDOW_WIDTH);
	m_background.set_height(WINDOW_HEIGHT);
	m_background.set_position(CENTER_X, CENTER_Y, 0.f);
	m_background.set_texture(TextureManager::getTexture("splash_screen.png", m_platform));

	Audio::playSample(Audio::JINGLE);

	// Information about the state
	needsInitialization(false);
}

void SplashState::update(float dt)
{
	if (m_timer.tick(dt) <= 0 || m_skipped)
	{
		stateStack.popState();
		stateStack.pushState<MenuState>(m_dependencies);
	}
}

void SplashState::handleInput()
{
	if (Input::isPressed(Input::ACTION) || Input::isPressed(Input::MENU))
		m_skipped = true;
}

void SplashState::render() const
{
	m_spriteRenderer->Begin();
	m_spriteRenderer->DrawSprite(m_background);
	m_spriteRenderer->End();
}

void SplashState::cleanUp()
{
	// This texture will not be used anymore
	TextureManager::unloadTexture("splash_screen.png");
}
