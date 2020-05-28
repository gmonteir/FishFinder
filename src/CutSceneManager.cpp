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
	mainScene.update(deltaTime, gameTime);
	randomScene.update(deltaTime, gameTime);
}


void CutSceneManager::CutScene::update(float deltaTime, float gameTime)
{
	timer -= deltaTime;
	if (!active) return;

	if (timer <= 0) {
		if (amount >= SCENETEXTS[current][text].size()) {
			amount = 0;
			text++;
			if (text >= SCENETEXTS[current].size()) {
				active = false;
				return;
			}
		}
		else {
			amount++;
		}
		timer = amount == SCENETEXTS[current][text].size() ? SCENE_TEXT_DELAY : SCENE_CHAR_DELAY;
	}
}