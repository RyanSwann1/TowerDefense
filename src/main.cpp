#include "Global.h"
#include "Utilities/XMLParser.h"
#include <iostream>
#include "Window.h"
#include "Texture.h"
#include "Sprite.h"
#include "Level.h"
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>

//https://en.cppreference.com/w/cpp/language/delete
//https://en.cppreference.com/w/cpp/language/rule_of_three

//http://www.alanzucconi.com/2016/02/03/2d-rotations/
//https://gamedev.stackexchange.com/questions/14602/what-are-atan-and-atan2-used-for-in-games

//https://www.youtube.com/watch?v=vElZc6zSIXM
//https://www.youtube.com/watch?v=8AjRD6mU96s&t=1466s
constexpr int PLAYER_STARTING_SCORE = 5;

void HAPI_Main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 1088), "SFML_WINDOW", sf::Style::Default);
	window.setFramerateLimit(60);

	Textures::getInstance().loadAllTextures();
	
	eGameDifficulty gameDifficulty = eGameDifficulty::NORMAL;
	std::string levelName = "mapOne.tmx";
	std::unique_ptr<Level> level = Level::loadLevel(levelName, gameDifficulty);
	if (!level)
	{
		std::cout << "Couldn't load level\n";
		return;
	}

	Sprite mouseRectSprite(sf::Vector2f(), static_cast<int>(eTileID::SELECTOR));
	sf::Vector2f mouseRectPosition;

	sf::Clock gameClock;
	float deltaTime = 0;

	int playerScore = PLAYER_STARTING_SCORE;

	int tileSize = Textures::getInstance().getTexture().getTileSize();
	sf::Event currentSFMLEvent;
	while (window.isOpen())
	{	
		while (window.pollEvent(currentSFMLEvent))
		{
			if (currentSFMLEvent.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (currentSFMLEvent.type == sf::Event::MouseButtonPressed)
			{
				level->addTurretAtPosition(mouseRectPosition, eTurretType::Cannon, playerScore);
			}
		}

		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

		mouseRectPosition.x = (mousePosition.x / tileSize) * tileSize;
		mouseRectPosition.y = (mousePosition.y / tileSize) * tileSize;
		mouseRectSprite.setPosition(mouseRectPosition);

		level->update(deltaTime, playerScore, gameDifficulty);

		//Render
		window.clear(sf::Color::Black);
		level->render(window);
		//window->render(mouseRectSprite);

		//Handle Events
		if (level->isEnded())
		{
			level = Level::loadLevel(levelName, gameDifficulty);
			playerScore = PLAYER_STARTING_SCORE;
		}

		deltaTime = gameClock.restart().asSeconds();
	}
}