#include "Floor.h"
#include "Constants.h"

using namespace glm;

Floor::Floor()
{
	int w, h, ncomps;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load((RESOURCE_DIR + "/terrain.bmp").c_str(), &w, &h, &ncomps, 0);
	unsigned bytePerPixel = ncomps;

	for (size_t i = 0; i < w; i++)
	{
		for (size_t j = 0; j < h; j++)
		{
			unsigned char* pixelOffset = data + (i + h * j) * bytePerPixel;
			terrain[i][j] = pixelOffset[0];
		}
	}
	
	std::vector<unsigned int> eleBuf;
	std::vector<float> posBuf;
	std::vector<float> texBuf;

	for (size_t z = 0; z < MAP_Z; z++)
	{
		for (size_t x = 0; x < MAP_X; x++)
		{
			posBuf.push_back(x);
			posBuf.push_back(terrain[x][z]);
			posBuf.push_back(z);

			texBuf.push_back(((float)x) / MAP_X);
			texBuf.push_back(((float)z) / MAP_Z);
		}
	}

	for (size_t z = 0; z < MAP_Z - 1; z++)
	{
		for (size_t x = 0; x < MAP_X - 1; x++)
		{
			unsigned int i1 = x + MAP_X * z;
			unsigned int i2 = i1 + 1;
			unsigned int i3 = i1 + MAP_X;
			unsigned int i4 = i3 + 1;

			eleBuf.push_back(i1);
			eleBuf.push_back(i2);
			eleBuf.push_back(i3);

			eleBuf.push_back(i2);
			eleBuf.push_back(i3);
			eleBuf.push_back(i4);
		}
	}

	Shapes::getInstance()->addShape(posBuf, texBuf, eleBuf, FLOOR_SHAPE);
}

shared_ptr<Floor> Floor::getInstance()
{
	static shared_ptr<Floor> instance(new Floor);
	return instance;
}

void Floor::draw(shared_ptr<MatrixStack>& M) const
{
	shared_ptr<Program> prog = ShaderManager::getInstance()->getShader(TEXTUREPROG);
	prog->bind();
	ShaderManager::getInstance()->sendUniforms(TEXTUREPROG, Textures::getInstance()->getTexture(FLOOR_TEXTURE));
	M->pushMatrix();
		M->translate(FLOOR_POSITION);
		M->scale(FLOOR_SIZE);
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
		Shapes::getInstance()->getShape(FLOOR_SHAPE)->at(0)->draw(prog);
	M->popMatrix();
	prog->unbind();
}

float Floor::getHeight(float x, float z) const
{
	int idX = (((floor(x) / MAP_X) + 1) / FLOOR_SIZE.x) * MAP_X;
	int idZ = (((floor(z) / MAP_Z) + 1) / FLOOR_SIZE.z) * MAP_Z;

	return terrain[idX][idZ] + FLOOR_POSITION.y;
}

bool Floor::isAboveFloor(const vec3 min, const vec3 max) const
{
	return min.y >= getHeight(min.x, min.z) && min.y >= getHeight(max.x, max.z);
}