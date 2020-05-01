#include "Player.h"
#include "Entities.h"
#include "Animate.h"
#include "Spawner.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace glm;

void Player::onCollision(Entity& collider)
{
	if (collider.isAlive())
	{
		if (collider.getTag() == NEMO_TAG)
		{
			collider.stop();
			collider.kill();
			Spawner::getInstance()->spawnNemo();
			Entities::getInstance()->decrementNumActive();
		}
		else if (collider.getTag() == POWERUP_TAG)
		{
			collider.stop();
			collider.remove();
			Entities::getInstance()->decrementNumActive();
			stamina += 1;
			if (stamina > 10)
				stamina = 10;
		}
	}
}

void Player::keyUpdate(float deltaTime, Keys input)
{
	int forward = 0;
	int right = 0;
	float boost = 0;
	vec3 deltas;

	if (input.keyPressed(Keys::FORWARD))
		forward += 1;
	if (input.keyPressed(Keys::BACKWARD))
		forward -= 1;

	if (input.keyPressed(Keys::BOOST) && stamina > 0) {
		boost = 30;
		stamina -= deltaTime;
		if (stamina < 0)
			stamina = 0;
	}

	if (input.keyPressed(Keys::LEFT))
		right -= 1;
	if (input.keyPressed(Keys::RIGHT))
		right += 1;

	if (input.keyPressed(Keys::ROTLEFT))
		rotate(rotationSpeed, 0);
	if (input.keyPressed(Keys::ROTRIGHT))
		rotate(-rotationSpeed, 0);

	deltas.x = forward * transform.getFacing().x + right * -transform.getFacing().z;
	deltas.y = forward * transform.getFacing().y;
	deltas.z = forward * transform.getFacing().z + right * transform.getFacing().x;
	transform.setVelocity(right == 0 && forward == 0 ? ORIGIN : normalize(deltas) * (speed + boost));
    
    if (boost > 0)
    	deltaTime *= 3;
	animate(deltaTime);
}

void Player::animate(float dt)
{
	animatePart(dt, &tail.y, &tailRight, -0.7, 0.7);
	animatePart(dt, &leftFin.y, &leftFinRight, -0.3, 0.3);
	leftFin.x = leftFin.y;
	animatePart(dt, &rightFin.y, &rightFinRight, -0.3, 0.3);
	rightFin.x = -rightFin.y;
}

void Player::rotate(float dx, float dy)
{
	beta -= radians(dx);
	alpha += radians(dy);
	if (alpha > radians(70.f))
		alpha = radians(70.f);
	else if (alpha < radians(-70.f))
		alpha = radians(-70.f);
	transform.setFacing(normalize(vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta))));
}

void Player::draw(shared_ptr<MatrixStack> &M)
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

	shared_ptr<Program> prog = ShaderManager::getInstance()->getShader(TEXTUREPROG);
	prog->bind();
	ShaderManager::getInstance()->sendUniforms(TEXTUREPROG, DORY_TEXTURE);
	M->pushMatrix();
	M->loadIdentity();
	M->translate(transform.getPosition());											// move dory to its world position
	M->rotate(atan2(transform.getFacing().x, transform.getFacing().z) + radians(80.f), YAXIS);	// orient dory to face forward
	M->scale(scale*transform.getSize()); 			  								// scale dory at the origin
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
	prog->unbind();
}