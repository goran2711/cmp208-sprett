#include "Button.h"

#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

Button::Button(const std::string & label, UInt32 inactiveColour, UInt32 activeColour, ButtonAction action)
	: m_label(label)
	, m_inactiveColour(inactiveColour)
	, m_activeColour(activeColour)
	, m_action(action)
{
	setupBackground();
}

void Button::render(gef::SpriteRenderer* renderer, gef::Font* font) const
{
	renderer->DrawSprite(m_bgSprite);
	font->RenderText(renderer, m_labelPosition, 1.f, -1, gef::TJ_CENTRE, m_label.c_str());
}

void Button::click()
{
	if (m_action)
		m_action();
}

void Button::setPosition(const gef::Vector4 & position)
{
	m_bgSprite.set_position(position);
	m_labelPosition = { position.x(), position.y() - 16.f, position.z() };
}

void Button::setWidth(float width)
{
	m_bgSprite.set_width(width);
}

void Button::toggleSelected()
{
	if (m_isSelected = !m_isSelected)
		m_bgSprite.set_colour(m_activeColour);
	else
		m_bgSprite.set_colour(m_inactiveColour);
}

void Button::setupBackground()
{
	m_bgSprite.set_width(CHAR_WIDTH * m_label.length());
	m_bgSprite.set_height(BUTTON_HEIGHT);
	m_bgSprite.set_colour(m_inactiveColour);
	m_bgSprite.set_position({ 0.f, 0.f, 0.f });
}
