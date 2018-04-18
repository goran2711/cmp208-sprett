#include "GameState.h"
#include "PausedState.h"
#include "GameOverState.h"
#include "../StateMachine.h"

#include "../../Camera.h"
#include "../../Filters.h"
#include "../../primitive_builder.h"
#include "../../../Common/Input.h"
#include "../../../Common/Audio.h"
#include "../../../Common/TextureManager.h"
#include "../../../Common/Common.h"

#include <algorithm>
#include <Box2D/Box2D.h>
#include <graphics/font.h>
#include <graphics/renderer_3d.h>
#include <graphics/sprite_renderer.h>

// Box2D /////////////////////////////////
constexpr float TIME_STEP = 1.f / 60.f; //
constexpr int VELOCITY_ITERATIONS = 6;  //
constexpr int POSITION_ITERATIONS = 2;  //
//////////////////////////////////////////

GameState::GameState(StateMachine& stateStack, const DependencyPack& dependencies, Scoreboard& highscores, bool crazyMode)
	:	State(stateStack)
	,	m_platform(dependencies.platform)
	,	m_spriteRenderer(dependencies.spriteRenderer)
	,	m_font(dependencies.font)
	,	m_renderer3d(dependencies.renderer3d)
	,	m_primitiveBuilder(dependencies.primitiveBuilder)
	,	m_dependencies(dependencies)
	,	m_highscores(highscores)
	,	m_isCrazyMode(crazyMode)
{
}

// Initialization is done one step at a time, rendering a loading screen between each step
void GameState::init()
{
	switch (m_initalizationProgress)
	{
	case 0:
		m_world = new b2World(b2Vec2(0.f, -9.81f));
		break;
	case 1:
		m_world->SetContactListener(&m_contactListener);
		break;
	case 2:
		m_camera = new Camera(CAMERA_SPAWN, &m_player, 70.f, ASPECT_RATIO);
		break;
	case 3:
		setupBackground();
		break;
	case 4:
		setupFloor();
		break;
	case 5:
		setupPlayer();
		break;
	case 6:
		setupBoundary();
		break;
	case 7:
		setupPlatforms();
		break;
	case 8:
		setupLights();
		break;
	case 9:
		setupPowerups();
		break;
	case 10:
		/**** FREE SPOT ****/
		break;
	default:
		Audio::startMusic();
		isInitialized = true;
	}	

	initProgress = m_initalizationProgress++ / (float)INIT_STAGES;
}

void GameState::setupPlayer()
{
	GOProperties props;
	props.type = b2_dynamicBody;
	props.size = PLAYER_DIM;
	props.position = PLAYER_SPAWN;
	props.mesh = m_primitiveBuilder->GetDefaultCubeMesh();
	props.fixedRotation = true;
	props.categoryBits = CATEGORY_PLAYER;
	props.maskBits = MASK_PLAYER;

	m_player.init(m_world.get(), props);
	m_player.setTexture(TextureManager::getTexture("player.png", m_platform));
}

void GameState::setupBoundary()
{
	m_boundary.init([&] (GameObject* thisPtr) {
		b2BodyDef wallDef;
		wallDef.type = b2_staticBody;
		wallDef.position = { 0.f, 0.f };
		wallDef.userData = thisPtr;

		b2Body* body = m_world->CreateBody(&wallDef);

		b2FixtureDef wallFixture;
		wallFixture.friction = 0.f;

		b2Filter filter;
		filter.maskBits = MASK_GROUND;
		filter.categoryBits = CATEGORY_GROUND;
		wallFixture.filter = filter;

		b2PolygonShape wallShape;
		wallFixture.shape = &wallShape;

		// Create two rectangular fixtures on either side of the screen that are the same height as the view plane
		const float viewPlaneLeft = m_camera->getViewPlaneLeft();
		const float viewPlaneRight = m_camera->getViewPlaneRight();
		const float playspaceHeight = m_camera->getViewPlaneHeight();

		const float boundaryWidth = 0.25f;
		const float boundaryHeight = playspaceHeight * 2.5f;

		wallShape.SetAsBox(boundaryWidth, boundaryHeight, { viewPlaneLeft, m_player.getPositionVec2().y }, 0.f);
		body->CreateFixture(&wallFixture);

		wallShape.SetAsBox(boundaryWidth, boundaryHeight, { viewPlaneRight, m_player.getPositionVec2().y }, 0.f);
		body->CreateFixture(&wallFixture);

		return body;
	});

	m_boundary.overrideType(GameObject::Type::Boundary);
}

void GameState::setupFloor()
{
	GOProperties props;
	props.type = b2_staticBody;
	props.position = { 0.f, -0.5f };
	props.size = { 36.f, 0.5f, 1.f };
	props.mesh = m_primitiveBuilder->GetDefaultCubeMesh();
	props.categoryBits = CATEGORY_GROUND;
	props.maskBits = MASK_GROUND;

	m_floor.init(m_world.get(), props);
	m_floor.overrideType(GameObject::Type::Floor);
}

void GameState::setupBackground()
{
	m_background.set_width(WINDOW_WIDTH);
	m_background.set_height(WINDOW_HEIGHT);
	m_background.set_position({ CENTER_X, CENTER_Y, 0.9f });
	m_background.set_texture(TextureManager::getTexture("background.png", m_platform));
}

void GameState::setupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = m_renderer3d->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(1.f, 1.f, 1.0f, 1.0f));

	default_point_light.set_position(gef::Vector4(-600.0f, 600.f, 400.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void GameState::setupPowerups()
{
	TextureManager::loadTexture("bounce_power.png", m_platform);
	TextureManager::loadTexture("jetpack_power.png", m_platform);
}

void GameState::suspend()
{
	Audio::stopMusic();
}

void GameState::resume()
{
	Audio::startMusic();
}

void GameState::restart()
{
	m_player.reset(PLAYER_SPAWN);
	m_camera->reset(CAMERA_SPAWN);

	for (int i = 0; i < NUM_PLATFORMS; ++i)
	{
		m_platforms[i].reset();

		float x = getRandomPlatformX();
		float y = 3.5f + (2.f * i);

		m_platforms[i].setPosition({ x, y });
	}

	m_powerups.clear();
}

void GameState::handleInput()
{
	if (Input::isPressed(Input::MENU))
		pauseGame();

	if (Input::isPressed(Input::JUMP))
		m_player.jump();

	// Register both analogue and digital movement
	if (Input::isDown(Input::RIGHT))
		m_player.move(1.f);
	else if (Input::isDown(Input::LEFT))
		m_player.move(-1.f);
	else
		m_player.move(Input::leftJoystickX());
}

void GameState::update(float dt)
{
	if (!isActiveState)
		return;

	m_fps = 1.f / dt;

	m_camera->update();

	m_world->Step(TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
	refreshPowerups();
	
	m_player.update(dt);
	m_boundary.setY(m_player.getPositionVec2().y);

	if (playerOutOfBounds())
	{
		gameOver();
		return;
	}

	updatePlatforms();
}

void GameState::render() const
{
	// Camera
	m_renderer3d->set_projection_matrix(m_camera->getProjectionMatrix(m_platform));
	m_renderer3d->set_view_matrix(m_camera->getViewMatrix());

	clearAndRenderBackground();
	renderWorld();
	renderUI();
}

void GameState::clearAndRenderBackground() const
{
	m_spriteRenderer->Begin();
	m_spriteRenderer->DrawSprite(m_background);
	m_spriteRenderer->End();
}

void GameState::renderWorld() const
{
	m_renderer3d->Begin(false);

	m_player.render(m_renderer3d);
	m_floor.render(m_renderer3d);

	for (auto& platform : m_platforms)
		platform.render(m_renderer3d);

	for (auto& powerup : m_powerups)
		powerup->render(m_renderer3d);

	m_renderer3d->End();
}

void GameState::renderUI() const
{
	m_spriteRenderer->Begin(false);

	m_font->RenderText(m_spriteRenderer, { 5.f,  5.f, -0.9f }, 1.f, UI_DEFAULT_COLOUR_2, gef::TJ_LEFT,
		"FPS: %.0f", m_fps);
	m_font->RenderText(m_spriteRenderer, { WINDOW_WIDTH - 5.f, 5.f, -0.9f }, 1.f, UI_DEFAULT_COLOUR_2, gef::TJ_RIGHT,
		"Score: %.0f", getScore());

	m_spriteRenderer->End();
}

void GameState::cleanUp()
{
	TextureManager::unloadTexture("bounce_power.png");
	TextureManager::unloadTexture("jetpack_power.png");

	cleanUpLights();
}

void GameState::cleanUpLights()
{
	gef::Default3DShaderData& default_shader_data = m_renderer3d->default_shader_data();
	default_shader_data.CleanUp();
}

void GameState::pauseGame()
{
	stateStack.pushState<PausedState>(*this, m_dependencies);
}

float GameState::getScore() const
{
	// Score is the y-coordinate of the buttom edge of the frustum
	return std::fmaxf(0.f, m_camera->getViewPlaneBottom());
}

bool GameState::playerOutOfBounds() const
{
	const float playerY = m_player.getPositionVec2().y;
	const float viewPlaneBottom = m_camera->getViewPlaneBottom();

	return playerY < viewPlaneBottom - 1.f;
}

bool GameState::addScore(float score)
{
	bool isNewHighscore = true;

	// Insert the score to the scoreboard, if it is greater than any of the old scores
	for (int i = 0; i < m_highscores.size(); ++i)
	{
		if (i > 0)
			isNewHighscore = false;

		if (score <= m_highscores[i])
			continue;

		std::swap(m_highscores[i], score);

		while (++i < m_highscores.size())
			std::swap(m_highscores[i], score);

		break;
	}

	return isNewHighscore;
}

void GameState::gameOver()
{
	Audio::playSample(Audio::GAME_OVER);

	const float finalScore = getScore();

	bool isNewHighscore = addScore(finalScore);

	stateStack.pushState<GameOverState>(*this, m_dependencies, finalScore, isNewHighscore);
}

void GameState::setupPlatforms()
{
	GOProperties props;
	props.type = b2_dynamicBody;
	props.size = PLATFORM_DIM;
	props.mesh = m_primitiveBuilder->GetDefaultCubeMesh();
	props.friction = 0.f;
	props.restitution = 1.f;
	props.gravityScale = 0.f;
	// Platforms are not sensors in crazy mode
	props.isSensor = (m_isCrazyMode ? false : true);
	props.categoryBits = CATEGORY_PLATFORM;
	props.maskBits = MASK_PLATFORM;

	float viewPlaneLeft = -(m_camera->getViewPlaneWidth() * 0.5f);

	for (int i = 0; i < NUM_PLATFORMS; ++i)
	{
		float x = getRandomPlatformX();
		float y = 3.5f + (2.f * i);

		props.position = { x, y };

		m_platforms[i].init(m_world.get(), props);
	}
}

void GameState::updatePlatforms()
{
	float viewPlaneTop = m_camera->getViewPlaneTop();
	float viewPlaneBottom = m_camera->getViewPlaneBottom();

	for (auto& platform : m_platforms)
	{
		platform.update();
		
		// Respawn the platform if it is out of view
		if (isOffScreen(&platform))
		{
			platform.reset();
			randomizePlatform(platform);

			float x = getRandomPlatformX();
			float y = viewPlaneTop + 1.f + rand() % 3;

			// 1/12 chance to spawn a powerup
			if (!(rand() % 12) && !platform.isMoving() && !platform.isBroken())
				spawnPowerup({ x, y + 1.f });

			platform.setPosition({ x, y });
		}
	}
}

void GameState::randomizePlatform(Platform& platform)
{
	const float modifier = getDifficultyModifier();
	float random = ((rand() % 100) / 100.f);

	if (random < modifier)
	{
		// Generate number from [0.5f, 1.5f) and scale it to use as velocity
		float vx = (0.5f + (rand() / (float)RAND_MAX)) * 5.f;
		// Generate number +/-1.f (random direction)
		vx *= rand() % 2 * 2.f - 1.f;
		
		platform.setMoving({ (vx ? vx : -1.f), 0.f });
		return;
	}
	
	// Slightly lower chance of a platform begin broken
	random = ((rand() % 100) / 100.f);
	if (random + 0.1f < modifier)
	{
		platform.setBroken();
		return;
	}
}

float GameState::getRandomPlatformX() const
{
	constexpr float PADDING = 1.5f;
	constexpr float PLATFORM_HX = PLATFORM_DIM_X * 0.5f;
	const float from = m_camera->getViewPlaneLeft() + PLATFORM_HX + PADDING;
	const int to = static_cast<int>(m_camera->getViewPlaneWidth() - PLATFORM_HX - PADDING);

	return from + rand() % to;
}

void GameState::spawnPowerup(const b2Vec2& position)
{
	GOProperties props;
	props.type = b2_staticBody;
	props.mesh = m_primitiveBuilder->GetDefaultCubeMesh();
	props.size = { 1.5f, 1.5f, 1.f };
	props.position = position;
	props.isSensor = true;
	props.maskBits = MASK_POWERUP;
	props.categoryBits = CATEGORY_POWERUP;

	m_powerups.push_back(new Powerup(m_world.get(), props));

	// Randomly choose powerup type
	switch ((PowerType)(rand() % (int)PowerType::NUM_POWERS))
	{
	case PowerType::JETPACK:
		m_powerups.back()->setPower<JetpackPower>();
		m_powerups.back()->setTexture(TextureManager::getTexture("jetpack_power.png", m_platform));
		break;
	case PowerType::BOUNCE:
		m_powerups.back()->setPower<BouncePower>();
		m_powerups.back()->setTexture(TextureManager::getTexture("bounce_power.png", m_platform));
		break;
	}
}

void GameState::refreshPowerups()
{
	auto begin = m_powerups.begin();
	auto end = m_powerups.end();

	// Destroy the physics bodies
	std::for_each(begin, end, [&](UniquePtr<Powerup>& powerup)
	{
		if (powerup->isCollected() || isOffScreen(powerup.get()))
			powerup->destroy(m_world.get());
	});

	// Remove the game objects using the erase-remove idiom
	m_powerups.erase(std::remove_if(begin, end,
	[&](UniquePtr<Powerup>& powerup)
	{
		return powerup->isCollected() || isOffScreen(powerup.get());
	}), end);
}

float GameState::getDifficultyModifier() const
{
	// Difficulity scales linearly based on height
	return std::fminf(MAX_DIFFICULITY, (1.0f / MAX_DIFF_HEIGHT) * (m_player.getPositionVec2().y - MODIFIERS_BEGIN) * MAX_DIFFICULITY);
}

bool GameState::isOffScreen(GameObject* object) const
{
	const float viewPlaneBottom = m_camera->getViewPlaneBottom();
	const float viewPlaneTop = m_camera->getViewPlaneTop();
	const float objectPosY = object->getPositionVec2().y;

	// Object is off screen (and then some)
	return (objectPosY < viewPlaneBottom - 2.f || objectPosY > viewPlaneTop + 20.f);
}
