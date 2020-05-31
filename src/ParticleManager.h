#pragma  once

#include <memory>
#include <vector>

#include "Program.h"
#include "Constants.h"
#include "Transform.h"

class ParticleManager
{
	ParticleManager();

	public:
		static ParticleManager& getInstance();

		~ParticleManager() {}

		/******* draws the particles ******/
		void processParticles(glm::vec3 translate, float deltaTime);

		void setCaptured(Transform *target) { captureMode = true; this->target = target; }
		void resetCaptured() { captureMode = false; }

	private:
		GLuint VertexArrayID;
		GLuint VertexBufferID;

		GLuint VertexArrayID2;
		GLuint VertexBufferID2;
		
		std::vector<float> bubbleBuf;
		std::vector<float> charBuf;

		bool captureMode;
		float elapsedTime;
		Transform *target;

		void init();

		/******* initializes the positions of the particles to render ********/
		void initParticleBuff(std::vector<float> *points, float scale, float shift);

};
