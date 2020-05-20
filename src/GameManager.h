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

	GameManager() : fpsCounter{}, gameStats{ INITIAL_TIME_LIMIT, NUM_CHARACTERS, true, INITIAL_STAMINA } {
		FT_Library ft;
		textRenderer = new RenderText(&ft, ShaderManager::getInstance()->getShader(GLYPHPROG));
	}

public:
	static std::shared_ptr<GameManager> getInstance();

	~GameManager() {}

	void update(float deltaTime, float gameTime);
	void draw();
	void lose() { gameStats.inGame = false; }

	float getStamina() { return gameStats.stamina; }
	void increaseStamina(float delta) { gameStats.stamina = min(gameStats.stamina + delta, MAX_STAMINA); }
	void decreaseStamina(float delta) { gameStats.stamina = max(gameStats.stamina - delta, 0.0f); }

	void decrementNumChar() { gameStats.charRemaining--; }

private:
	RenderText* textRenderer;
	FPSCounter fpsCounter;
	GameStats gameStats;
};

