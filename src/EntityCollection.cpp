#include "EntityCollection.h"
#include "Camera.h"

using namespace std;
using namespace glm;

EntityCollection::EntityCollection() : numActive(0) {
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				entities[i][j][k] = make_shared<vector<shared_ptr<Entity>>>();
			}
		}
	}
}

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
				for (int l = 0; l < entities[i][j][k]->size(); l++) {
					shared_ptr<Entity> entity = entities[i][j][k]->at(l);

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

void EntityCollection::draw(std::shared_ptr<MatrixStack>& M, vec4* planes) const
{
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				for (int l = 0; l < entities[i][j][k]->size(); l++) {
					shared_ptr<Entity> entity = entities[i][j][k]->at(l);

					if (!Camera::viewFrustCull(entity->getTransform().getPosition(), entity->getRadius(), planes)) {
						entity->draw(M);
					}
													
				}
			}
		}
	}
}

void EntityCollection::draw(shared_ptr<Program>& prog, shared_ptr<MatrixStack>& M, vec4* planes) const
{
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				for (int l = 0; l < entities[i][j][k]->size(); l++) {
					shared_ptr<Entity> entity = entities[i][j][k]->at(l);

					if (!Camera::viewFrustCull(entity->getTransform().getPosition(), entity->getRadius(), planes)) {
						entity->draw(prog, M);
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

int EntityCollection::mapXtoI(float x) {
	return (x + MAP_X) * (MAP_I / (MAP_X * 2.0f));
}

int EntityCollection::mapYtoJ(float y) {
	return (y + MAP_Y) * (MAP_J / (MAP_Y * 2.0f));
}

int EntityCollection::mapZtoK(float z) {
	return (z + MAP_Z) * (MAP_K / (MAP_Z * 2.0f));
}

float EntityCollection::mapItoX(int i) {
	return i * (MAP_X * 2.0f / MAP_I) - MAP_X;
}

float EntityCollection::mapJtoY(int j) {
	return j * (MAP_Y * 2.0f / MAP_J) - MAP_Y;
}

float EntityCollection::mapKtoZ(int k) {
	return k * (MAP_Z * 2.0f / MAP_K) - MAP_Z;
}

