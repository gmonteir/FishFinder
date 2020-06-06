#pragma once

#include "Shape.h"
#include "Entity.h"

#include <memory>
#include <vector>

class Spawner
{
	Spawner() : lastFrameTime(0) {}

public:
	static constexpr float SPAWN_DELAY = 5; // seconds

	static std::shared_ptr<Spawner> getInstance();

	~Spawner() {}

	void init(std::shared_ptr<Entity> player);
	void update(float deltaTime, float gameTime);
	Transform* spawnFollower();
	void spawnPowerup();
	void spawnCoral(int type);
	void spawnStaticEnemy();
	void spawnMovingEnemy(std::shared_ptr<Entity> player);

	void findSpawnPosition(std::shared_ptr<Entity>& entity, float offset);

private:
	float lastFrameTime;
};

