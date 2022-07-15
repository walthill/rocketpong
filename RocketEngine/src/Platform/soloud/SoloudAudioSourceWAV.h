#ifndef SOLOUD_AUDIO_SRC
#define SOLOUD_AUDIO_SRC

#include <soloud_wav.h>
#include <RocketEngine/audio/AudioSource.h>

namespace RKTEngine
{
	class SoloudAudioSourceWAV : public AudioSource
	{
		public:
			SoloudAudioSourceWAV(const std::string& path, AudioFileType type);
			virtual ~SoloudAudioSourceWAV();

			virtual void load(const std::string& path) override;

			inline SoLoud::Wav* getWAV() { return &mSource; }

		private:
			SoLoud::Wav	mSource;

	};
}

#endif // !SOLOUD_AUDIO_SRC
