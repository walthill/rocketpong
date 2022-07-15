#include "AudioManager.h"
#include <RocketEngine/audio/AudioCommand.h>

namespace RKTEngine
{
	AudioManager::AudioManager()
	{
		AudioCommand::initialize();

		//AudioSource* src = AudioSource::create("assets/audio/winwin.wav");
		//AudioCommand::play(src);
	}

	AudioManager::~AudioManager()
	{
		AudioCommand::cleanup();
	}
}