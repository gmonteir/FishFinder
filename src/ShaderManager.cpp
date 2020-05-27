#include "ShaderManager.h"
#include <iostream>

using namespace std;
using namespace glm;

shared_ptr<ShaderManager> ShaderManager::getInstance()
{
	static shared_ptr<ShaderManager> instance(new ShaderManager);
	return instance;
}

ShaderManager::ShaderManager()
{
	shaderProgs[SIMPLEPROG] = initSimpleProg();
	shaderProgs[SKYBOXPROG] = initSkyboxProg();
	shaderProgs[TEXTUREPROG] = initTextureProg();
	shaderProgs[GLYPHPROG] = initGlyphProg();
	shaderProgs[FLOORPROG] = initFloorProg();
	shaderProgs[REFLECTPROG] = initReflectProg();
	shaderProgs[FBOPROG] = initFBOProg();
	shaderProgs[BLURPROG] = initBlurProg();
	shaderProgs[PARTICLEPROG] = initParticleProg();

	cout << "ShaderManager: Initialized" << endl;
}

shared_ptr<Program> ShaderManager::initSimpleProg()
{
	shared_ptr<Program> prog = makeProgram("/simple_vert.glsl", "/simple_frag.glsl");
	prog->addUniform("P");
	prog->addUniform("M");
	prog->addUniform("V");
	prog->addUniform("MatAmb");
	prog->addUniform("MatDif");
	prog->addUniform("MatSpec");
	prog->addUniform("shine");
	addLightUniforms(prog);
	prog->addUniform("eye");
	prog->addAttribute("vertPos");
	prog->addAttribute("vertNor");
	prog->addAttribute("vertTex");
	return prog;
}

shared_ptr<Program> ShaderManager::initSkyboxProg()
{
	shared_ptr<Program> cubeProg = makeProgram("/cube_vert.glsl", "/cube_frag.glsl");
	cubeProg->addUniform("P");
	cubeProg->addUniform("M");
	cubeProg->addUniform("V");
	cubeProg->addAttribute("vertPos");
	cubeProg->addAttribute("vertNor");
	return cubeProg;
}

shared_ptr<Program> ShaderManager::initTextureProg()
{
	shared_ptr<Program> texProg = makeProgram("/tex_vert.glsl", "/tex_frag.glsl");
	texProg->addUniform("P");
	texProg->addUniform("M");
	texProg->addUniform("V");
	addLightUniforms(texProg);
	texProg->addUniform("Texture0");
	texProg->addAttribute("vertPos");
	texProg->addAttribute("vertNor");
	texProg->addAttribute("vertTex");
	return texProg;
}

shared_ptr<Program> ShaderManager::initGlyphProg()
{
	shared_ptr<Program> glyphProg = makeProgram("/glyph_vert.glsl", "/glyph_frag.glsl");
	glyphProg->addUniform("P");
	glyphProg->addUniform("text");
	glyphProg->addUniform("textColor");
	glyphProg->addAttribute("vertex");
	return glyphProg;
}

shared_ptr<Program> ShaderManager::initFloorProg()
{
	shared_ptr<Program> texProg = makeProgram("/tex_vert.glsl", "/floor_frag.glsl");
	texProg->addUniform("P");
	texProg->addUniform("M");
	texProg->addUniform("V");
	addLightUniforms(texProg);
	texProg->addUniform("targetPos");
	texProg->addUniform("eye");
	texProg->addUniform("time");
	texProg->addUniform("remaining");
	texProg->addUniform("Texture0");
	texProg->addUniform("Texture1");
	texProg->addAttribute("vertPos");
	texProg->addAttribute("vertNor");
	texProg->addAttribute("vertTex");
	return texProg;
}

shared_ptr<Program> ShaderManager::initReflectProg()
{
	std::shared_ptr<Program> reflProg = make_shared<Program>();
	reflProg->setVerbose(true);
	reflProg->setShaderNames(
		RESOURCE_DIR + "/simple_vert.glsl",
		RESOURCE_DIR + "/refl_frag.glsl");
	if (! reflProg->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	reflProg->addUniform("P");
	reflProg->addUniform("M");
	reflProg->addUniform("V");
	reflProg->addUniform("eye");
	//addLightUniforms(reflProg);
	reflProg->addAttribute("vertPos");
	reflProg->addAttribute("vertNor");
	reflProg->addAttribute("vertTex");
	return reflProg;
}

shared_ptr<Program> ShaderManager::initFBOProg()
{
	std::shared_ptr<Program> texProg = makeProgram("/pass_vert.glsl", "/tex_fbo_frag.glsl");
	texProg->addUniform("texBuf");
	texProg->addUniform("fTime");
	texProg->addUniform("targetPos");
	texProg->addAttribute("vertPos");
	return texProg;
}

shared_ptr<Program> ShaderManager::initBlurProg()
{
	std::shared_ptr<Program> texProg = makeProgram("/pass_vert.glsl", "/blur_frag.glsl");
	texProg->addUniform("texBuf");
	texProg->addUniform("fTime");
	texProg->addAttribute("vertPos");
	return texProg;
}

shared_ptr<Program> ShaderManager::initParticleProg()
{
	std::shared_ptr<Program> prog = makeProgram("/particle_vert.glsl", "/particle_frag.glsl");
	prog->addUniform("P");
	prog->addUniform("V");
	prog->addUniform("M");
	prog->addUniform("time");
	prog->addAttribute("vertPos");
	return prog;
}

 void ShaderManager::sendUniforms(int i, const shared_ptr<Texture> texture, const std::shared_ptr<Texture> blendTexture)
 {
	 shared_ptr<Program> prog = getShader(i);
	 if (i == SIMPLEPROG)
	 {
		 glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		 glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		 sendLightUniforms(prog);
		 glUniform3f(prog->getUniform("eye"), uniformData.eye.x, uniformData.eye.y, uniformData.eye.z);
	 }
	 else if (i == TEXTUREPROG)
	 {
		 glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		 glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		 sendLightUniforms(prog);
		 // This probably should be updated in the future to work with different textures
		 texture->bind(prog->getUniform("Texture0"));
	 }
	 else if (i == SKYBOXPROG)
	 {
		 glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		 glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
	 }
	 else if (i == FLOORPROG)
	 {
	 	 glActiveTexture(GL_TEXTURE0);
	 	 glBindTexture(GL_TEXTURE_2D, texture->getID());
	 	 glUniform1i(prog->getUniform("Texture0"), 0);

	 	 glActiveTexture(GL_TEXTURE1);
	 	 glBindTexture(GL_TEXTURE_2D, blendTexture->getID());
	 	 glUniform1i(prog->getUniform("Texture1"), 1);

		 glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		 glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		 sendLightUniforms(prog);
		 glUniform3f(prog->getUniform("targetPos"), uniformData.targetPos.x, uniformData.targetPos.y, uniformData.targetPos.z);
		 glUniform3f(prog->getUniform("eye"), uniformData.eye.x, uniformData.eye.y, uniformData.eye.z);
		 glUniform1f(prog->getUniform("time"), uniformData.time);
		 glUniform1i(prog->getUniform("remaining"), uniformData.remaining);
		 //texture->bind(prog->getUniform("Texture0"));
		 //blendTexture->bind(prog->getUniform("Texture1"));
	 }
	 else if (i == REFLECTPROG)
	 {
	     glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		 glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		 //sendLightUniforms(prog);
		 glUniform3f(prog->getUniform("eye"), uniformData.eye.x, uniformData.eye.y, uniformData.eye.z);
	 }
	 else if (i == FBOPROG)
	 {
		 glUniform3f(prog->getUniform("targetPos"), uniformData.targetPos.x, uniformData.targetPos.y, uniformData.targetPos.z);
	 }
	 else if (i == PARTICLEPROG)
	 {
	 	 glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		 glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		 glUniform1f(prog->getUniform("time"), uniformData.time);
	 }

 }

 void ShaderManager::addLightUniforms(std::shared_ptr<Program>& prog)
 {
	 for (size_t i = 0; i < NUM_LIGHTS; i++)
	 {
		 string number = to_string(i);

		 prog->addUniform(("pointLights[" + number + "].position").c_str());
		 prog->addUniform(("pointLights[" + number + "].constant").c_str());
		 prog->addUniform(("pointLights[" + number + "].linear").c_str());
		 prog->addUniform(("pointLights[" + number + "].quadratic").c_str());
	 }
 }

 void ShaderManager::sendLightUniforms(std::shared_ptr<Program>& prog)
 {
	 for (size_t i = 0; i < NUM_LIGHTS; i++)
	 {
		 string number = to_string(i);

		 glUniform3f(prog->getUniform(("pointLights[" + number + "].position").c_str()), POINT_LIGHTS[i].pos.x, POINT_LIGHTS[i].pos.y, POINT_LIGHTS[i].pos.z);
		 glUniform1f(prog->getUniform(("pointLights[" + number + "].constant").c_str()), POINT_LIGHTS[i].constant);
		 glUniform1f(prog->getUniform(("pointLights[" + number + "].linear").c_str()), POINT_LIGHTS[i].linear);
		 glUniform1f(prog->getUniform(("pointLights[" + number + "].quadratic").c_str()), POINT_LIGHTS[i].quadratic);
	 }
 }

 shared_ptr<Program> ShaderManager::makeProgram(const string& vertex, const string& fragment)
 {
	 shared_ptr<Program> prog = make_shared<Program>();
	 prog->setVerbose(true);
	 prog->setShaderNames(RESOURCE_DIR + vertex, RESOURCE_DIR + fragment);
	 if (!prog->init())
	 {
		 std::cerr << "Shader failed to compile: " << vertex << ", " << fragment << std::endl;
		 exit(1);
	 }
	 return prog;
 }
