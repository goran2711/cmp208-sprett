#pragma once
#include <gef.h>
#include <Box2D\Box2D.h>
#include "GameObject.h"

class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

private:
	bool checkPlayerPlatformCollision(GameObject* a, GameObject* b) const;
	bool checkPlayerFloorCollision(GameObject* a, GameObject* b) const;
	bool checkPlayerPowerupCollision(GameObject* a, GameObject* b) const;

	bool checkPlatformBoundaryCollision(GameObject* a, GameObject* b) const;

	template <typename T>
	T* findAndCast(GameObject::Type type, GameObject* a, GameObject* b) const
	{
		if (a->type() == type)
			return static_cast<T*>(a);

		if (b->type() == type)
			return static_cast<T*>(b);

		return nullptr;
	}
};

