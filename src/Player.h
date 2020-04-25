#pragma once

#include "Entity.h"
#include "Keys.h"

class Player : public Entity
{
public:
	Player(std::vector<std::shared_ptr<Shape>>& shapes) : Entity(shapes), score(0), speed(PLAYER_SPEED), rotationSpeed(CAMERA_SPEED),
		alpha(0), beta(-M_PI_2) {
		size = glm::vec3(PLAYER_SIZE);
		position = glm::vec3(0, size.y*scale.y*(max.y-min.y)/2 + FLOOR_POSITION.y + 0.1, 0);
		material = PLAYER_MATERIAL;
		tailRight = false;
	}
	virtual ~Player() {}

	void onOutOfBounds(float deltaTime) override {}
	void onCollision(Entity& collider) override;

	glm::vec3 getPosition() const { return position; }
	glm::vec3 getFacing() const { return facing; }
	void rotate(float dx, float dy);

	int getScore() { return score; }

	void keyUpdate(float deltaTime, Keys keyInput);

	void draw(std::shared_ptr<Program> &prog, std::shared_ptr<MatrixStack> &M) override;
	void animate(float dt);
	//static vec3 getInputDirection(int direction);

private:
	int score;
	float speed, rotationSpeed;
	float alpha, beta;

	glm::vec3 leftFin, rightFin, tail;
	bool tailRight;

	Keys keys;
};

float calculateShift(float minCoord, float maxCoord);
void setupPart(const std::vector<std::shared_ptr<Shape>> shape,
			   std::shared_ptr<MatrixStack> Model, int i, int pivot, glm::vec3 *angle);

