#include "Spawner.h"
#include "Powerup.h"
#include "Entities.h"
#include "Random.h"

using namespace std;
using namespace glm;

shared_ptr<Spawner> Spawner::getInstance() {
	static shared_ptr<Spawner> instance(new Spawner);
	return instance;
}

void Spawner::init()
{
	spawnNemo();
	spawnPowerup();

	coralTypes.push_back(TREE_CORAL_SHAPE);
	coralTypes.push_back(SOFT_CORAL_SHAPE);
	coralTypes.push_back(ELKHORN_CORAL_SHAPE);
	coralMaterials[0] = TREE_CORAL_MATERIAL;
	coralMaterials[1] = SOFT_CORAL_MATERIAL;
	coralMaterials[2] = ELKHORN_CORAL_MATERIAL;
	
	for (size_t i = 0; i < NUM_CORAL; i++)
	{
		int coralType = rand() % 3;

		spawnCoral(coralType);
	}
}

void Spawner::update(float deltaTime, float gameTime)
{
	// pull this out into a spawner class -- can be used for general random spawns
	if (gameTime - lastFrameTime > SPAWN_DELAY)
	{
		spawnPowerup();
		lastFrameTime = gameTime;
	}
}

void Spawner::spawnNemo()
{
	shared_ptr<Entity> e = spawnRandom(NEMO_SHAPE, NEMO_TAG);
	e->getTransform()
		.setVelocity(Random::spawnVel())
		.setSize(Random::spawnSize())
		.syncFacing();
	e->bringToFloor();
	totalSpawned++;
	Entities::getInstance()->incrementNumActive();
}

void Spawner::spawnPowerup()
{
	shared_ptr<Entity> e = spawnRandom(CUBE_SHAPE, POWERUP_TAG);
	e->getTransform()
		.setSize(vec3(POWERUP_SIZE))
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(POWERUP_MATERIAL);
	e->bringToFloor();
	totalSpawned++;
	Entities::getInstance()->incrementNumActive();
}

void Spawner::spawnCoral(int type)
{	
	shared_ptr<Entity> e = spawnRandom(coralTypes[type], CORAL_TAG);
	e->getTransform()
		.setSize(Random::spawnSize())
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(coralMaterials[type]);
	e->bringToFloor();
}

shared_ptr<Entity> Spawner::spawnRandom(const string& shapeName, string tag)
{
	shared_ptr<Entity> entity;
	
	if (tag == POWERUP_TAG)
	{
		entity = make_shared<Powerup>(shapeName);
	}
	else
	{
		entity = make_shared<Entity>(shapeName);
	}
	
	findSpawnPosition(entity);
	entity->setTag(tag);
	Entities::getInstance()->push_back(entity);
	return entity;
}

void Spawner::findSpawnPosition(shared_ptr<Entity>& entity)
{
	entity->getTransform().setPosition(Random::spawnPos());
	while (entity->hasCollided(*Entities::getInstance()))
	{
		entity->getTransform().setPosition(Random::spawnPos());
	}
}
