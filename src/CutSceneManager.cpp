#include "CutSceneManager.h"

#include <iostream>

using namespace std;
using namespace glm;

CutSceneManager::CutSceneManager() : cutScenes(), randomTimer(0)
{
	for (int i = 0; i < NUM_TEXTS; i++)
	{
		cutScenes[i].init(i);
	}

	cout << "CutSceneManager: Initialized" << endl;
}


CutSceneManager& CutSceneManager::getInstance()
{
	static CutSceneManager instance;
	return instance;
}


void CutSceneManager::update(float deltaTime, float gameTime)
{
	for (int i = 0; i < NUM_TEXTS; i++)
	{
		cutScenes[i].update(deltaTime, gameTime);
	}
}


void CutSceneManager::CutScene::update(float deltaTime, float gameTime)
{
	timer -= deltaTime;
	if (!active || timer > 0) return;

	if (amount >= CUTSCENETEXTS[sequence][current][text].size()) {
		amount = 0;
		text++;
		timer = SCENE_TEXT_DELAY;
		if (text >= CUTSCENETEXTS[sequence][current].size()) {
			active = false;
			return;
		}
	}
	else {
		amount++;
		timer = SCENE_CHAR_DELAY;
		currentText = CUTSCENETEXTS[sequence][current][text].substr(0, amount);
	}
	cout << "CutSceneManager: update " <<  currentText << endl;
}


bool CutSceneManager::shouldDraw() const
{
	for (int i = 0; i < NUM_TEXTS; i++)
	{
		if (cutScenes[i].shouldDraw())
			return true;
	}
	return false;
}


const string& CutSceneManager::getText() const
{
	for (int i = 0; i < NUM_TEXTS; i++)
	{
		cout << "Text: " << cutScenes[i].timer << ", " << cutScenes[i].active << ", " << cutScenes[i].currentText << endl;
		if (cutScenes[i].shouldDraw())
			return cutScenes[i].currentText;
	}
	return "";
}