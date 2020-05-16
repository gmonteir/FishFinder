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
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	if (gameTime > timeLimit)
		lose();
}

void GameManager::draw()
{
	shared_ptr<Program> prog;

	char stamina_stat[15];
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	prog = ShaderManager::getInstance()->getShader(GLYPHPROG);
	prog->bind();
	glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(proj));

	if (!inGame)
		textRenderer->drawText("Game Over", width / 2, height / 2, 2.00f, glm::vec3(0.2f, 1.0f, 0.2f));
	//sprintf(stamina_stat, "%.1f %%", 100 * playerBehavior->getStamina() / MAX_STAMINA);
	//textRenderer->drawText("Stamina: " + string(stamina_stat), 25.0f, height - 100.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));

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