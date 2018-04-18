#pragma once
#include "../../Util/Function.h"

#include <string>
#include <graphics/sprite.h>

namespace gef
{
	class SpriteRenderer;
	class Font;
}

/*
	A button consists of a sprite, a label and an action.
	The action is a function object that is called when the button is clicked.
*/

class Button
{
	using ButtonAction = Function<void(void)>;
public:
	Button(const std::string& label, UInt32 inactiveColour, UInt32 activeColour, ButtonAction action = nullptr);

	// Assumes SpriteRenderer::Begin() has already been called
	void render(gef::SpriteRenderer* renderer, gef::Font* font) const;
	void click();
    
    void toggleSelected();
	void setPosition(const gef::Vector4& position);
	void setWidth(float width);

	bool isSelected() const { return m_isSelected; }

    static constexpr float BUTTON_HEIGHT = 32.f;

private:
	void setupBackground();

	// Rough guess at the average width of a character
	static constexpr float CHAR_WIDTH = 18.f;

	ButtonAction m_action;

	bool m_isSelected = false;
	UInt32 m_activeColour;
	UInt32 m_inactiveColour;

	gef::Sprite m_bgSprite;
	std::string m_label;
	gef::Vector4 m_labelPosition = { 0.f, 0.f, 0.f };
};
