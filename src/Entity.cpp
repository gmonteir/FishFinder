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
	bool wasOutOfBoundsX = false;
	bool wasOutOfBoundsY = false;
	bool wasOutOfBoundsZ = false;

	bool wasInFloorX = false;
	bool wasInFloorY = false;
	bool wasInFloorZ = false;

	vec3 change = transform.getVelocity() * deltaTime;

	behavior->update(deltaTime);
	model.getAnimator().animate(deltaTime);
	if (change == ORIGIN) // if not moving, skip calculations
		return;

	transform.move(change.x * XAXIS);
	wasOutOfBoundsX = isOutOfBounds();
	wasInFloorX = isInFloor();
	if (wasOutOfBoundsX || wasInFloorX || hasCollided(entities))
		transform.move(-change.x * XAXIS);

	transform.move(change.y * YAXIS);
	wasOutOfBoundsY = isOutOfBounds();
	wasInFloorY = isInFloor();
	if (wasOutOfBoundsY || wasInFloorY || hasCollided(entities))
		transform.move(-change.y * YAXIS);

	transform.move(change.z * ZAXIS);
	wasOutOfBoundsZ = isOutOfBounds();
	wasInFloorZ = isInFloor();
	if (wasOutOfBoundsZ || wasInFloorZ || hasCollided(entities))
		transform.move(-change.z * ZAXIS);

	if (wasOutOfBoundsX || wasOutOfBoundsY || wasOutOfBoundsZ) // event trigger check 
	{

		behavior->onOutOfBounds(deltaTime);
		onOutOfBounds(deltaTime);
	}
	else if (wasInFloorX || wasInFloorY || wasInFloorZ) // event trigger check 
	{
		transform.move(2 * deltaTime * YAXIS * (1 + abs(transform.getVelocity().y)));
	}
}

void Entity::draw(shared_ptr<MatrixStack> &M)
{
	model.draw(M, transform);
}

void Entity::onOutOfBounds(float deltaTime)
{
	if (!isAlive())
		return;

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
			behavior->onCollision(*e->getBehavior()); 
			e->getBehavior()->onCollision(*getBehavior());
			return true;
		}
	}
	return false;
}

void Entity::bringToFloor(float offset) {
	transform.setPosition(glm::vec3(
		transform.getPosition().x,
		transform.getSize().y * model.getScaledSize().y / 2 + offset
		+ Floor::getInstance()->getHeight(transform.getPosition().x, transform.getPosition().z),
		transform.getPosition().z
	));
}