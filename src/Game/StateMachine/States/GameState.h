#pragma once
#include "../State.h"
#include "../../../Common/DependencyPack.h"
#include "../../../Common/Common.h"
#include "../../../Util/UniquePtr.h"

#include "../../ContactListener.h"
#include "../../Player.h"
#include "../../Platform.h"

#include <array>
#include <vector>
#include <graphics/sprite.h>
#include "../../../Util/Timer.h"

#include "../../Powerup.h"
#include "../../Powers.h"

// Highest possible difficulity modifier
constexpr float MAX_DIFFICULITY = 0.35f;
// Height at which the difficulity modifer is 100%
constexpr float MAX_DIFF_HEIGHT = 1000.f;
// Height at which platforms should start taking on random traits (broken/moving)
constexpr float MODIFIERS_BEGIN = 50.f;

constexpr unsigned	NUM_PLATFORMS	= 10U;
constexpr float		PLATFORM_DIM_X	= 3.f;
constexpr float		PLATFORM_DIM_Y	= .5f;

#define PLATFORM_DIM	{ PLATFORM_DIM_X, PLATFORM_DIM_Y, 1.f }
#define PLAYER_DIM		{ 1.f, 1.f, 1.f }
#define CAMERA_SPAWN	{ 0.f, 5.f, 12.f }
#define PLAYER_SPAWN	{ 0.f, 0.5f }

/*
	The game state is where all the game logic is located. The constructor takes a flag, 'crazyMode'
	which determines whether the platforms should be sensors or not (have a physics response to collisions).
	It also takes a reference to the scoreboard that's stored in the menu state. It is stored there so that
	it can be passed on to the highscore state later on.
*/

class Camera;
class b2World;

class GameState : public State
{
public:
	GameState(StateMachine& stateStack, const DependencyPack& dependencies, Scoreboard& highscores, bool crazyMode = false);
	
	void init() override;
	void suspend() override;
	void resume() override;

	void restart();
	void handleInput() override;
	void update(float dt) override;
	void render() const override;
	void cleanUp() override;
		
private:
	void setupPlayer();
	void setupBoundary();
	void setupFloor();
	void setupBackground();
	void setupLights();
	void setupPowerups();

	void cleanUpLights();

	void pauseGame();

	void clearAndRenderBackground() const;
	void renderWorld() const;
	void renderUI() const;

	float getScore() const;
	bool addScore(float score);
	bool playerOutOfBounds() const;
	void gameOver();

	void setupPlatforms();
	void updatePlatforms();
	// Randomizes the properties of a platform (broken/moving)
	void randomizePlatform(Platform& platform);
	// Returns a random x coordinate that's visible to the camera, and doesn't cause the platform to clip with the boundary
	float getRandomPlatformX() const;

	void spawnPowerup(const b2Vec2& position);
	// Destroys and erases powerups that have been collected or gone off-screen
	void refreshPowerups();

	float getDifficultyModifier() const;
	bool isOffScreen(GameObject* object) const;

	// Framework
	gef::Platform& m_platform;
	gef::SpriteRenderer* m_spriteRenderer	= nullptr;
	gef::Font* m_font						= nullptr;
	gef::Renderer3D* m_renderer3d			= nullptr;
	PrimitiveBuilder* m_primitiveBuilder	= nullptr;
	DependencyPack m_dependencies;

	gef::Sprite m_background;
	
	// Box2D
	UniquePtr<b2World> m_world;
	ContactListener m_contactListener;

	// Game objects
	UniquePtr<Camera> m_camera;

	Player m_player;
	GameObject m_boundary;
	GameObject m_floor;

	std::array<Platform, NUM_PLATFORMS> m_platforms;
	std::vector<UniquePtr<Powerup>> m_powerups;

	// Meta
	// Initialization is done one step at a time, rendering a loading screen in between.
	// These variables help keep track of this.
	static constexpr int INIT_STAGES = 10;
	int m_initalizationProgress = 0;

	float m_fps = 0.f;
	Scoreboard& m_highscores;

	bool m_isCrazyMode = false;
};