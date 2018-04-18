#include "PausedState.h"
#include "GameState.h"
#include "../StateMachine.h"

#include "../../../Common/Common.h"
#include "../../../Common/Input.h"

#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

PausedState::PausedState(StateMachine& stateStack, GameState& gameState, const DependencyPack& dependencies)
	:	State(stateStack)
	,	m_spriteRenderer(dependencies.spriteRenderer)
	,	m_font(dependencies.font)
	,	m_gameState(gameState)
{
	// Setup background
	m_background.set_width(WINDOW_WIDTH);
	m_background.set_height(WINDOW_HEIGHT);
	m_background.set_colour(0xAA000000);
	m_background.set_position({ CENTER_X, CENTER_Y, -0.9f });
	
	// Setup menu
	m_menu.emplaceButton("Resume", UI_DEFAULT_COLOUR_1, UI_HIGHLIGHT_COLOUR_1, [&] {
        stateStack.popState();	// Pop paused state
    });

	m_menu.emplaceButton("Restart", UI_DEFAULT_COLOUR_1, UI_HIGHLIGHT_COLOUR_1, [&] {
		stateStack.popState();	// Pop paused state
		m_gameState.restart();	// Restart game state
	});

	m_menu.emplaceButton("Main Menu", UI_DEFAULT_COLOUR_1, UI_HIGHLIGHT_COLOUR_1, [&] {
		stateStack.popState();	// Pop paused state
		stateStack.popState();	// Pop game state
	});

	m_menu.setCommonWidth(128.f);
	m_menu.positionVertically({ CENTER_X, CENTER_Y, -0.9f });

	// Information about the state
	needsInitialization(false);
	canSeeThrough(true);
}

void PausedState::handleInput()
{
	if (Input::isPressed(Input::MENU))
		stateStack.popState();
	
	m_menu.update();
}

void PausedState::render() const
{
	m_spriteRenderer->Begin(false);

	m_spriteRenderer->DrawSprite(m_background);
	m_font->RenderText(m_spriteRenderer, { CENTER_X, CENTER_Y - 128.f, -0.9f }, 1.f, -1, gef::TJ_CENTRE, "Game Paused");

	m_menu.render(m_spriteRenderer, m_font);

	m_spriteRenderer->End();
}
