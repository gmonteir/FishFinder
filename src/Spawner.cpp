#include "Spawner.h"
#include "Random.h"
#include "EntityCollection.h"

using namespace std;
using namespace glm;

const std::string Spawner::CHARACTER_SHAPES[NUM_CHARACTERS] {
	MARLIN_SHAPE,
	NEMO_SHAPE,
	SQUIRT_SHAPE,
	BLOAT_SHAPE,
	GURGLE_SHAPE,
	JENNY_SHAPE,
	CHARLIE_SHAPE
};

const std::string Spawner::CHARACTER_TEXTURES[NUM_CHARACTERS]{
	MARLIN_TEXTURE,
	NEMO_TEXTURE,
	SQUIRT_TEXTURE,
	BLOAT_TEXTURE,
	GURGLE_TEXTURE,
	JENNY_TEXTURE,
	CHARLIE_TEXTURE
};

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
	// spawnFollower(); moved to main so we can save the first target position
	spawnPowerup();
	
	for (size_t i = 0; i < NUM_CORAL; i++)
	{
		int coralType = rand() % 3;

		spawnCoral(coralType);
	}

	for (size_t i = 0; i < NUM_ENEMIES; i++)
	{
		spawnEnemy();
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

shared_ptr<Entity> Spawner::spawnFollower()
{
	static int i = 0;
	shared_ptr<Entity> e = spawnRandom(CHARACTER_SHAPES[i%NUM_CHARACTERS], Behavior::FOLLOWER, FOLLOWER_OFFSET);
	e->getTransform()
		.setVelocity(Random::spawnVel())
		.setSize(Random::spawnSize())
		.syncFacing();
	e->getModel().setTexture(CHARACTER_TEXTURES[i % NUM_CHARACTERS]);
	totalSpawned++;
	EntityCollection::getInstance()->incrementNumActive();
	i++;
	return e;
}

void Spawner::spawnPowerup()
{
	shared_ptr<Entity> e = spawnRandom(SPHERE_SHAPE, Behavior::POWERUP, POWERUP_OFFSET);
	e->getTransform()
		.setSize(vec3(POWERUP_SIZE))
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(POWERUP_MATERIAL);
	totalSpawned++;
	EntityCollection::getInstance()->incrementNumActive();
}

void Spawner::spawnCoral(int type)
{	
	shared_ptr<Entity> e = spawnRandom(coralTypes()[type], Behavior::NONE, FLOOR_OFFSET);
	e->getTransform()
		.setSize(Random::spawnSize())
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(coralMaterials()[type]);
}

void Spawner::spawnEnemy()
{
	shared_ptr<Entity> e = spawnRandom(ENEMY_SHAPE, Behavior::ENEMY, FLOOR_OFFSET);
	e->getTransform()
		.setSize(vec3(ENEMY_SIZE))
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(ENEMY_MATERIAL);
}

shared_ptr<Entity> Spawner::spawnRandom(const string& shapeName, int behavior, float offset)
{
	shared_ptr<Entity> entity = make_shared<Entity>(shapeName, behavior);
	findSpawnPosition(entity, offset);
	EntityCollection::getInstance()->addEntity(entity);
	return entity;
}

void Spawner::findSpawnPosition(shared_ptr<Entity>& entity, float offset)
{ 
	entity->getTransform().setPosition(Random::spawnPos());
	entity->bringToFloor(offset);

	int entityI = EntityCollection::getInstance()->mapXtoI(entity->getTransform().getPosition().x);
	int entityJ = EntityCollection::getInstance()->mapYtoJ(entity->getTransform().getPosition().y);
	int entityK = EntityCollection::getInstance()->mapZtoK(entity->getTransform().getPosition().z);

	while (entity->hasCollided(EntityCollection::getInstance()->entities, entityI, entityJ, entityK))
	{

		entity->getTransform().setPosition(Random::spawnPos());
		entity->bringToFloor(offset);

		entityI = EntityCollection::getInstance()->mapXtoI(entity->getTransform().getPosition().x);
		entityJ = EntityCollection::getInstance()->mapYtoJ(entity->getTransform().getPosition().y);
		entityK = EntityCollection::getInstance()->mapZtoK(entity->getTransform().getPosition().z);
	}
}
