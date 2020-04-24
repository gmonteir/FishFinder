#include "Entities.h"

using namespace std;
using namespace glm;

shared_ptr<Entities> Entities::getInstance() {
	static shared_ptr<Entities> instance(new Entities);
	return instance;
}

void Entities::init(vector<shared_ptr<Shape>>& s)
{
	shapes = s;
	spawnRandom();
}

void Entities::update(float deltaTime, float gameTime)
{
	shared_ptr<Entity> entity;
	for (int i = 0; i < size(); i++)
	{
		entity = at(i);
		entity->update(deltaTime, *this);
	}

	// pull this out into a spawner class -- can be used for general random spawns
	if (size() < MAX_SPAWN_ENTITIES && gameTime - lastFrameTime > SPAWN_DELAY)
	{
		//spawnRandom();
		lastFrameTime = gameTime;
	}
}

void Entities::draw(shared_ptr<Program>& prog, shared_ptr<MatrixStack>& M)
{
	for (shared_ptr<Entity> &entity : *this)
	{
		entity->draw(prog, M);
	}
}
void Entities::spawnRandom()
{
	totalSpawned++;
	unique_ptr<Entity> entity(new Entity(shapes));
	entity->randomRespawn();
	while (entity->hasCollided(*this))
	{
		entity->randomRespawn();
	}
	push_back(move(entity));
}
