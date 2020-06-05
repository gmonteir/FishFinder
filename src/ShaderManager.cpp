#include "ShaderManager.h"
#include "FBOManager.h"
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
	shaderProgs[DEPTHPROG] = initDepthProg();
	shaderProgs[FOGFBOPROG] = initFogFBOProg();
	shaderProgs[BLURFBOPROG] = initBlurProg();
	shaderProgs[WATERFBOPROG] = initWaterFBOProg();
	shaderProgs[PARTICLEPROG] = initParticleProg();
	shaderProgs[LIGHTDEPTHPROG] = initLightDepthProg();
	shaderProgs[CHARPARTICLEPROG] = initCharParticleProg();
	shaderProgs[CORALPROG] = initCoralProg();

	//luData.LP = glm::ortho(-ORTHO_SIZE, ORTHO_SIZE, -ORTHO_SIZE, ORTHO_SIZE, 1.0f, 1000.0f);
	luData.LP = glm::ortho(-ORTHO_SIZE, ORTHO_SIZE, -ORTHO_SIZE, ORTHO_SIZE, NEAR_PLANE, 2 * ORTHO_SIZE);
	luData.LV = glm::lookAt(POINT_LIGHTS[0].pos, vec3(0), vec3(0, 1, 0));

	cout << "ShaderManager: Initialized" << endl;
}

shared_ptr<Program> ShaderManager::initSimpleProg()
{
	shared_ptr<Program> prog = makeProgram("/simple_vert.glsl", "/simple_frag.glsl");
	prog->addUniform("P");
	prog->addUniform("M");
	prog->addUniform("V");
	prog->addUniform("LP");
	prog->addUniform("LV");
	prog->addUniform("time");
	prog->addUniform("MatAmb");
	prog->addUniform("MatDif");
	prog->addUniform("MatSpec");
	prog->addUniform("shine");
	addLightUniforms(prog);
	prog->addUniform("eye");
	prog->addUniform("shadowDepth");
	prog->addAttribute("vertPos");
	prog->addAttribute("vertNor");
	prog->addAttribute("vertTex");
	return prog;
}

shared_ptr<Program> ShaderManager::initCoralProg()
{
	shared_ptr<Program> prog = makeProgram("/simple_vert.glsl", "/coral_frag.glsl");
	prog->addUniform("P");
	prog->addUniform("M");
	prog->addUniform("V");
	prog->addUniform("LP");
	prog->addUniform("LV");
	prog->addUniform("time");
	prog->addUniform("MatAmb");
	prog->addUniform("MatDif");
	prog->addUniform("MatSpec");
	prog->addUniform("shine");
	addLightUniforms(prog);
	prog->addUniform("eye");
	prog->addUniform("Texture0");
	prog->addUniform("shadowDepth");
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
	texProg->addUniform("LP");
	texProg->addUniform("LV");
	addLightUniforms(texProg);
	texProg->addUniform("shadowDepth");
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
	texProg->addUniform("LP");
	texProg->addUniform("LV");
	addLightUniforms(texProg);
	texProg->addUniform("targetPos");
	texProg->addUniform("eye");
	texProg->addUniform("time");
	texProg->addUniform("remaining");
	texProg->addUniform("shadowDepth");
	texProg->addUniform("Texture0");
	texProg->addUniform("Texture1");
	texProg->addAttribute("vertPos");
	texProg->addAttribute("vertNor");
	texProg->addAttribute("vertTex");
	return texProg;
}

shared_ptr<Program> ShaderManager::initReflectProg()
{
	std::shared_ptr<Program> reflProg = makeProgram("/simple_vert.glsl", "/refl_frag.glsl");
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

shared_ptr<Program> ShaderManager::initDepthProg()
{
	std::shared_ptr<Program> texProg = makeProgram("/tex_vert.glsl", "/depth_frag.glsl");
	texProg->addUniform("P");
	texProg->addUniform("M");
	texProg->addUniform("V");
	texProg->addUniform("eye");
	texProg->addAttribute("vertPos");
	texProg->addAttribute("vertNor");
	texProg->addAttribute("vertTex");
	return texProg;
}

shared_ptr<Program> ShaderManager::initFogFBOProg()
{
	std::shared_ptr<Program> texProg = makeProgram("/pass_vert.glsl", "/fog_fbo_frag.glsl");
	texProg->addUniform("texBuf");
	texProg->addUniform("depthBuf");
	texProg->addAttribute("vertPos");
	return texProg;
}

shared_ptr<Program> ShaderManager::initBlurProg()
{
	std::shared_ptr<Program> texProg = makeProgram("/pass_vert.glsl", "/blur_frag.glsl");
	texProg->addUniform("texBuf");
	texProg->addUniform("time");
	texProg->addAttribute("vertPos");
	return texProg;
}

shared_ptr<Program> ShaderManager::initWaterFBOProg()
{
	std::shared_ptr<Program> texProg = makeProgram("/pass_vert.glsl", "/water_fbo_frag.glsl");
	texProg->addUniform("chaos");
	texProg->addUniform("confuse");
	texProg->addUniform("shake");
	texProg->addUniform("water");

	texProg->addUniform("texBuf");
	texProg->addUniform("time");
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

shared_ptr<Program> ShaderManager::initLightDepthProg()
{
	std::shared_ptr<Program> prog = makeProgram("/depth_vert.glsl", "/depth_frag.glsl");
	prog->addUniform("LP");
	prog->addUniform("LV");
	prog->addUniform("M");
	prog->addAttribute("vertPos");
	prog->addAttribute("vertNor");
	prog->addAttribute("vertTex");
	return prog;
}

shared_ptr<Program> ShaderManager::initCharParticleProg()
{
	std::shared_ptr<Program> prog = makeProgram("/char_particle_vert.glsl", "/char_particle_frag.glsl");
	prog->addUniform("P");
	prog->addUniform("V");
	prog->addUniform("M");
	prog->addUniform("time");
	prog->addAttribute("vertPos");
	return prog;
}

void ShaderManager::sendUniforms(int progIndex, const shared_ptr<Texture> texture, const std::shared_ptr<Texture> blendTexture)
{
	shared_ptr<Program> prog = getShader(progIndex);
	switch (progIndex)
	{
	case SIMPLEPROG:
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		glUniformMatrix4fv(prog->getUniform("LP"), 1, GL_FALSE, value_ptr(luData.LP));
		glUniformMatrix4fv(prog->getUniform("LV"), 1, GL_FALSE, value_ptr(luData.LV));
		glUniform1f(prog->getUniform("time"), uniformData.time);
		sendLightUniforms(prog);
		glUniform3f(prog->getUniform("eye"), uniformData.eye.x, uniformData.eye.y, uniformData.eye.z);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBOManager::getInstance().getTexBufId(int(FBOManager::SHADOW_BUFFER)));
		glUniform1i(prog->getUniform("shadowDepth"), 0);
		break;
	case TEXTUREPROG:
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		glUniformMatrix4fv(prog->getUniform("LP"), 1, GL_FALSE, value_ptr(luData.LP));
		glUniformMatrix4fv(prog->getUniform("LV"), 1, GL_FALSE, value_ptr(luData.LV));
		sendLightUniforms(prog);
		// This probably should be updated in the future to work with different textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBOManager::getInstance().getTexBufId(int(FBOManager::SHADOW_BUFFER)));
		glUniform1i(prog->getUniform("shadowDepth"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture->getID());
		glUniform1i(prog->getUniform("Texture0"), 1);
		break;
	case SKYBOXPROG:
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		break;
	case FLOORPROG:
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBOManager::getInstance().getTexBufId(int(FBOManager::SHADOW_BUFFER)));
		glUniform1i(prog->getUniform("shadowDepth"), 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture->getID());
		glUniform1i(prog->getUniform("Texture0"), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, blendTexture->getID());
		glUniform1i(prog->getUniform("Texture1"), 2);

		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		glUniformMatrix4fv(prog->getUniform("LP"), 1, GL_FALSE, value_ptr(luData.LP));
		glUniformMatrix4fv(prog->getUniform("LV"), 1, GL_FALSE, value_ptr(luData.LV));
		sendLightUniforms(prog);
		glUniform3f(prog->getUniform("targetPos"), uniformData.targetPos.x, uniformData.targetPos.y, uniformData.targetPos.z);
		glUniform3f(prog->getUniform("eye"), uniformData.eye.x, uniformData.eye.y, uniformData.eye.z);
		glUniform1f(prog->getUniform("time"), uniformData.time);
		glUniform1i(prog->getUniform("remaining"), uniformData.remaining);
		break;
	case REFLECTPROG:
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		//sendLightUniforms(prog);
		glUniform3f(prog->getUniform("eye"), uniformData.eye.x, uniformData.eye.y, uniformData.eye.z);
		break;
	case DEPTHPROG:
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		break;
	case WATERFBOPROG:
		glUniform1f(prog->getUniform("time"), uniformData.time);
	case FOGFBOPROG:
	case BLURFBOPROG:
		glUniform1i(prog->getUniform("texBuf"), 0);
		break;
	case PARTICLEPROG:
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		glUniform1f(prog->getUniform("time"), uniformData.time);
		break;
	case LIGHTDEPTHPROG:
		glUniformMatrix4fv(prog->getUniform("LP"), 1, GL_FALSE, value_ptr(luData.LP));
		glUniformMatrix4fv(prog->getUniform("LV"), 1, GL_FALSE, value_ptr(luData.LV));
		break;
	case CHARPARTICLEPROG:
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		glUniform1f(prog->getUniform("time"), uniformData.time);
		break;
	case CORALPROG:
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(uniformData.P));
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(uniformData.V));
		glUniformMatrix4fv(prog->getUniform("LP"), 1, GL_FALSE, value_ptr(luData.LP));
		glUniformMatrix4fv(prog->getUniform("LV"), 1, GL_FALSE, value_ptr(luData.LV));
		glUniform1f(prog->getUniform("time"), uniformData.time);
		sendLightUniforms(prog);
		glUniform3f(prog->getUniform("eye"), uniformData.eye.x, uniformData.eye.y, uniformData.eye.z);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FBOManager::getInstance().getTexBufId(int(FBOManager::SHADOW_BUFFER)));
		glUniform1i(prog->getUniform("shadowDepth"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture->getID());
		glUniform1i(prog->getUniform("Texture0"), 1);
		break;
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
