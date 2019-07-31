#pragma once

#include <string>
#include <vector>
#include <memory>

struct TileLayer;
struct sf::Vector2i;
namespace XMLParser
{
	void parseTexture(int& tileSize, sf::Vector2i& textureSize, int& columns, const std::string& fileName);

	bool parseLevel(const std::string& levelName, sf::Vector2i& levelSize, std::vector<TileLayer>& tileLayers,
		std::vector<sf::Vector2i>& entityPath, std::vector<sf::Vector2i>& buildingPlacementPosition,
		int& soilderSpawnRate, int& tankSpawnRate, int& planeSpawnRate);
}