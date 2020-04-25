#pragma once

#include "Shape.h"

#include <vector>

class Spawner
{
public:
	static constexpr float SPAWN_DELAY = 5; // seconds

	static std::shared_ptr<Spawner> getInstance();

	Spawner() : lastFrameTime(0), totalSpawned(0) {}
	~Spawner() {}

	void init(std::vector<std::shared_ptr<Shape>>& nemo, std::vector<std::shared_ptr<Shape>>& powerup);
	void update(float deltaTime, float gameTime);
	void spawnNemo();
	void spawnPowerup();
	void spawnRandom(std::vector<std::shared_ptr<Shape>>& shapes, std::string name);
	int getSpawned() const { return totalSpawned; }

private:
	float lastFrameTime;
	int totalSpawned;

	std::vector<std::shared_ptr<Shape>> nemos;
	std::vector<std::shared_ptr<Shape>> powerups;
};

