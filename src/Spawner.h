#pragma once

#include "Shape.h"
#include "Entity.h"

#include <vector>

class Spawner
{
	Spawner() : lastFrameTime(0), totalSpawned(0) {}

public:
	static constexpr float SPAWN_DELAY = 5; // seconds
	static const int* coralMaterials();
	static const std::string* coralTypes();

	static std::shared_ptr<Spawner> getInstance();
	static void findSpawnPosition(std::shared_ptr<Entity>& entity);

	~Spawner() {}

	void init();
	void update(float deltaTime, float gameTime);
	std::shared_ptr<Entity> spawnFollower();
	void spawnPowerup();
	void spawnCoral(int type);
	std::shared_ptr<Entity> spawnRandom(const std::string& shapeName, int behavior);

	int getSpawned() const { return totalSpawned; }

private:
	float lastFrameTime;
	int totalSpawned;
};

