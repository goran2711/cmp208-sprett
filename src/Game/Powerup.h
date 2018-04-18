#pragma once
#include "GameObject.h"
#include "Powers.h"

#include "../Util/UniquePtr.h"
#include <graphics/material.h>

class Player;

class Powerup : public GameObject
{
public:
	Powerup() : GameObject() { m_type = Type::Powerup; }
	Powerup(b2World* world, const GOProperties& properties);

	void reset();

	template <typename T>
	void setPower()
	{
		static_assert(std::is_base_of<Power, T>::value, "T must be derived from Power");

		m_power = new T();
		m_isCollected = false;
	}

	void apply(Player* player);

	bool isCollected() const { return m_isCollected; }

private:
	UniquePtr<Power> m_power = nullptr;
	bool m_isCollected = false;
};