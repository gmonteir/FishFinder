#pragma  once

#include <memory>

#include "Program.h"
#include "Constants.h"

class FBOManager
{
	static const int NUM_BUFFERS = 3;

	FBOManager();

	public:
		static FBOManager& getInstance();

		~FBOManager() {}

		void bindBuffer();
		void bindDepthBuffer();
		void processFog();
		void processBlur();
		void drawBuffer();

		void update(float deltaTime, float gameTime);

		void increaseBlurAmount(float delta) { blurAmount += delta; }
		void toggleEnabled() { enabled = !enabled; }
		void writeNextTexture() { write = true; }
		void toggleTexture() { texture = (texture + 1) % NUM_BUFFERS; }

		void toggleChaos() { chaos = !chaos; }
		void toggleConfuse() { confuse = !confuse; }
		void toggleShake() { shake = !shake; }

		bool isEnabled() { return enabled; }

		/* code to write out the FBO (texture) - helpful for debugging*/
		void writeTexture(const std::string filename, GLuint tex);

	private:
		GLuint frameBuf[NUM_BUFFERS];
		GLuint texBuf[NUM_BUFFERS];
		//geometry for texture render
		GLuint quadVertexArrayID;
		GLuint quadVertexBuffer;

		bool enabled;
		bool write; // debug
		int texture;
		float blurAmount;

		bool chaos, confuse, shake;
		
		void initFBOs();
		/**** geometry set up for a quad *****/
		void initQuad();

		/* Helper function to create the framebuffer object and associated texture to write to */
		void createFBO(GLuint fb, GLuint tex);

		/* Process Texture on the specificed texture  - could vary what it does based on
			shader  - works on inTex - runs shaders and output to textured quad */
		void processDrawTex(GLuint tex, int program);
		void drawTex(GLuint tex);
		void processBindTex(int prog, int frameIndex, int texIndex);
};
