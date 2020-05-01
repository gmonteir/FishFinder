#pragma once

#include "Entity.h"
#include "Keys.h"
#include "Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Nemo : public Entity
{
public:
	Nemo(const std::string& shapeName, std::shared_ptr<Entity> player) 
		: Entity(shapeName), player(player), speed(NEMO_SPEED), offset(NEMO_OFFSET), leftFin(ORIGIN), rightFin(ORIGIN), tail(ORIGIN), leftFoot(ORIGIN), rightFoot(ORIGIN),
		  leftFinRight(false), rightFinRight(false), tailRight(false), leftFootRight(false), rightFootRight(false) {
		// override Entity defaults:
		transform.setSize(glm::vec3(NEMO_SIZE))
			.setPosition(-10.0f * ZAXIS);
		bringToFloor();
		model.setMaterial(NEMO_MATERIAL);
	}
	virtual ~Nemo() {}

	void setPathVelocity();
	void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);
	void draw(std::shared_ptr<MatrixStack> &M) override;
	void animate(float dt);

private:
	std::shared_ptr<Entity> player;
	float speed, offset;
	glm::vec3 leftFin, rightFin, tail, leftFoot, rightFoot;
	bool leftFinRight, rightFinRight, tailRight, leftFootRight, rightFootRight;
};
