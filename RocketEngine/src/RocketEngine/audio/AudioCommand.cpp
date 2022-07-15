#include "AudioCommand.h"
#include <Platform/soloud/SoloudAudioEngine.h>

namespace RKTEngine
{
	AudioEngine* AudioCommand::spAudioAPI = new SoloudAudioEngine;
}