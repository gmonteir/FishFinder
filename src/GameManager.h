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
	GameManager() : timeLimit(INITIAL_TIME_LIMIT), charRemaining(NUM_CHARACTERS), inGame(true) {
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

private:
	int width, height;
	RenderText* textRenderer;
	float timeLimit;
	int charRemaining;
	bool inGame;
};

