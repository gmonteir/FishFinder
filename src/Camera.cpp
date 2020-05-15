#include "Camera.h"
#include "Keys.h"
#include "Floor.h"

#include <iostream>

using namespace std;
using namespace glm;

void Camera::update(float deltaTime, Transform& transform)
{
	float floorHeight;
	if (Keys::getInstance().keyPressed(Keys::ROTLEFT))
		interpolateRotation(1, 0, 1);
	if (Keys::getInstance().keyPressed(Keys::ROTRIGHT))
		interpolateRotation(-1, 0, 1);

	interpolatePosition(transform.getPosition(), deltaTime);
	transform.setFacing(getDirection());
	
	floorHeight = Floor::getInstance()->getHeight(eye.x, eye.z);
	if (eye.y < floorHeight)
		eye.y = floorHeight + CAMERA_FLOOR_OFFSET;
	updateLookAt();
}

void Camera::interpolateRotation(float dx, float dy, float deltaTime)
{
	vec3 direction;
	float finalBeta = beta - radians(dx * CAMERA_SPEED);
	float finalAlpha = alpha + radians(dy * CAMERA_SPEED);
	if (finalAlpha > radians(80.f))
		finalAlpha = radians(80.f);
	else if (finalAlpha < radians(-80.f))
		finalAlpha = radians(-80.f);
	beta = mix(beta, finalBeta, CAMERA_SPEED * deltaTime);
	alpha = mix(alpha, finalAlpha, CAMERA_SPEED * deltaTime);

	updateDirection();
}

void Camera::interpolatePosition(vec3 pos, float deltaTime)
{
	position = mix(position, pos, CAMERA_SPEED * deltaTime);
	updateEye();
}

//void Camera::moveForward(float delta)
//{
//	vec3 view = normalize(LA - eye);
//	eye = eye + delta * view;
//	LA = vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta)) + eye;
//}
//
//void Camera::moveRight(float delta)
//{
//	vec3 view = normalize(LA - eye);
//	vec3 u = normalize(cross(view, upVector));
//	eye = eye + delta * u;
//	LA = vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta)) + eye;
//}