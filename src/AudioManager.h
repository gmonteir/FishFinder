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

	void playSoundEffect(int effect);
	void stopSoundEffect(int effect);
	void reset();
	
private:
	ISoundEngine* engine;
	ISoundSource* mainLoop;
	ISoundSource* bump;
	ISoundSource* found;
	ISoundSource* win;
	ISoundSource* lose;
	ISoundSource* boost;

	ISound* mainLoopS;
	ISound* bumpS;
	ISound* foundS;
	ISound* winS;
	ISound* loseS;
	ISound* boostS;
};
