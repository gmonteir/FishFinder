#pragma once

#include "Entity.h"
#include "Constants.h"

#include <vector>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Powerup : public Entity
{
public:
	Powerup(const std::string& shapeName) : Entity(shapeName, Behavior::POWERUP) {}
};

