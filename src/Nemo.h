#pragma once

#include "Entity.h"
#include "Constants.h"

class Nemo : public Entity
{
public:
	Nemo(const std::string& shapeName, std::shared_ptr<Entity> player) 
		: Entity(shapeName), player(player), speed(NEMO_SPEED), offset(NEMO_OFFSET) {
		// override Entity defaults:
		transform.setSize(glm::vec3(NEMO_SIZE))
			.setPosition(-10.0f * ZAXIS);
		bringToFloor();
		model.setTexture(NEMO_TEXTURE);
		model.setProgram(TEXTUREPROG);
	}
	virtual ~Nemo() {}

	void setPathVelocity();
	void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

private:
	std::shared_ptr<Entity> player;
	float speed, offset;
};
