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
		offset(THIRD_PERSON_OFFSET), reverse(false), initializedMouse(false) {
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
	void cursorCallback(float xpos, float ypos);

	/* deltas should be from -1 to 1 */
	void interpolateRotation(float dx, float dy, float deltaTime);
	void interpolatePosition(glm::vec3 pos, float deltaTime);

	glm::vec4* extractVFPlanes(glm::mat4 P, glm::vec4* planes);

	static int viewFrustCull(glm::vec3 center, float radius, glm::vec4* planes);
	
	void firstPerson() { reverse = false; offset = FIRST_PERSON_OFFSET; }
	void secondPerson() { reverse = true; offset = SECOND_PERSON_OFFSET; }
	void thirdPerson() { reverse = false; offset = THIRD_PERSON_OFFSET; }
	int getReverse() const { return reverse ? -1 : 1; }

private:
	glm::vec3 position, direction;
	glm::vec3 eye, LA, upVector;
	glm::vec2 offset;
	bool reverse; // flip the direction of the camera
	float alpha, beta;

	glm::vec2 mouse;
	bool initializedMouse;
	
	void updateDirection() {
		direction = normalize(glm::vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta)));
	}
	void updateEye() { eye = position + offset.x * direction + offset.y * upVector; }
	void updateLookAt() { LA = position; } // eye + float(getReverse()) * direction;

	//void moveForward(float delta); // speed
	//void moveRight(float delta); // speed

};

