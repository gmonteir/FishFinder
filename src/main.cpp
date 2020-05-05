/*
 * Program 3 base code - includes modifications to shape and initGeom in preparation to load
 * multi shape objects 
 * CPE 471 Cal Poly Z. Wood + S. Sueda + I. Dunn
 */

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Shapes.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"
#include "Draw.h"
#include "Keys.h"
#include "Camera.h"
#include "Entities.h"
#include "Spawner.h"
#include "ShaderManager.h"
#include "RenderText.h"
#include "Textures.h"
#include "Floor.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <functional>
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Shape to be used (from obj file)
	shared_ptr<Shape> shape;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	//example data that might be useful when trying to compute bounds on multi-shape
	vec3 lightDir = vec3(0, 1, 0);
	float lightPosX;

	// texture for skymap
	unsigned int cubeMapTexture;

	int drawMode = 0;

	shared_ptr<Entity> player;
	shared_ptr<Behavior::PlayerBehavior> playerBehavior;
	shared_ptr<Entity> squirt;
	Camera camera;
	RenderText *textRenderer;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
 			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			drawMode = 1;
 		}
 		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
 			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			drawMode = 0;
 		}
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			camera.firstPerson();
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			camera.secondPerson();
		}
		if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		{
			camera.thirdPerson();
		}
		Keys::getInstance().update(key, action);
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY)
	{
		playerBehavior->rotate(deltaX, deltaY);

		//int width, height;
		//glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		//player->rotate(deltaX / width, deltaY / height);
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX << " Pos Y " << posY << endl;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	unsigned int createSky(string dir, vector<string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(false);
		for(GLuint i = 0; i < faces.size(); i++) {
    		unsigned char *data = 
			stbi_load((dir+faces[i]).c_str(), &width, &height, &nrChannels, 0);
			if (data) {
    			glTexImage2D(
        			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
        			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			} else {
				std::cout << "failed to load: " << (dir+faces[i]).c_str() << std::endl;
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

		cout << " creating cube map any errors : " << glGetError() << endl;
		return textureID;
	}
	// Code to load in the textures
	void initTex()
	{
		vector<std::string> faces {
    	"uw_rt.jpg",
    	"uw_lf.jpg",
    	"uw_up.jpg",
    	"uw_dn.jpg",
    	"uw_ft.jpg",
    	"uw_bk.jpg"
		}; 
		cubeMapTexture = createSky(RESOURCE_DIR + "/underwater/",  faces);
	}

	void init()
	{
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		initTex();

		FT_Library ft;
		textRenderer = new RenderText(&ft, ShaderManager::getInstance()->getShader(GLYPHPROG));
	 }

	void initEntities()
	{
		player = make_shared<Entity>(DORY_SHAPE, Behavior::PLAYER);
		playerBehavior = dynamic_pointer_cast<Behavior::PlayerBehavior>(player->getBehavior());

		squirt = make_shared<Entity>(SQUIRT_SHAPE, Behavior::NONE);
		squirt->getModel().setTexture(SQUIRT_TEXTURE);
		squirt->getModel().setProgram(TEXTUREPROG);
		squirt->getTransform().setPosition(vec3(5, 0, -10));
		squirt->bringToFloor();

		Entities::getInstance()->push_back(player);
		Entities::getInstance()->push_back(squirt);
		Spawner::getInstance()->init();
	}

	void update(float deltaTime, float gameTime)
	{
		Spawner::getInstance()->update(deltaTime, gameTime);
		Entities::getInstance()->update(deltaTime);
		camera.update(player->getTransform());
	}

	/* helper functions for sending matrix data to the GPU */
	mat4 SetProjectionMatrix(shared_ptr<Program> curShade) {
	    int width, height;
	    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	    float aspect = width/(float)height;
	    mat4 Projection = perspective(radians(50.0f), aspect, 0.1f, 100.0f);
	    glUniformMatrix4fv(curShade->getUniform("P"), 1, GL_FALSE, value_ptr(Projection));
	    return Projection;
	}

	void render(int fps)
	{
		shared_ptr<Program> prog;
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);		

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Leave this code to just draw the meshes alone */
		float aspect = width/(float)height;

		// Create the matrix stacks
		auto P = make_shared<MatrixStack>();
		auto Model = make_shared<MatrixStack>();
		// Apply perspective projection.
		P->pushMatrix();
		P->perspective(45.0f, aspect, 0.01f, 10000.0f);
		mat4 V = camera.getView();

		uniforms *commonUniforms = new uniforms {P->topMatrix(), V, lightDir, vec3(1), camera.getEye()};
		ShaderManager::getInstance()->setData(commonUniforms);
		// draw the floor and the nemos
		Entities::getInstance()->draw(Model);
		Floor::getInstance()->draw(Model);

		// draw test heightmap plane
		/*prog = ShaderManager::getInstance()->getShader(TEXTUREPROG);
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		Model->pushMatrix();
			Model->loadIdentity();
			Model->translate(vec3(100, -200, -120));
			Model->scale(vec3(2, 1, 2));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(Model->topMatrix()));
			Textures::getInstance()->getTexture(GROUND_TEXTURE)->bind(prog->getUniform("Texture0"));
			drawSamplePlane(prog);
		Model->popMatrix();
		prog->unbind();*/
		
		//draw the sky box
		prog = ShaderManager::getInstance()->getShader(SKYBOXPROG);
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		glDepthFunc(GL_LEQUAL);
		Model->pushMatrix();
			Model->loadIdentity();
			Model->translate(vec3(0, 5, 0));
			Model->translate(camera.getEye());
			Model->rotate(radians(-180.f), vec3(0, 1, 0));
			Model->scale(vec3(1000));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(Model->topMatrix()) );
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
			Shapes::getInstance()->getShape(CUBE_SHAPE)->at(0)->draw(prog);
		glDepthFunc(GL_LESS);
		Model->popMatrix();
		prog->unbind();

		/* FreeType */
		char stamina_stat[15];
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		prog = ShaderManager::getInstance()->getShader(GLYPHPROG);
		prog->bind();
		glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(proj));
		textRenderer->drawText("Active Objects: " + to_string(Entities::getInstance()->getNumActive()), 25.0f, height - 50.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
		sprintf(stamina_stat, "%.1f %%", 100* playerBehavior->getStamina()/MAX_STAMINA);
		textRenderer->drawText("Stamina: " + string(stamina_stat), 25.0f, height - 100.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
		textRenderer->drawText("FPS: " + to_string(fps), 25.0f, 25.0f, 0.75f, glm::vec3(0.1));
        prog->unbind();
        P->popMatrix();
        glDisable(GL_BLEND);

	}	
};

int main(int argc, char **argv)
{
	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(512, 512);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state
	application->init();
	application->initEntities();
	double gameTime = 0; // keep track of how long we have been in the game.
	int frameCount = 0;
	int fps = 0;
	double accumulator = 0;
	double currentTime = glfwGetTime();

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		double newTime = glfwGetTime();
		double deltaTime = newTime - currentTime;
		accumulator += deltaTime;
		currentTime = newTime;

		application->update(deltaTime, gameTime);
		// Render scene.
		application->render(fps);
		frameCount += 1;
		if (accumulator > 1) // 1 second
		{
			fps = (int)((float)frameCount/accumulator);
			accumulator = 0;
			frameCount = 0;
		}
		gameTime += deltaTime;

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
