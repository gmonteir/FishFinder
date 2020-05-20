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
		void drawBuffer();

		void toggle() { shouldProcess = !shouldProcess; }

	private:
		GLuint frameBuf[2];
		GLuint texBuf[2];
		//geometry for texture render
		GLuint quadVertexArrayID;
		GLuint quadVertexBuffer;

		bool firstTime;
		bool shouldProcess;
		
		void initFBOs();
		/**** geometry set up for a quad *****/
		void initQuad();

		/* Helper function to create the framebuffer object and associated texture to write to */
		void createFBO(GLuint fb, GLuint tex);

		/* Process Texture on the specificed texture  - could vary what it does based on
			shader  - works on inTex - runs shaders and output to textured quad */
		void processDrawTex(GLuint tex, int program);
};
