#include "ContactListener.h"
#include "../Common/Audio.h"
#include "Player.h"
#include "Platform.h"
#include "Powerup.h"

void ContactListener::BeginContact(b2Contact * contact)
{
 	void* A = contact->GetFixtureA()->GetBody()->GetUserData();
	void* B = contact->GetFixtureB()->GetBody()->GetUserData();

	if (!A || !B)
		return;

	GameObject* objectA = static_cast<GameObject*>(A);
	GameObject* objectB = static_cast<GameObject*>(B);

	if (checkPlayerPowerupCollision(objectA, objectB))
		return;

	if (checkPlayerPlatformCollision(objectA, objectB))
		return;

	if (checkPlayerFloorCollision(objectA, objectB))
		return;

	if (checkPlatformBoundaryCollision(objectA, objectB))
		return;
}

void ContactListener::EndContact(b2Contact * contact)
{
	void* A = contact->GetFixtureA()->GetBody()->GetUserData();
	void* B = contact->GetFixtureB()->GetBody()->GetUserData();

	if (!A || !B)
		return;

	GameObject* objectA = static_cast<GameObject*>(A);
	GameObject* objectB = static_cast<GameObject*>(B);

	if (checkPlayerPowerupCollision(objectA, objectB))
		return;
}

bool ContactListener::checkPlayerPowerupCollision(GameObject * a, GameObject * b) const
{
	Player* player = findAndCast<Player>(GameObject::Type::Player, a, b);
	Powerup* powerup = findAndCast<Powerup>(GameObject::Type::Powerup, a, b);

	if (!player || !powerup)
		return false;

	powerup->apply(player);
	return true;
}

bool ContactListener::checkPlayerPlatformCollision(GameObject* a, GameObject* b) const
{
	Player* player = findAndCast<Player>(GameObject::Type::Player, a, b);
	Platform* platform = findAndCast<Platform>(GameObject::Type::Platform, a, b);

	if (!player || !platform)
		return false;

	player->collide(platform);
	return true;
}

bool ContactListener::checkPlayerFloorCollision(GameObject * a, GameObject * b) const
{
	Player* player = findAndCast<Player>(GameObject::Type::Player, a, b);
	GameObject* floor = findAndCast<GameObject>(GameObject::Type::Floor, a, b);

	if (!player || !floor)
		return false;

	player->m_hasJumped = false;
	return true;
}

bool ContactListener::checkPlatformBoundaryCollision(GameObject * a, GameObject * b) const
{
	Platform* platform = findAndCast<Platform>(GameObject::Type::Platform, a, b);
	GameObject* boundary = findAndCast<GameObject>(GameObject::Type::Boundary, a, b);

	if (!platform || !boundary)
		return false;

	platform->changeDirection();
	return true;
}
