#include "Spawner.h"
#include "Random.h"
#include "EntityCollection.h"
#include "GameManager.h"

using namespace std;
using namespace glm;

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
		spawnCoral(rand() % 3);
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
	if (GameManager::getInstance()->getCharRemaining() <= 0)
	{
		cout << "Spawner: No more Characters" << endl;
		return nullptr;
	}
	const Character& c = CHARACTERS[NUM_CHARACTERS - GameManager::getInstance()->getCharRemaining()];
	shared_ptr<Entity> e = make_shared<Entity>(c.shape, Behavior::FOLLOWER);
	findSpawnPosition(e, FOLLOWER_FLOOR_OFFSET);
	e->getTransform()
		.setVelocity(Random::spawnVel())
		.setSize(c.size)
		.syncFacing();
	e->getModel().setTexture(c.texture);

	EntityCollection::getInstance()->addEntity(e);
	return e;
}

void Spawner::spawnPowerup()
{
	shared_ptr<Entity> e = make_shared<Entity>(SPHERE_SHAPE, Behavior::POWERUP);
	findSpawnPosition(e, POWERUP_OFFSET);
	e->getTransform()
		.setSize(vec3(POWERUP_SIZE))
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(POWERUP_MATERIAL);

	EntityCollection::getInstance()->addEntity(e);
}

void Spawner::spawnCoral(int type)
{
	shared_ptr<Entity> e = make_shared<Entity>(CORAL_SHAPES[type], Behavior::NONE);
	findSpawnPosition(e, CORAL_FLOOR_OFFSET);
	e->getTransform()
		.setSize(Random::spawnSize())
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(CORAL_MATERIALS[type]);

	EntityCollection::getInstance()->addEntity(e);
}

void Spawner::spawnEnemy()
{
	shared_ptr<Entity> e = make_shared<Entity>(ENEMY_SHAPE, Behavior::ENEMY);
	findSpawnPosition(e, Random::range(ENEMY_FLOOR_OFFSET_RANGE));
	e->getTransform()
		.setSize(vec3(ENEMY_SIZE))
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(ENEMY_MATERIAL);

	EntityCollection::getInstance()->addEntity(e);
}

void Spawner::findSpawnPosition(shared_ptr<Entity>& entity, float offset)
{ 
	entity->getTransform().setPosition(Random::spawnPos());
	entity->bringToFloor(offset);

	int entityI = EntityCollection::mapXtoI(entity->getTransform().getPosition().x);
	int entityJ = EntityCollection::mapYtoJ(entity->getTransform().getPosition().y);
	int entityK = EntityCollection::mapZtoK(entity->getTransform().getPosition().z);

	while (entity->hasCollided(EntityCollection::getInstance()->entities, entityI, entityJ, entityK))
	{

		entity->getTransform().setPosition(Random::spawnPos());
		entity->bringToFloor(offset);

		entityI = EntityCollection::getInstance()->mapXtoI(entity->getTransform().getPosition().x);
		entityJ = EntityCollection::getInstance()->mapYtoJ(entity->getTransform().getPosition().y);
		entityK = EntityCollection::getInstance()->mapZtoK(entity->getTransform().getPosition().z);
	}
}
