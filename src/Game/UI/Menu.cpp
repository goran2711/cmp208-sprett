#include "Menu.h"

#include "../../Common/Input.h"
#include "../../Common/Audio.h"
#include "../../Common/Common.h"

void Menu::update()
{
	if (Input::isPressed(Input::ACTION))
		click();

	switch (m_alignment)
	{
		case Alignment::VERTICAL:
		{
			if (Input::isPressed(Input::DOWN))
				nextButton();

			if (Input::isPressed(Input::UP))
				previousButton();
		}
		break;
		case Alignment::HORIZONTAL:
		{
			if (Input::isPressed(Input::RIGHT))
				nextButton();

			if (Input::isPressed(Input::LEFT))
				previousButton();
		}
		break;
	}
}

void Menu::nextButton()
{
	if (m_selectedButton < m_buttons.size() - 1)
	{
		Audio::playSample(Audio::BUTTON_SELECT);

		m_buttons[m_selectedButton].toggleSelected();
		m_buttons[++m_selectedButton].toggleSelected();
	}
}

void Menu::previousButton()
{
	if (m_selectedButton > 0)
	{
		Audio::playSample(Audio::BUTTON_SELECT);

		m_buttons[m_selectedButton].toggleSelected();
		m_buttons[--m_selectedButton].toggleSelected();
	}
}

void Menu::click()
{
	Audio::playSample(Audio::BUTTON_CLICK);

	m_buttons[m_selectedButton].click();
}

void Menu::selectButton(unsigned idx)
{
	m_buttons[m_selectedButton].toggleSelected();

	m_selectedButton = idx;
	m_buttons[m_selectedButton].toggleSelected();
}

void Menu::setCommonWidth(float width)
{
	m_commonWidth = width;

	for (auto& button : m_buttons)
		button.setWidth(width);
}

void Menu::positionVertically(const gef::Vector4& position)
{
	const unsigned NUM_BUTTONS = m_buttons.size();
	const float SPACING = Button::BUTTON_HEIGHT * 1.5f;

	// Calculate the position of the first button and build from there
	float acc = 0.f;
	for (int i = 0; i < NUM_BUTTONS - 1; ++i)
		acc += SPACING;

	acc /= 2.f;

	float y = position.y() - acc;

	// Place every button in relation to the first one
	for (int i = 0; i < NUM_BUTTONS; ++i)
		m_buttons[i].setPosition({
		position.x(),
		y + (SPACING * i),
		position.z()
	});

	m_alignment = Alignment::VERTICAL;
}

void Menu::positionHorizontally(const gef::Vector4& position)
{
	const unsigned NUM_BUTTONS = m_buttons.size();
	const float SPACING = m_commonWidth * 0.5f + m_commonWidth * 0.6f;

	// Calculate the position of the first button and build from there
	float acc = 0.f;
	for (int i = 0; i < NUM_BUTTONS - 1; ++i)
		acc += SPACING;

	acc /= 2.f;

	float x = position.x() - acc;

	// Place every button in relation to the first one
	for (int i = 0; i < NUM_BUTTONS; ++i)
		m_buttons[i].setPosition({
		x + (SPACING * i),
		position.y(),
		position.z()
	});

	m_alignment = Alignment::HORIZONTAL;
}

void Menu::render(gef::SpriteRenderer* renderer, gef::Font* font) const
{
	for (auto& button : m_buttons)
		button.render(renderer, font);
}
