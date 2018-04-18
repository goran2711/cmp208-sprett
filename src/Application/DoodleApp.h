#pragma once
#include "../Game/primitive_builder.h"
#include "../Game/StateMachine/StateMachine.h"
#include "../Game/UI/ProgressBar.h"
#include "../Util/UniquePtr.h"

#include <system/application.h>
#include <graphics/sprite.h>

namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
}

class DoodleApp : public gef::Application
{
public:
	DoodleApp(gef::Platform& platform);

	void Init();
	void CleanUp();
	bool Update(float dt);
	void Render();

private:
	void InitFont();

	UniquePtr<gef::Font> m_font						= nullptr;
	UniquePtr<gef::SpriteRenderer> m_spriteRenderer	= nullptr;
	UniquePtr<gef::Renderer3D> m_renderer3d			= nullptr;
	UniquePtr<PrimitiveBuilder> m_primitiveBuilder	= nullptr;
	
	StateMachine m_stateMachine;

	gef::Sprite m_background;
	ProgressBar m_progressBar;
};
