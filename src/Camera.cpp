#include "Camera.h"
#include "Keys.h"

#include <iostream>

using namespace std;
using namespace glm;

void Camera::update(Transform& transform)
{
	if (Keys::getInstance().keyPressed(Keys::ROTLEFT))
		rotate(1, 0);
	if (Keys::getInstance().keyPressed(Keys::ROTRIGHT))
		rotate(-1, 0);

	setPosition(transform.getPosition());
	transform.setFacing(getFacing());
}

void Camera::rotate(float dx, float dy)
{
	vec3 direction;
	beta -= radians(dx * CAMERA_SPEED);
	alpha += radians(dy * CAMERA_SPEED);
	if (alpha > radians(80.f))
		alpha = radians(80.f);
	else if (alpha < radians(-80.f))
		alpha = radians(-80.f);
	direction = getFacing();
	eye = position + offset.x * direction + offset.y * upVector;
	LA = eye + float(getReverse()) * direction;
}

void Camera::setPosition(vec3 pos)
{
	vec3 direction(getFacing());
	position = pos;
	eye = position + offset.x * direction + offset.y * upVector;
	LA = eye + float(getReverse()) * direction;
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