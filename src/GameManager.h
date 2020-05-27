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
		FPSCounter() : fps(0), frameCount(0), accumulator(0) {}
		int fps;
		int frameCount;
		float accumulator;
	};

	struct GameStats {
		GameStats() : timeRemaining(INITIAL_TIME_LIMIT), charRemaining(NUM_CHARACTERS), 
			gameState(GAME_ACTIVE), stamina(INITIAL_STAMINA) {}
		float timeRemaining;
		int charRemaining;
		int gameState;
		float stamina;
	};

	struct Scene {
		Scene() : active(false), current(-1), text(0), amount(0), timer(SCENE_TEXT_DELAY) {}
		bool active;
		int current;
		int text;
		int amount;
		float timer;
	};

	GameManager();

public:
	static std::shared_ptr<GameManager> getInstance();

	~GameManager() {}

	void update(float deltaTime, float gameTime);
	void draw();
	void lose() { gameStats.gameState = GAME_LOST; }
	void win() { gameStats.gameState = GAME_WON; }

	int getCharRemaining() { return gameStats.charRemaining; }

	float getStamina() { return gameStats.stamina; }
	void increaseStamina(float delta) { gameStats.stamina = glm::min(gameStats.stamina + delta, MAX_STAMINA); }
	void decreaseStamina(float delta) { gameStats.stamina = glm::max(gameStats.stamina - delta, 0.0f); }

	void decrementNumChar() { gameStats.charRemaining--; }

	void drawText(int alignment, const std::string& text, float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);
	void drawTextWithFloat(int alignment, const char* format, float num,
		float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);

	void drawCutSceneText();
	void nextCutScene() { scene.active = true; scene.current++; }

private:
	RenderText* textRenderer;
	FPSCounter fpsCounter;
	GameStats gameStats;

	Scene scene;
	int width, height;
};

