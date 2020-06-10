#pragma  once

#include "Program.h"
#include "Texture.h"
#include "Constants.h"

#include <memory>

// Shader variables
struct uniforms {
	glm::mat4 P;
	glm::mat4 V;
	glm::vec3 eye;
	glm::vec3 targetPos;
	float time;
	int remaining; // characters
};

class ShaderManager
{
	struct LightUniforms {
		glm::mat4 LP;
		glm::mat4 LV;
	};

	public:

		static constexpr int NUM_LIGHTS = 3;
		static struct Light POINT_LIGHTS[NUM_LIGHTS];

		static std::shared_ptr<ShaderManager> getInstance();

		// Shader Manager
		ShaderManager();
		~ShaderManager() {}

		std::shared_ptr<Program> initSimpleProg();
	    std::shared_ptr<Program> initSkyboxProg();
	    std::shared_ptr<Program> initTextureProg();
	    std::shared_ptr<Program> initGlyphProg();
	    std::shared_ptr<Program> initFloorProg();
		std::shared_ptr<Program> initReflectProg();
		std::shared_ptr<Program> initDepthProg();
		std::shared_ptr<Program> initFogFBOProg();
		std::shared_ptr<Program> initBlurProg();
		std::shared_ptr<Program> initWaterFBOProg();
		std::shared_ptr<Program> initParticleProg();
		std::shared_ptr<Program> initLightDepthProg();
		std::shared_ptr<Program> initCharParticleProg();
		std::shared_ptr<Program> initStaminaProg();
		std::shared_ptr<Program> initCoralProg();

		std::shared_ptr<Program> getShader(int i) const { return shaderProgs[i]; }
	    void setData(const uniforms& common) { uniformData = common; }
		void sendUniforms(int progIndex, const std::shared_ptr<Texture> texture=nullptr,
			const std::shared_ptr<Texture> blendTexture=nullptr);

		uniforms uniformData;
		LightUniforms luData;
	private:
		std::shared_ptr<Program> shaderProgs[NUM_SHADERS];

		void addLightUniforms(std::shared_ptr<Program>& prog);
		void sendLightUniforms(std::shared_ptr<Program>& prog);

		std::shared_ptr<Program> makeProgram(const std::string& vertex, const std::string& fragment);
};
