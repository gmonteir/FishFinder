#include "AudioManager.h"

using namespace std;

AudioManager::AudioManager()
{
	engine = createIrrKlangDevice();
}

AudioManager& AudioManager::getInstance()
{
	static AudioManager instance;
	return instance;
}

void AudioManager::startMusicLoop()
{
	mainLoop = engine->play2D((RESOURCE_DIR + "/" + MUSIC_LOOP).c_str(), true);
}