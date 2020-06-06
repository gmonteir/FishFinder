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
#include "CutSceneManager.h"
#include "GameManager.h"
#include "FBOManager.h"
#include "ParticleManager.h"
#include "Textures.h"
#include "Floor.h"
#include "AudioManager.h"

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

	//example data that might be useful when trying to compute bounds on multi-shape
	vec3 targetPos = vec3(0, 0, -10);

	shared_ptr<Entity> player;
	shared_ptr<Behavior::PlayerBehavior> playerBehavior;
	shared_ptr<Entity> testChar;
	Camera camera;

	bool showTopCamera = false;
	mat4 topView = lookAt(vec3(0, 100, 0), vec3(0, 99, 1), YAXIS);

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) override
	{
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
 			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
 		}
 		if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
 			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
 		}
		if (action == GLFW_PRESS) {
			switch (key)
			{
			case GLFW_KEY_ESCAPE: // Close Game
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_R: // Reset Game
				reset();
				break;
			case GLFW_KEY_SPACE: // Start Game
				start();
				break;
			case GLFW_KEY_1: // 1st person
				camera.firstPerson();
				break;
			case GLFW_KEY_2: // 2nd person
				camera.secondPerson();
				break;
			case GLFW_KEY_3: // 3rd person
				camera.thirdPerson();
				break;
			case GLFW_KEY_4: // Side person
				camera.sidePerson();
				break;
			case GLFW_KEY_COMMA: // cheat stamina
				GameManager::getInstance().increaseStamina(STAMINA_INCREMENT);
				break;
			case GLFW_KEY_B: // Blur screen
				FBOManager::getInstance().increaseBlurAmount(BLUR_INCREMENT);
				break;
			case GLFW_KEY_F: // FBO toggle
				FBOManager::getInstance().toggleEnabled();
				break;
			case GLFW_KEY_T: // write texture
				FBOManager::getInstance().writeNextTexture();
				break;
			case GLFW_KEY_Y: // draw next Effect
				FBOManager::getInstance().toggleTexture();
				break;
			case GLFW_KEY_X: // Confusion Effect
				FBOManager::getInstance().triggerConfuse();
				break;
			case GLFW_KEY_C: // Chaos Effect
				FBOManager::getInstance().toggleChaos();
				break;
			case GLFW_KEY_V: // Shake Effect
				FBOManager::getInstance().triggerShake();
				break;
			case GLFW_KEY_N: // Water Effect
				FBOManager::getInstance().toggleWater();
				break;
			case GLFW_KEY_P: // top Camera
				showTopCamera = !showTopCamera;
				break;
			}
		}
		if (GameManager::getInstance().inGame())
			Keys::getInstance().update(key, action);
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
		Skybox::getInstance(); // initialize skybox
		player = make_shared<Entity>(DORY_SHAPE, int(Behavior::PLAYER));
		playerBehavior = dynamic_pointer_cast<Behavior::PlayerBehavior>(player->getBehavior());

		/*
		testChar = make_shared<Entity>(SHARK_SHAPE, int(Behavior::NONE));
		testChar->getModel().setTexture(SHARK_TEXTURE);
		testChar->getTransform().setSize(vec3(3*PLAYER_SIZE));
		testChar->getTransform().setPosition(vec3(5, 0, 20));
		testChar->bringToFloor();
		*/

		EntityCollection::getInstance()->addEntity(player);
		//EntityCollection::getInstance()->addEntity(testChar);

		Spawner::getInstance()->init(player);
		playerBehavior->setTarget(Spawner::getInstance()->spawnFollower());
		CutSceneManager::getInstance().nextCutScene();

		/*for (int i = 0; i < 85; i++)
			Spawner::getInstance()->spawnFollower();*/

		FBOManager::getInstance().increaseBlurAmount(BLUR_INCREMENT);
	}

	void start()
	{
		GameManager::getInstance().play();
		FBOManager::getInstance().increaseBlurAmount(BLUR_INCREMENT);
		FBOManager::getInstance().triggerConfuse();

		cout << "Play!" << endl;
	}

	void reset()
	{
		EntityCollection::getInstance()->reset();
		GameManager::getInstance().reset();
		playerBehavior->reset();
		EntityCollection::getInstance()->addEntity(player);
		Spawner::getInstance()->init(player);
		playerBehavior->setTarget(Spawner::getInstance()->spawnFollower());

		CutSceneManager::getInstance().reset();
		CutSceneManager::getInstance().nextCutScene();
		FBOManager::getInstance().increaseBlurAmount(BLUR_INCREMENT);
		cout << "Resetted!" << endl;
	}

	void update(float deltaTime, float gameTime)
	{
		GameManager::getInstance().update(deltaTime, gameTime);
		if (GameManager::getInstance().inGame())
		{
			Spawner::getInstance()->update(deltaTime, gameTime);
			CutSceneManager::getInstance().update(deltaTime, gameTime);
			EntityCollection::getInstance()->update(deltaTime);

			FBOManager::getInstance().update(deltaTime, gameTime);
		}
		camera.update(deltaTime, player->getTransform());
	}

	void renderScene(shared_ptr<MatrixStack> Model, vec4* planes, float deltaTime)
	{
		Skybox::getInstance().draw(Model, camera.getEye());
		Floor::getInstance()->draw(Model);
		EntityCollection::getInstance()->draw(Model, planes);
		ParticleManager::getInstance().processParticles(player->getTransform().getPosition(), deltaTime);
	}

	void renderSceneToFBO(int fbo, int progIndex, shared_ptr<MatrixStack> Model, vec4* planes)
	{
		FBOManager::getInstance().bindBuffer(fbo);
		shared_ptr<Program> prog = ShaderManager::getInstance()->getShader(progIndex);
		prog->bind();
			ShaderManager::getInstance()->sendUniforms(LIGHTDEPTHPROG);
			EntityCollection::getInstance()->draw(prog, Model, planes);
			Floor::getInstance()->draw(prog, Model);
		prog->unbind();
	}

	void render(float deltaTime, float gameTime)
	{
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(WindowManager::instance->getHandle(), &width, &height);
		glViewport(0, 0, width, height);
		
		/* Leave this code to just draw the meshes alone */
		float aspect = width/(float)height;

		// Create the matrix stacks
		auto P = make_shared<MatrixStack>();
		auto Model = make_shared<MatrixStack>();

		// Apply perspective projection.
		P->pushMatrix();
		P->perspective(45.0f, aspect, 0.01f, 10000.0f);

		vec4 planes[6];
		camera.extractVFPlanes(P->topMatrix(), planes);

		uniforms commonUniforms{
			P->topMatrix(),									// P
			showTopCamera ? topView : camera.getView(),		// V
			camera.getEye(),								// Camera Position
			playerBehavior->getTargetPos(),					// Goal/Target Position
			gameTime,										// Game Time
			GameManager::getInstance().getCharRemaining()	// Characters Remaining
		};
		ShaderManager::getInstance()->setData(commonUniforms);
		P->popMatrix();

		// ---------------------- drawing ----------------- //
		if (FBOManager::getInstance().isEnabled())
		{
			//glCullFace(GL_FRONT);
			renderSceneToFBO(int(FBOManager::SHADOW_BUFFER), LIGHTDEPTHPROG, Model, planes);
			//glCullFace(GL_BACK);
			renderSceneToFBO(int(FBOManager::DEPTH_BUFFER), DEPTHPROG, Model, planes);

			FBOManager::getInstance().bindBuffer(int(FBOManager::MAIN_BUFFER));
			renderScene(Model, planes, deltaTime);

			FBOManager::getInstance().processFog();
			FBOManager::getInstance().processBlur();
			player->draw(Model);
			FBOManager::getInstance().drawBuffer();
		}
		else
		{
			FBOManager::getInstance().bindScreen();
			renderScene(Model, planes, deltaTime);
		}

		GameManager::getInstance().draw();
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

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state
	application->init();
	application->initEntities();
	//AudioManager::getInstance().startMusicLoop();

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
		application->render(deltaTime, gameTime);


		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
