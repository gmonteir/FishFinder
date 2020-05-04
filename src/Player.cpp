#include "Player.h"
#include "Nemo.h"
#include "Entities.h"
#include "Spawner.h"

#include <iostream>

using namespace std;
using namespace glm;

void Player::onCollision(Entity& collider)
{
	if (collider.isAlive())
	{
		if (collider.getTag() == NEMO_TAG)
		{
			if (!previousCharacter)
				previousCharacter = this;
			dynamic_cast<Nemo&>(collider).setTarget(previousCharacter);
			collider.kill();
			Spawner::getInstance()->spawnNemo();
			Entities::getInstance()->decrementNumActive();
			previousCharacter = &collider;
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

void Player::setPathVelocity(float deltaTime)
{
	int forward = 0;
	int right = 0;
	float boost = 0;
	vec3 deltas;

	if (Keys::getInstance().keyPressed(Keys::FORWARD))
		forward += 1;
	if (Keys::getInstance().keyPressed(Keys::BACKWARD))
		forward -= 1;

	if (Keys::getInstance().keyPressed(Keys::BOOST) && stamina > 0) {
		boost = 30;
		stamina -= deltaTime;
		if (stamina < 0)
			stamina = 0;
	}

	if (Keys::getInstance().keyPressed(Keys::LEFT))
		right -= 1;
	if (Keys::getInstance().keyPressed(Keys::RIGHT))
		right += 1;

	if (Keys::getInstance().keyPressed(Keys::ROTLEFT))
		rotate(rotationSpeed, 0);
	if (Keys::getInstance().keyPressed(Keys::ROTRIGHT))
		rotate(-rotationSpeed, 0);

	deltas.x = forward * transform.getFacing().x + right * -transform.getFacing().z;
	deltas.y = forward * transform.getFacing().y;
	deltas.z = forward * transform.getFacing().z + right * transform.getFacing().x;
	transform.setVelocity(right == 0 && forward == 0 ? ORIGIN : normalize(deltas) * (speed + boost));
    
    model.getAnimator().setAnimationSpeed(boost > 0 ? 3 : 1);
}

void Player::update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities)
{
	setPathVelocity(deltaTime);
	Entity::update(deltaTime, entities);
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