#pragma once

#include "Shape.h"
#include "Entity.h"

#include <vector>

class Spawner
{
	Spawner() : lastFrameTime(0) {}

public:
	static constexpr float SPAWN_DELAY = 5; // seconds

	static std::shared_ptr<Spawner> getInstance();

	~Spawner() {}

	void init();
	void update(float deltaTime, float gameTime);
	Transform* spawnFollower();
	void spawnPowerup();
	void spawnCoral(int type);
	void spawnStaticEnemy();
	void spawnMovingEnemy();

	void findSpawnPosition(std::shared_ptr<Entity>& entity, float offset);

private:
	float lastFrameTime;
};

