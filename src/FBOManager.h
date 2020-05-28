#pragma  once

#include <memory>

#include "Program.h"
#include "Constants.h"

class FBOManager
{

	FBOManager();

	public:
		static constexpr int MAIN_BUFFER = 0;
		static constexpr int BLUR_BUFFER = 1;
		static constexpr int FOG_BUFFER = 2;
		static constexpr int DEPTH_BUFFER = 3;
		static constexpr int SHADOW_BUFFER = 4;
		static constexpr int NUM_BUFFERS = 5;


		static FBOManager& getInstance();

		~FBOManager() {}

		void bindScreen() const;
		void bindBuffer(int bufferIndex) const;
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
		void toggleWater() { water = !water; }

		bool isEnabled() const { return enabled; }

		/* code to write out the FBO (texture) - helpful for debugging*/
		void writeTexture(const std::string filename, GLuint tex);

		GLuint getTexBufId(int index) { return texBuf[index]; }

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

		bool chaos, confuse, shake, water;
		
		void initFBOs();
		/**** geometry set up for a quad *****/
		void initQuad();

		/* Helper function to create the framebuffer object and associated texture to write to */
		void createFBO(GLuint fb, GLuint tex);

		void createDepthFBO(GLuint fb, GLuint tex);

		/* Process Texture on the specificed texture  - could vary what it does based on
			shader  - works on inTex - runs shaders and output to textured quad */
		void processDrawTex(GLuint tex, int program);
		void drawTex(GLuint tex);
		void processBindTex(int prog, int frameIndex, int texIndex);
};
