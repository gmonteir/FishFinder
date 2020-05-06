#pragma once

#include "Transform.h"
#include "Constants.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

public:
	Camera() : position(), eye(), LA(-ZAXIS), upVector(YAXIS), 
		alpha(0), beta(-M_PI_2), offset(THIRD_PERSON_OFFSET), reverse(false) {}
	~Camera() {}
	void reset() {}

	glm::mat4 getView() { return glm::lookAt(eye, LA, upVector); }
	glm::vec3 getEye() { return eye; }
	glm::vec3 getFacing() {
		return normalize(glm::vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta)));
	}

	void update(Transform& transform);

	/* deltas should be from -1 to 1 */
	void rotate(float dx, float dy);
	void setPosition(glm::vec3 pos);
	
	void firstPerson() { reverse = false; offset = FIRST_PERSON_OFFSET; }
	void secondPerson() { reverse = true; offset = SECOND_PERSON_OFFSET; }
	void thirdPerson() { reverse = false; offset = THIRD_PERSON_OFFSET; }

private:
	glm::vec3 position;
	glm::vec3 eye, LA, upVector;
	glm::vec2 offset;
	float alpha, beta;
	bool reverse; // flip the direction of the camera

	int getReverse() { return reverse ? -1 : 1; }

	//void moveForward(float delta); // speed
	//void moveRight(float delta); // speed

};

