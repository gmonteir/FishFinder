#include "FBOManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"

#include <iostream>

using namespace std;
using namespace glm;

FBOManager::FBOManager() : blurAmount(0), enabled(true), write(false), texture(0),
	chaos(false), confuse(false), shake(false), water(true)
{
	initFBOs();
	initQuad();
	cout << "FBOManager: Initialized" << endl;
}

FBOManager& FBOManager::getInstance()
{
	static FBOManager instance;
	return instance;
}

void FBOManager::bindScreen() const
{
	//set up to render to first FBO stored in array position 0
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBOManager::bindBuffer(int bufferIndex) const
{
	//set up to render to first FBO stored in array position 0
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[bufferIndex]);
	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBOManager::processFog()
{
	if (!enabled) return;

	glDisable(GL_DEPTH_TEST);
	bindBuffer(FOG_BUFFER);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texBuf[DEPTH_BUFFER]);
	processDrawTex(texBuf[MAIN_BUFFER], FOGFBOPROG);
	glEnable(GL_DEPTH_TEST);
}

void FBOManager::processBlur()
{
	if (!enabled) return;

	glDisable(GL_DEPTH_TEST);
	for (size_t i = 0; i < round(blurAmount); i++)
	{
		bindBuffer((i + 1) % 2);
		processDrawTex(texBuf[i % 2], BLURFBOPROG);
	}
	glEnable(GL_DEPTH_TEST);
}

void FBOManager::drawBuffer()
{
	if (!enabled) return;

	/* code to write out the FBO (texture) just once  - this is for debugging*/
	if (write) {
		for (int i = 0; i < NUM_BUFFERS; i++)
		{
			writeTexture("texture" + to_string(i) + ".png", texBuf[i]);
		}
		write = false;
	}

	bindScreen();

	glDisable(GL_DEPTH_TEST);
	shared_ptr<Program> fboProg = ShaderManager::getInstance()->getShader(WATERFBOPROG);
	fboProg->bind();
	glUniform1f(fboProg->getUniform("chaos"), chaos);
	glUniform1f(fboProg->getUniform("confuse"), confuse);
	glUniform1f(fboProg->getUniform("shake"), shake);
	glUniform1f(fboProg->getUniform("water"), water);
	ShaderManager::getInstance()->sendUniforms(WATERFBOPROG);
	drawTex(texBuf[texture]);
	fboProg->unbind();
	glEnable(GL_DEPTH_TEST);
}

void FBOManager::update(float deltaTime, float gameTime)
{
	blurAmount = mix(blurAmount, 0.0f, deltaTime * RECOVERY_SPEED);
}

void FBOManager::writeTexture(const std::string filename, GLuint tex)
{
	assert(GLTextureWriter::WriteImage(tex, filename));
	cout << "FBOManager: Wrote out texture to " << filename << endl;
}

void FBOManager::initFBOs()
{
	int width, height;
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	GLuint depthBuf[NUM_BUFFERS];

	//create two frame buffer objects to toggle between
	//make two FBOs and two textures
	glGenFramebuffers(NUM_BUFFERS, frameBuf);
	glGenTextures(NUM_BUFFERS, texBuf);
	glGenRenderbuffers(NUM_BUFFERS, depthBuf);

	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

	for (int i = 0; i < NUM_BUFFERS; i++)
	{
		//create another FBO so we can swap back and forth
		createFBO(frameBuf[i], texBuf[i]);

		//set up depth necessary since we are rendering a mesh that needs depth test
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuf[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf[i]);
		//more FBO set up
		glDrawBuffers(1, drawBuffers);
	}
}

void FBOManager::initQuad()
{
	//this one doesn't need depth - its just an image to process into
	//now set up a simple quad for rendering FBO
	glGenVertexArrays(1, &quadVertexArrayID);
	glBindVertexArray(quadVertexArrayID);

	static const GLfloat quadVertexData[] = {
	 -1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);

}

void FBOManager::createFBO(GLuint fb, GLuint tex)
{
	//initialize FBO (global memory)
	int width, height;
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	//set up framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	//set up texture
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "Error setting up frame buffer - exiting" << endl;
		exit(0);
	}
}

void FBOManager::processDrawTex(GLuint tex, int program)
{
	shared_ptr<Program> fboProg = ShaderManager::getInstance()->getShader(program);
	//example applying of 'drawing' the FBO texture
	//this shader just draws right now
	fboProg->bind();
	ShaderManager::getInstance()->sendUniforms(program);
	drawTex(tex);
	fboProg->unbind();
}

void FBOManager::drawTex(GLuint tex)
{
	//set up inTex as my input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	glBindVertexArray(quadVertexArrayID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

void FBOManager::processBindTex(int prog, int frameIndex, int texIndex)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[frameIndex]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	processDrawTex(texBuf[texIndex], prog);
}