#include "Powerup.h"
#include "Player.h"
#include "Powers.h"

#include <graphics/renderer_3d.h>

Powerup::Powerup(b2World* world, const GOProperties& properties)
	:	GameObject(world, properties)
{
	m_type = Type::Powerup;
}

void Powerup::reset()
{
	m_power = nullptr;
	m_isCollected = false;
}

void Powerup::apply(Player* player)
{
	if (player->hasPower() || !m_power)
		return;

	player->givePower(std::move(m_power));
	m_isCollected = true;
}
