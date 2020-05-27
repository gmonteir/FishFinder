#pragma once

#include "Shape.h"
#include "Entity.h"

#include <vector>

class Spawner
{
	Spawner() : lastFrameTime(0), totalSpawned(0) {}

public:
	static constexpr float SPAWN_DELAY = 5; // seconds

	static std::shared_ptr<Spawner> getInstance();

	~Spawner() {}

	void init();
	void update(float deltaTime, float gameTime);
	std::shared_ptr<Entity> spawnFollower();
	void spawnPowerup();
	void spawnCoral(int type);
	void spawnEnemy();

	void findSpawnPosition(shared_ptr<Entity>& entity, float offset);

	int getSpawned() const { return totalSpawned; }

private:
	float lastFrameTime;
	int totalSpawned;
};

