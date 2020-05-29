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

	randomTimer = Random::range(RANDOM_TEXT_TIME_RANGE);
	cout << "CutSceneManager: Initialized" << endl;
}


CutSceneManager& CutSceneManager::getInstance()
{
	static CutSceneManager instance;
	return instance;
}


void CutSceneManager::update(float deltaTime, float gameTime)
{
	bool updating = false;
	for (int i = 0; i < NUM_TEXTS; i++)
	{
		if (updating)
		{
			cutScenes[i].stop();
		}
		else
		{
			updating = cutScenes[i].update(deltaTime, gameTime) || updating;
		}
	}

	if (!updating) {
		randomUpdate(deltaTime);
	}
}


bool CutSceneManager::CutScene::update(float deltaTime, float gameTime)
{
	timer -= deltaTime;
	if (!active || timer > 0) return shouldDraw();

	if (amount >= CUTSCENETEXTS[sequence][current][text].size()) {
		amount = 0;
		text++;
		timer = SCENE_TEXT_DELAY;
		if (text >= CUTSCENETEXTS[sequence][current].size()) {
			active = false;
		}
	}
	else {
		amount++;
		timer = SCENE_CHAR_DELAY;
		currentText = CUTSCENETEXTS[sequence][current][text].substr(0, amount);
	}
	return true;
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
		if (cutScenes[i].shouldDraw())
			return cutScenes[i].currentText;
	}
	return "";
}


void CutSceneManager::randomUpdate(float deltaTime)
{
	randomTimer -= deltaTime;
	if (randomTimer <= 0)
	{
		cutScenes[RANDOM_TEXTS].start(0);
		randomTimer = Random::range(RANDOM_TEXT_TIME_RANGE);
	}
}