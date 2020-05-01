#include "Model.h"
#include "Draw.h"

#include <iostream>

using namespace std;
using namespace glm;

void Model::draw(shared_ptr<MatrixStack> &M, const Transform& transform) const
{
	shared_ptr<Program> prog = ShaderManager::getInstance()->getShader(program);
	prog->bind();
	ShaderManager::getInstance()->sendUniforms(program);
	M->pushMatrix();
	M->translate(transform.getPosition());
	M->rotate(transform.getXZAngle(), YAXIS);
	M->scale(scale*transform.getSize());
	M->translate(vec3(-1)*shift);
	if (texture)
	{
		texture->bind(prog->getUniform("Texture0"));
	}
	else
	{
		SetMaterial(prog, material);
	}
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
	for (auto& shape : shapes)
	{
		shape->draw(prog);
	}
	M->popMatrix();
	prog->unbind();
}

void Model::extractMinMax()
{
	min.x = min.y = min.z = 1.1754E+38F;
	max.x = max.y = max.z = -1.1754E+38F;

	for (auto& shape : shapes) 
	{
		if (shape->min.x < min.x) min.x = shape->min.x;
		if (shape->max.x > max.x) max.x = shape->max.x;

		if (shape->min.y < min.y) min.y = shape->min.y;
		if (shape->max.y > max.y) max.y = shape->max.y;

		if (shape->min.z < min.z) min.z = shape->min.z;
		if (shape->max.z > max.z) max.z = shape->max.z;
	}
}

void Model::extractShiftScale()
{
	// Center it at the origin
	float maxExtent, xExtent, yExtent, zExtent;
	xExtent = max.x-min.x;
	yExtent = max.y-min.y;
	zExtent = max.z-min.z;
	if (xExtent >= yExtent && xExtent >= zExtent)
	{
		maxExtent = xExtent;
	}
	if (yExtent >= xExtent && yExtent >= zExtent)
	{
		maxExtent = yExtent;
	}
	if (zExtent >= xExtent && zExtent >= yExtent)
	{
		maxExtent = zExtent;
	}
	float xShift, yShift, zShift;
	xShift = min.x + (xExtent / 2.0f);
	yShift = min.y + (yExtent / 2.0f);
	zShift = min.z + (zExtent / 2.0f);
	shift = vec3(xShift, yShift, zShift);
	max.x -= xShift;
	min.x -= xShift;
	max.y -= yShift;
	min.y -= yShift;
	max.z -= zShift;
	min.z -= zShift;
	scale = vec3(2.0f/maxExtent);
}