#include "Animator.h"

using namespace std;
using namespace glm;

unique_ptr<Animator> Animator::createAnimator(const string& modelName)
{
	if (modelName == DORY_SHAPE)
		return unique_ptr<Animator>(new DoryAnimator);
	else if (modelName == NEMO_SHAPE)
		return unique_ptr<Animator>(new NemoAnimator);
	else if (modelName == SQUIRT_SHAPE)
		return unique_ptr<Animator>(new SquirtAnimator);
	else if (modelName == JENNY_SHAPE)
		return unique_ptr<Animator>(new JennyAnimator);
	else if (modelName == CHARLIE_SHAPE)
		return unique_ptr<Animator>(new CharlieAnimator);
	return unique_ptr<Animator>(new NoAnimator);
}

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
void Animator::setupPart(const vector<shared_ptr<Shape>>& shapes,
			   shared_ptr<MatrixStack> Model, int i, int pivot, const vec3& angle)
{
	float x = calculateShift(shapes.at(pivot)->min.x, shapes.at(pivot)->max.x);
	float y = calculateShift(shapes.at(pivot)->min.y, shapes.at(pivot)->max.y);
	float z = calculateShift(shapes.at(pivot)->min.z, shapes.at(pivot)->max.z);
	Model->pushMatrix();
		Model->translate(vec3(x, y, z));
		Model->rotate(angle.z, ZAXIS);
		Model->rotate(angle.y, YAXIS);
		Model->rotate(angle.x, XAXIS);
		Model->translate(vec3(-x, -y, -z));
}

// Calcualtes the shift to be centered at the origin
float Animator::calculateShift(float minCoord, float maxCoord)
{
	float coordExtent = maxCoord - minCoord;
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

void Animator::DoryAnimator::animate(float deltaTime)
{
	animatePart(deltaTime * animationSpeed, &tail.y, &tailRight, -0.7, 0.7);
	animatePart(deltaTime * animationSpeed, &leftFin.y, &leftFinRight, -0.3, 0.3);
	leftFin.x = leftFin.y;
	animatePart(deltaTime * animationSpeed, &rightFin.y, &rightFinRight, -0.3, 0.3);
	rightFin.x = -rightFin.y;
}

void Animator::DoryAnimator::drawModel(shared_ptr<MatrixStack>& M,
	shared_ptr<Program> prog, const vector<shared_ptr<Shape>>& shapes) const
{
	/* Dory Parts by Index
	0 - Face
	1 - Top Fin
	2 - Left Fin Joint
	3 - Right Fin Joint
	4 - Left Fin
	5 - Right Fin
	6 - Tail
	7 - Tail Joint */
	int shapeSize = shapes.size();
	for (int i = 0; i < shapeSize; ++i)
	{
		if (i == 4)
		{
			setupPart(shapes, M, i, 2, leftFin);
		}
		else if (i == 5)
		{
			setupPart(shapes, M, i, 3, rightFin);
		}
		else if (i == 6)
		{
			setupPart(shapes, M, i, 7, tail);
		}
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		shapes[i]->draw(prog);
		if (i == 4 || i == 5 || i == 6)
		{
			M->popMatrix();
		}
	}
}

void Animator::NemoAnimator::animate(float dt)
{
	animatePart(dt, &tail.y, &tailRight, -0.7, 0.7);
	animatePart(dt, &leftFin.y, &leftFinRight, -0.6, 0.6);
	leftFin.x = leftFin.y;
	animatePart(dt, &rightFin.y, &rightFinRight, -0.6, 0.6);
	rightFin.x = -rightFin.y;
	animatePart(dt, &leftFoot.x, &leftFootRight, -0.6, 0.6);
	rightFoot.x = -leftFoot.x;
}

void Animator::NemoAnimator::drawModel(shared_ptr<MatrixStack>& M, 
	shared_ptr<Program> prog, const vector<shared_ptr<Shape>>& shapes) const
{
	/* Nemo Parts by Index
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
	int shapeSize = shapes.size();
	for (int i = 0; i < shapeSize; ++i)
	{
		if (i == 2) // left fin
			setupPart(shapes, M, i, 7, leftFin);
		else if (i == 3) // right fin
			setupPart(shapes, M, i, 8, rightFin);
		else if (i == 4) // left foot
			setupPart(shapes, M, i, 9, leftFoot);
		else if (i == 5) // right foot
			setupPart(shapes, M, i, 10, rightFoot);
		else if (i == 6) // tail
			setupPart(shapes, M, i, 11, tail);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		shapes[i]->draw(prog);
		if (i >= 2 && i <= 6)
			M->popMatrix();
	}
}

void Animator::SquirtAnimator::animate(float dt)
{
	animatePart(dt, &leftFin.z, &leftFinRight, -0.6, 0.6);
	leftFin.x = leftFin.z;
	animatePart(dt, &rightFin.z, &rightFinRight, -0.6, 0.6);
	rightFin.x = -rightFin.z;
	animatePart(dt, &leftFoot.z, &leftFootRight, -0.6, 0.6);
	rightFoot.z = -leftFoot.z;
}

void Animator::SquirtAnimator::drawModel(shared_ptr<MatrixStack>& M, 
	shared_ptr<Program> prog, const vector<shared_ptr<Shape>>& shapes) const
{
	/* Squirt Parts by Index
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
	int shapeSize = shapes.size();
	for (int i = 0; i < shapeSize; ++i)
	{
		if (i == 1) // left fin
			setupPart(shapes, M, i, 5, leftFin);
		else if (i == 2) // right fin
			setupPart(shapes, M, i, 6, rightFin);
		else if (i == 3) // left foot
			setupPart(shapes, M, i, 7, leftFoot);
		else if (i == 4) // right foot
			setupPart(shapes, M, i, 8, rightFoot);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		shapes.at(i)->draw(prog);
		if (i >= 1  && i <= 4)
			M->popMatrix();
	}
}

void Animator::JennyAnimator::animate(float deltaTime)
{
	animatePart(deltaTime, &tail.y, &tailRight, -0.7, 0.7);
	animatePart(deltaTime, &leftFin.y, &leftFinRight, -0.3, 0.3);
	leftFin.x = leftFin.y;
	animatePart(deltaTime, &rightFin.y, &rightFinRight, -0.3, 0.3);
	rightFin.x = -rightFin.y;
}

void Animator::JennyAnimator::drawModel(shared_ptr<MatrixStack>& M,
	shared_ptr<Program> prog, const vector<shared_ptr<Shape>>& shapes) const
{
	/* Jenny Parts by Index
	0 - Face
	1 - Top Fin
	2 - Left Fin
	3 - Right Fin
	4 - Tail
	5 - Left Fin Joint
	6 - Right Fin Joint
	7 - Tail Joint */
	int shapeSize = shapes.size();
	for (int i = 0; i < shapeSize; ++i)
	{
		if (i == 2) // left fin
			setupPart(shapes, M, i, 5, leftFin);
		else if (i == 3)
			setupPart(shapes, M, i, 6, rightFin);
		else if (i == 4)
			setupPart(shapes, M, i, 7, tail);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		shapes[i]->draw(prog);
		if (i == 2 || i == 3 || i == 4)
			M->popMatrix();
	}
}

void Animator::CharlieAnimator::animate(float deltaTime)
{
	animatePart(deltaTime, &tail.y, &tailRight, -0.7, 0.7);
	animatePart(deltaTime, &leftFin.y, &leftFinRight, -0.3, 0.3);
	leftFin.x = leftFin.y;
	animatePart(deltaTime, &rightFin.y, &rightFinRight, -0.3, 0.3);
	rightFin.x = -rightFin.y;
}

void Animator::CharlieAnimator::drawModel(shared_ptr<MatrixStack>& M,
	shared_ptr<Program> prog, const vector<shared_ptr<Shape>>& shapes) const
{
	/* Charlie Parts by Index
	0 - Face
	1 - Top Fin
	2 - Left Fin
	3 - Right Fin
	4 - Tail
	5 - Left Fin Joint
	6 - Right Fin Joint
	7 - Tail Joint */
	int shapeSize = shapes.size();
	for (int i = 0; i < shapeSize; ++i)
	{
		if (i == 2) // left fin
			setupPart(shapes, M, i, 5, leftFin);
		else if (i == 3)
			setupPart(shapes, M, i, 6, rightFin);
		else if (i == 4)
			setupPart(shapes, M, i, 7, tail);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		shapes[i]->draw(prog);
		if (i == 2 || i == 3 || i == 4)
			M->popMatrix();
	}
}