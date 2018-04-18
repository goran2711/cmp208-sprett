#pragma once
#include "GameObject.h"
#include "../Util/UniquePtr.h"

#include <graphics/material.h>

class Platform;
struct Power;

class Player : public GameObject
{
	friend class ContactListener;
public:
	Player() : GameObject() { m_type = Type::Player; }
	Player(b2World* world, const GOProperties& properties) : GameObject(world, properties) { m_type = Type::Player; }

	void reset(const b2Vec2& position);
	void update(float dt);

	void jump();
	void move(float magnitude);

	void applyForce(const b2Vec2& force);

	void givePower(UniquePtr<Power>&& power);

	void setBounceForce(float bounceForce) { m_bounceForce = bounceForce; }

	bool hasPower() const { return m_currentPower != nullptr; }

	constexpr static float DEFAULT_BOUNCE_FORCE = 15.f;
private:
	void collide(Platform* platform);

	gef::Material m_material;
	bool m_hasJumped = false;
	
	float m_bounceForce = DEFAULT_BOUNCE_FORCE;

	float m_maxSpeed = 10.f;
	const float m_acceleration = 2.5f;
	const float m_deceleration = 0.8f;

	UniquePtr<Power> m_currentPower = nullptr;
};

