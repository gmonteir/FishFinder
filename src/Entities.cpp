#include "Entities.h"

using namespace std;
using namespace glm;

void Entities::init(vector<shared_ptr<Shape>>& s)
{
	shapes = s;
}

void Entities::update(float deltaTime, float gameTime)
{
	shared_ptr<Entity> entity;
	auto iter = begin();
	while (iter != end())
	{
		entity = *iter;
		//if (entity->isAlive()) // Removing code, not needed for lab
		//{
			entity->update(deltaTime, *this);
			iter++;
		//}
		//else
		//	iter = erase(iter);
	}
	if (size() < MAX_SPAWN_ENTITIES && gameTime - lastFrameTime > SPAWN_DELAY)
	{
		spawnRandom();
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
