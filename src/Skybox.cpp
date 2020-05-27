#include "Skybox.h"
#include "ShaderManager.h"
#include "Shapes.h"
#include "stb_image.h"

#include <iostream>

using namespace std;
using namespace glm;


const vector<std::string> Skybox::faces{
	"uw_rt.jpg",
	"uw_lf.jpg",
	"uw_up.jpg",
	"uw_dn.jpg",
	"uw_ft.jpg",
	"uw_bk.jpg"
};

Skybox::Skybox()
{
	cubeMapTexture = createSky(RESOURCE_DIR + "/underwater/", faces);
	cout << "Skybox loaded" << endl;
}

Skybox& Skybox::getInstance()
{
	static Skybox instance;
	return instance;
}

void Skybox::draw(shared_ptr<MatrixStack>& M, vec3 position) const
{
	//draw the sky box
	shared_ptr<Program> prog = ShaderManager::getInstance()->getShader(SKYBOXPROG);
	prog->bind();
	ShaderManager::getInstance()->sendUniforms(SKYBOXPROG);
	draw(prog, M, position);
	prog->unbind();
}

void Skybox::draw(shared_ptr<Program>& prog, shared_ptr<MatrixStack>& M, vec3 position) const
{
	glDepthFunc(GL_LEQUAL);
	M->pushMatrix();
	M->loadIdentity();
	M->translate(vec3(0, 5, 0));
	M->translate(position);
	M->rotate(radians(-180.f), YAXIS);
	M->scale(vec3(500));
	glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	Shapes::getInstance()->getShape(CUBE_SHAPE)->at(0)->draw(prog);
	glDepthFunc(GL_LESS);
	M->popMatrix();
}

unsigned int Skybox::createSky(const string& dir, const vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false);
	for (GLuint i = 0; i < faces.size(); i++) {
		unsigned char* data =
			stbi_load((dir + faces[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "failed to load: " << (dir + faces[i]).c_str() << std::endl;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	cout << " creating cube map any errors : " << glGetError() << endl;
	return textureID;
}