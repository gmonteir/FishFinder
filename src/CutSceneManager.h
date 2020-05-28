#pragma once

#include "Constants.h"

#include <string>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CutSceneManager
{
	struct CutScene {
		CutScene() : active(false), sequence(0), current(-1), text(0), amount(0), 
			timer(0), currentText("") {}
		bool active;
		int sequence;
		int current, text, amount;
		float timer;
		std::string currentText;

		void init(int sequenceIndex) { sequence = sequenceIndex; }
		bool shouldDraw() const { return active || timer > 0; }
		void update(float deltaTime, float gameTime);
		void next() { active = true; current++; text = 0; amount = 0; }
	};

	CutSceneManager();

public:
	static CutSceneManager& getInstance();

	~CutSceneManager() {}

	CutSceneManager(const CutSceneManager&) = delete;
	CutSceneManager& operator= (const CutSceneManager&) = delete;

	void update(float deltaTime, float gameTime);

	bool shouldDraw() const;
	const std::string& getText() const;
	void nextCutScene() { cutScenes[MAIN_TEXTS].next(); }

private:
	CutScene cutScenes[NUM_TEXTS];
	float randomTimer;
};

