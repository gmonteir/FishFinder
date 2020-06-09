#include "AudioManager.h"

using namespace std;

AudioManager::AudioManager()
{
	engine = createIrrKlangDevice();

	mainLoop = engine->addSoundSourceFromFile((RESOURCE_DIR + "/" + MUSIC_LOOP).c_str());
	bump = engine->addSoundSourceFromFile((RESOURCE_DIR + "/" + COLLISION_EFFECT).c_str());
	found = engine->addSoundSourceFromFile((RESOURCE_DIR + "/" + FOUND_EFFECT).c_str());
	win = engine->addSoundSourceFromFile((RESOURCE_DIR + "/" + WIN_EFFECT).c_str());
	lose = engine->addSoundSourceFromFile((RESOURCE_DIR + "/" + LOSE_EFFECT).c_str());
	boost = engine->addSoundSourceFromFile((RESOURCE_DIR + "/" + BOOST_EFFECT).c_str());
}

AudioManager& AudioManager::getInstance()
{
	static AudioManager instance;
	return instance;
}

void AudioManager::playSoundEffect(int effect)
{
	switch (effect)
	{
		case MAIN_MUSIC:
			mainLoopS = engine->play2D(mainLoop, true, false, true);
		case COLLISION_MUSIC:
			bumpS = engine->play2D(bump, false, false, true);
		case FOUND_MUSIC:
			foundS = engine->play2D(found, false, false, true);
		case WIN_MUSIC:
			winS = engine->play2D(win, false, false, true);
		case LOSE_MUSIC:
			loseS = engine->play2D(lose, false, false, true);
		case BOOST_MUSIC:
			boostS = engine->play2D(boost, false, false, true);
	}
}