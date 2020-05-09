#include "EntityCollection.h"

std::shared_ptr<EntityCollection> EntityCollection::getInstance()
{
	static shared_ptr<EntityCollection> instance(new EntityCollection);
	return instance;
}

void EntityCollection::update(float deltaTime)
{
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				if (!entities[i][j][k]->empty()) {
					for (int l = 0; l < entities[i][j][k]->size(); l++) {
						shared_ptr<Entity> entity = entities[i][j][k]->at(l);

						if (entity != NULL) {
							if (entity->shouldRemove())
							{
								entities[i][j][k]->erase(remove(entities[i][j][k]->begin(),
									entities[i][j][k]->end(),
									entities[i][j][k]->at(l)), entities[i][j][k]->end());
							}
							else
							{
								entity->update(deltaTime, entities, i, j, k);
								vec3 entityPos = entity->getTransform().getPosition();

								int entityI = mapXtoI(entityPos.x);
								int entityJ = mapYtoJ(entityPos.y);
								int entityK = mapZtoK(entityPos.z);

								if (entityI != i || entityJ != j || entityK != k) {
									entities[entityI][entityJ][entityK]->push_back(entity);
									entities[i][j][k]->erase(remove(entities[i][j][k]->begin(),
										entities[i][j][k]->end(),
										entities[i][j][k]->at(l)), entities[i][j][k]->end());
								}
							}
						}
					}
				}
			}
		}
	}
}

void EntityCollection::draw(std::shared_ptr<MatrixStack>& M)
{
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				if (!entities[i][j][k]->empty()) {
					for (int l = 0; l < entities[i][j][k]->size(); l++) {
						if (entities[i][j][k]->at(l) != NULL)
							entities[i][j][k]->at(l)->draw(M);
					}
				}
			}
		}
	}
}

void EntityCollection::addEntity(shared_ptr<Entity>& entity) {
	vec3 entityWorldPos = entity->getTransform().getPosition();
	int collectionX = mapXtoI(entityWorldPos.x);
	int collectionY = mapYtoJ(entityWorldPos.y);
	int collectionZ = mapZtoK(entityWorldPos.z);

	entities[collectionX][collectionY][collectionZ]->push_back(entity);
}

shared_ptr<vector<shared_ptr<Entity>>> EntityCollection::getEntitiesFromCollectionPos(int i, 
	int j, int k) {
	return entities[i][j][k];
}

int EntityCollection::mapXtoI(float x) {
	return (x - -1 * MAP_X) * (MAP_I / (float)(MAP_X * 2));
}

int EntityCollection::mapYtoJ(float y) {
	return (y - -1 * MAP_Y) * (MAP_J / (float)(MAP_Y * 2));
}

int EntityCollection::mapZtoK(float z) {
	return (z - -1 * MAP_Z) * (MAP_K / (float)(MAP_Z * 2));
}

float EntityCollection::mapItoX(int i) {
	return i * (MAP_X * 2 / (float)MAP_I) - MAP_X;
}

float EntityCollection::mapJtoY(int j) {
	return j * (MAP_Y * 2 / (float)MAP_J) - MAP_Y;
}

float EntityCollection::mapKtoZ(int k) {
	return k * (MAP_Z * 2 / (float)MAP_K) - MAP_Z;
}

