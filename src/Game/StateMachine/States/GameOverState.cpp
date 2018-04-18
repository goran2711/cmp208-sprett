#include "GameOverState.h"
#include "GameState.h"
#include "MenuState.h"
#include "../StateMachine.h"

#include "../../../Common/Common.h"

#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

GameOverState::GameOverState(StateMachine & stateStack, GameState & gameState, const DependencyPack& dependencies, float score, bool isHighscore)
	:	State(stateStack)
	,	m_spriteRenderer(dependencies.spriteRenderer)
	,	m_font(dependencies.font)
	,	m_gameState(gameState)
	,	m_score(score)
	,	m_isHighscore(isHighscore)
{
	// Setup background
	m_background.set_colour(0xAA000000);
	m_background.set_width(WINDOW_WIDTH);
	m_background.set_height(WINDOW_HEIGHT);
	m_background.set_position({ CENTER_X, CENTER_Y, -0.9f });

	// Setup menu
	m_menu.emplaceButton("Restart", UI_DEFAULT_COLOUR_1, UI_HIGHLIGHT_COLOUR_1, [&] {
		stateStack.popState();	// Pop game over state
		m_gameState.restart();	// Restart
	});

	m_menu.emplaceButton("Main Menu", UI_DEFAULT_COLOUR_1, UI_HIGHLIGHT_COLOUR_1, [&] {
		stateStack.popState();	// Pop game over state
		stateStack.popState();	// Pop game state
	});

	m_menu.emplaceButton("Exit", UI_DEFAULT_COLOUR_2, UI_HIGHLIGHT_COLOUR_2, [&] {
		stateStack.shouldExit(true);
	});

	m_menu.setCommonWidth(128.f);
	m_menu.positionVertically({ CENTER_X, CENTER_Y, -0.9f });

	// Information about the state
	needsInitialization(false);
	canSeeThrough(true);
}

void GameOverState::handleInput()
{
	m_menu.update();
}

void GameOverState::render() const
{
	m_spriteRenderer->Begin(false);

	m_spriteRenderer->DrawSprite(m_background);

	m_font->RenderText(m_spriteRenderer, { CENTER_X, CENTER_Y * 0.4f, -0.9f }, 1.f, -1, gef::TJ_CENTRE, "Game Over!");
	m_font->RenderText(m_spriteRenderer, { CENTER_X, CENTER_Y * 0.52f, -0.9f }, 1.f, -1, gef::TJ_CENTRE, "Your score: %.2f", m_score);
	
	if (m_isHighscore)
	{
		m_font->RenderText(m_spriteRenderer, { CENTER_X, CENTER_Y * 0.6f, -0.9f }, 1.f, UI_HIGHLIGHT_COLOUR_2, gef::TJ_CENTRE,
			"New highscore, congratulations!");
	}

	m_menu.render(m_spriteRenderer, m_font);

	m_spriteRenderer->End();
}
