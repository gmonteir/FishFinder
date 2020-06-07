#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Entity.h"

#include <vector>

class EntityCollection
{
	EntityCollection() { reset(); }

public:
	static std::shared_ptr<EntityCollection> getInstance();

	~EntityCollection() {}

	void reset();

	void update(float deltaTime);
	void draw(std::shared_ptr<MatrixStack>& M, glm::vec4* planes) const;
	void draw(std::shared_ptr<Program>& prog, std::shared_ptr<MatrixStack>& M, 
		glm::vec4* planes) const;

	bool hasCollided(Entity& entity) const;

	void addEntity(std::shared_ptr<Entity>& entity);

private:
	std::shared_ptr<std::vector<std::shared_ptr<Entity>>> entities[MAP_I][MAP_J][MAP_K];

	static int mapXtoI(float x) { return (x + MAP_X) * MAP_I / 2.0f / MAP_X; }
	static int mapYtoJ(float y) { return (y + MAP_Y) * MAP_J / 2.0f / MAP_Y; }
	static int mapZtoK(float z) { return (z + MAP_Z) * MAP_K / 2.0f / MAP_Z; }
	static float mapItoX(int i) { return i * MAP_X * 2.0f / MAP_I - MAP_X; }
	static float mapJtoY(int j) { return j * MAP_Y * 2.0f / MAP_J - MAP_Y; }
	static float mapKtoZ(int k) { return k * MAP_Z * 2.0f / MAP_K - MAP_Z; }
};

