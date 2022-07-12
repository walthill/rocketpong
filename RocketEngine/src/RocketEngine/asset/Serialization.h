#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <string>
#include <yaml-cpp/yaml.h>

/*
	How to serialize non-UI Actors???
		- Will need to store unique member variables (speed, etc)
		- must register seperately from
*/

namespace RKTEngine
{
	struct Scene;
	class UILabel;
	class GameObject;
	class TextComponent;
	class TransformComponent;
	class SpriteComponent;

	class Serialization
	{
		public:
			static void serializeScene(Scene* scene);

			static Scene* deserializeScene(const std::string& sceneName);
			static unsigned char* loadByteData(const std::string& path);
		private:
			static void serializeGameObject(YAML::Emitter& out, GameObject* gameObject);
			static void serializeUILabel(YAML::Emitter& out, UILabel* uiLabel);
			static void serializeTransform(YAML::Emitter& out, TransformComponent* transform);
			static void serializeSprite(YAML::Emitter& out, SpriteComponent* sprite);
			static void serializeTextComponent(YAML::Emitter& out, TextComponent* transform);

			static void deserializeUILabels(YAML::Node& node, Scene* scene);
			static void deserializeGameObjects(YAML::Node& node, Scene* scene);
	};
}

#endif // !SERIALIZATION_H
