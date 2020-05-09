#include "Spawner.h"
#include "Random.h"
#include "EntityCollection.h"

using namespace std;
using namespace glm;

const int* Spawner::coralMaterials()
{
	// protect against potential static initialization ordering error
	static const int materials[] = {
		TREE_CORAL_MATERIAL,
		SOFT_CORAL_MATERIAL,
		ELKHORN_CORAL_MATERIAL
	};
	return materials;
}

const string* Spawner::coralTypes()
{
	// protect against potential static initialization ordering error
	static const string types[] = {
		TREE_CORAL_SHAPE,
		SOFT_CORAL_SHAPE,
		ELKHORN_CORAL_SHAPE
	};
	return types;
}

shared_ptr<Spawner> Spawner::getInstance() {
	static shared_ptr<Spawner> instance(new Spawner);
	return instance;
}

// must be done after player is in Entities
void Spawner::init()
{
	spawnFollower();
	spawnPowerup();
	
	for (size_t i = 0; i < NUM_CORAL; i++)
	{
		int coralType = rand() % 3;

		spawnCoral(coralType);
	}
}

void Spawner::update(float deltaTime, float gameTime)
{
	if (gameTime - lastFrameTime > SPAWN_DELAY)
	{
		spawnPowerup();
		lastFrameTime = gameTime;
	}
}

void Spawner::spawnFollower()
{
	static int i = 0;
	shared_ptr<Entity> e = spawnRandom(i % 2 == 0 ? NEMO_SHAPE : SQUIRT_SHAPE, Behavior::FOLLOWER);
	e->getTransform()
		.setVelocity(Random::spawnVel())
		.setSize(Random::spawnSize())
		.syncFacing();
	e->bringToFloor();
	totalSpawned++;
	EntityCollection::getInstance()->incrementNumActive();
	i++;
}

void Spawner::spawnPowerup()
{
	shared_ptr<Entity> e = spawnRandom(CUBE_SHAPE, Behavior::POWERUP);
	e->getTransform()
		.setSize(vec3(POWERUP_SIZE))
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(POWERUP_MATERIAL);
	e->bringToFloor();
	totalSpawned++;
	EntityCollection::getInstance()->incrementNumActive();
}

void Spawner::spawnCoral(int type)
{	
	shared_ptr<Entity> e = spawnRandom(coralTypes()[type], Behavior::NONE);
	e->getTransform()
		.setSize(Random::spawnSize())
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(coralMaterials()[type]);
	e->bringToFloor();
}

shared_ptr<Entity> Spawner::spawnRandom(const string& shapeName, int behavior)
{
	shared_ptr<Entity> entity = make_shared<Entity>(shapeName, behavior);
	findSpawnPosition(entity);
	EntityCollection::getInstance()->addEntity(entity);
	return entity;
}

void Spawner::findSpawnPosition(shared_ptr<Entity>& entity)
{ 
	entity->getTransform().setPosition(Random::spawnPos());

	int entityI = EntityCollection::getInstance()->mapXtoI(entity->getTransform().getPosition().x);
	int entityJ = EntityCollection::getInstance()->mapYtoJ(entity->getTransform().getPosition().y);
	int entityK = EntityCollection::getInstance()->mapZtoK(entity->getTransform().getPosition().z);

	while (entity->hasCollided(*EntityCollection::getInstance()->entities[entityI][entityJ][entityK]))
	{
		entity->getTransform().setPosition(Random::spawnPos());
	}
}
