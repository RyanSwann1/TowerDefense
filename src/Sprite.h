#pragma once

#include "Global.h"
#include <functional>
#include <SFML/Graphics.hpp>

class Texture;
struct Frame;
class Sprite
{
public:
	Sprite();
	Sprite(sf::Vector2f startingPosition, int tileID);

	int getID() const;
	sf::Vector2i getSize() const;
	sf::Vector2f getPosition() const;
	Frame getFrame() const;
	Rectangle getFrameRect() const;
	Texture& getTexture() const;

	void setID(int tileID);
	void setPosition(sf::Vector2f position);

private:
	std::reference_wrapper<Texture> m_texture;
	sf::Vector2f m_position;
	sf::Vector2i m_size;
	int m_tileID;
};