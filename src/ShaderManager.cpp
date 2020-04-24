
#include "ShaderManager.h"

ShaderManager::ShaderManager(const string& resourceDirectory) :
	resourceDirectory(resourceDirectory)
{
	shaderProgs[SIMPLEPROG] = initSimpleProg();
	shaderProgs[SKYBOXPROG] = initSkyboxProg();
	shaderProgs[TEXTUREPROG] = initTextureProg();
	shaderProgs[GLYPHPROG] = initGlyphProg();
}

shared_ptr<Program> ShaderManager::initSimpleProg()
{
	std::shared_ptr<Program> prog = make_shared<Program>();
	prog->setVerbose(true);
	prog->setShaderNames(
		resourceDirectory + "/simple_vert.glsl",
		resourceDirectory + "/simple_frag.glsl");
	if (! prog->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	prog->addUniform("P");
	prog->addUniform("M");
	prog->addUniform("V");
	prog->addUniform("MatAmb");
	prog->addUniform("MatDif");
	prog->addUniform("MatSpec");
	prog->addUniform("shine");
	prog->addUniform("lightDir");
	prog->addUniform("lightCol");
	prog->addUniform("eye");
	prog->addAttribute("vertPos");
	prog->addAttribute("vertNor");
	prog->addAttribute("vertTex");
	return prog;
}

shared_ptr<Program> ShaderManager::initSkyboxProg()
{
	std::shared_ptr<Program> cubeProg = make_shared<Program>();
	cubeProg->setVerbose(true);
	cubeProg->setShaderNames(
		resourceDirectory + "/cube_vert.glsl",
		resourceDirectory + "/cube_frag.glsl");
	if (! cubeProg->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
		cubeProg->addUniform("P");
	cubeProg->addUniform("M");
	cubeProg->addUniform("V");
	cubeProg->addAttribute("vertPos");
	cubeProg->addAttribute("vertNor");
	return cubeProg;
}

shared_ptr<Program> ShaderManager::initTextureProg()
{
	std::shared_ptr<Program> texProg = make_shared<Program>();
	texProg->setVerbose(true);
	texProg->setShaderNames(
		resourceDirectory + "/tex_vert.glsl",
		resourceDirectory + "/tex_frag.glsl");
	if (! texProg->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	texProg->addUniform("P");
	texProg->addUniform("M");
	texProg->addUniform("V");
	texProg->addUniform("lightDir");
	texProg->addUniform("Texture0");
	texProg->addAttribute("vertPos");
	texProg->addAttribute("vertNor");
	texProg->addAttribute("vertTex");
	return texProg;
}

shared_ptr<Program> ShaderManager::initGlyphProg()
{
	std::shared_ptr<Program> glyphProg = make_shared<Program>();
	glyphProg->setVerbose(true);
	glyphProg->setShaderNames(
		resourceDirectory + "/glyph_vert.glsl",
		resourceDirectory + "/glyph_frag.glsl");
	if (! glyphProg->init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	glyphProg->addUniform("P");
	glyphProg->addUniform("text");
	glyphProg->addUniform("textColor");
	glyphProg->addAttribute("vertex");
	return glyphProg;
}