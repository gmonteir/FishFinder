#pragma  once

#include <memory>

#include "Program.h"
#include "Constants.h"

class FBOManager
{
	public:
		static FBOManager& getInstance();

		// Shader Manager Constants
		FBOManager();
		~FBOManager() {}
		void bindBuffer();
		void drawBuffer();

	private:
		GLuint frameBuf[2];
		GLuint texBuf[2];
		GLuint depthBuf;
		//geometry for texture render
		GLuint quad_VertexArrayID;
		GLuint quad_vertexbuffer;
		bool firstTime;
		
		void initFBOs();
		void initVAO();
		/**** geometry set up for a quad *****/
		void initQuad();

		/* Helper function to create the framebuffer object and associated texture to write to */
		void createFBO(GLuint& fb, GLuint& tex);

		/* Process Texture on the specificed texture  - could vary what it does based on
			shader  - works on inTex - runs shaders and output to textured quad */
		void processDrawTex();
};
