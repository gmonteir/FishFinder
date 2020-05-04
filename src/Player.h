#pragma once

#include "Entity.h"
#include "Keys.h"
#include "Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Player : public Entity
{
public:
	Player(const std::string shapeName) 
		: Entity(shapeName), score(0), speed(PLAYER_SPEED), stamina(INITIAL_STAMINA),
		rotationSpeed(CAMERA_SPEED), alpha(0), beta(-M_PI_2), previousCharacter(nullptr) {
		// override Entity defaults:
		transform.setSize(glm::vec3(PLAYER_SIZE));
		bringToFloor(FLOOR_OFFSET);
		model.setTexture(DORY_TEXTURE);
		model.setProgram(TEXTUREPROG);
	}
	virtual ~Player() {}

	void setPathVelocity(float deltaTime);
	void update(float deltaTime, std::vector<std::shared_ptr<Entity>>& entities);

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Entity& collider) override;

	void rotate(float dx, float dy);

	int getScore() const { return score; }
	float getStamina() const { return stamina; }

private:
	int score;
	float speed, rotationSpeed;
	float alpha, beta;
	float stamina;

	Entity* previousCharacter;
};

