#pragma once
#include <graphics/sprite.h>

namespace gef
{
	class SpriteRenderer;
	class Font;
}

class ProgressBar
{
public:
	explicit ProgressBar(const gef::Vector4& position, float width, float height);

	void render(gef::SpriteRenderer* renderer, gef::Font* font, float progress) const;

private:
	constexpr static float BORDER_SIZE = 8.f;

	gef::Sprite m_background;
	gef::Sprite m_progressBackground;
	mutable gef::Sprite m_progress;
};

