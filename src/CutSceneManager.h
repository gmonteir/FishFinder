#pragma once

#include "Constants.h"

#include <string>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CutSceneManager
{

	CutSceneManager() : active(false), 
		current(-1), text(0), amount(0), timer(SCENE_TEXT_DELAY) {}

public:
	static CutSceneManager& getInstance();

	~CutSceneManager() {}

	CutSceneManager(const CutSceneManager&) = delete;
	CutSceneManager& operator= (const CutSceneManager&) = delete;

	void update(float deltaTime, float gameTime);

	bool shouldDraw() const { return active || timer > 0; }
	std::string getText() const { return SCENE1TEXT[text].substr(0, amount); }
	void nextCutScene() { active = true; current++; }

private:
	bool active;
	int current, text, amount;
	float timer;
};

