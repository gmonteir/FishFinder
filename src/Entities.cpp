#include "Entities.h"
#include "ShaderManager.h"

using namespace std;
using namespace glm;

shared_ptr<Entities> Entities::getInstance() {
	static shared_ptr<Entities> instance(new Entities);
	return instance;
}

void Entities::update(float deltaTime)
{
	shared_ptr<Entity> entity;
	for (int i = 0; i < size(); i++)
	{
		entity = at(i);
		entity->update(deltaTime, *this);
	}
}

void Entities::draw(shared_ptr<MatrixStack>& M)
{
	for (shared_ptr<Entity> &entity : *this)
	{
		entity->draw(M);
	}
}
