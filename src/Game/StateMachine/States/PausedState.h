#pragma once
#include "../State.h"

#include "../../UI/Menu.h"
#include "../../../Common/DependencyPack.h"

#include <graphics/sprite.h>

/*
	The paused state lets the player restart the level or return to the main menu
*/

class GameState;

class PausedState : public State
{
public:
	PausedState(StateMachine& stateStack, GameState& gameState, const DependencyPack& dependencies);

	void handleInput() override;
	void render() const override;

private:
	gef::Sprite m_background;
	gef::SpriteRenderer* m_spriteRenderer = nullptr;
	gef::Font* m_font = nullptr;

	Menu m_menu;
	GameState& m_gameState;
};