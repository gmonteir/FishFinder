#include "GameManager.h"
#include "WindowManager.h"
#include "Constants.h"
#include "CutSceneManager.h"

#include <iostream>

using namespace std;
using namespace glm;

GameManager::GameManager() : fpsCounter(), gameStats() {
	FT_Library ft;
	textRenderer = new RenderText(&ft, ShaderManager::getInstance()->getShader(GLYPHPROG));
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	cout << "GameManager: Initialized" << endl;
}


shared_ptr<GameManager> GameManager::getInstance() 
{
	static shared_ptr<GameManager> instance(new GameManager);
	return instance;
}


void GameManager::update(float deltaTime, float gameTime) 
{
	gameStats.timeRemaining -= deltaTime;
	if (gameStats.timeRemaining <= 0)
	{
		lose();
		gameStats.timeRemaining = 0;
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

	if (gameStats.gameState == GAME_LOST)
		drawText(CENTER, "Game Over", width / 2, height / 2, TITLE_FONT_SIZE, UI_RED_COLOR);
	else if (gameStats.gameState == GAME_WON)
		drawText(CENTER, "You won!", width / 2, height / 2, TITLE_FONT_SIZE, UI_GREEN_COLOR);

	drawText(LEFT, "Characters Remaining: " + to_string(gameStats.charRemaining), UI_LEFT_MARGIN, height - UI_LINE_OFFSET);
	drawTextWithFloat(LEFT, "Stamina: %.1f %%", 100 * gameStats.stamina / MAX_STAMINA, UI_LEFT_MARGIN, height - 2 * UI_LINE_OFFSET);
	if (gameStats.gameState != GAME_WON) {
		drawTextWithFloat(CENTER, "%.1f s", gameStats.timeRemaining, width / 2, height - 2 * UI_LINE_OFFSET, 2 * UI_FONT_SIZE,
			gameStats.timeRemaining > WARNING_TIME ? UI_COLOR : UI_RED_COLOR);
	}

	drawCutSceneText();

	drawText(LEFT, "FPS: " + to_string(fpsCounter.fps), UI_LEFT_MARGIN, UI_BOTTOM_MARGIN);
	 
	// ------------------- End Drawing -------------------- //
	prog->unbind();
	glDisable(GL_BLEND);
}

void GameManager::drawText(int alignment, const std::string& text, float x, float y, float scale, glm::vec3 color)
{
	// TODO: make work with aspect ratios and window size
	textRenderer->drawText(alignment, text, x, y, scale, color);
}

void GameManager::drawTextWithFloat(int alignment, const char* format, float num,
	float x, float y, float scale, glm::vec3 color)
{
	char buffer[40];
	sprintf(buffer, format, num);
	drawText(alignment, buffer, x, y, scale, color);
}

void GameManager::drawCutSceneText()
{
	if (!CutSceneManager::getInstance().shouldDraw()) return;
	drawText(CENTER, CutSceneManager::getInstance().getText(), width / 2, UI_BOTTOM_MARGIN + 2 * UI_LINE_OFFSET, SCENE_FONT_SIZE, UI_LAVENDER_COLOR);
}