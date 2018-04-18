#include "MenuState.h"
#include "GameState.h"
#include "HighscoreState.h"
#include "../StateMachine.h"

#include "../../../Common/TextureManager.h"

#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

MenuState::MenuState(StateMachine& nss, const DependencyPack& dependencies)
	:	State(nss)
	,	m_spriteRenderer(dependencies.spriteRenderer)
	,	m_font(dependencies.font)
	,	m_dependencies(dependencies)
{
	// Initialise the scoreboard
	m_highscores.assign(0.f);

	// Setup the background
	m_background.set_width(WINDOW_WIDTH);
	m_background.set_height(WINDOW_HEIGHT);
	m_background.set_position({ CENTER_X, CENTER_Y, 0.9f });
	m_background.set_texture(TextureManager::getTexture("title_background.png", dependencies.platform));

	// Setup the menu
	m_menu.emplaceButton("Start", UI_DEFAULT_COLOUR_1, UI_HIGHLIGHT_COLOUR_1, [&] {
		stateStack.pushState<GameState>(m_dependencies, m_highscores);
	});

	m_menu.emplaceButton("Crazy", UI_DEFAULT_COLOUR_1, UI_HIGHLIGHT_COLOUR_1, [&] {
		stateStack.pushState<GameState>(m_dependencies, m_highscores, true);
	});

	m_menu.emplaceButton("Highscores", UI_DEFAULT_COLOUR_1, UI_HIGHLIGHT_COLOUR_1, [&] {
		stateStack.pushState<HighscoreState>(m_dependencies, m_highscores);
	});

	m_menu.emplaceButton("Exit", UI_DEFAULT_COLOUR_2, UI_HIGHLIGHT_COLOUR_2, [&] {
		stateStack.shouldExit(true);
	});

	m_menu.setCommonWidth(128.f);
	m_menu.positionHorizontally({ CENTER_X, CENTER_Y * 1.25f, -0.9f });
	
	// Information about the state
	needsInitialization(false);
	canSeeThrough(false);
}

void MenuState::resume()
{
	// Select the first button when the player returns from another state
	m_menu.selectButton(0);
}

void MenuState::handleInput()
{
	m_menu.update();
}

void MenuState::render() const
{
	m_spriteRenderer->Begin();

	m_spriteRenderer->DrawSprite(m_background);
	m_menu.render(m_spriteRenderer, m_font);

	m_spriteRenderer->End();
}
