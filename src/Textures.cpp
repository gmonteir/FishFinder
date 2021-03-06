#include "Textures.h"
#include "Constants.h"

#include <iostream>

using namespace std;

Textures::Textures()
{
	addTexture(RESOURCE_DIR + "/player.jpg", PLAYER_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/clown1.jpg", CLOWN1_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/turtle.jpg", TURTLE_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/mother.jpg", MOTHER_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/father.jpg", FATHER_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/puffer.jpg", PUFFER_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/clown2.jpg", CLOWN2_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/royal.jpg", ROYAL_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/shark.jpg", SHARK_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/ground.bmp", FLOOR_TEXTURE, GL_CLAMP_TO_EDGE);
	addTexture(RESOURCE_DIR + "/coral.jpg", CORAL_TEXTURE, GL_CLAMP_TO_EDGE);
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