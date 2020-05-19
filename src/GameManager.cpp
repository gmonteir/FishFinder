#include "GameManager.h"
#include "WindowManager.h"
#include <iostream>

using namespace std;
using namespace glm;


shared_ptr<GameManager> GameManager::getInstance() 
{
	static shared_ptr<GameManager> instance(new GameManager);
	return instance;
}


void GameManager::update(float deltaTime, float gameTime) 
{
	timeRemaining -= deltaTime;
	if (timeRemaining <= 0)
	{
		lose();
		timeRemaining = 0;
	}
}

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

	if (!inGame)
		textRenderer->drawText("Game Over", width / 2, height / 2, 2.00f, glm::vec3(0.2f, 1.0f, 0.2f));

	textRenderer->drawText("Characters Remaining: " + to_string(charRemaining), 25.0f, height - 50.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
	sprintf(buffer, "%.1f %%", 100 * stamina / MAX_STAMINA);
	textRenderer->drawText("Stamina: " + string(buffer), 25.0f, height - 100.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
	sprintf(buffer, "%.1f s", timeRemaining);
	textRenderer->drawText("Time Remaining: " + string(buffer), 25.0f, height - 150.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));

	prog->unbind();
	glDisable(GL_BLEND);
}

void GameManager::lose()
{
	inGame = false;
}

void GameManager::renderText(const std::string text, float x, float y, float scale, glm::vec3 color)
{
	textRenderer->drawText(text, x, y, scale, color);
}


void GameManager::increaseStamina(float delta)
{
	stamina += delta;
	if (stamina > MAX_STAMINA)
		stamina = MAX_STAMINA;
}

void GameManager::decreaseStamina(float delta)
{
	stamina -= delta;
	if (stamina < 0)
		stamina = 0;
}