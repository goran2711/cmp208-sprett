#include "DoodleApp.h"

#include "../Common/Common.h"
#include "../Common/DependencyPack.h"
#include "../Game/StateMachine/States/SplashState.h"

#include "../Common/Input.h"
#include "../Common/Audio.h"
#include "../Common/TextureManager.h"

#include <ctime>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>

DoodleApp::DoodleApp(gef::Platform& platform)
	:	Application(platform)
	,	m_progressBar({ CENTER_X, CENTER_Y, 0.9f }, 384.f, 64.f)
{
	srand((unsigned)time(0));
}

void DoodleApp::Init()
{
	// Initialise framework
	m_renderer3d = gef::Renderer3D::Create(platform_);
	m_spriteRenderer = gef::SpriteRenderer::Create(platform_);
	m_primitiveBuilder = new PrimitiveBuilder(platform_);
	
	InitFont();
	Input::init(platform_);
	Audio::init(platform_);

	// Create a background sprite for the loading screen
	m_background.set_width(WINDOW_WIDTH);
	m_background.set_height(WINDOW_HEIGHT);
	m_background.set_position({ CENTER_X, CENTER_Y, 0.9f });
	m_background.set_texture(TextureManager::getTexture("background.png", platform_));

	// Provide the state machine a function for rendering the loading screen
	m_stateMachine.setLoadingScreen(
		[&] (float progress)
	{
		m_spriteRenderer->Begin();
		m_spriteRenderer->DrawSprite(m_background);
		m_progressBar.render(m_spriteRenderer.get(), m_font.get(), progress);
		m_spriteRenderer->End();
	});

	// Pass the dependencies a bit more neatly
	DependencyPack dependencies(platform_);
	dependencies.spriteRenderer		= m_spriteRenderer.get();
	dependencies.font				= m_font.get();
	dependencies.renderer3d			= m_renderer3d.get();
	dependencies.primitiveBuilder	= m_primitiveBuilder.get();

	// Push the splash screen
	m_stateMachine.pushState<SplashState>(dependencies, 1.5f);
}

bool DoodleApp::Update(float dt)
{
	Input::update();	
	
	return m_stateMachine.update(dt);
}

void DoodleApp::Render()
{	
	m_stateMachine.render();
}

void DoodleApp::CleanUp()
{
	m_stateMachine.cleanUp();
	
	Input::cleanUp();
	Audio::cleanUp();
	TextureManager::cleanUp();
}

void DoodleApp::InitFont()
{
	m_font = new gef::Font(platform_);
	m_font->Load("comic_sans");
}