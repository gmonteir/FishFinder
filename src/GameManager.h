#pragma once

#include "Constants.h"
#include "RenderText.h"
#include "ShaderManager.h"
#include "AudioManager.h"
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
			gameState(GAME_TITLE), stamina(INITIAL_STAMINA) {}
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

	struct TitleTexts {
		TitleTexts() : title(TITLE_TEXT), startText(START_TEXT) { startText.active = true; }
		std::string title;
		BlinkText startText;

		void reset() { startText = BlinkText(START_TEXT); startText.active = true; }
		void update(float deltaTime) { startText.update(deltaTime); }
	};

	GameManager();

public:

	static GameManager& getInstance();

	~GameManager() {}

	void reset();

	void update(float deltaTime, float gameTime);
	void draw();
	void play();
	void lose();
	void win();

	bool inGame() { return gameStats.gameState > GAME_TITLE; }
	int getCharRemaining() { return gameStats.charRemaining; }

	float getStamina() { return gameStats.stamina; }
	void increaseStamina(float delta) { gameStats.stamina = glm::min(gameStats.stamina + delta, MAX_STAMINA); }
	void decreaseStamina(float delta) { gameStats.stamina = glm::max(gameStats.stamina - delta, 0.0f); }

	void decrementNumChar() { gameStats.charRemaining--; }

private:
	RenderText* textRenderer;
	FPSCounter fpsCounter;
	GameStats gameStats;
	TitleTexts titleTexts;

	BlinkText restartText;
	int width, height;

	void drawText(int alignment, const std::string& text, 
		float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);
	void drawText(int alignment, const std::string& text, float x, float y, float xscale, float yscale, glm::vec3 color);
	void drawTextWithFloat(int alignment, const char* format, float num,
		float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);
	void drawBlinkText(int alignment, const BlinkText& blinkText, 
		float x, float y, float scale = UI_FONT_SIZE, glm::vec3 color = UI_COLOR);
	void drawCutSceneText();
	void drawInGameStats();
	void drawStamina();
	void drawTimeRemaining();
	void drawFPS();

	void drawTitleScreen();
	void drawWinScreen();
	void drawLoseScreen();
};

