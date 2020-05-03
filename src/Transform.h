#pragma once

#include "Constants.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
	Transform() : position(ORIGIN), velocity(ORIGIN), size(1.0), facing(-ZAXIS) {}
	virtual ~Transform() {}

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getVelocity() const { return velocity; }
	glm::vec3 getSize() const { return size; }
	glm::vec3 getFacing() const { return facing; }
	float getXZAngle() const { return atan2(facing.x, facing.z); }

	Transform& syncFacing() { if (velocity != ORIGIN) setFacing(velocity); return *this; }
	Transform& move(const glm::vec3& change) { position += change; return *this; }
	/* Move by velocity and deltaTime */
	Transform& move(const float deltaTime) { return move(velocity * deltaTime); }

	Transform& setPosition(const glm::vec3& pos) { this->position = pos; return *this; }
	Transform& setVelocity(const glm::vec3& vel) { this->velocity = vel; return *this; }
	Transform& setFacing(const glm::vec3& facing) { this->facing = normalize(facing); return *this; }

	Transform& setSize(const float size) { return this->setSize(glm::vec3(size)); }
	Transform& setSize(const glm::vec3& size) { this->size = size; return *this; }

private:
	glm::vec3 position, velocity, size, facing;
	
};

