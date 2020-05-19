#include "FBOManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"

#include <iostream>

using namespace std;
using namespace glm;

FBOManager& FBOManager::getInstance()
{
	static FBOManager instance;
	return instance;
}

FBOManager::FBOManager() : firstTime(true)
{
	initFBOs();
	initVAO();
	initQuad();
	cout << "Initialized FBOManager" << endl;
}

void FBOManager::bindBuffer()
{

	//set up to render to first FBO stored in array position 0
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuf[0]);
	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBOManager::drawBuffer()
{

	/* code to write out the FBO (texture) just once  - this is for debugging*/
	/* Note that texBuf[0] corresponds to frameBuf[0] */
	if (firstTime) {
		assert(GLTextureWriter::WriteImage(texBuf[0], "texture_output.png"));
		firstTime = false;
	}

	//regardless NOW set up to render to the screen = 0
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* now draw the actual output  to the default framebuffer - ie display */
	/* note the current base code is just using one FBO and texture - will need
	  to change that  - we pass in texBuf[0] right now */
	processDrawTex();
}

void FBOManager::initFBOs()
{
	int width, height;
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);

	//create two frame buffer objects to toggle between
	//make two FBOs and two textures
	CHECKED_GL_CALL(glGenFramebuffers(2, frameBuf));
	CHECKED_GL_CALL(glGenTextures(2, texBuf));
	CHECKED_GL_CALL(glGenRenderbuffers(1, &depthBuf));

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

void FBOManager::initVAO()
{
	GLuint vao;
	//generate the VAO
	CHECKED_GL_CALL(glGenVertexArrays(1, &vao));
	CHECKED_GL_CALL(glBindVertexArray(vao));
}

void FBOManager::initQuad()
{
	//this one doesn't need depth - its just an image to process into
	//now set up a simple quad for rendering FBO
	CHECKED_GL_CALL(glGenVertexArrays(1, &quad_VertexArrayID));
	CHECKED_GL_CALL(glBindVertexArray(quad_VertexArrayID));

	static const GLfloat g_quad_vertex_buffer_data[] = {
	 -1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	};

	CHECKED_GL_CALL(glGenBuffers(1, &quad_vertexbuffer));
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer));
	CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW));

}

void FBOManager::createFBO(GLuint& fb, GLuint& tex)
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

void FBOManager::processDrawTex()
{
	shared_ptr<Program> fboProg = ShaderManager::getInstance()->getShader(FBOPROG);
	//set up inTex as my input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texBuf[0]);
	//example applying of 'drawing' the FBO texture
	//this shader just draws right now
	cout << "Process Texture " << endl;
	fboProg->bind();
	CHECKED_GL_CALL(glUniform1i(fboProg->getUniform("texBuf"), 0));
	CHECKED_GL_CALL(glUniform1f(fboProg->getUniform("fTime"), glfwGetTime()));
	CHECKED_GL_CALL(glEnableVertexAttribArray(0));
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer));
	CHECKED_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
	CHECKED_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
	CHECKED_GL_CALL(glDisableVertexAttribArray(0));
	fboProg->unbind();
	/*if (glGetShaderiv(GL_COMPILE_STATUS) == 0)
	{
		glGetShaderInfoLog()
	}*/
	cout << "Texture Processed" << endl;
}