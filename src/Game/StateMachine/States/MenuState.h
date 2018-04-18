#pragma once
#include "../State.h"

#include "../../UI/Menu.h"
#include "../../../Common/Common.h"
#include "../../../Common/DependencyPack.h"

#include <array>
#include <graphics/sprite.h>

/*
	The menu state lets the player choose which game mode
	to play, and gives them the option to look at the scoreboard or quit.

	The scoreboard is stored in the menu state for continuity between the game state and highscore state.
*/

class MenuState : public State
{
public:
	MenuState(StateMachine& nss, const DependencyPack& dependencies);

	void resume() override;

	void handleInput() override;
	void render() const override;
	
private:
	// Will use
	gef::SpriteRenderer* m_spriteRenderer	= nullptr;
	gef::Font* m_font						= nullptr;

	// Pass on
	DependencyPack m_dependencies;

	Scoreboard m_highscores;

	Menu m_menu;
	gef::Sprite m_background;
};