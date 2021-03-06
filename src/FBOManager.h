#pragma  once

#include <memory>

#include "Program.h"
#include "Constants.h"

class FBOManager
{
	struct DebugData {
		DebugData() : enabled(true), write(false), fog(true) {}
		bool enabled;
		bool write; // debug
		bool fog;
	};

	struct FBOData {
		FBOData() : chaos(false), water(true), 
			blurAmount(0), confuseTimer(0), shakeTimer(0) {}
		bool chaos, water;
		float blurAmount, confuseTimer, shakeTimer;
	};

	FBOManager();

	void initFBOs();
	/**** geometry set up for a quad *****/
	void initQuad();
	/* Helper function to create the framebuffer object and associated texture to write to */
	void createFBO(GLuint fb, GLuint tex);
	void createDepthFBO(GLuint fb, GLuint tex);

	public:
		static constexpr int MAIN_BUFFER = 0;
		static constexpr int BLUR_BUFFER = 1;
		static constexpr int FOG_BUFFER = 2;
		static constexpr int DEPTH_BUFFER = 3;
		static constexpr int SHADOW_BUFFER = 4;
		static constexpr int NUM_BUFFERS = 5;

		static constexpr float CONFUSE_TIME = 5; // seconds
		static constexpr float SHAKE_TIME = 0.5; // seconds


		static FBOManager& getInstance();

		~FBOManager() {}

		void bindScreen() const;
		void bindBuffer(int bufferIndex);
		void processFog();
		void processBlur();
		void drawBuffer();

		void update(float deltaTime, float gameTime);

		void increaseBlurAmount(float delta) { data.blurAmount += delta; }
		void toggleEnabled() { debug.enabled = !debug.enabled; }
		void writeNextTexture() { debug.write = true; }
		void toggleFog() { debug.fog = !debug.fog; }

		void toggleChaos() { data.chaos = !data.chaos; }
		void triggerConfuse() { data.confuseTimer = CONFUSE_TIME; }
		void triggerShake() { data.shakeTimer = SHAKE_TIME; }
		void toggleWater() { data.water = !data.water; }

		bool isEnabled() const { return debug.enabled; }

		GLuint getTexBufId(int index) { return texBuf[index]; }

	private:
		GLuint frameBuf[NUM_BUFFERS];
		GLuint texBuf[NUM_BUFFERS];
		//geometry for texture render
		GLuint quadVertexArrayID;
		GLuint quadVertexBuffer;

		FBOData data;
		DebugData debug;
		int activeBuffer;

		/* Process Texture on the specificed texture  - could vary what it does based on
			shader  - works on inTex - runs shaders and output to textured quad */
		void processDrawTex(int program, GLuint tex);
		void drawTex(GLuint tex);

		/* code to write out the FBO (texture) - helpful for debugging*/
		void writeTexture(const std::string filename, GLuint tex);
};
