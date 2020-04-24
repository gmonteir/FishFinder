#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Entity.h"

#include <iostream>
#include <vector>
#include <functional>

class Entities : public std::vector<std::shared_ptr<Entity>>
{
public:
	static constexpr float SPAWN_DELAY = 5; // seconds

	static std::shared_ptr<Entities> getInstance();

	Entities() : lastFrameTime(0), totalSpawned(0) {}
	~Entities() {}
	
	void init(std::vector<std::shared_ptr<Shape>>& shape); // Note: add player beforehand
	void update(float deltaTime, float gameTime);
	void draw(std::shared_ptr<Program>& prog, std::shared_ptr<MatrixStack>& M);
	void spawnRandom();
	int getSpawned() const { return totalSpawned; }

private:
	float lastFrameTime;
	int totalSpawned;

	std::vector<std::shared_ptr<Shape>> shapes;
};

