#include "GameManager.h"
#include "WindowManager.h"
#include "Constants.h"
#include "CutSceneManager.h"

#include <iostream>

using namespace std;
using namespace glm;

// -------------------------- BlinkText --------------------- //

void GameManager::BlinkText::update(float deltaTime)
{
	if (!active) return;
	blinkTimer -= deltaTime;
	if (blinkTimer <= 0)
	{
		toggleDrawing();
		blinkTimer = TEXT_BLINK_DELAY;
	}
}


// -------------------------- GameManager --------------------- //

GameManager::GameManager() : fpsCounter(), gameStats(), restartText(BLINK_TEXT), titleTexts() {
	FT_Library ft;
	textRenderer = new RenderText(&ft, ShaderManager::getInstance()->getShader(GLYPHPROG));
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	cout << "GameManager: Initialized" << endl;
}


GameManager& GameManager::getInstance() 
{
	static GameManager instance;
	return instance;
}


void GameManager::reset()
{
	gameStats = GameStats();
	restartText = BlinkText(BLINK_TEXT);
	titleTexts.reset();
	cout << "GameManager reset" << endl;
}


void GameManager::update(float deltaTime, float gameTime) 
{
	switch (gameStats.gameState)
	{
	case GAME_TITLE:
		titleTexts.update(deltaTime);
		break;
	case GAME_ACTIVE:
		gameStats.timeRemaining -= deltaTime;
		if (gameStats.timeRemaining <= 0)
		{
			lose();
			gameStats.timeRemaining = 0;
		}
		break;
	case GAME_WON:
	case GAME_LOST:
		restartText.update(deltaTime);
		break;
	default:
		break;
	}

	fpsCounter.accumulator += deltaTime;
	fpsCounter.frameCount += 1;
	if (fpsCounter.accumulator > 1) // 1 second
	{
		fpsCounter.fps = (int)((float)fpsCounter.frameCount / fpsCounter.accumulator);
		fpsCounter.accumulator = 0;
		fpsCounter.frameCount = 0;
	}
}

/* FreeType */
void GameManager::draw()
{
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);
	shared_ptr<Program> prog;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	prog = ShaderManager::getInstance()->getShader(GLYPHPROG);
	prog->bind();
	glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(proj));

	// ------------------- Drawing -------------------- //

	switch (gameStats.gameState)
	{
	case GAME_TITLE:
		drawTitleScreen();
		break;
	case GAME_ACTIVE:
		drawInGameStats();
		drawStamina();
		drawCutSceneText();
		drawTimeRemaining();
		break;
	case GAME_WON:
		drawInGameStats();
		drawStamina();
		drawWinScreen();
		break;
	case GAME_LOST:
		drawInGameStats();
		drawStamina();
		drawCutSceneText();
		drawTimeRemaining();
		drawLoseScreen();
		break;
	}
	drawFPS();
	 
	// ------------------- End Drawing -------------------- //
	prog->unbind();
	glDisable(GL_BLEND);
}

void GameManager::play()
{
	if (gameStats.gameState == GAME_TITLE) {
		gameStats.gameState = GAME_ACTIVE;
	}
}

void GameManager::lose()
{
	if (gameStats.gameState == GAME_ACTIVE) {
		AudioManager::getInstance().stopSoundEffect(MAIN_MUSIC);
		AudioManager::getInstance().playSoundEffect(LOSE_MUSIC);
		gameStats.gameState = GAME_LOST;
		restartText.active = true;
	}
}

void GameManager::win()
{
	if (gameStats.gameState == GAME_ACTIVE) {
		AudioManager::getInstance().stopSoundEffect(MAIN_MUSIC);
		AudioManager::getInstance().playSoundEffect(WIN_MUSIC);
		gameStats.gameState = GAME_WON;
		restartText.active = true;
	}
}

void GameManager::drawText(int alignment, const std::string& text, float x, float y, float scale, glm::vec3 color)
{
	// TODO: make work with aspect ratios and window size
	textRenderer->drawText(alignment, text, x, y, scale, color);
}


void GameManager::drawText(int alignment, const std::string& text, float x, float y, float xscale, float yscale, glm::vec3 color)
{
	// TODO: make work with aspect ratios and window size
	textRenderer->drawText(alignment, text, x, y, xscale, yscale, color);
}

void GameManager::drawTextWithFloat(int alignment, const char* format, float num,
	float x, float y, float scale, glm::vec3 color)
{
	char buffer[40];
	sprintf(buffer, format, num);
	drawText(alignment, buffer, x, y, scale, color);
}

void GameManager::drawBlinkText(int alignment, const BlinkText& blinkText,
	float x, float y, float scale, glm::vec3 color)
{
	if (!blinkText.shouldDraw) return;
	drawText(alignment, blinkText.text, x, y, scale, color);
}

void GameManager::drawCutSceneText()
{
	if (!CutSceneManager::getInstance().shouldDraw()) return;
	drawText(CENTER, CutSceneManager::getInstance().getText(), width / 2, UI_BOTTOM_MARGIN + 2 * UI_LINE_OFFSET, SCENE_FONT_SIZE, UI_LAVENDER_COLOR);
}

void GameManager::drawInGameStats()
{
	drawText(LEFT, "Characters Remaining: " + to_string(gameStats.charRemaining), UI_LEFT_MARGIN, height - UI_LINE_OFFSET);
	//drawTextWithFloat(LEFT, "Stamina: %.1f %%", 100 * gameStats.stamina / MAX_STAMINA, UI_LEFT_MARGIN, height - 2 * UI_LINE_OFFSET);
}

// Function which return string by concatenating it. 
string repeat(string s, int n)
{
	// Copying given string to temparory string. 
	string s1 = s;

	for (int i = 1; i < n; i++)
		s += s1; // Concatinating strings 

	if (n > 0)
		return s;
	
	return "";
}

void GameManager::drawStamina()
{
	string s1 = repeat("I", int(gameStats.stamina) * 2);
	drawText(LEFT, "Stamina: ", UI_LEFT_MARGIN, height - 2.1 * UI_LINE_OFFSET);
	// drawText(LEFT, s1, UI_LEFT_MARGIN, height - 3.1 * UI_LINE_OFFSET, 0.1 * UI_FONT_SIZE, 1.5 * UI_FONT_SIZE,
	// 	vec3(1 - gameStats.stamina / 100, gameStats.stamina / 100, 0));
	drawText(LEFT, s1, UI_LEFT_MARGIN + 120/UI_FONT_SIZE, height - 2.1 * UI_LINE_OFFSET, 0.1 * UI_FONT_SIZE, 1.5 * UI_FONT_SIZE,
	 	vec3(1 - gameStats.stamina / 100, gameStats.stamina / 100, 0));
}

void GameManager::drawTimeRemaining()
{
	drawTextWithFloat(CENTER, "%.1f s", gameStats.timeRemaining, width / 2, height - 2 * UI_LINE_OFFSET, 2 * UI_FONT_SIZE,
		gameStats.timeRemaining > WARNING_TIME ? UI_COLOR : UI_RED_COLOR);
}

void GameManager::drawFPS()
{
	drawText(LEFT, "FPS: " + to_string(fpsCounter.fps), UI_LEFT_MARGIN, UI_BOTTOM_MARGIN);
}


// ------------------- Drawing Screens -------------------- //

void GameManager::drawTitleScreen()
{
	drawText(CENTER, titleTexts.title, width / 2, height * 3 / 4, TITLE_FONT_SIZE, UI_GREEN_COLOR);
	drawBlinkText(CENTER, titleTexts.startText, width / 2, height / 4, TITLE_FONT_SIZE / 2, UI_GREEN_COLOR);
}

void GameManager::drawWinScreen()
{
	drawText(CENTER, WIN_TEXT, width / 2, height / 2, TITLE_FONT_SIZE, UI_GREEN_COLOR);
	drawBlinkText(CENTER, restartText, width / 2, height / 4, TITLE_FONT_SIZE / 2, UI_GREEN_COLOR);
}

void GameManager::drawLoseScreen()
{
	drawText(CENTER, LOSE_TEXT, width / 2, height / 2, TITLE_FONT_SIZE, UI_RED_COLOR);
	drawBlinkText(CENTER, restartText.text, width / 2, height / 4, TITLE_FONT_SIZE / 2, UI_RED_COLOR);
}