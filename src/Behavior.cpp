#include "Behavior.h"
#include "Spawner.h"
#include "Keys.h"
#include "GameManager.h"
#include "FBOManager.h"
#include "CutSceneManager.h"

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
	case ENEMY:
		return unique_ptr<Behavior>(new EnemyBehavior(transform, model));
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
	transform.setSize(glm::vec3(PLAYER_SIZE))
		.setSpeed(PLAYER_SPEED);
	bringToFloor(FOLLOWER_FLOOR_OFFSET);
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
		speechTime -= deltaTime;
		boost = 30;
		GameManager::getInstance()->decreaseStamina(deltaTime);
		FBOManager::getInstance().increaseBlurAmount(deltaTime);
		if (speechTime <= 0) {
			CutSceneManager::getInstance().startCutScene(BOOST_TEXTS);
			resetSpeechTime();
		}
	}
	else
		resetSpeechTime();

	if (Keys::getInstance().keyPressed(Keys::LEFT))
		right -= 1;
	if (Keys::getInstance().keyPressed(Keys::RIGHT))
		right += 1;

	deltas.x = forward * transform.getFacing().x + right * -transform.getFacing().z;
	deltas.y = forward * transform.getFacing().y;
	deltas.z = forward * transform.getFacing().z + right * transform.getFacing().x;
	transform.interpolateVelocity(right == 0 && forward == 0 
		? ORIGIN : normalize(deltas) * (transform.getSpeed() - slow + boost), deltaTime);

	if (slow > 0)
		slow = mix(slow, 0.0f, RECOVERY_SPEED * deltaTime);

	immuneTime -= deltaTime;

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
		target = &Spawner::getInstance()->spawnFollower()->getTransform();
		CutSceneManager::getInstance().nextCutScene();
		previousCharacter = &collider.transform;
		GameManager::getInstance()->decrementNumChar();
		break;
	case POWERUP:
		collider.remove();
		GameManager::getInstance()->increaseStamina(STAMINA_INCREMENT);
		break;
	case ENEMY:
		if (immuneTime > 0) break;
		slow = PLAYER_SPEED;
		immuneTime = IMMUNITY_TIME;
		transform.setVelocity(ORIGIN);
		FBOManager::getInstance().increaseBlurAmount(BLUR_INCREMENT);
		FBOManager::getInstance().triggerShake();
		CutSceneManager::getInstance().startCutScene(ENEMY_TEXTS);
		break;
	}
}

// ----------------------------- FOLLOWER ----------------------------- //
void Behavior::FollowerBehavior::start()
{
	transform.setSpeed(FOLLOWER_SPEED);
	model.setProgram(TEXTUREPROG);
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
	vec3 difference(target->getPosition() - transform.getPosition());
	vec3 direction(normalize(difference));
	float distance = length(difference) - offset;

	transform.setVelocity(distance * direction)
	//transform.interpolateVelocity(distance > offset / 4 ? distance * direction : ORIGIN, deltaTime * speed)
		.setFacing(difference);
}


// ----------------------------- POWERUP ----------------------------- //
void Behavior::PowerupBehavior::start()
{
	model.setProgram(REFLECTPROG);
}

void Behavior::PowerupBehavior::update(float deltaTime)
{
	transform.move(vec3(0, sin(timer) * deltaTime, 0));
	transform.setFacing(transform.getFacing() + deltaTime * vec3(sin(timer), 0, cos(timer)));
	timer -= deltaTime;

	if (timer <= 0)
	{
		remove();
	}
}

// ----------------------------- ENEMY ----------------------------- //

void Behavior::EnemyBehavior::start()
{
	timer = Random::range(ENEMY_TIMER_RANGE);
}

void Behavior::EnemyBehavior::update(float deltaTime)
{
	timer -= deltaTime;

	if (timer <= 0)
	{
		transform.setVelocity(Random::facingXZ());
		timer = Random::range(ENEMY_TIMER_RANGE);
	}
}

