#include "XMLParser.h"
#include "Base64.h"
#include "tinyxml.h"
#include <assert.h>
#include <vector>
#include "../Level.h"
#include "../Texture.h"

std::vector<TileLayer> parseTileLayers(const TiXmlElement& rootElement, sf::Vector2i levelSize, std::vector<TileLayer>& tileLayers);
std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, sf::Vector2i levelSize);
std::vector<sf::Vector2i> parseObjectLayer(const TiXmlElement & rootElement, int tileSize, const std::string& layerName, std::vector<sf::Vector2i>& objects);
void parseProperties(const TiXmlElement& rootElement, int& soilderSpawnRate, int& tankSpawnRate, int& planeSpawnRate);

void XMLParser::parseTexture(int& tileSize, sf::Vector2i& textureSize, int& columns, const std::string& fileName)
{
	TiXmlDocument file;
	bool fileLoaded = file.LoadFile(DATA_DIRECTORY + fileName);
	assert(fileLoaded);

	const auto& rootElement = file.RootElement();
	for (const auto* tileSheetElement = rootElement->FirstChildElement();
		tileSheetElement != nullptr; tileSheetElement = tileSheetElement->NextSiblingElement())
	{
		if (tileSheetElement->Value() != std::string("tileset"))
		{
			continue;
		}

		tileSheetElement->FirstChildElement()->Attribute("width", &textureSize.x);
		tileSheetElement->FirstChildElement()->Attribute("height", &textureSize.y);
		tileSheetElement->Attribute("tilewidth", &tileSize);
		columns = textureSize.x / tileSize;
	}
}

bool XMLParser::parseLevel(const std::string & levelName, sf::Vector2i & levelSize, std::vector<TileLayer>& tileLayers, 
	std::vector<sf::Vector2i>& entityPath, std::vector<sf::Vector2i>& buildingPlacementPosition,
	int& soilderSpawnRate, int& tankSpawnRate, int& planeSpawnRate)
{
	TiXmlDocument file;
	if (!file.LoadFile(DATA_DIRECTORY + levelName))
	{
		return false;
	}

	const auto& rootElement = file.RootElement();
	int tileSize = 0;
	rootElement->Attribute("width", &levelSize.x);
	rootElement->Attribute("height", &levelSize.y);
	rootElement->Attribute("tilewidth", &tileSize);

	parseTileLayers(*rootElement, levelSize, tileLayers);
	parseObjectLayer(*rootElement, tileSize, "Entity Path Layer", entityPath);
	parseProperties(*rootElement, soilderSpawnRate, tankSpawnRate, planeSpawnRate);

	//Reverse the entity path
	std::vector<sf::Vector2i> reversedEntityPath;
	reversedEntityPath.reserve(entityPath.size());
	for (int i = entityPath.size() - 1; i >= 0; --i)
	{
		reversedEntityPath.push_back(entityPath[i]);
	}

	entityPath = reversedEntityPath;


	parseObjectLayer(*rootElement, tileSize, "Building Placement Layer", buildingPlacementPosition);

	return true;
}

std::vector<sf::Vector2i> parseObjectLayer(const TiXmlElement & rootElement, int tileSize, const std::string& layerName, std::vector<sf::Vector2i>& objects)
{
	assert(objects.empty());

	for (const auto* entityElementRoot = rootElement.FirstChildElement(); entityElementRoot != nullptr; entityElementRoot = entityElementRoot->NextSiblingElement())
	{
		if (entityElementRoot->Value() != std::string("objectgroup") || entityElementRoot->Attribute("name") != layerName)
		{
			continue;
		}

		for (const auto* entityElement = entityElementRoot->FirstChildElement(); entityElement != nullptr; entityElement = entityElement->NextSiblingElement())
		{
			sf::Vector2i position;
			entityElement->Attribute("x", &position.x);
			entityElement->Attribute("y", &position.y);
			position.y -= tileSize; //Tiled Hack

			objects.emplace_back(position.x, position.y);
		}
	}

	assert(!objects.empty());
	return objects;
}

void parseProperties(const TiXmlElement& rootElement, int & soilderSpawnRate, int & tankSpawnRate, int & planeSpawnRate)
{
	for (const auto* xmlElement = rootElement.FirstChildElement(); xmlElement != nullptr; xmlElement = xmlElement->NextSiblingElement())
	{
		if (xmlElement->Value() != std::string("properties"))
		{
			continue;
		}

		for (const auto* propertyElement = xmlElement->FirstChildElement(); propertyElement != nullptr; propertyElement = propertyElement->NextSiblingElement())
		{	
			std::string propertyName = propertyElement->FirstAttribute()->Value();
			if (propertyName == "SOILDER_SPAWN_RATE")
			{
				propertyElement->Attribute("value", &soilderSpawnRate);
			}
			else if (propertyName == "TANK_SPAWN_RATE")
			{
				propertyElement->Attribute("value", &tankSpawnRate);
			}
			else if (propertyName == "PLANE_SPAWN_RATE")
			{
				propertyElement->Attribute("value", &planeSpawnRate);
			}
		}
	}
}

std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, sf::Vector2i levelSize)
{
	std::vector<std::vector<int>> tileData;
	tileData.reserve(levelSize.y);

	std::string decodedIDs; //Base64 decoded information
	const TiXmlElement* dataNode = nullptr; //Store our node once we find it
	for (const TiXmlElement* e = tileLayerElement.FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("data"))
		{
			dataNode = e;
		}
	}
	assert(dataNode);

	Base64 base64;
	const TiXmlText* text = dataNode->FirstChild()->ToText();
	const std::string t = text->Value();
	decodedIDs = base64.base64_decode(t);

	const std::vector<int> layerColumns(levelSize.x);
	for (int i = 0; i < levelSize.y; ++i)
	{
		tileData.push_back(layerColumns);
	}

	for (int rows = 0; rows < levelSize.y; ++rows)
	{
		for (int cols = 0; cols < levelSize.x; ++cols)
		{
			tileData[rows][cols] = *((int*)decodedIDs.data() + rows * levelSize.x + cols) - 1;
		}
	}

	return tileData;
}

std::vector<TileLayer> parseTileLayers(const TiXmlElement & rootElement, sf::Vector2i levelSize, std::vector<TileLayer>& tileLayers)
{
	assert(tileLayers.empty());
	for (const auto* tileLayerElement = rootElement.FirstChildElement();
		tileLayerElement != nullptr; tileLayerElement = tileLayerElement->NextSiblingElement())
	{
		if (tileLayerElement->Value() != std::string("layer"))
		{
			continue;
		}

		std::vector<std::vector<int>> tileMap = decodeTileLayer(*tileLayerElement, levelSize);
		tileLayers.emplace_back(std::move(tileMap));
	}

	assert(!tileLayers.empty());
	return tileLayers;
}