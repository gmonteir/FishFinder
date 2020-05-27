#pragma once

#include "Constants.h"

#include <random>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Random
{
public:
	static glm::vec3 spawnPos() { return vec3xz(-MAX_SPAWN_DISTANCE, MAX_SPAWN_DISTANCE); }
	static glm::vec3 spawnVel() { return vec3xz(-MAX_SPAWN_VELOCITY, MAX_SPAWN_VELOCITY); }
	static glm::vec3 spawnSize() { return vec3xyz(MIN_SPAWN_SIZE, MAX_SPAWN_SIZE); }

	static glm::vec3 facingXZ();

	static float range(const float min, const float max);
	static float range(const glm::vec2& interval) { return range(interval.x, interval.y); }
	/* same xyz */
	static glm::vec3 vec3xyz(const float min, const float max);
	/* different xz, y = 0 */
	static glm::vec3 vec3xz(const float min, const float max);

private:
	static std::random_device rd;
};

