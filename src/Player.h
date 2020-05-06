#pragma once

#include "Entity.h"
#include "Keys.h"
#include "Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Player : public Entity
{
public:
	Player(const std::string shapeName) : Entity(shapeName, Behavior::PLAYER) {}
	virtual ~Player() {}

	void rotate(float dx, float dy) { static_pointer_cast<Behavior::PlayerBehavior>(behavior)->rotate(dx, dy); }
	int getScore() const { return static_pointer_cast<Behavior::PlayerBehavior>(behavior)->getScore(); }
	float getStamina() const { return static_pointer_cast<Behavior::PlayerBehavior>(behavior)->getStamina(); }
};

