#pragma once

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "Transform.h"
#include "Model.h"
#include "Constants.h"
#include "Random.h"

#include <memory>

#define _USE_MATH_DEFINES
#include <math.h>

class Behavior
{
public:
	static constexpr int NONE = 0;
	static constexpr int PLAYER = 1;
	static constexpr int FOLLOWER = 2;
	static constexpr int POWERUP = 3;
	static constexpr int ENEMY = 4;

	class NoBehavior;
	class PlayerBehavior;
	class FollowerBehavior;
	class PowerupBehavior;
	class EnemyBehavior;

	static std::unique_ptr<Behavior> createBehavior(int behavior, Transform& transform, Model& model);

	Behavior(int type, Transform& transform, Model& model)
		: type(type), transform(transform), model(model), toRemove(false) {}
	virtual ~Behavior() {}

	virtual void start() = 0;
	virtual void update(float deltaTime) = 0;

	virtual void onOutOfBounds(float deltaTime) = 0;
	virtual void onCollision(Behavior& collider) = 0;
	
	bool isType(int type) { return this->type == type; }
	int getType() { return type; }

	bool removed() { return toRemove; }
	void remove() { toRemove = true; }

	void bringToFloor(float offset = 0);

protected:
	Transform& transform;
	Model &model;

	int type;
	bool toRemove;
};

class Behavior::NoBehavior : public Behavior
{
public:
	NoBehavior(Transform& transform, Model& model) : Behavior(NONE, transform, model) {}
	virtual ~NoBehavior() {}

	void start() override {}
	void update(float deltaTime) override {}

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Behavior& collider) override {}
};

class Behavior::PlayerBehavior : public Behavior
{
public:
	PlayerBehavior(Transform& transform, Model& model)
		: Behavior(PLAYER, transform, model), score(0), slow(0), boost(0),
		previousCharacter(&transform), target(nullptr), immuneTime(0) {
		resetSpeechTime();
	}

	virtual ~PlayerBehavior() {}

	void start() override;
	void update(float deltaTime) override;

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Behavior& collider) override;

	int getScore() const { return score; }
	glm::vec3 getTargetPos() const { return target->getPosition(); }

	void setTarget(Transform *newTarget) { target = newTarget; }

private:
	int score;
	float slow, boost;
	float immuneTime, speechTime;

	Transform* previousCharacter;
	Transform* target;

	void checkBoost(float deltaTime);
	void resetSpeechTime() { speechTime = Random::range(BOOST_TEXT_TIME_RANGE); }
};

class Behavior::FollowerBehavior : public Behavior
{
public:
	FollowerBehavior(Transform& transform, Model& model)
		: Behavior(FOLLOWER, transform, model), target(nullptr), 
		offset(FOLLOWER_OFFSET), following(false) {}
	virtual ~FollowerBehavior() {}

	void start() override;
	void update(float deltaTime) override;

	void onOutOfBounds(float deltaTime) override;
	/* Note: called twice per collision check, once for this and once for the collider */
	void onCollision(Behavior& collider) override {}

	void setTarget(Transform* newTarget) { target = newTarget; }

	bool isFollowing() const { return target != nullptr; }

private:
	Transform* target;
	float offset;

	bool following;

	void setPathVelocity(float deltaTime);
};

class Behavior::PowerupBehavior : public Behavior
{
public:
	PowerupBehavior(Transform& transform, Model& model) 
		: Behavior(POWERUP, transform, model), timer(POWERUP_DESPAWN_TIME) {}
	virtual ~PowerupBehavior() {}

	void start() override;
	void update(float deltaTime) override;

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Behavior& collider) override {}

private:
	float timer;
};

class Behavior::EnemyBehavior : public Behavior
{
public:
	EnemyBehavior(Transform& transform, Model& model)
		: Behavior(ENEMY, transform, model), timer(0) {}
	virtual ~EnemyBehavior() {}

	void start() override;
	void update(float deltaTime) override;

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Behavior& collider) override {}

private:
	float timer;
};

#endif