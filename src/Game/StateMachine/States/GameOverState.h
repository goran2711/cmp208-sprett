#pragma once
#include "../State.h"

#include "../../UI/Menu.h"
#include "../../../Common/DependencyPack.h"

#include <graphics/sprite.h>

/*
	The game over state gives the player the option to retry, go back to the menu or quit the game.
	It also lets the player know their score, and if it is the new high score.
*/

class GameState;

class GameOverState : public State
{
public:
	GameOverState(StateMachine& stateStack, GameState& gameState, const DependencyPack& dependencies, float score, bool isHighscore);

	void handleInput() override;
	void render() const override;

private:
	gef::Sprite m_background;
	gef::SpriteRenderer* m_spriteRenderer	= nullptr;
	gef::Font* m_font						= nullptr;

	const float m_score;
	const bool m_isHighscore;

	Menu m_menu;
	GameState& m_gameState;
};