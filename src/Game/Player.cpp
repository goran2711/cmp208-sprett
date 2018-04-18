#include "Player.h"
#include "../Common/Audio.h"
#include "Platform.h"
#include "Powers.h"
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>

void Player::reset(const b2Vec2& position)
{
	setPosition(position);
	setVelocity({ 0.f, 0.f });
	setAngularVelocity(0.f);
	m_hasJumped = false;

	if (m_currentPower)
	{
		m_currentPower->expire();
		m_currentPower = nullptr;
	}
}

void Player::update(float dt)
{
	if (m_currentPower)
	{
		m_currentPower->update(dt);

		if (m_currentPower->isExpired)
		{
			m_currentPower->expire();
			m_currentPower = nullptr;
		}
	}

	GameObject::update();
}

void Player::jump()
{
	if (m_hasJumped)
		return;

	m_body->ApplyLinearImpulseToCenter({ 0.f, 10.f }, true);
	m_hasJumped = true;
}

void Player::move(float magnitude)
{
	if (!m_hasJumped)
		return;

	auto vel = m_body->GetLinearVelocity();
	float acceleration = m_acceleration * std::fabsf(magnitude);
	float desiredX = 0.f;

	if (magnitude > 0.f)
		desiredX = b2Min(vel.x + acceleration, m_maxSpeed);
	else if (magnitude < 0.f)
		desiredX = b2Max(vel.x - acceleration, -m_maxSpeed);
	else
		desiredX = vel.x * m_deceleration;

	float deltaV = desiredX - vel.x;
	float impulse = m_body->GetMass() * deltaV;

	m_body->ApplyLinearImpulseToCenter({ impulse, 0.f }, true);
}

void Player::applyForce(const b2Vec2 & force)
{
	m_body->ApplyForceToCenter(force, true);
}

void Player::givePower(UniquePtr<Power>&& power)
{
	m_currentPower = std::move(power);

	if (m_currentPower)
		m_currentPower->deploy(this);
}

void Player::collide(Platform* platform)
{
	if (platform->isBroken())
		return;

	// Collision response if the player's "feet" are within the top half of the platform and ...
	float playerBottom = getPositionVec2().y - (getSize().y() * 0.25f);
	float platformTop = platform->getPositionVec2().y + (platform->getSize().y() * 0.25f);

	if (playerBottom < platformTop)
		return;

	// ... if the player is falling
	auto vel = m_body->GetLinearVelocity();
	if (vel.y > 0.f)
		return;

	// Update active powerup and use the appropriate sound effect
	auto soundEffect = Audio::BOUNCE;
	if (m_currentPower)
	{
		m_currentPower->trigger();
		soundEffect = Audio::BEDSPRING;
	}

	// Bounce
	m_body->SetLinearVelocity({ vel.x, m_bounceForce });
	Audio::playSample(soundEffect);
}
