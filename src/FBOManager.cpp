#include "FBOManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"

#include <iostream>

using namespace std;
using namespace glm;

FBOManager& FBOManager::getInstance()
{
	static FBOManager instance;
	return instance;
}

FBOManager::FBOManager()
{
	int width, height;
	glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);
	glViewport(0, 0, width, height);
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
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	//create another FBO so we can swap back and forth
	createFBO(frameBuf[1], texBuf[1]);
	//this one doesn't need depth - its just an image to process into
	initQuad();
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
/*	if (FirstTime) {
		assert(GLTextureWriter::WriteImage(texBuf[0], "Texture_output.png"));
		FirstTime = 0;
	}*/

	//regardless NOW set up to render to the screen = 0
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* now draw the actual output  to the default framebuffer - ie display */
	/* note the current base code is just using one FBO and texture - will need
	  to change that  - we pass in texBuf[0] right now */
	processDrawTex(texBuf[0]);
}

void FBOManager::processDrawTex(GLuint inTex)
{
	shared_ptr<Program> fboProg = ShaderManager::getInstance()->getShader(FBOPROG);
	//set up inTex as my input texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inTex);
	//example applying of 'drawing' the FBO texture
	//this shader just draws right now
	cout << "Process Texture " << endl;
	fboProg->bind();
	glUniform1i(fboProg->getUniform("texBuf"), 0);
	glUniform1f(fboProg->getUniform("fTime"), glfwGetTime());
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	cout << "Unbinding...." << endl;
	fboProg->unbind();
	cout << "Texture Processed" << endl;
}

void FBOManager::initQuad()
{
	//now set up a simple quad for rendering FBO
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);

	static const GLfloat g_quad_vertex_buffer_data[] = {
	 -1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 -1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

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