#pragma once

#include "Entity.h"
#include "Keys.h"
#include "Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Nemo : public Entity
{
public:
	Nemo(std::vector<std::shared_ptr<Shape>>& shapes) 
		: Entity(shapes), leftFin(ORIGIN), rightFin(ORIGIN), tail(ORIGIN), leftFoot(ORIGIN), rightFoot(ORIGIN),
		  leftFinRight(false), rightFinRight(false), tailRight(false), leftFootRight(false), rightFootRight(false) {
		// override Entity defaults:
		size = glm::vec3(NEMO_SIZE);
		position = glm::vec3(0, size.y*scale.y*(max.y-min.y)/2 + FLOOR_POSITION.y + 0.1, -10);
		material = NEMO_MATERIAL;
	}
	virtual ~Nemo() {}

	void draw(std::shared_ptr<MatrixStack> &M) override;
	void animate(float dt);

private:
	glm::vec3 leftFin, rightFin, tail, leftFoot, rightFoot;
	bool leftFinRight, rightFinRight, tailRight, leftFootRight, rightFootRight;
};
