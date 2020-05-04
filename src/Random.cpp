#include "Random.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace glm;

random_device Random::rd;

vec3 Random::facingXZ()
{
	float direction = Random::range(0, 2 * M_PI);
	return vec3(cos(direction), 0, sin(direction));
}

float Random::range(const float min, const float max)
{
	uniform_real_distribution<float> randPos(min, max);
	return randPos(rd);
}

/* same xyz */
vec3 Random::vec3xyz(const float min, const float max)
{
	uniform_real_distribution<float> randPos(min, max);
	return vec3(randPos(rd));
}

/* different xz, y = 0 */
vec3 Random::vec3xz(const float min, const float max)
{
	uniform_real_distribution<float> randPos(min, max);
	return vec3(randPos(rd), 0, randPos(rd));
}