#pragma once
#include <gef.h>

/*
	Box2D collision filters describe which objects should collide with each other
*/

// Categories
constexpr UInt16 CATEGORY_DEFAULT	= (1 << 0);
constexpr UInt16 CATEGORY_PLAYER	= (1 << 1);
constexpr UInt16 CATEGORY_PLATFORM	= (1 << 2);
constexpr UInt16 CATEGORY_GROUND	= (1 << 3);
constexpr UInt16 CATEGORY_POWERUP	= (1 << 4);

// Masks
constexpr UInt16 MASK_ALL		= ((1 << 16) - 1);
// Player collides with everything except other players
constexpr UInt16 MASK_PLAYER	= MASK_ALL ^ CATEGORY_PLAYER;
// Platform collides with ground, other platforms and the player
constexpr UInt16 MASK_PLATFORM	= CATEGORY_GROUND | CATEGORY_PLATFORM | CATEGORY_PLAYER;
// Ground collides with everything
constexpr UInt16 MASK_GROUND	= MASK_ALL;
// Powerups collide with the player
constexpr UInt16 MASK_POWERUP	= CATEGORY_PLAYER;