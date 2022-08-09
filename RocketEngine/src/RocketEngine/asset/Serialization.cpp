#include "Serialization.h"
#include <fstream>
#include <RocketEngine/Defines.h>
#include "scene/SceneManager.h"
#include <RKTUtils/Profiling.h>
#include <RocketEngine/core/Log.h>
#include <cereal/archives/json.hpp>

namespace RKTEngine
{
	void Serialization::serializeScene(Scene* scene)
	{
		RKT_PROFILE_FUNCTION();
		RKT_CORE_TRACE("Serializing scene '{0}'", scene->name);

		{
			std::ofstream out("assets/scenes/" + scene->name + ".scene", std::ios::out);
			cereal::JSONOutputArchive ar(out);
			ar(MAKE_NVP("Scene Data", *scene));
		}
	}


	Scene* Serialization::deserializeScene(const std::string& sceneName)
	{
		RKT_PROFILE_FUNCTION();
		RKT_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		Scene* newScene = new Scene{sceneName, std::vector<GameObject*>()};
		auto sceneToDeserialize = *newScene;
		{
			std::ifstream in("assets/scenes/" + sceneName + ".scene", std::ios::in);
			cereal::JSONInputArchive iar(in);
			iar(MAKE_NVP("Scene Data", sceneToDeserialize));
		}

		*newScene = sceneToDeserialize;
		return newScene;
	}

	unsigned char* Serialization::loadByteData(const std::string& path)
	{
		uint32 bufferSize = 1 << 20;
		unsigned char* buffer = new unsigned char[bufferSize];
		std::basic_ifstream<unsigned char> infile(path, std::ios::in | std::ifstream::binary);

		//get length of file
		infile.seekg(0, std::ios::end);
		auto length = infile.tellg();
		infile.seekg(0, std::ios::beg);

		// don't overflow the buffer
		if (length > bufferSize)
		{
			length = bufferSize;
		}

		//read file
		infile.read(buffer, length);

		return buffer;
	}
}