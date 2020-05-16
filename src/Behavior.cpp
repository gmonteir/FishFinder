#include "Behavior.h"
#include "Entities.h"
#include "Spawner.h"
#include "Keys.h"
#include "GameManager.h"

#include <iostream>

using namespace std;
using namespace glm;

unique_ptr<Behavior> Behavior::createBehavior(int behavior, Transform& transform, Model& model)
{
	switch (behavior)
	{
	case PLAYER:
		return unique_ptr<Behavior>(new PlayerBehavior(transform, model));
	case FOLLOWER:
		return unique_ptr<Behavior>(new FollowerBehavior(transform, model));
	case POWERUP:
		return unique_ptr<Behavior>(new PowerupBehavior(transform, model));
	default:
		return unique_ptr<Behavior>(new NoBehavior(transform, model));
	}
}

void Behavior::bringToFloor(float offset) {
	transform.setPosition(glm::vec3(
		transform.getPosition().x,
		transform.getSize().y * model.getScaledSize().y / 2 + offset
			+ Floor::getInstance()->getHeight(transform.getPosition().x, transform.getPosition().z),
		transform.getPosition().z
	));
}

// ----------------------------- PLAYER ----------------------------- //
void Behavior::PlayerBehavior::start()
{
	transform.setSize(glm::vec3(PLAYER_SIZE));
	bringToFloor(FOLLOWER_OFFSET);
	model.setTexture(DORY_TEXTURE);
	model.setProgram(TEXTUREPROG);
}

void Behavior::PlayerBehavior::update(float deltaTime)
{
	int forward = 0;
	int right = 0;
	float boost = 0;
	vec3 deltas;

	if (Keys::getInstance().keyPressed(Keys::FORWARD))
		forward += 1;
	if (Keys::getInstance().keyPressed(Keys::BACKWARD))
		forward -= 1;

	if (Keys::getInstance().keyPressed(Keys::BOOST) 
		&& GameManager::getInstance()->getStamina() > 0) {
		boost = 30;
		GameManager::getInstance()->decreaseStamina(deltaTime);
	}

	if (Keys::getInstance().keyPressed(Keys::LEFT))
		right -= 1;
	if (Keys::getInstance().keyPressed(Keys::RIGHT))
		right += 1;

	deltas.x = forward * transform.getFacing().x + right * -transform.getFacing().z;
	deltas.y = forward * transform.getFacing().y;
	deltas.z = forward * transform.getFacing().z + right * transform.getFacing().x;
	transform.interpolateVelocity(right == 0 && forward == 0 ? ORIGIN : normalize(deltas) * (speed + boost), deltaTime);

	model.getAnimator().setAnimationSpeed(boost > 0 ? 3 : 1);
}

void Behavior::PlayerBehavior::onCollision(Behavior& collider)
{
	FollowerBehavior* follower;
	switch (collider.getType())
	{
	case FOLLOWER:
		follower = &dynamic_cast<FollowerBehavior&>(collider);
		if (follower->isFollowing())
			return;
		follower->setTarget(previousCharacter);
		follower->followTarget();
		target = &Spawner::getInstance()->spawnFollower()->getTransform();
		Entities::getInstance()->decrementNumActive();
		previousCharacter = &collider.transform;
		GameManager::getInstance()->decrementNumChar();
		break;
	case POWERUP:
		collider.remove();
		Entities::getInstance()->decrementNumActive();
		GameManager::getInstance()->increaseStamina(STAMINA_INCREMENT);
		break;
	}
}

// ----------------------------- FOLLOWER ----------------------------- //
void Behavior::FollowerBehavior::start()
{
	static int i = 0; // Need to abstract textures from behavior
	model.setTexture(i % 2 == 0 ? NEMO_TEXTURE : SQUIRT_TEXTURE);
	model.setProgram(TEXTUREPROG);
	i++;
}

void Behavior::FollowerBehavior::update(float deltaTime)
{
	if (target)
		setPathVelocity(deltaTime);
}

void Behavior::FollowerBehavior::onOutOfBounds(float deltaTime)
{
	if (following)
		return;

	transform.setVelocity(-transform.getVelocity())
		.move(deltaTime)
		.syncFacing();
}

void Behavior::FollowerBehavior::setPathVelocity(float deltaTime)
{
	vec3 direction(target->getPosition() - transform.getPosition());
	vec3 normal(normalize(direction));

	transform.interpolateVelocity(length(direction) > offset ? normal * speed : ORIGIN, deltaTime)
		.syncFacing();
}


// ----------------------------- POWERUP ----------------------------- //
void Behavior::PowerupBehavior::update(float deltaTime)
{
	transform.move(vec3(0, sin(timer) * deltaTime, 0));
	transform.setFacing(transform.getFacing() + deltaTime * vec3(sin(timer), 0, cos(timer)));
	timer -= deltaTime;

	if (timer <= 0)
	{
		remove();
		Entities::getInstance()->decrementNumActive();
	}
}
