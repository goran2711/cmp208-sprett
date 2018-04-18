#include "TextureManager.h"
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>

std::unordered_map<std::string, gef::Texture*> TextureManager::m_textures;

void TextureManager::cleanUp()
{
	for (auto& texture : m_textures)
		delete texture.second;
}

gef::Texture* TextureManager::getTexture(const std::string& filename, gef::Platform& platform)
{
	if (m_textures.find(filename) == m_textures.end())
		loadTexture(filename, platform);
	
	return m_textures[filename];
}

bool TextureManager::unloadTexture(const std::string& filename)
{
	gef::Texture* texture = m_textures[filename];
	m_textures.erase(filename);

	if (!texture)
		return false;

	delete texture;
	texture = nullptr;

	return true;
}

void TextureManager::loadTexture(const std::string& filename, gef::Platform& platform)
{
	gef::ImageData data;
	gef::PNGLoader loader;
	
	loader.Load(filename.c_str(), platform, data);
	
	m_textures[filename] = gef::Texture::Create(platform, data);
}
