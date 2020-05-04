#include "Nemo.h"

#include <iostream>

using namespace std;
using namespace glm;

void Nemo::setPathVelocity()
{
	vec3 direction(target->getTransform().getPosition() - transform.getPosition());
	vec3 normal(normalize(direction));

	transform.setVelocity(length(direction) > offset ? normal * speed : ORIGIN)
		.syncFacing();
}

void Nemo::update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities)
{
	if (target)
		setPathVelocity();
	Entity::update(deltaTime, entities);
}