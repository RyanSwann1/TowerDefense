#include "Sprite.h"
#include "Texture.h"
#include <assert.h>

Sprite::Sprite()
	: m_texture(Textures::getInstance().getTexture()),
	m_position(),
	m_size(Textures::getInstance().getTexture().getTileSize(), Textures::getInstance().getTexture().getTileSize()),
	m_tileID(static_cast<int>(eTileID::INVALID))
{}

Sprite::Sprite(sf::Vector2f startingPosition, int tileID)
	: m_texture(Textures::getInstance().getTexture()),
	m_position(startingPosition),
	m_size(Textures::getInstance().getTexture().getTileSize(), Textures::getInstance().getTexture().getTileSize()),
	m_tileID(tileID)
{}

int Sprite::getID() const
{
	return m_tileID;
}

sf::Vector2i Sprite::getSize() const
{
	return m_size;
}

sf::Vector2f Sprite::getPosition() const
{
	return m_position;
}

Texture & Sprite::getTexture() const
{
	return m_texture.get();
}

Frame Sprite::getFrame() const
{
	return m_texture.get().getFrame(m_tileID);
}

Rectangle Sprite::getFrameRect() const
{
	return m_texture.get().getFrameRect(m_tileID);
}

void Sprite::setID(int tileID)
{
	m_tileID = tileID;
}

void Sprite::setPosition(sf::Vector2f position)
{
	m_position = position;
}