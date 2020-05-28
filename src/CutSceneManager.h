#pragma once

#include "Constants.h"

#include <string>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CutSceneManager
{
	struct CutScene {
		CutScene() : active(false), current(-1), text(0), amount(0), 
			timer(SCENE_TEXT_DELAY), currentText(nullptr) {}
		bool active;
		int current, text, amount;
		float timer;
		std::string* currentText;

		bool shouldDraw() const { return active || timer > 0; }
		void update(float deltaTime, float gameTime);
		void next() { active = true; current++; text = 0; }
		std::string getText() const { return SCENETEXTS[current][text].substr(0, amount); }
	};

	CutSceneManager() : mainScene(), randomScene() {}

public:
	static CutSceneManager& getInstance();

	~CutSceneManager() {}

	CutSceneManager(const CutSceneManager&) = delete;
	CutSceneManager& operator= (const CutSceneManager&) = delete;

	void update(float deltaTime, float gameTime);

	bool shouldDraw() const { return mainScene.shouldDraw() || randomScene.shouldDraw(); }
	std::string getText() const { return mainScene.getText(); }
	void nextCutScene() { mainScene.next(); }

private:
	CutScene mainScene;
	CutScene randomScene;
};

