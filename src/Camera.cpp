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

	position = transform.getPosition();
	interpolateEye(deltaTime);
	transform.interpolateFacing(getDirection(), deltaTime);
	
	floorHeight = Floor::getInstance()->getHeight(eye.x, eye.z);
	if (eye.y < floorHeight)
		eye.y = floorHeight + CAMERA_FLOOR_OFFSET;
	interpolateLookAt(deltaTime);
}

void Camera::cursorCallback(float xpos, float ypos)
{
	float deltaX = mouse.x - xpos;
	float deltaY = mouse.y - ypos;

	// check whether or not mouseX and mouseY have been initialized yet
	// currently a hacky check of whether or not the deltas are unrealistic.
	if (initializedMouse)
		interpolateRotation(deltaX, deltaY, MOUSE_SENSITIVITY);

	mouse.x = xpos;
	mouse.y = ypos;
	initializedMouse = true;
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

/* VFC code starts here */
vec4* Camera::extractVFPlanes(mat4 P, vec4* planes) 
{
	vec4 Left, Right, Bottom, Top, Near, Far;

	/* composite matrix */
	mat4 comp = P * getView();
	vec3 n; //use to pull out normal

	Left.x = comp[0][3] + comp[0][0];
	Left.y = comp[1][3] + comp[1][0];
	Left.z = comp[2][3] + comp[2][0];
	Left.w = comp[3][3] + comp[3][0];

	n = vec3(Left.x, Left.y, Left.z);
	planes[0] = Left / length(n);

	Right.x = comp[0][3] - comp[0][0];
	Right.y = comp[1][3] - comp[1][0];
	Right.z = comp[2][3] - comp[2][0];
	Right.w = comp[3][3] - comp[3][0];
	n = vec3(Right.x, Right.y, Right.z);
	planes[1] = Right / length(n);

	Bottom.x = comp[0][3] + comp[0][1];
	Bottom.y = comp[1][3] + comp[1][1];
	Bottom.z = comp[2][3] + comp[2][1];
	Bottom.w = comp[3][3] + comp[3][1];
	n = vec3(Bottom.x, Bottom.y, Bottom.z);
	planes[2] = Bottom / length(n);

	Top.x = comp[0][3] - comp[0][1];
	Top.y = comp[1][3] - comp[1][1];
	Top.z = comp[2][3] - comp[2][1];
	Top.w = comp[3][3] - comp[3][1];
	n = vec3(Top.x, Top.y, Top.z);
	planes[3] = Top / length(n);

	Near.x = comp[0][3] + comp[0][2];
	Near.y = comp[1][3] + comp[1][2];
	Near.z = comp[2][3] + comp[2][2];
	Near.w = comp[3][3] + comp[3][2];
	n = vec3(Near.x, Near.y, Near.z);
	planes[4] = Near / length(n);

	Far.x = comp[0][3] - comp[0][2];
	Far.y = comp[1][3] - comp[1][2];
	Far.z = comp[2][3] - comp[2][2];
	Far.w = comp[3][3] - comp[3][2];
	n = vec3(Far.x, Far.y, Far.z);
	planes[5] = Far / length(n);

	return planes;
}

bool Camera::shouldCull(vec3 center, float radius, vec4* planes) 
{
	float dist;
	for (int i = 0; i < 6; i++) {
		dist = distToPlane(planes[i].x, planes[i].y, planes[i].z, planes[i].w, center);
		//test against each plane
		if (dist < -radius)
			return true;
	}
	return false;
}