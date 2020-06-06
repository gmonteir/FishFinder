#pragma once

#include "Constants.h"

#include <iostream>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
	Transform() : position(ORIGIN), velocity(ORIGIN), size(1.0), facing(-ZAXIS), speed(1) {}
	~Transform() {}

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getVelocity() const { return velocity; }
	glm::vec3 getSize() const { return size; }
	glm::vec3 getFacing() const { return facing; }
	float getSpeed() const { return speed; }
	float getXZAngle() const { return atan2(facing.x, facing.z); }
	float getYAngle() const { return atan2(facing.y, length(glm::vec2(facing.x, facing.z))); }

	Transform& move(const glm::vec3& change) { position += change; return *this; }
	/* Move by velocity and deltaTime */
	Transform& move(float deltaTime) { return move(velocity * deltaTime); }

	Transform& interpolateVelocity(const glm::vec3& vel, float delta) 
		{ return setVelocity(velocity = mix(this->velocity, vel, delta)); }
	Transform& interpolateVelocityBySpeed(const glm::vec3& vel, float deltaTime) 
		{ return interpolateVelocity(vel, deltaTime * speed); }
	Transform& setPosition(const glm::vec3& pos) { this->position = pos; return *this; }
	Transform& setVelocity(const glm::vec3& vel) { this->velocity = vel; return *this; }

	Transform& setSize(const float size) { return this->setSize(glm::vec3(size)); }
	Transform& setSize(const glm::vec3& size) { this->size = size; return *this; }

	Transform& setFacing(const glm::vec3& facing) { this->facing = normalize(facing); return *this; }
	Transform& syncFacing() { if (velocity != ORIGIN) setFacing(velocity); return *this; }
	Transform& interpolateSyncFacing(float deltaTime) { if (velocity != ORIGIN) interpolateFacing(velocity, deltaTime * speed); return *this; }
	Transform& interpolateFacing(const glm::vec3& face, float deltaTime)
		{ return setFacing(mix(this->facing, face, deltaTime * speed)); }

	Transform& setSpeed(float speed) { this->speed = speed; return *this; }

private:
	glm::vec3 position, velocity, size, facing;
	float speed;
	
};

