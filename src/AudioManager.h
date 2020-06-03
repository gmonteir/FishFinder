#pragma once

#include "Constants.h"
#include "irrKlang.h"

using namespace irrklang;

class AudioManager
{
	AudioManager();

public:
	static AudioManager& getInstance();

	~AudioManager() {}

	void startMusicLoop();
	void playSoundEffect(int effect);
	void reset();
	
private:
	ISoundEngine* engine;
	ISound* mainLoop;
	
};
