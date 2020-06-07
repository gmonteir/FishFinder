#include "Entity.h"
#include "Draw.h"
#include "Random.h"
#include "EntityCollection.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

void Entity::update(float deltaTime)
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
	if (wasOutOfBoundsX || wasInFloorX || EntityCollection::getInstance()->hasCollided(*this))
		transform.move(-change.x * XAXIS);

	transform.move(change.y * YAXIS);
	wasOutOfBoundsY = isOutOfBounds();
	wasInFloorY = isInFloor();
	if (wasOutOfBoundsY || wasInFloorY || EntityCollection::getInstance()->hasCollided(*this))
		transform.move(-change.y * YAXIS);

	transform.move(change.z * ZAXIS);
	wasOutOfBoundsZ = isOutOfBounds();
	wasInFloorZ = isInFloor();
	if (wasOutOfBoundsZ || wasInFloorZ || EntityCollection::getInstance()->hasCollided(*this))
		transform.move(-change.z * ZAXIS);

	if (wasOutOfBoundsX || wasOutOfBoundsY || wasOutOfBoundsZ) // event trigger check 
	{
		behavior->onOutOfBounds(deltaTime);
	}
	else if (wasInFloorX || wasInFloorY || wasInFloorZ) // event trigger check 
	{
		transform.move(2 * deltaTime * YAXIS * (1 + abs(transform.getVelocity().y)));
	}
}

void Entity::draw(shared_ptr<MatrixStack> &M) const
{
	model.draw(M, transform);
}

void Entity::draw(shared_ptr<Program>& prog, shared_ptr<MatrixStack>& M) const
{
	model.draw(prog, M, transform);
}

bool Entity::hasCollided(Entity &entity)
{
	shared_ptr<Behavior> b = entity.behavior;
	if (b->getType() == Behavior::FOLLOWER 
		&& static_pointer_cast<Behavior::FollowerBehavior>(b)->isFollowing())
	{
		return false;
	}

	vec3 myMin(getMinBoundCoordinate());
	vec3 myMax(getMaxBoundCoordinate());
	vec3 eMin(entity.getMinBoundCoordinate());
	vec3 eMax(entity.getMaxBoundCoordinate());

	if (myMin.x <= eMax.x && eMin.x <= myMax.x
		&& myMin.y <= eMax.y && eMin.y <= myMax.y
		&& myMin.z <= eMax.z && eMin.z <= myMax.z)
	{
		behavior->onCollision(*b);
		b->onCollision(*behavior);
		return true;
	}
	else
	{
		return false;
	}
}

bool Entity::hasCollided(vector<shared_ptr<Entity>> &collectionEntities)
{
	shared_ptr<Entity> e;
	for (int i = 0; i < collectionEntities.size(); i++)
	{
		e = collectionEntities[i];
		if (&(*e) != &(*this) && hasCollided(*e)) {
			return true;
		}
	}
	return false;
}