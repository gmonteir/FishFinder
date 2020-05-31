#include "ParticleManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"
#include "MatrixStack.h"

#include <iostream>

using namespace std;
using namespace glm;

ParticleManager::ParticleManager() : captureMode(false), elapsedTime(0)
{
	init();
	cout << "ParticleManager: Initialized" << endl;
}

// fix to generate x y z
void ParticleManager::initParticleBuff(std::vector<float> *points, float scale, float shift)
{
	const float divisor = 10000.0;
	float x, y, z;
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		// gets float between 0 1 and maps it between -5 5
		x = scale * ((rand() % (int)divisor)/divisor) - shift;
		y = scale * ((rand() % (int)divisor)/divisor) - shift;
		z = scale * ((rand() % (int)divisor)/divisor) - shift;
		points->push_back(x);
		points->push_back(y);
		points->push_back(z);
	}
}

void ParticleManager::init()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	// Initialize the vertex array object for the particles
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Bubbles
	initParticleBuff(&bubbleBuf, 10, 5);
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, bubbleBuf.size()*sizeof(float), bubbleBuf.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0); // send x, y, and z
	
	// Character Particles
	glGenVertexArrays(1, &VertexArrayID2);
	glBindVertexArray(VertexArrayID2);

	initParticleBuff(&charBuf, 0.2, 0.1);
	glGenBuffers(1, &VertexBufferID2);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID2);
	glBufferData(GL_ARRAY_BUFFER, charBuf.size()*sizeof(float), charBuf.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0); // send x, y, and z

	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ParticleManager& ParticleManager::getInstance()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	static ParticleManager instance;
	return instance;
}

void ParticleManager::processParticles(vec3 playerTranslate, float deltaTime)
{
	shared_ptr<Program> particleProg = ShaderManager::getInstance()->getShader(PARTICLEPROG);
	particleProg->bind();
	ShaderManager::getInstance()->sendUniforms(PARTICLEPROG);
	auto Model = make_shared<MatrixStack>();
	// Bubbles
	Model->pushMatrix();
		Model->loadIdentity();
		Model->translate(playerTranslate);
		glUniformMatrix4fv(particleProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		glBindVertexArray(VertexArrayID);
		glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
	Model->popMatrix();

	particleProg->unbind();

	if (captureMode)
	{
		elapsedTime += deltaTime;
		if (elapsedTime > 0.5)
		{
			captureMode = false;
			elapsedTime = 0;
		}
		particleProg = ShaderManager::getInstance()->getShader(CHARPARTICLEPROG);
		particleProg->bind();
		ShaderManager::getInstance()->sendUniforms(CHARPARTICLEPROG);

		// Character Particles
		Model->pushMatrix();
			Model->loadIdentity();
			Model->translate(targetPos);
			glUniformMatrix4fv(particleProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			glBindVertexArray(VertexArrayID2);
			glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
		Model->popMatrix();

		particleProg->unbind();
	}

	glDisable(GL_BLEND);
}