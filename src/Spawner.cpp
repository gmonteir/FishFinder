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
void Spawner::init(shared_ptr<Entity> player)
{
	this->player = player;

	spawnPowerup();
	for (size_t i = 0; i < NUM_CORAL; i++)
	{
		spawnCoral(Random::integer(NUM_CORAL_TYPES));
	}

	for (size_t i = 0; i < NUM_STATIC_ENEMIES; i++)
	{
		spawnStaticEnemy();
	}

	for (size_t i = 0; i < NUM_MOVING_ENEMIES; i++)
	{
		spawnMovingEnemy();
	}
	cout << "Spawner init" << endl;
}

void Spawner::update(float deltaTime, float gameTime)
{
	if (gameTime - lastFrameTime > SPAWN_DELAY)
	{
		spawnPowerup();
		lastFrameTime = gameTime;
	}
}

Transform* Spawner::spawnFollower()
{
	if (GameManager::getInstance().getCharRemaining() <= 0)
	{
		cout << "Spawner: No more Characters" << endl;
		return nullptr;
	}
	const Character& c = CHARACTERS[NUM_CHARACTERS - GameManager::getInstance().getCharRemaining()];
	shared_ptr<Entity> e = make_shared<Entity>(c.shape, int(Behavior::FOLLOWER));
	findSpawnPosition(e, FOLLOWER_FLOOR_OFFSET);
	e->getTransform()
		.setVelocity(Random::spawnVel())
		.setSize(c.size)
		.setSpeed(FOLLOWER_SPEED)
		.syncFacing();
	e->getModel().setTexture(c.texture);

	EntityCollection::getInstance()->addEntity(e);
	return &e->getTransform();
}

void Spawner::spawnPowerup()
{
	shared_ptr<Entity> e = make_shared<Entity>(SPHERE_SHAPE, int(Behavior::POWERUP));
	findSpawnPosition(e, POWERUP_OFFSET);
	e->getTransform()
		.setSize(vec3(POWERUP_SIZE))
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(POWERUP_MATERIAL)
		.setTextureProgram(REFLECTPROG)
		.enableTexture();

	EntityCollection::getInstance()->addEntity(e);
}

void Spawner::spawnCoral(int type)
{
	shared_ptr<Entity> e = make_shared<Entity>(CORAL_SHAPES[type], int(Behavior::NONE));
	findSpawnPosition(e, CORAL_FLOOR_OFFSET);
	e->getTransform()
		.setSize(Random::spawnSize())
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(CORAL_MATERIALS[type]);

	EntityCollection::getInstance()->addEntity(e);
}

void Spawner::spawnStaticEnemy()
{
	shared_ptr<Entity> e = make_shared<Entity>(STATIC_ENEMY_SHAPE, int(Behavior::STATICENEMY));
	findSpawnPosition(e, Random::range(ENEMY_FLOOR_OFFSET_RANGE));
	e->getTransform()
		.setSize(vec3(ENEMY_SIZE))
		.setFacing(Random::facingXZ());
	e->getModel().setMaterial(ENEMY_MATERIAL);

	EntityCollection::getInstance()->addEntity(e);
}

void Spawner::spawnMovingEnemy()
{
	shared_ptr<Entity> e = make_shared<Entity>(MOVING_ENEMY_SHAPE, int(Behavior::MOVINGENEMY));
	findSpawnPosition(e, Random::range(SHARK_FLOOR_OFFSET_RANGE));
	e->getTransform()
		.setSize(vec3(SHARK_SIZE))
		.setSpeed(SHARK_SPEED)
		.setFacing(Random::facingXZ());
	e->getModel().setTexture(SHARK_TEXTURE)
		.setMaterial(SHARK_MATERIAL);

	dynamic_pointer_cast<Behavior::MovingEnemyBehavior>(e->getBehavior())->setTarget(&player->getTransform());

	EntityCollection::getInstance()->addEntity(e);
}

void Spawner::findSpawnPosition(shared_ptr<Entity>& entity, float offset)
{ 
	entity->getTransform().setPosition(Random::spawnPos());
	entity->bringToFloor(offset);

	int entityI = EntityCollection::mapXtoI(entity->getTransform().getPosition().x);
	int entityJ = EntityCollection::mapYtoJ(entity->getTransform().getPosition().y);
	int entityK = EntityCollection::mapZtoK(entity->getTransform().getPosition().z);

	while (entity->distance(*player) < SPAWN_DISTANCE_FROM_PLAYER 
		|| entity->hasCollided(EntityCollection::getInstance()->entities, entityI, entityJ, entityK))
	{

		entity->getTransform().setPosition(Random::spawnPos());
		entity->bringToFloor(offset);

		entityI = EntityCollection::getInstance()->mapXtoI(entity->getTransform().getPosition().x);
		entityJ = EntityCollection::getInstance()->mapYtoJ(entity->getTransform().getPosition().y);
		entityK = EntityCollection::getInstance()->mapZtoK(entity->getTransform().getPosition().z);
	}
}
