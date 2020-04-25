#include "Textures.h"


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