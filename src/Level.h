#pragma once

#include <vector>
#include "Global.h"
#include "GameObjects.h"
#include "Timer.h"
#include "Particle.h"
#include <SFML/Graphics.hpp>

class Window;
struct TileLayer
{
	TileLayer(std::vector<std::vector<int>>&& tileData);

	void render(Window& window, sf::Vector2i levelSize) const;

	std::vector<std::vector<int>> m_tileData;
};

class Level
{
public:
	static std::unique_ptr<Level> loadLevel(const std::string& levelName, eGameDifficulty gameDifficulty);
	
	void addTurretAtPosition(sf::Vector2f position, eTurretType turretType, int& playerScore);
	
	bool isEnded() const;

	void update(float deltaTime, int& playerScore, eGameDifficulty gameDifficulty);
	void render(sf::RenderWindow& window);

private:
	Level();
	std::vector<TileLayer> m_tileLayers;
	std::vector<sf::Vector2i> m_unitMovementPath;
	std::vector<Turret> m_turrets;
	std::vector<Unit> m_units;
	std::vector<Projectile> m_projectiles;
	std::vector<Particle> m_particles;
	sf::Vector2i m_levelSize;
	Timer m_spawnTimer;
	int m_spawnedUnitCount;
	int m_unitsReachedDestination;
	int m_soilderSpawnRate;
	int m_tankSpawnRate;
	int m_planeSpawnRate;

	void spawnNextUnit(eGameDifficulty gameDifficulty);
	void handleInactiveEntities();
	void handleCollisions(int& playerScore);
	void handleParticles();
};