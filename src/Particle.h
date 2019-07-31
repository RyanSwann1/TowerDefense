#pragma once

#include "Sprite.h"
#include "Timer.h"

class Window;
class Particle
{
public:
	Particle(sf::Vector2f startingPosition, int tileID);

	bool isExpired() const;

	void update(float deltaTime);
	void render(Window& window) const;

private:
	Sprite m_sprite;
	sf::Vector2f m_position;
	Timer m_aliveTimer;
};