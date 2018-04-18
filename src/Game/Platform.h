#pragma once
#include "GameObject.h"
#include <graphics/material.h>

class Platform : public GameObject
{
public:
	Platform();
	Platform(b2World* world, const GOProperties& properties);

	void changeDirection();
	
	void reset();

	void setMoving(const b2Vec2& velocity);
	bool isMoving() const { return m_isMoving; }

	void setBroken();
	bool isBroken() const { return m_isBroken; }

private:
	bool m_isBroken = false;
	bool m_isMoving = false;
};

