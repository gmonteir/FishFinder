#pragma  once

#include <memory>

#include "Program.h"
#include "Constants.h"

class FBOManager
{
	FBOManager();

	public:
		static FBOManager& getInstance();

		~FBOManager() {}

		void bindBuffer();
		void blur();
		void drawBuffer();

		void update(float deltaTime, float gameTime);

		void increaseBlurAmount(float delta) { blurAmount += delta; }
		void toggleEnabled() { enabled = !enabled; }

		/* code to write out the FBO (texture) - helpful for debugging*/
		void writeTexture(const std::string filename);

	private:
		GLuint frameBuf[2];
		GLuint texBuf[2];
		//geometry for texture render
		GLuint quadVertexArrayID;
		GLuint quadVertexBuffer;

		bool enabled;
		float blurAmount;
		
		void initFBOs();
		/**** geometry set up for a quad *****/
		void initQuad();

		/* Helper function to create the framebuffer object and associated texture to write to */
		void createFBO(GLuint fb, GLuint tex);

		/* Process Texture on the specificed texture  - could vary what it does based on
			shader  - works on inTex - runs shaders and output to textured quad */
		void processDrawTex(GLuint tex, int program);
};
