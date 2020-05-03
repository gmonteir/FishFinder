#pragma once

#include "Entity.h"
#include "Constants.h"

class Nemo : public Entity
{
public:
	Nemo(const std::string& shapeName) 
		: Entity(shapeName), target(nullptr), speed(NEMO_SPEED), offset(NEMO_OFFSET) {
		// override Entity defaults:
		transform.setSize(glm::vec3(NEMO_SIZE));
		model.setTexture(NEMO_TEXTURE);
		model.setProgram(TEXTUREPROG);
	}
	virtual ~Nemo() {}

	void setPathVelocity();
	void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

	void setTarget(Entity* newTarget) { target = newTarget; }

private:
	Entity *target;
	float speed, offset;
};
