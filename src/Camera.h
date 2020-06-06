#pragma once

#include "Transform.h"
#include "Constants.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

public:
	Camera() : position(), LA(), upVector(YAXIS), alpha(0), beta(-M_PI_2), 
		offset(THIRD_PERSON_OFFSET), reverse(false), initializedMouse(false) {
		updateDirection();
		eye = calcEye();
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

	glm::vec4* extractVFPlanes(glm::mat4 P, glm::vec4* planes);
	
	void firstPerson() { reverse = false; offset = FIRST_PERSON_OFFSET; }
	void secondPerson() { reverse = true; offset = SECOND_PERSON_OFFSET; }
	void thirdPerson() { reverse = false; offset = THIRD_PERSON_OFFSET; }
	void sidePerson()  { reverse = false; offset = SIDE_PERSON_OFFSET; }
	int getReverse() const { return reverse ? -1 : 1; }

	static bool shouldCull(glm::vec3 center, float radius, glm::vec4* planes);

private:
	glm::vec3 position, direction;
	glm::vec3 eye, LA, upVector;
	glm::vec3 offset;
	bool reverse; // flip the direction of the camera
	float alpha, beta;

	glm::vec2 mouse;
	bool initializedMouse;
	
	void updateDirection() 
		{ direction = normalize(glm::vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta))); }
	glm::vec3 calcEye() { return position + offset.z * direction + offset.y * upVector; }
	void interpolateEye(float deltaTime) { eye = mix(eye, calcEye(), deltaTime * CAMERA_SPEED); }
	void interpolateLookAt(float deltaTime) { LA = mix(LA, position, deltaTime * CAMERA_SPEED); } 
	// eye + float(getReverse()) * direction; // adjust to fix first person camera

	//void moveForward(float delta); // speed
	//void moveRight(float delta); // speed

	static float distToPlane(float A, float B, float C, float D, glm::vec3 point)
		{ return A * point.x + B * point.y + C * point.z + D; }
};

