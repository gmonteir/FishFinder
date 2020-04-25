#pragma once

#include <GLFW/glfw3.h>

class Keys
{
	struct Key
	{
		int keyCode;
		bool pressed;
	};


	public:
		static constexpr int FORWARD = 0;
		static constexpr int LEFT = 1;
		static constexpr int BACKWARD = 2;
		static constexpr int RIGHT = 3;
		static constexpr int ROTLEFT = 4;
		static constexpr int ROTRIGHT = 5;
		static constexpr int BOOST = 6;
		static constexpr int NUM_KEYS = 7;
		static int DEFAULT_KEYS[NUM_KEYS];

		Keys();
		~Keys() {}

		void update(int keyCode, int action);
		bool keyPressed(int keyIndex) const { return keys[keyIndex].pressed; }

	private:
		Key keys[NUM_KEYS];
};

