#include "Spawner.h"
#include "Shapes.h"
#include "Powerup.h"
#include "Entities.h"

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
	totalSpawned++;
	spawnRandom(*Shapes::getInstance()->getShape(NEMO_SHAPE), NEMO_TAG);
	Entities::getInstance()->incrementNumActive();
}

void Spawner::spawnPowerup()
{
	totalSpawned++;
	shared_ptr<Entity> e = spawnRandom(*Shapes::getInstance()->getShape(CUBE_SHAPE), POWERUP_TAG);
	Entities::getInstance()->incrementNumActive();
	e->setMaterial(POWERUP_MATERIAL);
	e->getTransform().setVelocity(ORIGIN);
}

void Spawner::spawnCoral(int type)
{	
	shared_ptr<Entity> e = spawnRandom(*Shapes::getInstance()->getShape(coralTypes[type]), CORAL_TAG);
	e->getTransform().setVelocity(ORIGIN);
	e->setMaterial(coralMaterials[type]);
}

shared_ptr<Entity> Spawner::spawnRandom(vector<shared_ptr<Shape>>& shapes, string tag)
{
	shared_ptr<Entity> entity;
	
	if (tag == POWERUP_TAG)
	{
		entity = make_shared<Powerup>(shapes);
	}
	else
	{
		entity = make_shared<Entity>(shapes);
	}
	
	entity->setTag(tag);
	entity->randomRespawn();
	while (entity->hasCollided(*Entities::getInstance()))
	{
		entity->randomRespawn();
	}
	Entities::getInstance()->push_back(entity);
	return entity;
}
