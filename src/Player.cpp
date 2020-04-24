#include "Player.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace glm;


void Player::onCollision(Entity& collider)
{
	if (collider.isAlive())
	{
		score++;
		collider.stop();
		collider.remove();
	}
}

void Player::keyUpdate(Keys input)
{
	int forward = 0;
	int right = 0;
	vec3 deltas;

	if (input.keyPressed(Keys::FORWARD))
		forward += 1;
	if (input.keyPressed(Keys::BACKWARD))
		forward -= 1;

	if (input.keyPressed(Keys::LEFT))
		right -= 1;
	if (input.keyPressed(Keys::RIGHT))
		right += 1;

	if (input.keyPressed(Keys::ROTLEFT))
		rotate(rotationSpeed, 0);
	if (input.keyPressed(Keys::ROTRIGHT))
		rotate(-rotationSpeed, 0);

	deltas.x = forward * facing.x + right * -facing.z;
	deltas.y = forward * facing.y;
	deltas.z = forward * facing.z + right * facing.x;
	velocity = (right == 0 && forward == 0) ? ORIGIN : normalize(deltas) * speed;
}

void Player::rotate(float dx, float dy)
{
	beta -= radians(dx);
	alpha += radians(dy);
	if (alpha > radians(70.f))
		alpha = radians(70.f);
	else if (alpha < radians(-70.f))
		alpha = radians(-70.f);
	facing = normalize(vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * cos(M_PI_2 - beta)));
}

//vec3 Player::getInputDirection(int direction)
//{
//	switch (direction)
//	{
//	case Keys::FORWARD:
//		moveForward(speed);
//		break;
//	case Keys::LEFT:
//		moveRight(-speed);
//		break;
//	case Keys::BACKWARD:
//		moveForward(-speed);
//		break;
//	case Keys::RIGHT:
//		moveRight(speed);
//		break;
//	case Keys::ROTLEFT:
//		rotate(speed * 0.01, 0);
//		break;
//	case Keys::ROTRIGHT:
//		rotate(-speed * 0.01, 0);
//		break;
//	default:
//		break;
//	}
//}