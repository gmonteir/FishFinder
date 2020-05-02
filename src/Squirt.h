#pragma once

#include "Entity.h"
#include "Keys.h"
#include "Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Squirt : public Entity
{
public:
	Squirt(std::vector<std::shared_ptr<Shape>>& shapes) 
		: Entity(shapes), leftFin(ORIGIN), rightFin(ORIGIN), leftFoot(ORIGIN), rightFoot(ORIGIN),
		  leftFinRight(false), rightFinRight(false), leftFootRight(false), rightFootRight(false) {
		// override Entity defaults:
		size = glm::vec3(NEMO_SIZE);
		position = glm::vec3(5, size.y*scale.y*(max.y-min.y)/2 + FLOOR_POSITION.y + 0.1, -10);
	}
	virtual ~Squirt() {}

	void draw(std::shared_ptr<MatrixStack> &M) override;
	void animate(float dt);

private:
	glm::vec3 leftFin, rightFin, leftFoot, rightFoot;
	bool leftFinRight, rightFinRight, leftFootRight, rightFootRight;
};
