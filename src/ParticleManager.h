#pragma  once

#include <memory>
#include <vector>

#include "Program.h"
#include "Constants.h"

class ParticleManager
{
	ParticleManager();

	public:
		static ParticleManager& getInstance();

		~ParticleManager() {}

		/******* draws the particles ******/
		void processParticles();

	private:
		GLuint VertexArrayID;
		GLuint VertexBufferID;
		std::vector<float> buf;

		void init();

		/******* initializes the positions of the particles to render ********/
		void initParticleBuff(std::vector<float> *points);

};
