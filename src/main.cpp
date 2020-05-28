/*
 * Finding Dory
 * CPE 476 Cal Poly Zoe Wood, Steven Pineda, Nathan Lui, Garrett Monteiro, Nick Ryan
 */

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Shapes.h"
#include "WindowManager.h"
#include "Skybox.h"
#include "Keys.h"
#include "Camera.h"
#include "Spawner.h"
#include "ShaderManager.h"
#include "GameManager.h"
#include "FBOManager.h"
#include "ParticleManager.h"
#include "Textures.h"
#include "Floor.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <glad/glad.h>

#include <functional>
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include "EntityCollection.h"

using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:
	WindowManager* windowManager = nullptr;

	//example data that might be useful when trying to compute bounds on multi-shape
	vec3 lightPos = vec3(0, 70, 0);
	vec3 targetPos = vec3(0, 0, -10);

	int drawMode = 0;

	shared_ptr<Entity> player;
	shared_ptr<Behavior::PlayerBehavior> playerBehavior;
	shared_ptr<Entity> testChar;
	Camera camera;
	Camera topCamera;

	bool TOP_CAM = false;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) override
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
		if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
		{
			GameManager::getInstance()->increaseStamina(STAMINA_INCREMENT);
		}
		if (key == GLFW_KEY_B && action == GLFW_PRESS)
		{
			FBOManager::getInstance().increaseBlurAmount(BLUR_INCREMENT);
		}
		if (key == GLFW_KEY_F && action == GLFW_PRESS)
		{
			FBOManager::getInstance().toggleEnabled();
		}
		if (key == GLFW_KEY_T && action == GLFW_PRESS)
		{
			FBOManager::getInstance().writeNextTexture();
		}
		if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		{
			FBOManager::getInstance().toggleTexture();
		}
		if (key == GLFW_KEY_X && action == GLFW_PRESS)
		{
			FBOManager::getInstance().toggleConfuse();
		}
		if (key == GLFW_KEY_C && action == GLFW_PRESS)
		{
			FBOManager::getInstance().toggleChaos();
		}
		if (key == GLFW_KEY_V && action == GLFW_PRESS)
		{
			FBOManager::getInstance().toggleShake();
		}
		if (key == GLFW_KEY_N && action == GLFW_PRESS)
		{
			FBOManager::getInstance().toggleWater();
		}
		Keys::getInstance().update(key, action);
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			TOP_CAM = !TOP_CAM;
		}
	}

	void mouseCallback(GLFWwindow* window, int button, int action, int mods) override
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX << " Pos Y " << posY << endl;

			cout << "X: " << player->getTransform().getPosition().x << " Y: " << player->getTransform().getPosition().y << " Z: " << player->getTransform().getPosition().z << endl;
		}
	}

	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) override
	{
		camera.cursorCallback(xpos, ypos);
	}

	void scrollCallback(GLFWwindow* window, double deltaX, double deltaY) override
	{
		camera.interpolateRotation(deltaX, deltaY, MOUSE_SENSITIVITY);
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void init()
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(.12f, .34f, .56f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 }

	void initEntities()
	{
		player = make_shared<Entity>(DORY_SHAPE, int(Behavior::PLAYER));
		playerBehavior = dynamic_pointer_cast<Behavior::PlayerBehavior>(player->getBehavior());

		/*
		testChar = make_shared<Entity>(MARLIN_SHAPE, int(Behavior::NONE));
		testChar->getModel().setTexture(MARLIN_TEXTURE);
		testChar->getModel().setProgram(TEXTUREPROG);
		testChar->getTransform().setSize(vec3(PARENT_SIZE));
		testChar->getTransform().setPosition(vec3(5, 0, -10));
		testChar->bringToFloor();
		*/

		EntityCollection::getInstance()->addEntity(player);
		//EntityCollection::getInstance()->addEntity(testChar);

		Spawner::getInstance()->init();
		playerBehavior->setTarget(&Spawner::getInstance()->spawnFollower()->getTransform());

		/*for (int i = 0; i < 85; i++)
			Spawner::getInstance()->spawnFollower();*/

		Skybox::getInstance(); // initialize skybox
	}

	void update(float deltaTime, float gameTime)
	{
		Spawner::getInstance()->update(deltaTime, gameTime);
		GameManager::getInstance()->update(deltaTime, gameTime);
		EntityCollection::getInstance()->update(deltaTime);
		camera.update(deltaTime, player->getTransform());

		FBOManager::getInstance().update(deltaTime, gameTime);
	}



	void render()
	{
		shared_ptr<Program> prog;
		Camera* Cam;

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);
		
		/* Leave this code to just draw the meshes alone */
		float aspect = width/(float)height;

		// Create the matrix stacks
		auto P = make_shared<MatrixStack>();
		auto Model = make_shared<MatrixStack>();
		// Apply perspective projection.
		P->pushMatrix();
		P->perspective(45.0f, aspect, 0.01f, 10000.0f);
		mat4 V = camera.getView();

		vec4 planes[6];
		
		camera.ExtractVFPlanes(P->topMatrix(), V, planes);

		if (TOP_CAM) {
			V = lookAt(vec3(0, 100, 0), vec3(0, 99, 1), YAXIS);
			//topCamera.setEye(vec3(0, 100, 0));
			//topCamera.setLA(vec3(0, 99, 0));
			//Cam = &topCamera;
		}

		targetPos = playerBehavior->getTargetPos();
		float time = glfwGetTime();
		int remaining = GameManager::getInstance()->getGameStats().charRemaining;
		uniforms commonUniforms{P->topMatrix(), V, camera.getEye(), targetPos, time, remaining};
		ShaderManager::getInstance()->setData(commonUniforms);
		P->popMatrix();

		// ---------------------- drawing depth buffer ----------------- //
		if (FBOManager::getInstance().isEnabled())
		{
			FBOManager::getInstance().bindBuffer(int(FBOManager::DEPTH_BUFFER));
			prog = ShaderManager::getInstance()->getShader(DEPTHPROG);
			prog->bind();
			ShaderManager::getInstance()->sendUniforms(DEPTHPROG);
			EntityCollection::getInstance()->draw(prog, Model, planes);
			Floor::getInstance()->draw(prog, Model);
			Skybox::getInstance().draw(prog, Model, camera.getEye());
			prog->unbind();

			FBOManager::getInstance().bindBuffer(int(FBOManager::MAIN_BUFFER));
		}
		else
		{
			FBOManager::getInstance().bindScreen();
		}

		// ---------------------- drawing ----------------- //

		/* draw the complete scene from a top down camera */

		EntityCollection::getInstance()->draw(Model, planes);
		Floor::getInstance()->draw(Model);
		Skybox::getInstance().draw(Model, camera.getEye());
		ParticleManager::getInstance().processParticles();

		FBOManager::getInstance().processFog();
		FBOManager::getInstance().processBlur();

		player->draw(Model);
		FBOManager::getInstance().drawBuffer();

		GameManager::getInstance()->draw();
	}	
};

int main(int argc, char **argv)
{
	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(int(WINDOW_WIDTH), int(WINDOW_HEIGHT));
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state
	application->init();
	application->initEntities();

	double gameTime = 0; // keep track of how long we have been in the game.
	double currentTime = glfwGetTime();
	glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		double newTime = glfwGetTime();
		double deltaTime = newTime - currentTime;
		currentTime = newTime;
		gameTime += deltaTime;

		application->update(deltaTime, gameTime);
		// Render scene.
		application->render();


		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
