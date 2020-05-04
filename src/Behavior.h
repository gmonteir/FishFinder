#pragma once

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "Transform.h"
#include "Model.h"
#include "Constants.h"

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

	class NoBehavior;
	class PlayerBehavior;
	class FollowerBehavior;
	class PowerupBehavior;

	static std::unique_ptr<Behavior> createBehavior(int behavior, Transform& transform, Model& model);

	Behavior(int type, Transform& transform, Model& model)
		: type(type), transform(transform), model(model),
		tag("DEFAULT"), toRemove(false) {}
	virtual ~Behavior() {}

	virtual void start() = 0;
	virtual void update(float deltaTime) = 0;

	virtual void onOutOfBounds(float deltaTime) = 0;
	virtual void onCollision(Behavior& collider) = 0;
	
	bool isType(int type) { return this->type == type; }
	int getType() { return type; }

	std::string getTag() const { return this->tag; }
	void setTag(std::string tag) { this->tag = tag; }

	void remove() { toRemove = true; }

	void bringToFloor(float offset = 0);

protected:
	Transform& transform;
	Model &model;

	int type;
	string tag;
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
		: Behavior(PLAYER, transform, model), score(0), speed(PLAYER_SPEED), stamina(INITIAL_STAMINA),
		rotationSpeed(CAMERA_SPEED), alpha(0), beta(-M_PI_2), previousCharacter(&transform) {}
	virtual ~PlayerBehavior() {}

	void start() override;
	void update(float deltaTime) override;

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Behavior& collider) override;

	void rotate(float dx, float dy);

	int getScore() const { return score; }
	float getStamina() const { return stamina; }

private:
	int score;
	float speed, rotationSpeed;
	float alpha, beta;
	float stamina;

	Transform* previousCharacter;
};

class Behavior::FollowerBehavior : public Behavior
{
public:
	FollowerBehavior(Transform& transform, Model& model)
		: Behavior(FOLLOWER, transform, model), target(nullptr), 
		speed(NEMO_SPEED), offset(FOLLOWING_OFFSET), following(false) {}
	virtual ~FollowerBehavior() {}

	void start() override;
	void update(float deltaTime) override;

	void onOutOfBounds(float deltaTime) override;
	void onCollision(Behavior& collider) override {}

	void setTarget(Transform* newTarget) { target = newTarget; }

	bool isFollowing() { return following; }
	void followTarget() { following = true; }

private:
	Transform* target;
	float speed, offset;

	bool following;
};

class Behavior::PowerupBehavior : public Behavior
{
public:
	PowerupBehavior(Transform& transform, Model& model) 
		: Behavior(POWERUP, transform, model), timer(POWERUP_DESPAWN_TIME) {}
	virtual ~PowerupBehavior() {}

	void start() override {}
	void update(float deltaTime) override;

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Behavior& collider) override {}

private:
	float timer;
};

#endif