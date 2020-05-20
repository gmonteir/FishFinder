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
		textRenderer->drawText("Game Over", width / 2, height / 2, 2.00f, glm::vec3(0.2f, 1.0f, 0.2f));

	//textRenderer->drawText("Active Objects: " + to_string(EntityCollection::getInstance()->getNumActive()), 25.0f, height - 50.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
	textRenderer->drawText("Characters Remaining: " + to_string(gameStats.charRemaining), 25.0f, height - 50.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
	sprintf(buffer, "%.1f %%", 100 * gameStats.stamina / MAX_STAMINA);
	textRenderer->drawText("Stamina: " + string(buffer), 25.0f, height - 100.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
	sprintf(buffer, "%.1f s", gameStats.timeRemaining);
	textRenderer->drawText("Time Remaining: " + string(buffer), 25.0f, height - 150.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
	textRenderer->drawText("FPS: " + to_string(fpsCounter.fps), 25.0f, 25.0f, 0.75f, glm::vec3(0.1));

	// ------------------- End Drawing -------------------- //
	prog->unbind();
	glDisable(GL_BLEND);
}