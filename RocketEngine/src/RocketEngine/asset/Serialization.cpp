#include "Serialization.h"
#include <RocketEngine/Defines.h>
#include <fstream>
#include "scene/SceneManager.h"
#include <RocketEngine/actor/UILabel.h>
#include <RocketEngine/component/TextComponent.h>
#include <RocketEngine/actor/Actor.h>
#include <RocketEngine/core/Log.h>

//added from https://github.com/TheCherno/Hazel
//under Apache 2.0 License
namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<RKTEngine::Rotation>
	{
		static Node encode(const RKTEngine::Rotation& rhs)
		{
			Node node;
			node.push_back(rhs.angle);
			node.push_back(rhs.rotationAxis.x);
			node.push_back(rhs.rotationAxis.y);
			node.push_back(rhs.rotationAxis.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, RKTEngine::Rotation& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.angle = node[0].as<float>();
			rhs.rotationAxis = node[1].as<glm::vec3>();
			return true;
		}
	};

}

namespace RKTEngine
{

	//added from https://github.com/TheCherno/Hazel
	//under Apache 2.0 License
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	YAML::Emitter& operator<<(YAML::Emitter& out, const Rotation& rot)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << rot.angle << rot.rotationAxis << YAML::EndSeq;
		return out;
	}

	//TODO: add guid/uuid, object name

	void Serialization::serializeScene(Scene* scene)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << scene->name;
		out << YAML::Key << "UI" << YAML::Value << YAML::BeginSeq;

		for (size_t i = 0; i < scene->textUIs.size(); i++)
		{
			serializeUILabel(out, scene->textUIs[i]);
		}

		out << YAML::EndSeq;
		/*out << YAML::Key << "gameobjs" << YAML::Value << YAML::BeginSeq;

		for (size_t i = 0; i < scene->entities.size(); i++)
		{
		}

		out << YAML::EndSeq;*/
		out << YAML::EndMap;

		std::ofstream fout("assets/scenes/" + scene->name + ".scene");
		fout << out.c_str();
	}

	void Serialization::serializeUILabel(YAML::Emitter& out, UILabel* uiLabel)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Label" << YAML::Value << uiLabel->mpGameObject->name;

		serializeTransform(out, uiLabel->getTransform());
		serializeTextComponent(out, uiLabel->mpGameObject->getUILabel());

		out << YAML::EndMap;
	}

	void Serialization::serializeTransform(YAML::Emitter& out, TransformComponent* transform)
	{
		if (transform != nullptr)
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << transform->getPosition();
			out << YAML::Key << "Rotation" << YAML::Value << transform->getRotation();
			out << YAML::Key << "Scale" << YAML::Value << transform->getScale();

			out << YAML::EndMap; // TransformComponent
		}
	}

	void Serialization::serializeTextComponent(YAML::Emitter& out, TextComponent* text)
	{
		if (text != nullptr)
		{
			out << YAML::Key << "TextComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Text" << YAML::Value << text->getData().mText;
			out << YAML::Key << "Color" << YAML::Value << text->getData().mColor.getColorAlpha();
			out << YAML::Key << "FontName" << YAML::Value << text->getData().mFontName;
			out << YAML::Key << "FontSize" << YAML::Value << text->getData().mFontSize;
			out << YAML::EndMap; // TextComponent
		}
	}

	Scene* Serialization::deserializeScene(const std::string& sceneName)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile("assets/scenes/" + sceneName + ".scene");
		}
		catch (YAML::ParserException e)
		{
			RKT_CORE_ERROR("Failed to load .scene file '{0}'\n     {1}", "assets/scenes/" + sceneName + ".scene", e.what());
			return nullptr;
		}

		if (!data["Name"])
			return nullptr;

		RKT_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		Scene* loadedScene = new Scene();
		loadedScene->name = sceneName;

		auto uiElements = data["UI"];
		if (uiElements)
		{
			for (auto entity : uiElements)
			{
				std::string name = entity["Label"].as<std::string>();
				int fakeuuid = 123456789;
				RKT_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", fakeuuid, name);

				UILabel* deserializedUI = new UILabel(glm::vec2(0), "");
				deserializedUI->mpGameObject->name = name;

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto tr = deserializedUI->getTransform();
					tr->setPosition(transformComponent["Position"].as<glm::vec2>());
					tr->setRotation(transformComponent["Rotation"].as<Rotation>());
					tr->setScale(transformComponent["Scale"].as<glm::vec2>());
				}

				auto textComponent = entity["TextComponent"];
				if (textComponent)
				{
					deserializedUI->setText(textComponent["Text"].as<std::string>());
					deserializedUI->setColor(Color(textComponent["Color"].as<glm::vec4>()));
					deserializedUI->setFont(textComponent["FontName"].as<std::string>());
					deserializedUI->setFontSize(textComponent["FontSize"].as<int>());
				}

				loadedScene->textUIs.push_back(deserializedUI);
			}
		}

		return loadedScene;

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