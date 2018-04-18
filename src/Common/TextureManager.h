#pragma once
#include <unordered_map>
#include <string>

/*
	The texture manager works as a cache, where each texture is loaded on demand.
	This means scene/state-specific textures are only loaded when the scene/state
	 is entered. It also means that the textures are already ready, should the
	 player leave and the enter the same scene/state again.
	Textures can be unloaded explicitly if desired.
*/

namespace gef
{
	class Platform;
	class Texture;
}

class TextureManager
{
public:
	TextureManager() = delete;

	static void cleanUp();

	static void loadTexture(const std::string& filename, gef::Platform& platform);
	static bool unloadTexture(const std::string& filename);

	static gef::Texture* getTexture(const std::string& filename, gef::Platform& platform);

private:
	static std::unordered_map<std::string, gef::Texture*> m_textures;
};

