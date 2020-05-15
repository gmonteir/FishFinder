#pragma once

#include "Transform.h"
#include "Constants.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

public:
	Camera() : position(), upVector(YAXIS), alpha(0), beta(-M_PI_2), 
		offset(THIRD_PERSON_OFFSET), reverse(false) {
		updateDirection();
		updateEye();
		updateLookAt();
	}
	~Camera() {}
	void reset() {}

	glm::mat4 getView() { return glm::lookAt(eye, LA, upVector); }
	glm::vec3 getEye() { return eye; }
	glm::vec3 getDirection() { return direction; }

	void update(float deltaTime, Transform& transform);

	/* deltas should be from -1 to 1 */
	void interpolateRotation(float dx, float dy, float deltaTime);
	void interpolatePosition(glm::vec3 pos, float deltaTime);
	
	void firstPerson() { reverse = false; offset = FIRST_PERSON_OFFSET; }
	void secondPerson() { reverse = true; offset = SECOND_PERSON_OFFSET; }
	void thirdPerson() { reverse = false; offset = THIRD_PERSON_OFFSET; }
	int getReverse() { return reverse ? -1 : 1; }

private:
	glm::vec3 position, direction;
	glm::vec3 eye, LA, upVector;
	glm::vec2 offset;
	float alpha, beta;
	bool reverse; // flip the direction of the camera
	
	void updateDirection() {
		direction = normalize(glm::vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta)));
	}
	void updateEye() { eye = position + offset.x * direction + offset.y * upVector; }
	void updateLookAt() { LA = eye + float(getReverse()) * direction; }

	//void moveForward(float delta); // speed
	//void moveRight(float delta); // speed

};

