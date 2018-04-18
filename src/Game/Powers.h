#pragma once
#include <gef.h>

enum class PowerType : char
{
	JETPACK,
	BOUNCE,
	NUM_POWERS
};

class Player;

struct Power
{
	virtual ~Power() = default;

	virtual void deploy(Player* player) = 0;
	virtual void expire() = 0;

	// Used by powerups with timers
	virtual void update(float dt) { };

	// Used by powerups with charges
	virtual void trigger() { };

	virtual PowerType type() const = 0;

	bool isExpired = false;
	Player* m_player = nullptr;
};

#include "../Util/Timer.h"

struct JetpackPower : Power
{
	void deploy(Player* player) override;
	void update(float dt) override;
	void expire() override;

	PowerType type() const override { return PowerType::JETPACK; }

	constexpr static float UPWARD_FORCE = 25.f;
	InvocationTimer timer;
	const float time = 3.f;
};

struct BouncePower : Power
{
	void deploy(Player* player) override;
	void trigger() override;
	void expire() override;

	PowerType type() const override { return PowerType::BOUNCE; }

	constexpr static float BOUNCE_FORCE = 25.f;
	int charges = 3;
};