#include "Entity.h"
#include "Draw.h"
#include "Random.h"


#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

void Entity::update(float deltaTime, shared_ptr<vector<shared_ptr<Entity>>>(&entities)[MAP_I][MAP_J][MAP_K],
	int i, int j, int k)
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
	if (wasOutOfBoundsX || wasInFloorX || hasCollided(entities, i, j, k))
		transform.move(-change.x * XAXIS);

	transform.move(change.y * YAXIS);
	wasOutOfBoundsY = isOutOfBounds();
	wasInFloorY = isInFloor();
	if (wasOutOfBoundsY || wasInFloorY || hasCollided(entities, i, j, k))
		transform.move(-change.y * YAXIS);

	transform.move(change.z * ZAXIS);
	wasOutOfBoundsZ = isOutOfBounds();
	wasInFloorZ = isInFloor();
	if (wasOutOfBoundsZ || wasInFloorZ || hasCollided(entities, i, j, k))
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

void Entity::draw(shared_ptr<MatrixStack> &M)
{
	model.draw(M, transform);
}

bool Entity::hasCollided(Entity &entity) const
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

	return myMin.x <= eMax.x && eMin.x <= myMax.x
		&& myMin.y <= eMax.y && eMin.y <= myMax.y
		&& myMin.z <= eMax.z && eMin.z <= myMax.z;
}

bool Entity::hasCollided(shared_ptr<vector<shared_ptr<Entity>>>(&entities)[MAP_I][MAP_J][MAP_K],
	int i, int j, int k)
{

	if (hasCollided(*entities[i][j][k]) ||
		i > 0 && hasCollided(*entities[i - 1][j][k]) || 
		i < MAP_I - 1 && hasCollided(*entities[i + 1][j][k]) ||
		j > 0 && hasCollided(*entities[i][j - 1][k]) ||
		j < MAP_J - 1 && hasCollided(*entities[i][j + 1][k]) ||
		k > 0 && hasCollided(*entities[i][j][k - 1]) ||
		k < MAP_K - 1 && hasCollided(*entities[i][j][k + 1]))
		return true;

	return false;

}

bool Entity::hasCollided(vector<shared_ptr<Entity>> &collectionEntities) {
	shared_ptr<Entity> e;

	for (int i = 0; i < collectionEntities.size(); i++)
	{
		e = collectionEntities[i];
		if (&(*e) != &(*this) && hasCollided(*e)) {
			behavior->onCollision(*e->getBehavior());
			e->getBehavior()->onCollision(*getBehavior());
			return true;
		}
	}
	return false;
}