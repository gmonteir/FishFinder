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
		rotationSpeed(CAMERA_SPEED), alpha(0), beta(-M_PI_2) {
		// override Entity defaults:
		transform.setSize(glm::vec3(PLAYER_SIZE));
		bringToFloor();
		model.setTexture(DORY_TEXTURE);
		model.setMaterial(PLAYER_MATERIAL);
		model.setProgram(TEXTUREPROG);
	}
	virtual ~Player() {}

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Entity& collider) override;

	void rotate(float dx, float dy);

	int getScore() const { return score; }
	float getStamina() const { return stamina; }

	void keyUpdate(float deltaTime, Keys keyInput);

	//static vec3 getInputDirection(int direction);

private:
	int score;
	float speed, rotationSpeed;
	float alpha, beta;
	float stamina;

	glm::vec3 leftFin, rightFin, tail;
	bool leftFinRight, rightFinRight, tailRight;

	Keys keys;
};

