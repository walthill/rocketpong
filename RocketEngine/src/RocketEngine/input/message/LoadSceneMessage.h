#ifndef LOAD_SCENE_MSG_H
#define LOAD_SCENE_MSG_H

#include "Message.h"
#include <sstream>

namespace RKTEngine
{
	class LoadSceneMessage : public Message
	{
	public:
		LoadSceneMessage(const std::string& sceneName, bool destroyScene = true, bool activateNewScene = true)
			: sceneToLoad(sceneName), destroyExistingScene(destroyScene), setNewSceneActive(activateNewScene), Message(MessageType::LOAD_SCENE) {};

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "LoadSceneMessage: Shutting down";
			return ss.str();
		};

		std::string sceneToLoad;
		bool destroyExistingScene;
		bool setNewSceneActive;

		EVENT_CLASS_TYPE(LOAD_SCENE)
		EVENT_CLASS_CATEGORY(MSG_CATEGORY_APP)
	};
}
#endif // !LOAD_SCENE_MSG_H
