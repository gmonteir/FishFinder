#include "Entity.h"
#include "Draw.h"
#include "Random.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

void Entity::update(float deltaTime, std::vector<std::shared_ptr<Entity>> &entities)
{
	if (!isAlive())
		return;

	vec3 change = transform.getVelocity() * deltaTime;
	transform.move(change.x * XAXIS);
	if (hasCollided(entities))
		transform.move(-change.x * XAXIS);
	transform.move(change.y * YAXIS);
	if (hasCollided(entities))
		transform.move(-change.y * YAXIS);
	transform.move(change.z * ZAXIS);
	if (hasCollided(entities))
		transform.move(-change.z * ZAXIS);

	if (isOutOfBounds()) // event trigger check 
		onOutOfBounds(deltaTime);
}

void Entity::draw(shared_ptr<MatrixStack> &M)
{
	model.draw(M, transform);
}

void Entity::onOutOfBounds(float deltaTime)
{
	transform.setVelocity(-transform.getVelocity())
		.move(deltaTime)
		.syncFacing();
}

bool Entity::hasCollided(Entity &entity) const
{
	/*if (!entity->isAlive())
		return false;*/
	vec3 myMin(getMinBoundCoordinate());
	vec3 myMax(getMaxBoundCoordinate());
	vec3 eMin(entity.getMinBoundCoordinate());
	vec3 eMax(entity.getMaxBoundCoordinate());

	return myMin.x <= eMax.x && eMin.x <= myMax.x
		&& myMin.y <= eMax.y && eMin.y <= myMax.y
		&& myMin.z <= eMax.z && eMin.z <= myMax.z;
}

bool Entity::hasCollided(std::vector<std::shared_ptr<Entity>> &entities)
{
	shared_ptr<Entity> e;
	for (int i = 0; i < entities.size(); i++)
	{
		e = entities[i];
		if (&(*e) != &(*this) && hasCollided(*e)) {
			onCollision(*e); 
			e->onCollision(*this);
			return true;
		}
	}
	return false;
}