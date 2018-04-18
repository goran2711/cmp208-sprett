#pragma once
#include "Button.h"

#include <vector>

/*
	A menu is a collection of buttons. The menu takes care of user input,
	and highlights and clicks the appropriate buttons based on this.
	It also plays the corresponding sound effects.
*/

class Menu
{
public:
	enum class Alignment : char
	{
		HORIZONTAL,
		VERTICAL
	};

	void update();

	// Assumes SpriteRenderer::Begin() has already been called
	void render(gef::SpriteRenderer* renderer, gef::Font* font) const;
	
	template <typename ... Args>
	void emplaceButton(Args&& ... args)
	{
		m_buttons.emplace_back(std::forward<Args>(args)...);

		if (m_buttons.size() == 1)
			m_buttons.back().toggleSelected();
	}

	void selectButton(unsigned idx);

	void setCommonWidth(float width);
	void positionVertically(const gef::Vector4& position);
	void positionHorizontally(const gef::Vector4& position);

private:
	void nextButton();
	void previousButton();
	void click();

	std::vector<Button> m_buttons;
	size_t m_selectedButton = 0;

	Alignment m_alignment;
	float m_commonWidth = -1;
};