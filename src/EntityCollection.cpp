#include "EntityCollection.h"
#include "Camera.h"

using namespace std;
using namespace glm;

std::shared_ptr<EntityCollection> EntityCollection::getInstance()
{
	static shared_ptr<EntityCollection> instance(new EntityCollection);
	return instance;
}

void EntityCollection::reset() {
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				entities[i][j][k] = make_shared<vector<shared_ptr<Entity>>>();
			}
		}
	}
	cout << "EntityCollection reset" << endl;
}

void EntityCollection::update(float deltaTime)
{
	shared_ptr<Entity> entity;
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				for (int l = 0; l < entities[i][j][k]->size(); l++) {
					entity = entities[i][j][k]->at(l);

					if (entity->shouldRemove())
					{
						entities[i][j][k]->erase(remove(entities[i][j][k]->begin(),
							entities[i][j][k]->end(),
							entities[i][j][k]->at(l)), entities[i][j][k]->end());
					}
					else
					{
						entity->update(deltaTime);
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
	shared_ptr<Entity> entity;
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				for (int l = 0; l < entities[i][j][k]->size(); l++) {
					entity = entities[i][j][k]->at(l);

					if (!Camera::shouldCull(entity->getTransform().getPosition(), entity->getRadius(), planes)) {
						entity->draw(M);
					}
													
				}
			}
		}
	}
}

void EntityCollection::draw(shared_ptr<Program>& prog, shared_ptr<MatrixStack>& M, vec4* planes) const
{
	shared_ptr<Entity> entity;
	for (int i = 0; i < MAP_I; i++) {
		for (int j = 0; j < MAP_J; j++) {
			for (int k = 0; k < MAP_K; k++) {
				for (int l = 0; l < entities[i][j][k]->size(); l++) {
					entity = entities[i][j][k]->at(l);

					if (!Camera::shouldCull(entity->getTransform().getPosition(), entity->getRadius(), planes)) {
						entity->draw(prog, M);
					}
				}
			}
		}
	}
}

bool EntityCollection::hasCollided(Entity& entity) const
{
	int i = EntityCollection::mapXtoI(entity.getTransform().getPosition().x);
	int j = EntityCollection::mapYtoJ(entity.getTransform().getPosition().y);
	int k = EntityCollection::mapZtoK(entity.getTransform().getPosition().z);

	return entity.hasCollided(*entities[i][j][k]) 
		|| i > 0         && entity.hasCollided(*entities[i - 1][j][k]) 
		|| i < MAP_I - 1 && entity.hasCollided(*entities[i + 1][j][k]) 
		|| j > 0         && entity.hasCollided(*entities[i][j - 1][k]) 
		|| j < MAP_J - 1 && entity.hasCollided(*entities[i][j + 1][k]) 
		|| k > 0         && entity.hasCollided(*entities[i][j][k - 1]) 
		|| k < MAP_K - 1 && entity.hasCollided(*entities[i][j][k + 1]);
}

void EntityCollection::addEntity(shared_ptr<Entity>& entity) {
	vec3 entityWorldPos = entity->getTransform().getPosition();
	int collectionX = mapXtoI(entityWorldPos.x);
	int collectionY = mapYtoJ(entityWorldPos.y);
	int collectionZ = mapZtoK(entityWorldPos.z);

	entities[collectionX][collectionY][collectionZ]->push_back(entity);
}

