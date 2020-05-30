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

	struct BlinkText {
		BlinkText(const std::string& text) : text(text), active(false), blinkTimer(0), shouldDraw(false) {}
		std::string text;
		bool active;
		float blinkTimer;
		bool shouldDraw;

		void update(float deltaTime);

		void toggleDrawing() { shouldDraw = !shouldDraw; }
	};

	GameManager();

public:
	static GameManager& getInstance();

	~GameManager() {}

	void reset();

	void update(float deltaTime, float gameTime);
	void draw();
	void lose();
	void win();

	int getCharRemaining() { return gameStats.charRemaining; }

	float getStamina() { return gameStats.stamina; }
	void increaseStamina(float delta) { gameStats.stamina = glm::min(gameStats.stamina + delta, MAX_STAMINA); }
	void decreaseStamina(float delta) { gameStats.stamina = glm::max(gameStats.stamina - delta, 0.0f); }

	void decrementNumChar() { gameStats.charRemaining--; }

	void drawText(int alignment, const std::string& text, float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);
	void drawTextWithFloat(int alignment, const char* format, float num,
		float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);

	void drawCutSceneText();

private:
	RenderText* textRenderer;
	FPSCounter fpsCounter;
	GameStats gameStats;

	BlinkText restartText;
	int width, height;
};

