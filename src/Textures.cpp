#include "Textures.h"
#include "Constants.h"

Textures::Textures()
{
	addTexture(RESOURCE_DIR + "/dory.jpg", DORY_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/nemo.jpg", NEMO_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/squirt.jpg", SQUIRT_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/ground.bmp", FLOOR_TEXTURE, GL_CLAMP_TO_EDGE);
}

shared_ptr<Textures> Textures::getInstance()
{
	static shared_ptr<Textures> instance(new Textures);
	return instance;
}

void Textures::addTexture(const string& filename, const string& key, int mode)
{
	shared_ptr<Texture> temp = make_shared<Texture>();
	temp->setFilename(filename);
	temp->init();
	temp->setUnit(currentUnit);
	temp->setWrapModes(mode, mode);
	allTextures[key] = temp;
	currentUnit++;
}

shared_ptr<Texture> Textures::getTexture(const string& key)
{
	return allTextures[key];
}