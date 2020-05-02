#include "Squirt.h"
#include "Entities.h"
#include "Spawner.h"
#include "Animate.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace glm;

void Squirt::animate(float dt)
{
	animatePart(dt, &leftFin.z, &leftFinRight, -0.6, 0.6);
	leftFin.x = leftFin.z;
	animatePart(dt, &rightFin.z, &rightFinRight, -0.6, 0.6);
	rightFin.x = -rightFin.z;
	animatePart(dt, &leftFoot.z, &leftFootRight, -0.6, 0.6);
	rightFoot.z = -leftFoot.z;
}

void Squirt::draw(shared_ptr<MatrixStack> &M)
{
	/* Dory Parts by Index
	0 - body
	1 - left fin
	2 - right fin
	3 - left foot
	4 - right foot
	5 - left fin joint
	6 - right fin joint
	7 - left foot joint
	8 - right foot joint
	*/

	shared_ptr<Program> prog = ShaderManager::getInstance()->getShader(TEXTUREPROG);
	prog->bind();
	ShaderManager::getInstance()->sendUniforms(TEXTUREPROG, SQUIRT_TEXTURE);
	M->pushMatrix();
	M->loadIdentity();
	M->translate(position);											// move squirt to its world position
	//M->rotate(atan2(facing.x, facing.z) + radians(80.f), YAXIS);	// orient squirt to face forward
	M->scale(scale*size); 			  								// scale squirt at the origin
	M->translate(vec3(-1)*shift); 									// shift squirt to origin
	int shapeSize = shapes.size();
	for (int i = 0; i < shapeSize; ++i)
	{
		if (i == 1) // left fin
			setupPart(shapes, M, i, 5, &leftFin);
		else if (i == 2) // right fin
			setupPart(shapes, M, i, 6, &rightFin);
		else if (i == 3) // left foot
			setupPart(shapes, M, i, 7, &leftFoot);
		else if (i == 4) // right foot
			setupPart(shapes, M, i, 8, &rightFoot);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		shapes.at(i)->draw(prog);
		if (i >= 1  && i <= 4)
			M->popMatrix();
	}
	M->popMatrix();
	prog->unbind();
}