#include "HighscoreState.h"
#include "../StateMachine.h"

#include "../../../Common/Common.h"
#include "../../../Common/TextureManager.h"

#include <graphics/font.h>
#include <graphics/sprite_renderer.h>

HighscoreState::HighscoreState(StateMachine& nss, const DependencyPack& dependencies, const Scoreboard& highscores)
	:	State(nss)
	,	m_spriteRenderer(dependencies.spriteRenderer)
	,	m_font(dependencies.font)
	,	m_highscores(highscores)
{
	m_menu.emplaceButton("Return", UI_DEFAULT_COLOUR_2, UI_HIGHLIGHT_COLOUR_2, [&] {
		stateStack.popState();
	});

	m_menu.positionHorizontally({ CENTER_X, CENTER_Y * 1.75f, -0.9f });

	m_background.set_width(WINDOW_WIDTH);
	m_background.set_height(WINDOW_HEIGHT);
	m_background.set_position({ CENTER_X, CENTER_Y, 0.9f });
	m_background.set_texture(TextureManager::getTexture("background.png", dependencies.platform));

	m_backdrop.set_width(WINDOW_WIDTH * 0.5f);
	m_backdrop.set_height(WINDOW_HEIGHT * 0.9f);
	m_backdrop.set_position({ CENTER_X, CENTER_Y, 0.9f });
	m_backdrop.set_colour(0x99000000);

	needsInitialization(false);
	canSeeThrough(false);
}

void HighscoreState::handleInput()
{
	m_menu.update();
}

void HighscoreState::render() const
{
	m_spriteRenderer->Begin();

	m_spriteRenderer->DrawSprite(m_background);
	m_spriteRenderer->DrawSprite(m_backdrop);

	m_font->RenderText(m_spriteRenderer, { CENTER_X, CENTER_Y * 0.2f, -0.9f }, 1.25f, 0xFF8A83CF, gef::TJ_CENTRE, "Highscores");

	for (int i = 0; i < m_highscores.size(); ++i)
	{
		float score = m_highscores[i];

		m_font->RenderText(m_spriteRenderer, { CENTER_X, (CENTER_Y * 0.4f) + 32.f * i, -0.9f }, 1.f, -1, gef::TJ_CENTRE,
			"#%d: %.2f", i + 1, score);
	}

	m_menu.render(m_spriteRenderer, m_font);

	m_spriteRenderer->End();
}
