#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Entity.h"

#include <vector>

class EntityCollection
{
	EntityCollection();

public:
	static std::shared_ptr<EntityCollection> getInstance();

	std::shared_ptr<std::vector<std::shared_ptr<Entity>>> entities[MAP_I][MAP_J][MAP_K];

	~EntityCollection() {}

	void update(float deltaTime);
	void draw(std::shared_ptr<MatrixStack>& M, glm::vec4* planes) const;
	void draw(std::shared_ptr<Program>& prog, std::shared_ptr<MatrixStack>& M, 
		glm::vec4* planes) const;

	void addEntity(std::shared_ptr<Entity>& entity);

	int getNumActive() { return numActive; }
	void incrementNumActive() { numActive++; }
	void decrementNumActive() { numActive--; }

	static int mapXtoI(float x);
	static int mapYtoJ(float y);
	static int mapZtoK(float z);
	static float mapItoX(int i);
	static float mapJtoY(int j);
	static float mapKtoZ(int k);

private:
	int numActive;
};

