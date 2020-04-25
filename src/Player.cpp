#include "Player.h"
#include "Entities.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace glm;


void Player::onCollision(Entity& collider)
{
	if (collider.isAlive())
	{
		score++;
		collider.stop();
		collider.remove();
		Entities::getInstance()->spawnRandom();
	}
}

void Player::keyUpdate(float deltaTime, Keys input)
{
	int forward = 0;
	int right = 0;
	vec3 deltas;

	if (input.keyPressed(Keys::FORWARD))
		forward += 1;
	if (input.keyPressed(Keys::BACKWARD))
		forward -= 1;

	if (input.keyPressed(Keys::LEFT))
		right -= 1;
	if (input.keyPressed(Keys::RIGHT))
		right += 1;

	if (input.keyPressed(Keys::ROTLEFT))
		rotate(rotationSpeed, 0);
	if (input.keyPressed(Keys::ROTRIGHT))
		rotate(-rotationSpeed, 0);

	deltas.x = forward * facing.x + right * -facing.z;
	deltas.y = forward * facing.y;
	deltas.z = forward * facing.z + right * facing.x;
	velocity = (right == 0 && forward == 0) ? ORIGIN : normalize(deltas) * speed;


	animate(deltaTime);
	//leftFin += vec3(0, lfNext, 0);
	//rightFin += vec3(0, rfNext, 0);
	//cout << tail.y << endl;
}

void Player::animate(float dt)
{
	if (tailRight)
	{
		tail.y += dt;
		if (tail.y > 0.7)
			tailRight = false;
	}
	else
	{
		tail.y -= dt;
		if (tail.y < -0.7)
			tailRight = true;
	}
}

void Player::rotate(float dx, float dy)
{
	beta -= radians(dx);
	alpha += radians(dy);
	if (alpha > radians(70.f))
		alpha = radians(70.f);
	else if (alpha < radians(-70.f))
		alpha = radians(-70.f);
	facing = normalize(vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta)));
}

// pivot around given part
void setupPart(const vector<shared_ptr<Shape>> shapes,
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
float calculateShift(float minCoord, float maxCoord)
{
	float coordExtent;
	coordExtent = maxCoord - minCoord;
	return minCoord + (coordExtent/2.0f);
}

void Player::draw(shared_ptr<Program> &prog, shared_ptr<MatrixStack> &M)
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
	//prog = ShaderManager::getInstance()->getShader(TEXTUREPROG);
	//prog->bind();
	//texture->bind(prog->getUniform("Texture0"));
	M->pushMatrix();
	M->loadIdentity();
	M->translate(position);											// move dory to its world position
	M->rotate(atan2(facing.x, facing.z) + radians(80.f), YAXIS);	// orient dory to face forward
	M->scale(scale*size); 			  								// scale dory at the origin
	M->translate(vec3(-1)*shift); 									// shift dory to origin
	int shapeSize = shapes.size();
	for (int i = 0; i < shapeSize; ++i)
	{
		if (i == 4)
		{
			setupPart(shapes, M, i, 2, &leftFin);
		}
		else if (i == 5)
		{
			setupPart(shapes, M, i, 3, &rightFin);
		}
		else if (i == 6)
		{
			setupPart(shapes, M, i, 7, &tail);
		}
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		shapes.at(i)->draw(prog);
		if (i == 4 || i == 5 || i == 6)
		{
			M->popMatrix();
		}
	}
	M->popMatrix();

	/* draw  dory
		prog = shaderManager->getShader(TEXTUREPROG);
		prog->bind();
		glUniform3f(prog->getUniform("lightDir"), lightDir.x, lightDir.y, lightDir.z);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		doryTexture->bind(prog->getUniform("Texture0"));
		dory->draw(prog, Model);
		prog->unbind();
	*/
}

//vec3 Player::getInputDirection(int direction)
//{
//	switch (direction)
//	{
//	case Keys::FORWARD:
//		moveForward(speed);
//		break;
//	case Keys::LEFT:
//		moveRight(-speed);
//		break;
//	case Keys::BACKWARD:
//		moveForward(-speed);
//		break;
//	case Keys::RIGHT:
//		moveRight(speed);
//		break;
//	case Keys::ROTLEFT:
//		rotate(speed * 0.01, 0);
//		break;
//	case Keys::ROTRIGHT:
//		rotate(-speed * 0.01, 0);
//		break;
//	default:
//		break;
//	}
//}