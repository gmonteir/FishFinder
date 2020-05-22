#include "FBOManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"

#include <iostream>

using namespace std;
using namespace glm;

FBOManager::FBOManager() : blurAmount(0), enabled(true)
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

void FBOManager::bindBuffer()
{
	if (enabled)
		//set up to render to first FBO stored in array position 0
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[0]);
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBOManager::blur()
{
	if (!enabled) return;

	glDisable(GL_DEPTH_TEST);
	for (size_t i = 0; i < round(blurAmount); i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[(i + 1) % 2]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processDrawTex(texBuf[i % 2], BLURPROG);
	}
	glEnable(GL_DEPTH_TEST);
}

void FBOManager::drawBuffer()
{
	if (!enabled) return;

	/* code to write out the FBO (texture) just once  - this is for debugging*/
	//if (firstTime) {
	//	writeTexture("texture_output.png");
	//	firstTime = false;
	//}

	// render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	processDrawTex(texBuf[0], FBOPROG);
	glEnable(GL_DEPTH_TEST);
}

void FBOManager::update(float deltaTime, float gameTime)
{
	blurAmount = mix(blurAmount, 0.0f, deltaTime * RECOVERY_SPEED);
}

void FBOManager::writeTexture(const std::string filename)
{
	assert(GLTextureWriter::WriteImage(texBuf[0], filename));
	cout << "FBOManager: Wrote out texture to " << filename << endl;
}

void FBOManager::initFBOs()
{
	int width, height;
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	GLuint depthBuf;

	//create two frame buffer objects to toggle between
	//make two FBOs and two textures
	glGenFramebuffers(2, frameBuf);
	glGenTextures(2, texBuf);
	glGenRenderbuffers(1, &depthBuf);

	//create one FBO
	createFBO(frameBuf[0], texBuf[0]);

	//set up depth necessary since we are rendering a mesh that needs depth test
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

	//more FBO set up
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	//create another FBO so we can swap back and forth
	createFBO(frameBuf[1], texBuf[1]);
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
	//set up inTex as my input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	//example applying of 'drawing' the FBO texture
	//this shader just draws right now
	fboProg->bind();
	glUniform1i(fboProg->getUniform("texBuf"), 0);
	glUniform1f(fboProg->getUniform("fTime"), glfwGetTime());
	ShaderManager::getInstance()->sendUniforms(FBOPROG);
	glBindVertexArray(quadVertexArrayID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	fboProg->unbind();
}