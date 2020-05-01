#pragma once

#include "Shape.h"
#include "Entity.h"

#include <vector>

class Spawner
{
public:
	static constexpr float SPAWN_DELAY = 5; // seconds

	static std::shared_ptr<Spawner> getInstance();

	Spawner() : lastFrameTime(0), totalSpawned(0) {}
	~Spawner() {}

	void init();
	void update(float deltaTime, float gameTime);
	void spawnNemo();
	void spawnPowerup();
	void spawnCoral(int type);
	std::shared_ptr<Entity> spawnRandom(std::vector<std::shared_ptr<Shape>>& shapes, std::string name);
	void findSpawnPosition(std::shared_ptr<Entity>& entity);

	int getSpawned() const { return totalSpawned; }

private:
	float lastFrameTime;
	int totalSpawned;

	std::vector<std::shared_ptr<Shape>> nemos;
	std::vector<std::shared_ptr<Shape>> powerups;
	std::vector<std::string> coralTypes;
	int coralMaterials[3];
};

