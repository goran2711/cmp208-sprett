#include "Platform.h"
#include "Filters.h"
#include <graphics/renderer_3d.h>

constexpr UInt32 DEFAULT_COLOUR		= 0xFF33DD33;
constexpr UInt32 BROKEN_COLOUR		= 0xFF3333DD;
constexpr UInt32 MOVING_COLOUR	= 0xFFDD3333;

Platform::Platform()
{
	m_type = Type::Platform;
	setColour(DEFAULT_COLOUR);
}

Platform::Platform(b2World* world, const GOProperties& properties)
	:	GameObject(world, properties)
{
	m_type = Type::Platform;
	setColour(DEFAULT_COLOUR);
}

void Platform::changeDirection()
{
	auto vel = m_body->GetLinearVelocity();
	vel.x *= -1;

	m_body->SetLinearVelocity(vel);
}

void Platform::reset()
{
	m_isBroken = false;
	m_isMoving = false;
	setVelocity({ 0.f, 0.f });
	setAngularVelocity(0.f);
	setColour(DEFAULT_COLOUR);

	// Default collision
	setFilters(MASK_PLATFORM, CATEGORY_PLATFORM);
}

void Platform::setMoving(const b2Vec2& velocity)
{
	m_isMoving = true;
	
	m_body->SetLinearVelocity(velocity);
	setColour(MOVING_COLOUR);

	m_body->SetAngularVelocity(0.f);
}

void Platform::setBroken()
{	
	m_isBroken = true;

	setFilters(MASK_PLATFORM ^ CATEGORY_PLAYER, CATEGORY_PLATFORM);
	setColour(BROKEN_COLOUR);
}
