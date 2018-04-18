#pragma once

namespace gef
{
	class Platform;
	class Renderer3D;
	class SpriteRenderer;
	class Font;
	class AudioManager;
}

class PrimitiveBuilder;

struct DependencyPack
{
	DependencyPack(gef::Platform& platform_) : platform(platform_) {}
	gef::Platform& platform;
	gef::SpriteRenderer* spriteRenderer	= nullptr;
	gef::Font* font						= nullptr;
	gef::Renderer3D* renderer3d			= nullptr;

	PrimitiveBuilder* primitiveBuilder	= nullptr;
};