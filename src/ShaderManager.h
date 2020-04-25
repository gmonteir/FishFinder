#pragma  once

#include <iostream>
#include <memory>

#include "Program.h"


using namespace std;

#define SIMPLEPROG 0
#define SKYBOXPROG 1
#define TEXTUREPROG 2
#define GLYPHPROG 3

class ShaderManager
{

	public:
		static constexpr int NUM_SHADERS = 4;

		static shared_ptr<ShaderManager> getInstance();

		// Shader Manager Constants
		ShaderManager(const string& resourceDirectory);
		~ShaderManager() {}

		shared_ptr<Program> initSimpleProg();
	    shared_ptr<Program> initSkyboxProg();
	    shared_ptr<Program> initTextureProg();
	    shared_ptr<Program> initGlyphProg();
	    shared_ptr<Program> getShader(int i) { return shaderProgs[i]; }
	private:
		string resourceDirectory;
		shared_ptr<Program> shaderProgs[NUM_SHADERS];
};
