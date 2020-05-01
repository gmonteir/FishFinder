#ifndef _ANIMATE_H_
#define _ANIMATE_H_

#include "Shape.h"
#include "MatrixStack.h"
#include <vector>

void animatePart(float dt, float *angle, bool *movingRight, float low, float high);
float calculateShift(float minCoord, float maxCoord);
void setupPart(const std::vector<std::shared_ptr<Shape>> shape,
	std::shared_ptr<MatrixStack> Model, int i, int pivot, glm::vec3* angle);

#endif
