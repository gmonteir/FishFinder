#pragma  once

#include "Program.h"
#include "Texture.h"
#include "Constants.h"

#include <memory>

// Light Constants
struct light {
	glm::vec3 pos;
	float constant;
	float linear;
	float quadratic;
};

constexpr int NUM_LIGHTS = 3;
static light POINT_LIGHTS[NUM_LIGHTS] = {
	{ glm::vec3(0, 30, 0), 1.0, 0.007, 0.0002 },
	{ glm::vec3(100, 70, -100), 1.0, 0.007, 0.0002 },
	{ glm::vec3(100, 50, 150), 1.0, 0.007, 0.0002 }
};

// Shader variables
struct uniforms {
	glm::mat4 P;
	glm::mat4 V;
	glm::vec3 eye;
	glm::vec3 targetPos;
	float time;
};

class ShaderManager
{

	public:
		static std::shared_ptr<ShaderManager> getInstance();

		// Shader Manager
		ShaderManager();
		~ShaderManager() {}

		std::shared_ptr<Program> initSimpleProg();
	    std::shared_ptr<Program> initSkyboxProg();
	    std::shared_ptr<Program> initTextureProg();
	    std::shared_ptr<Program> initGlyphProg();
	    std::shared_ptr<Program> initFloorProg();
		std::shared_ptr<Program> initFBOProg();
		std::shared_ptr<Program> initBlurProg();

		std::shared_ptr<Program> getShader(int i) const { return shaderProgs[i]; }
	    void setData(const uniforms& common) { uniformData = common; }
		void sendUniforms(int i, const std::shared_ptr<Texture> texture=nullptr, 
			const std::shared_ptr<Texture> blendTexture=nullptr);

	private:
		std::shared_ptr<Program> shaderProgs[NUM_SHADERS];
		uniforms uniformData;

		void addLightUniforms(std::shared_ptr<Program>& prog);
		void sendLightUniforms(std::shared_ptr<Program>& prog);

		std::shared_ptr<Program> makeProgram(const std::string& vertex, const std::string& fragment);
};
