#include "FBOManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"

#include <iostream>

using namespace std;
using namespace glm;

FBOManager::FBOManager() : data(), debug(), activeBuffer(0)
{
	initFBOs();
	initQuad();
	cout << "FBOManager: Initialized" << endl;
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
		if (i == SHADOW_BUFFER)
		{
			createDepthFBO(frameBuf[i], texBuf[i]);
			//bind with framebuffer's depth buffer
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texBuf[i], 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			continue;
		}
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

void FBOManager::createDepthFBO(GLuint fb, GLuint tex)
{
	//initialize FBO (global memory)
	int width, height;
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	//set up framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	//set up texture
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "Error setting up frame buffer - exiting" << endl;
		exit(0);
	}
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

void FBOManager::bindBuffer(int bufferIndex)
{
	activeBuffer = bufferIndex;
	//set up to render to first FBO stored in array position 0
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[bufferIndex]);
	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBOManager::processFog()
{
	if (!debug.enabled) return;

	glDisable(GL_DEPTH_TEST);
	bindBuffer(FOG_BUFFER);
	shared_ptr<Program> fboProg = ShaderManager::getInstance()->getShader(FOGFBOPROG);
	fboProg->bind();
		ShaderManager::getInstance()->sendUniforms(FOGFBOPROG);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texBuf[DEPTH_BUFFER]);
		drawTex(texBuf[MAIN_BUFFER]);
	fboProg->unbind();
	glEnable(GL_DEPTH_TEST);
}

void FBOManager::processBlur()
{
	if (!debug.enabled) return;

	glDisable(GL_DEPTH_TEST);
	int buffers[] = { activeBuffer, BLUR_BUFFER };
	for (size_t i = 0; i < round(data.blurAmount); i++)
	{
		bindBuffer(buffers[(i + 1) % 2]);
		processDrawTex(BLURFBOPROG, texBuf[buffers[i % 2]]);
	}
	glEnable(GL_DEPTH_TEST);
}

void FBOManager::drawBuffer()
{
	if (!debug.enabled) return;

	/* code to write out the FBO (texture) just once  - this is for debugging*/
	if (debug.write) {
		writeTexture("texture" + to_string(debug.texture) + ".png", texBuf[debug.texture]);
		debug.write = false;
	}

	bindScreen();

	glDisable(GL_DEPTH_TEST);
	shared_ptr<Program> fboProg = ShaderManager::getInstance()->getShader(WATERFBOPROG);
	fboProg->bind();
	glUniform1f(fboProg->getUniform("chaos"), data.chaos);
	glUniform1f(fboProg->getUniform("confuse"), data.confuseTimer > 0);
	glUniform1f(fboProg->getUniform("shake"), data.shakeTimer > 0);
	glUniform1f(fboProg->getUniform("water"), data.water);
	ShaderManager::getInstance()->sendUniforms(WATERFBOPROG);
	drawTex(texBuf[activeBuffer]);
	fboProg->unbind();
	glEnable(GL_DEPTH_TEST);
}

void FBOManager::update(float deltaTime, float gameTime)
{
	data.blurAmount = mix(data.blurAmount, 0.0f, deltaTime * RECOVERY_SPEED);
	if (data.shakeTimer > 0) { data.shakeTimer -= deltaTime; }
	if (data.confuseTimer > 0) { data.confuseTimer -= deltaTime; }
}


void FBOManager::processDrawTex(int program, GLuint tex)
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

void FBOManager::writeTexture(const std::string filename, GLuint tex)
{
	assert(GLTextureWriter::WriteImage(tex, filename));
	cout << "FBOManager: Wrote out texture to " << filename << endl;
}