#pragma once

#include "Global.h"
#include <string>
#include <vector>
#include <memory>
#include "Texture.h"
#include <SFML/Graphics.hpp>

class Textures
{
public:
	static Textures& getInstance()
	{
		static Textures instance;
		return instance;
	}

	bool loadAllTextures();

	Texture& getTexture();

private:
	std::unique_ptr<Texture> m_texture;
	bool m_texturesLoaded = false;
};

struct Frame
{
	Frame(int x, int y);

	int x;
	int y;
};

class Texture
{
public:
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture&& operator=(Texture&&) = delete;

	static std::unique_ptr<Texture> load(const std::string& xmlFileName, const std::string& textureFileName);
	
	Frame getFrame(int ID) const;
	int getTileSize() const;
	Rectangle getFrameRect(int tileID) const;	
	sf::Vector2i getSize() const;

private:
	Texture();
	sf::Texture m_texture;
	std::vector<Frame> m_frames;
	sf::Vector2i m_textureSize;
	int m_columns;
	int m_tileSize;

	void loadInFrames();
	bool loadTexture(const std::string& xmlFileName, const std::string& textureFileName);
};