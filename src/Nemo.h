#pragma once

#include "Entity.h"
#include "Constants.h"

class Nemo : public Entity
{
public:
	Nemo(const std::string& shapeName) : Entity(shapeName, Behavior::FOLLOWER) {}
	virtual ~Nemo() {}
};
