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
#include "WindowManager.h"
#include "GLTextureWriter.h"
#include "Draw.h"
#include "Keys.h"
#include "Camera.h"
#include "Entities.h"
#include "ShaderManager.h"
#include "RenderText.h"
#include "Player.h"

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

	// Our shader program
	std::shared_ptr<Program> prog;

	// Shape to be used (from obj file)
	shared_ptr<Shape> shape;
	shared_ptr<vector<shared_ptr<Shape>>> dummyShapes;
	shared_ptr<vector<shared_ptr<Shape>>> doryShapes;
	shared_ptr<vector<shared_ptr<Shape>>> nemoShapes;
	shared_ptr<vector<shared_ptr<Shape>>> cube;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	//example data that might be useful when trying to compute bounds on multi-shape
	vec3 gMin;
	vec3 gMax;
	vec3 doryMin;
	vec3 doryMax;
	vec3 tMin;
	vec3 tMax;
	vec3 lightDir = vec3(0.2, 1, 0.5);
	float lightPosX;

	// texture for skymap
	unsigned int cubeMapTexture;
	shared_ptr<Texture> doryTexture;

	int drawMode = 0;

	shared_ptr<Player> player;
	shared_ptr<Entity> floor;
	shared_ptr<Entity> dory;
	Camera camera;
	Keys keyInput;
	ShaderManager *shaderManager;
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
		keyInput.update(key, action);
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY)
	{
		player->rotate(deltaX, deltaY);

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
	void initTex(const std::string& resourceDirectory)
	{
		doryTexture = make_shared<Texture>();
		doryTexture->setFilename(resourceDirectory + "/dory.jpg");
		doryTexture->init();
		doryTexture->setUnit(0);
		doryTexture->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		vector<std::string> faces {
    	"uw_rt.jpg",
    	"uw_lf.jpg",
    	"uw_up.jpg",
    	"uw_dn.jpg",
    	"uw_ft.jpg",
    	"uw_bk.jpg"
		}; 
		cubeMapTexture = createSky(resourceDirectory + "/underwater/",  faces);
	}

	void init(const std::string& resourceDirectory)
	{
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		//initialize the textures we might use
		initTex(resourceDirectory);
		shaderManager = new ShaderManager(resourceDirectory);

		FT_Library ft;
		textRenderer = new RenderText(&ft, shaderManager->getShader(GLYPHPROG));
	 }

	void initGeom(const std::string& resourceDirectory)
	{
		shared_ptr<Shape> cube = make_shared<Shape>();
		cube->loadSingleShapeMesh(resourceDirectory + "/cube.obj");
		this->cube = make_shared<vector<shared_ptr<Shape>>>();
		this->cube->push_back(cube);

		dummyShapes = make_shared<vector<shared_ptr<Shape>>>();
		loadMultipleShapeMesh(dummyShapes, &gMin, &gMax, resourceDirectory + "/dummy.obj");

		doryShapes = make_shared<vector<shared_ptr<Shape>>>();
		loadMultipleShapeMesh(doryShapes, &doryMin, &doryMax, resourceDirectory + "/dory.obj");

		nemoShapes = make_shared<vector<shared_ptr<Shape>>>();
		shared_ptr<Shape> nemo = make_shared<Shape>();
		nemo->loadSingleShapeMesh(resourceDirectory + "/nemo.obj");
		nemoShapes->push_back(nemo);
	}

	void initEntities()
	{
		floor = make_shared<Entity>(*cube, FLOOR_POSITION, ORIGIN, FLOOR_SIZE, -ZAXIS, 2);
		floor->remove(); // mark as dead so that it doesnt change color when player collides
		dory = make_shared<Entity>(*doryShapes, vec3(5, 5, -20), ORIGIN, vec3(5));
		dory->setTexture(doryTexture);
		player = make_shared<Player>(*cube);

		Entities::getInstance()->push_back(player);
		Entities::getInstance()->push_back(floor);
		Entities::getInstance()->init(*nemoShapes);
	}

	void SetModel(shared_ptr<Program>& p, shared_ptr<MatrixStack>& M)
	{
		glUniformMatrix4fv(p->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
	}

	void update(float deltaTime, float gameTime)
	{
		player->keyUpdate(keyInput);
		Entities::getInstance()->update(deltaTime, gameTime);
		camera.update(player->getPosition(), player->getFacing());
	}

	void render(int fps)
	{
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
		P->perspective(45.0f, aspect, 0.01f, 200.0f);
		mat4 V = camera.getView();

		// draw the floor and the nemos
		prog = shaderManager->getShader(SIMPLEPROG);
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		glUniform3f(prog->getUniform("lightDir"), lightDir.x, lightDir.y, lightDir.z);
		glUniform3f(prog->getUniform("lightCol"), 1, 1, 1);
		glUniform3f(prog->getUniform("eye"), camera.getEye().x, camera.getEye().y, camera.getEye().z);
		floor->draw(prog, Model);
		Entities::getInstance()->draw(prog, Model);
		prog->unbind();

		//draw the sky box
		prog = shaderManager->getShader(SKYBOXPROG);
		prog->bind();
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		glDepthFunc(GL_LEQUAL);
		Model->pushMatrix();
			Model->loadIdentity();
			Model->translate(vec3(0, 5, 0));
			Model->translate(camera.getEye());
			Model->rotate(radians(-180.f), vec3(0, 1, 0));
			Model->scale(vec3(100));
			glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,value_ptr(Model->topMatrix()) );
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
			cube->at(0)->draw(prog);
		glDepthFunc(GL_LESS);
		Model->popMatrix();
		prog->unbind();

		// draw  dory
		prog = shaderManager->getShader(TEXTUREPROG);
		prog->bind();
		glUniform3f(prog->getUniform("lightDir"), lightDir.x, lightDir.y, lightDir.z);
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V));
		doryTexture->bind(prog->getUniform("Texture0"));
		dory->draw(prog, Model);
		prog->unbind();

		/* FreeType */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		prog = shaderManager->getShader(GLYPHPROG);
		prog->bind();
		glm::mat4 proj = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(proj));
		textRenderer->drawText("Active Objects: " + to_string(Entities::getInstance()->getSpawned() - player->getScore()), 25.0f, height - 50.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
		textRenderer->drawText("Score: " + to_string(player->getScore()), 25.0f, height - 100.0f, 0.75f, glm::vec3(0.2f, 1.0f, 0.2f));
		textRenderer->drawText("FPS: " + to_string(fps), 25.0f, 25.0f, 0.75f, glm::vec3(0.1));
        prog->unbind();
        P->popMatrix();
        glDisable(GL_BLEND);

	}	
};

int main(int argc, char **argv)
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(512, 512);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state
	application->init(resourceDir);
	application->initGeom(resourceDir);
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
