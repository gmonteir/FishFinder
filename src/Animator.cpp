#include "Animator.h"

using namespace std;
using namespace glm;

void Animator::animatePart(float dt, float *angle, bool *movingRight, float low, float high)
{
	if (*movingRight)
	{
		*angle += dt;
		if (*angle > high)
			*movingRight = false;
	}
	else
	{
		*angle -= dt;
		if (*angle < low)
			*movingRight = true;
	}
}

// pivot around given part
void Animator::setupPart(const vector<shared_ptr<Shape>> shapes,
			   shared_ptr<MatrixStack> Model, int i, int pivot, vec3 *angle)
{
	float x = calculateShift(shapes.at(pivot)->min.x, shapes.at(pivot)->max.x);
	float y = calculateShift(shapes.at(pivot)->min.y, shapes.at(pivot)->max.y);
	float z = calculateShift(shapes.at(pivot)->min.z, shapes.at(pivot)->max.z);
	Model->pushMatrix();
		Model->translate(vec3(x, y, z));
		Model->rotate(angle->z, vec3(0, 0, 1));
		Model->rotate(angle->y, vec3(0, 1, 0));
		Model->rotate(angle->x, vec3(1, 0, 0));
		Model->translate(vec3(-x, -y, -z));
}

// Calcualtes the shift to be centered at the origin
float Animator::calculateShift(float minCoord, float maxCoord)
{
	float coordExtent;
	coordExtent = maxCoord - minCoord;
	return minCoord + (coordExtent/2.0f);
}

void Animator::NoAnimator::drawModel(shared_ptr<MatrixStack>& M, 
	shared_ptr<Program> prog, const vector<shared_ptr<Shape>>& shapes) const
{
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
	for (auto& shape : shapes)
	{
		shape->draw(prog);
	}
}