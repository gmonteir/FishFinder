#include "GameManager.h"
#include "WindowManager.h"

#include <iostream>

using namespace std;
using namespace glm;

GameManager::GameManager() : fpsCounter{}, gameStats{ INITIAL_TIME_LIMIT, NUM_CHARACTERS, true, INITIAL_STAMINA } {
	FT_Library ft;
	textRenderer = new RenderText(&ft, ShaderManager::getInstance()->getShader(GLYPHPROG));

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
	int width, height;
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	shared_ptr<Program> prog;

	char buffer[15];
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	prog = ShaderManager::getInstance()->getShader(GLYPHPROG);
	prog->bind();
	glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(proj));

	// ------------------- Drawing -------------------- //
	if (!gameStats.inGame)
		textRenderer->drawText("Game Over", width / 2, height / 2, TITLE_FONT_SIZE, UI_COLOR);

	textRenderer->drawText("Characters Remaining: " + to_string(gameStats.charRemaining), UI_LEFT_MARGIN, height - UI_LINE_OFFSET, UI_FONT_SIZE, UI_COLOR);
	sprintf(buffer, "%.1f %%", 100 * gameStats.stamina / MAX_STAMINA);
	textRenderer->drawText("Stamina: " + string(buffer), UI_LEFT_MARGIN, height - 2 * UI_LINE_OFFSET, UI_FONT_SIZE, UI_COLOR);
	sprintf(buffer, "%.1f s", gameStats.timeRemaining);
	textRenderer->drawText("Time Remaining: " + string(buffer), UI_LEFT_MARGIN, height - 3 * UI_LINE_OFFSET, UI_FONT_SIZE, UI_COLOR);
	textRenderer->drawText("FPS: " + to_string(fpsCounter.fps), UI_LEFT_MARGIN, UI_BOTTOM_MARGIN, UI_FONT_SIZE, UI_COLOR);
	 
	// ------------------- End Drawing -------------------- //
	prog->unbind();
	glDisable(GL_BLEND);
}