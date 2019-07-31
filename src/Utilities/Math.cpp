#include "Math.h"

sf::Vector2f Math::getDirection(sf::Vector2f currentPosition, sf::Vector2f destination)
{
	sf::Vector2f vect = sf::Vector2f(destination.x, destination.y) - sf::Vector2f(currentPosition.x, currentPosition.y);
	float length = sqrt(vect.x * vect.x + vect.y * vect.y);

	vect.x /= length;
	vect.y /= length;

	return vect;
}

bool Math::isWithinRange(sf::Vector2f currentPosition, sf::Vector2f targetPosition, float attackRange)
{
	sf::Vector2f vect = targetPosition - currentPosition;
	float length = sqrt(vect.x * vect.x + vect.y * vect.y);
	
	return length <= attackRange;
}