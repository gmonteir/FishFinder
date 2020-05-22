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
		
		/*
		void bindBuffer();
		void blur();
		void drawBuffer();

		void update(float deltaTime, float gameTime);

		void increaseBlurAmount(float delta) { blurAmount += delta; }
		void toggleEnabled() { enabled = !enabled; }
		*/
		/* code to write out the FBO (texture) - helpful for debugging*/
		//void writeTexture(const std::string filename);

	private:
		GLuint VertexArrayID;
		GLuint VertexBufferID;
		std::vector<float> buf;

		void init();

		/******* initializes the positions of the particles to render ********/
		void initParticleBuff(std::vector<float> *points);


		/*
		GLuint frameBuf[2];
		GLuint texBuf[2];
		//geometry for texture render
		GLuint quadVertexArrayID;
		GLuint quadVertexBuffer;

		bool enabled;
		float blurAmount;
		
		void initFBOs();
		*/
		/**** geometry set up for a quad *****/
		//void initQuad();

		/* Helper function to create the framebuffer object and associated texture to write to */
		//void createFBO(GLuint fb, GLuint tex);

		/* Process Texture on the specificed texture  - could vary what it does based on
			shader  - works on inTex - runs shaders and output to textured quad */
		//void processDrawTex(GLuint tex, int program);
};
