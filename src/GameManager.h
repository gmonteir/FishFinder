#pragma once

#include "Constants.h"
#include "RenderText.h"
#include "ShaderManager.h"
#include <memory>

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GameManager
{
	GameManager() : timeRemaining(INITIAL_TIME_LIMIT), charRemaining(NUM_CHARACTERS), inGame(true), stamina(INITIAL_STAMINA) {
		FT_Library ft;
		textRenderer = new RenderText(&ft, ShaderManager::getInstance()->getShader(GLYPHPROG));
	}

public:
	static std::shared_ptr<GameManager> getInstance();

	~GameManager() {}

	void update(float deltaTime, float gameTime);
	void draw();
	void lose();
	void renderText(const std::string text, float x, float y, float scale, glm::vec3 color);

	float getStamina() { return stamina; }
	void increaseStamina(float delta);
	void decreaseStamina(float delta);

	void decrementNumChar() { charRemaining--; }

private:
	RenderText* textRenderer;
	float timeRemaining;
	int charRemaining;
	bool inGame;
	float stamina;
};

