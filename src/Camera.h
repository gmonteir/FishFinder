#pragma once

#include "Transform.h"
#include "Constants.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{

public:
	Camera() : eye(), LA(-ZAXIS), upVector(YAXIS), 
		/*alpha(0), beta(-M_PI_2),*/ offset(THIRD_PERSON_OFFSET), reverse(false) {}
	~Camera() {}
	void reset() {}

	glm::mat4 getView() { return glm::lookAt(eye, LA, upVector); }
	glm::vec3 getEye() { return eye; }

	void update(const Transform& transform);
	
	void firstPerson() { reverse = false; offset = FIRST_PERSON_OFFSET; }
	void secondPerson() { reverse = true; offset = SECOND_PERSON_OFFSET; }
	void thirdPerson() { reverse = false; offset = THIRD_PERSON_OFFSET; }

private:
	glm::vec3 eye, LA, upVector;
	glm::vec2 offset;
	//float alpha, beta;
	bool reverse; // flip the direction of the camera

	int getReverse() { return reverse ? -1 : 1; }

	//void rotate(float dx, float dy);
	//void moveForward(float delta); // speed
	//void moveRight(float delta); // speed

};

