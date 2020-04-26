#include "Entity.h"
#include "Draw.h"

#include <iostream>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

void Entity::randomRespawn()
{
	random_device rd;
	uniform_real_distribution<float> randPos(-MAX_SPAWN_DISTANCE, MAX_SPAWN_DISTANCE);
	uniform_real_distribution<float> randVel(-MAX_SPAWN_VELOCITY, MAX_SPAWN_VELOCITY);
	uniform_real_distribution<float> randSize(MIN_SPAWN_SIZE, MAX_SPAWN_SIZE);
	if (tag == POWERUP_TAG)
		size = vec3(POWERUP_SIZE);
	else
		size = vec3(randSize(rd));
	position = vec3(randPos(rd), scale.y*size.y*(max.y-min.y)/2 + FLOOR_POSITION.y + 0.2, randPos(rd));
	velocity = vec3(randVel(rd), 0, randVel(rd));
	facing = velocity;
}

void Entity::update(float deltaTime, std::vector<std::shared_ptr<Entity>> &entities)
{
	if (!isAlive())
		return;

	vec3 change = velocity * deltaTime;
	position.x += change.x;
	if (hasCollided(entities))
		position.x -= change.x;
	position.y += change.y;
	if (hasCollided(entities))
		position.y -= change.y;
	position.z += change.z;
	if (hasCollided(entities))
		position.z -= change.z;

	if (isOutOfBounds()) // event trigger check 
		onOutOfBounds(deltaTime);
}

void Entity::draw(shared_ptr<MatrixStack> &M)
{
	shared_ptr<Program> prog = ShaderManager::getInstance()->getShader(SIMPLEPROG);
	prog->bind();
	ShaderManager::getInstance()->sendUniforms(SIMPLEPROG);
	M->pushMatrix();
	M->translate(position);
	M->rotate(atan2(facing.x, facing.z), YAXIS);
	M->scale(scale*size);
	M->translate(vec3(-1)*shift);
	if (texture)
	{
		texture->bind(prog->getUniform("Texture0"));
	}
	else
	{
		SetMaterial(prog, material);
	}
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
	for (auto& shape : shapes)
	{
		shape->draw(prog);
	}
	M->popMatrix();
	prog->unbind();
}

void Entity::onOutOfBounds(float deltaTime)
{
	velocity = -velocity;
	position += velocity * deltaTime;
	facing = velocity;
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

void Entity::extractMinMax()
{
	min.x = min.y = min.z = 1.1754E+38F;
	max.x = max.y = max.z = -1.1754E+38F;

	for (auto& shape : shapes) 
	{
		if (shape->min.x < min.x) min.x = shape->min.x;
		if (shape->max.x > max.x) max.x = shape->max.x;

		if (shape->min.y < min.y) min.y = shape->min.y;
		if (shape->max.y > max.y) max.y = shape->max.y;

		if (shape->min.z < min.z) min.z = shape->min.z;
		if (shape->max.z > max.z) max.z = shape->max.z;
	}
	extractShiftScale();
}

void Entity::extractShiftScale()
{
	// Center it at the origin
	float maxExtent, xExtent, yExtent, zExtent;
	xExtent = max.x-min.x;
	yExtent = max.y-min.y;
	zExtent = max.z-min.z;
	if (xExtent >= yExtent && xExtent >= zExtent)
	{
		maxExtent = xExtent;
	}
	if (yExtent >= xExtent && yExtent >= zExtent)
	{
		maxExtent = yExtent;
	}
	if (zExtent >= xExtent && zExtent >= yExtent)
	{
		maxExtent = zExtent;
	}
	float xShift, yShift, zShift;
	xShift = min.x + (xExtent / 2.0f);
	yShift = min.y + (yExtent / 2.0f);
	zShift = min.z + (zExtent / 2.0f);
	shift = vec3(xShift, yShift, zShift);
	max.x -= xShift;
	min.x -= xShift;
	max.y -= yShift;
	min.y -= yShift;
	max.z -= zShift;
	min.z -= zShift;
	scale = vec3(2.0f/maxExtent);
}