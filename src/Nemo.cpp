#include "Nemo.h"
#include "Entities.h"
#include "Spawner.h"
#include "Animate.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace glm;

void Nemo::animate(float dt)
{
	
	animatePart(dt, &tail.y, &tailRight, -0.7, 0.7);
	animatePart(dt, &leftFin.y, &leftFinRight, -0.6, 0.6);
	leftFin.x = leftFin.y;
	animatePart(dt, &rightFin.y, &rightFinRight, -0.6, 0.6);
	rightFin.x = -rightFin.y;
	animatePart(dt, &leftFoot.x, &leftFootRight, -0.6, 0.6);
	rightFoot.x = -leftFoot.x;
}

void Nemo::draw(shared_ptr<MatrixStack> &M)
{
	/* Dory Parts by Index
	0 - body
	1 - top fin
	2 - left fin
	3 - right fin
	4 - left foot
	5 - right foot
	6 - tail
	7 - left fin joint
	8 - right fin joint
	9 - left foot joint
	10 - right foot joint
	11 - tail joint
	*/

	shared_ptr<Program> prog = ShaderManager::getInstance()->getShader(TEXTUREPROG);
	prog->bind();
	ShaderManager::getInstance()->sendUniforms(TEXTUREPROG, NEMO_TEXTURE);
	M->pushMatrix();
	M->loadIdentity();
	M->translate(position);											// move dory to its world position
	//M->rotate(atan2(facing.x, facing.z) + radians(80.f), YAXIS);	// orient dory to face forward
	M->scale(scale*size); 			  								// scale dory at the origin
	M->translate(vec3(-1)*shift); 									// shift dory to origin
	int shapeSize = shapes.size();
	for (int i = 0; i < shapeSize; ++i)
	{
		if (i == 2) // left fin
			setupPart(shapes, M, i, 7, &leftFin);
		else if (i == 3) // right fin
			setupPart(shapes, M, i, 8, &rightFin);
		else if (i == 4) // left foot
			setupPart(shapes, M, i, 9, &leftFoot);
		else if (i == 5) // right foot
			setupPart(shapes, M, i, 10, &rightFoot);
		else if (i == 6) // tail
			setupPart(shapes, M, i, 11, &tail);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		shapes.at(i)->draw(prog);
		if (i >= 2  && i <= 6)
			M->popMatrix();
	}
	M->popMatrix();
	prog->unbind();
}