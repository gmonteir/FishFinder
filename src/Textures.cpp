#include "Textures.h"
#include "Constants.h"

#include <iostream>

using namespace std;

Textures::Textures()
{
	addTexture(RESOURCE_DIR + "/dory.jpg", DORY_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/nemo.jpg", NEMO_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/squirt.jpg", SQUIRT_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/jenny.jpg", JENNY_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/charlie.jpg", CHARLIE_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/bloat.jpg", BLOAT_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/marlin.jpg", MARLIN_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/gurgle.jpg", GURGLE_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/shark.jpg", SHARK_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/ground.bmp", FLOOR_TEXTURE, GL_CLAMP_TO_EDGE);
	initCausticsText();

	cout << "Textures: Initialized" << endl;
}

void Textures::initCausticsText()
{
	string title;
	for (int i = 0; i < NUM_CAUSTICS; ++i)
	{
		title = "/caustics/caustic" + to_string(i) + ".jpg";
		addTexture(RESOURCE_DIR + title.c_str(), CAUSTIC_TEXTURE + to_string(i), GL_REPEAT);
	}
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