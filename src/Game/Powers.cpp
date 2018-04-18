#include "Powers.h"
#include "Player.h"
#include "Filters.h"

#include "../Common/Audio.h"

// Jetpack power /////////////////////////////////

void JetpackPower::deploy(Player* player)
{
	timer.reset(time, [&] { isExpired = true; });

	if (player->getVelocity().y < 0.f)
		player->setYVelocity(0.f);

	// Disable collision with platforms while powerup is active
	player->setFilters(MASK_PLAYER ^ CATEGORY_PLATFORM, CATEGORY_PLAYER);

	m_player = player;
	Audio::playSample(Audio::JETPACK);
}

void JetpackPower::update(float dt)
{
	timer.tick(dt);

	// Apply thrust for half of the duration
	// (to keep the collision disabled until the player has slowed down a bit)
	if (timer.getTime() > time * 0.5f)
		m_player->applyForce({ 0.f, UPWARD_FORCE });
}

void JetpackPower::expire()
{	
	// Reset collision filters
	m_player->setFilters(MASK_PLAYER, CATEGORY_PLAYER);
}

// Bounce power //////////////////////////////////

void BouncePower::deploy(Player* player)
{
	m_player = player;
	player->setBounceForce(BouncePower::BOUNCE_FORCE);
}

void BouncePower::trigger()
{
	charges -= 1;

	if (charges <= 0)
		isExpired = true;
}

void BouncePower::expire()
{
	m_player->setBounceForce(Player::DEFAULT_BOUNCE_FORCE);
}

