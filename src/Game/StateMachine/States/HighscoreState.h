#pragma once
#include "../State.h"

#include "../../UI/Menu.h"
#include "../../../Common/Common.h"
#include "../../../Common/DependencyPack.h"

#include <array>
#include <graphics/sprite.h>

class HighscoreState : public State
{
public:
	HighscoreState(StateMachine& nss, const DependencyPack& dependencies, const Scoreboard& highscores);

	void handleInput() override;
	void render() const override;

private:
	gef::SpriteRenderer* m_spriteRenderer	= nullptr;
	gef::Font* m_font						= nullptr;

	const Scoreboard& m_highscores;

	Menu m_menu;
	gef::Sprite m_background;
	gef::Sprite m_backdrop;
};