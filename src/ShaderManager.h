#pragma  once

#include <iostream>
#include <memory>

#include "Program.h"
#include "Texture.h"
#include "Constants.h"

using namespace std;
using namespace glm;

// Light Constants
struct light {
	vec3 pos;
	float constant;
	float linear;
	float quadratic;
};

constexpr int NUM_LIGHTS = 3;
static light POINT_LIGHTS[NUM_LIGHTS] = {
	{ vec3(0, 30, 0), 1.0, 0.007, 0.0002 },
	{ vec3(100, 70, -100), 1.0, 0.007, 0.0002 },
	{ vec3(100, 50, 150), 1.0, 0.007, 0.0002 }
};

// Shader Constants
struct uniforms {
	mat4 P;
	mat4 V;
	vec3 eye;
	vec3 targetPos;
};

class ShaderManager
{

	public:
		static shared_ptr<ShaderManager> getInstance();

		// Shader Manager Constants
		ShaderManager(const string& resourceDirectory);
		~ShaderManager() {}

		shared_ptr<Program> initSimpleProg();
	    shared_ptr<Program> initSkyboxProg();
	    shared_ptr<Program> initTextureProg();
	    shared_ptr<Program> initGlyphProg();
	    shared_ptr<Program> initFloorProg();
		shared_ptr<Program> initFBOProg();

	    shared_ptr<Program> getShader(int i) const { return shaderProgs[i]; }
	    void setData(shared_ptr<uniforms> common) { uniformData = common; }
		void sendUniforms(int i, const std::shared_ptr<Texture> texture, const std::shared_ptr<Texture> blendTexture=NULL);

	private:
		string resourceDirectory;
		shared_ptr<Program> shaderProgs[NUM_SHADERS];
		shared_ptr<uniforms> uniformData;
		void addLightUniforms(std::shared_ptr<Program>& prog);
		void sendLightUniforms(std::shared_ptr<Program>& prog);

		std::shared_ptr<Program> makeProgram(const std::string& vertex, const std::string& fragment);
};
