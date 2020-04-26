#include "Spawner.h"
#include "Entities.h"

using namespace std;
using namespace glm;

shared_ptr<Spawner> Spawner::getInstance() {
	static shared_ptr<Spawner> instance(new Spawner);
	return instance;
}

void Spawner::init(vector<shared_ptr<Shape>>& nemos, vector<shared_ptr<Shape>>& powerups)
{
	this->nemos = nemos;
	this->powerups = powerups;
	spawnNemo();
	spawnPowerup();
}

void Spawner::update(float deltaTime, float gameTime)
{
	// pull this out into a spawner class -- can be used for general random spawns
	if (Entities::getInstance()->size() < MAX_SPAWN_ENTITIES 
		&& gameTime - lastFrameTime > SPAWN_DELAY)
	{
		spawnPowerup();
		lastFrameTime = gameTime;
	}
}

void Spawner::spawnNemo()
{
	spawnRandom(nemos, NEMO_TAG);
}

void Spawner::spawnPowerup()
{
	spawnRandom(powerups, POWERUP_TAG);
	Entities::getInstance()->at(Entities::getInstance()->size()-1)->setMaterial(POWERUP_MATERIAL);
}

void Spawner::spawnRandom(vector<shared_ptr<Shape>>& shapes, string tag)
{
	totalSpawned++;
	unique_ptr<Entity> entity(new Entity(shapes));
	entity->setTag(tag);
	entity->randomRespawn();
	while (entity->hasCollided(*Entities::getInstance()))
	{
		entity->randomRespawn();
	}
	Entities::getInstance()->push_back(move(entity));
}
