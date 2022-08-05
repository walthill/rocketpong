#ifndef AUDIO_SRC_H
#define AUDIO_SRC_H

#include <RKTUtils/Trackable.h>

namespace RKTEngine
{
	class AudioSource : public RKTUtil::Trackable
	{
		public:
			enum class AudioFileType { NONE = 0, WAV = 1, MP3 = 2};

			AudioSource(AudioFileType type);
			virtual ~AudioSource() {};

			virtual void load(const std::string& path) =0;
			
			inline bool isInitialized() { return mInitialized; }
			inline AudioFileType getType() { return mFileType; }
			static AudioSource* create(const std::string& path, AudioFileType fileType = AudioFileType::WAV);
			
		protected:
			AudioFileType mFileType;
			bool mInitialized = false;
	};
}

#endif // !AUDIO_SRC_H
