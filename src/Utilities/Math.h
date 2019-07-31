#pragma once

#include "../Global.h"
#include "../GameObjects.h"

namespace Math
{
	sf::Vector2f getDirection(sf::Vector2f currentPosition, sf::Vector2f destination);
	bool isWithinRange(sf::Vector2f currentPosition, sf::Vector2f targetPosition, float attackRange);
}