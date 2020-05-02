#include "Player.h"
#include "Entities.h"
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
    
    model.getAnimator().setAnimationSpeed(boost > 0 ? 3 : 1);
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