#include "ProgressBar.h"
#include "../../Common/Common.h"

#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

ProgressBar::ProgressBar(const gef::Vector4& position, float width, float height)
{
	// Setup background
	m_background.set_width(width);
	m_background.set_height(height);
	m_background.set_colour(UI_DEFAULT_COLOUR_1);
	m_background.set_position(position);

	// Setup unfilled progress bar
	m_progressBackground.set_width(width - BORDER_SIZE * 2);
	m_progressBackground.set_height(height - BORDER_SIZE * 2);
	m_progressBackground.set_colour(UI_DEFAULT_COLOUR_2);
	m_progressBackground.set_position(position);

	// Setup filled progress bar
	m_progress.set_height(height - BORDER_SIZE * 2);
	m_progress.set_colour(UI_HIGHLIGHT_COLOUR_2);
}

void ProgressBar::render(gef::SpriteRenderer* renderer, gef::Font* font, float progress) const
{
	// Calculate width of progress bar
	m_progress.set_width((m_background.width() - 2 * BORDER_SIZE) * progress);

	// Calculate position of progress bar
	// (since position is the center of the sprite and the sprite should look like it grows from the left)
	gef::Vector4 progressPosition = m_background.position();

	const float empty = m_background.position().x() - (m_background.width() * 0.5f) + BORDER_SIZE;
	const float full = m_background.position().x();

	progressPosition.set_x(empty + (full - empty) * progress);
	m_progress.set_position(progressPosition);

	// Render
	renderer->DrawSprite(m_background);
	renderer->DrawSprite(m_progressBackground);
	renderer->DrawSprite(m_progress);
	font->RenderText(renderer, { CENTER_X, CENTER_Y - 16.f, -0.9f }, 1.f, -1, gef::TJ_CENTRE, "Loading... (%.f%%)", progress * 100);
}
