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
			break;
		case COLLISION_MUSIC:
			bumpS = engine->play2D(bump, false, false, true);
			break;
		case FOUND_MUSIC:
			foundS = engine->play2D(found, false, false, true);
			break;
		case WIN_MUSIC:
			winS = engine->play2D(win, false, false, true);
			break;
		case LOSE_MUSIC:
			loseS = engine->play2D(lose, false, false, true);
			break;
		case BOOST_MUSIC:
			boostS = engine->play2D(boost, true, false, true);
			break;
	}
}

void AudioManager::stopSoundEffect(int effect)
{
	switch (effect)
	{
	case MAIN_MUSIC:
		if (mainLoopS != nullptr)
			mainLoopS->stop();
		break;
	case COLLISION_MUSIC:
		if (bumpS != nullptr)
			bumpS->stop();
		break;
	case FOUND_MUSIC:
		if (foundS != nullptr)
			foundS->stop();
		break;
	case WIN_MUSIC:
		if (winS != nullptr)
			winS->stop();
		break;
	case LOSE_MUSIC:
		if (loseS != nullptr)
			loseS->stop();
		break;
	case BOOST_MUSIC:
		if (boostS != nullptr)
			boostS->stop();
		break;
	}
}

void AudioManager::reset()
{
	if (mainLoopS != nullptr)
		mainLoopS->stop();

	if (bumpS != nullptr)
		bumpS->stop();

	if (foundS != nullptr)
		foundS->stop();

	if (winS != nullptr)
		winS->stop();

	if (loseS != nullptr)
		loseS->stop();

	if (boostS != nullptr)
		boostS->stop();

	AudioManager::getInstance().playSoundEffect(MAIN_MUSIC);
}