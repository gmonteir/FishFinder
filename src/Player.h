#pragma once

#include "Entity.h"
#include "Keys.h"
#include "Constants.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Player : public Entity
{
public:
	Player(std::vector<std::shared_ptr<Shape>>& shapes) 
		: Entity(shapes), score(0), speed(PLAYER_SPEED), stamina(INITIAL_STAMINA),
		rotationSpeed(CAMERA_SPEED), alpha(0), beta(-M_PI_2), leftFinRight(false), rightFinRight(false), 
		tailRight(false), tail(ORIGIN), rightFin(ORIGIN), leftFin(ORIGIN) {
		// override Entity defaults:
		transform.setSize(glm::vec3(PLAYER_SIZE));
		bringToFloor();
		model.setMaterial(PLAYER_MATERIAL);
	}
	virtual ~Player() {}

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Entity& collider) override;

	void rotate(float dx, float dy);

	int getScore() const { return score; }
	float getStamina() const { return stamina; }

	void keyUpdate(float deltaTime, Keys keyInput);

	void draw(std::shared_ptr<MatrixStack> &M) override;
	void animate(float dt);

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

