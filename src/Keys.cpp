#include "Keys.h"

using namespace std;

int Keys::DEFAULT_KEYS[] = {
	GLFW_KEY_W,
	GLFW_KEY_A,
	GLFW_KEY_S,
	GLFW_KEY_D,
	GLFW_KEY_Q,
	GLFW_KEY_E
};

Keys::Keys() : keys()
{
	for (size_t i = 0; i < NUM_KEYS; i++)
	{
		keys[i].keyCode = DEFAULT_KEYS[i];
	}
}

void Keys::update(int keyCode, int action)
{
	for (int i = 0; i < NUM_KEYS; ++i) {
		if (keyCode == keys[i].keyCode) {
			keys[i].pressed = action;
			return;
		}
	}
}