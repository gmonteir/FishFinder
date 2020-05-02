#include "Camera.h"
#include "Keys.h"

using namespace std;
using namespace glm;

void Camera::update(const Transform& transform)
{
	eye = transform.getPosition() + offset.x * transform.getFacing() + offset.y * upVector;
	LA = eye + float(getReverse()) * transform.getFacing();
}
//
//void Camera::rotate(float dx, float dy)
//{
//	beta -= dx;
//	alpha += dy;
//	if (alpha > radians(80.f))
//		alpha = radians(80.f);
//	else if (alpha < radians(-80.f))
//		alpha = radians(-80.f);
//	LA = vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta)) + eye;
//}
//
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