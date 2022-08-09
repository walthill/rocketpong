#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <string>

namespace RKTEngine
{
	struct Scene;

	class Serialization
	{
		public:
			static void serializeScene(Scene* scene);

			static Scene* deserializeScene(const std::string& sceneName);
			static unsigned char* loadByteData(const std::string& path);
		private:
	};
}

#endif // !SERIALIZATION_H
