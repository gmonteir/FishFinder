#include "ParticleManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"
#include "MatrixStack.h"

#include <iostream>

using namespace std;
using namespace glm;

ParticleManager::ParticleManager() 
{
	init();
	cout << "ParticleManager: Initialized" << endl;
}

// fix to generate x y z
void ParticleManager::initParticleBuff(std::vector<float> *points)
{
	const float divisor = 10000.0;
	float x, y, z;
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		// gets float between 0 1 and maps it between -1.5 2
		x = 10 * ((rand() % (int)divisor)/divisor) - 5;
		y = 10 * ((rand() % (int)divisor)/divisor) - 5;
		z = 10 * ((rand() % (int)divisor)/divisor) - 5;
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
	initParticleBuff(&buf);
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, buf.size()*sizeof(float), buf.data(), GL_STATIC_DRAW);
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

void ParticleManager::processParticles()
{
	shared_ptr<Program> particleProg = ShaderManager::getInstance()->getShader(PARTICLEPROG);
	particleProg->bind();
	ShaderManager::getInstance()->sendUniforms(PARTICLEPROG);
	auto Model = make_shared<MatrixStack>();
	Model->pushMatrix();
		Model->loadIdentity();
		Model->translate(ShaderManager::getInstance()->uniformData.eye);
		glUniformMatrix4fv(particleProg->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
		glBindVertexArray(VertexArrayID);
		glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
	Model->popMatrix();
	particleProg->unbind();
	glDisable(GL_BLEND);
}