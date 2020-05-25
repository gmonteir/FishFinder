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
	struct FPSCounter {
		int fps;
		int frameCount;
		float accumulator;
	};

	struct GameStats {
		float timeRemaining;
		int charRemaining;
		bool inGame;
		float stamina;
	};

	GameManager();

public:
	static std::shared_ptr<GameManager> getInstance();

	~GameManager() {}

	void update(float deltaTime, float gameTime);
	void draw();
	void lose() { gameStats.inGame = false; }

	float getStamina() { return gameStats.stamina; }
	void increaseStamina(float delta) { gameStats.stamina = glm::min(gameStats.stamina + delta, MAX_STAMINA); }
	void decreaseStamina(float delta) { gameStats.stamina = glm::max(gameStats.stamina - delta, 0.0f); }

	void decrementNumChar() { gameStats.charRemaining--; }

	void drawText(int alignment, const std::string& text, float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);
	void drawTextWithFloat(int alignment, const char* format, float num,
		float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);

private:
	RenderText* textRenderer;
	FPSCounter fpsCounter;
	GameStats gameStats;

	int width, height;
};

