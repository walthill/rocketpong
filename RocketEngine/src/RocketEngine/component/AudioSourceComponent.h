#ifndef AUDIO_SRC_COMP_H
#define AUDIO_SRC_COMP_H

#include "Component.h"

namespace RKTEngine
{

	class AudioSource;

	/***************************************************************************//**
	 * @brief 	Data used in every audio source component.
	 *
	 * This struct contains data and constructors that will serve as foundational data
	 * for every audio source
	 ******************************************************************************/
	struct AudioSourceComponentData
	{
		std::string audioFileName;
		AudioSource* audioSource;

		///Default constructor sets all values to zero
		AudioSourceComponentData() : audioFileName(""), audioSource(nullptr) {};
		AudioSourceComponentData(const std::string& audioFile) : audioFileName(audioFile), audioSource(nullptr) {};
	};

	const AudioSourceComponentData ZERO_AUDIO_SRC_DATA;

	/***************************************************************************//**
	 * @brief 	The component for Audio Sources.
	 *
	 * This class, derived from Component, contains data needed to play sounds.
	 ******************************************************************************/
	class AudioSourceComponent : public Component
	{
	public:

		/**********************************************************************//**
		* Creates component with the given id value.
		*
		* @param id ComponentId value assigned to this instance of the Component class
		*************************************************************************/
		AudioSourceComponent(const ComponentId& id);

		///Default deconstructor
		~AudioSourceComponent();

		///Load audio from file
		void load();
		///Cleanup
		void cleanup();

		void play(float vol = -1.0f);

		///Access the Audio Source component data
		AudioSourceComponentData* getData() { return &mAudioSourceData; }
	
		void setData(const AudioSourceComponentData& data);

	private:
		AudioSourceComponentData mAudioSourceData;
	};
}
#endif // !MESH_COMP_H
