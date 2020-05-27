#pragma once

#include "Program.h"
#include "MatrixStack.h"
#include "Entity.h"
#include "Camera.h"

using namespace glm;
using namespace std;

#include <vector>

class EntityCollection
{
	EntityCollection() : numActive(0) {
		for (int i = 0; i < MAP_I; i++) {
			for (int j = 0; j < MAP_J; j++) {
				for (int k = 0; k < MAP_K; k++) {
					entities[i][j][k] = make_shared<vector<shared_ptr<Entity>>>();
				}
			}
		}
	}

public:
	static std::shared_ptr<EntityCollection> getInstance();

	shared_ptr<vector<shared_ptr<Entity>>> entities[MAP_I][MAP_J][MAP_K];

	~EntityCollection() {}

	void update(float deltaTime);
	void draw(std::shared_ptr<MatrixStack>& M, vec4* planes);

	void addEntity(shared_ptr<Entity>& entity);
	shared_ptr<vector<shared_ptr<Entity>>> getEntitiesFromCollectionPos(int i, int j, int k);

	int getNumActive() { return numActive; }
	void incrementNumActive() { numActive++; }
	void decrementNumActive() { numActive--; }

	int mapXtoI(float x);
	int mapYtoJ(float y);
	int mapZtoK(float z);
	float mapItoX(int i);
	float mapJtoY(int j);
	float mapKtoZ(int k);

private:
	int numActive;
};

