#include "CutSceneManager.h"

#include <iostream>

using namespace std;
using namespace glm;


CutSceneManager& CutSceneManager::getInstance()
{
	static CutSceneManager instance;
	return instance;
}


void CutSceneManager::update(float deltaTime, float gameTime)
{
	timer -= deltaTime;
	if (!active) return;

	if (timer <= 0) {
		if (amount >= SCENE1TEXT[text].size()) {
			amount = 0;
			text++;
			if (text >= SCENE1TEXT.size()) {
				text = 0;
				active = false;
			}
		}
		else {
			amount++;
		}
		timer = amount == SCENE1TEXT[text].size() ? SCENE_TEXT_DELAY : SCENE_CHAR_DELAY;
	}
}