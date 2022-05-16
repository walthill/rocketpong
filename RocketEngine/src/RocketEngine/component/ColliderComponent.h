#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

namespace RKTEngine
{
	struct ColliderData
	{
		std::string tag;

		ColliderData(const std::string& t = "") : tag(t) {};
	};

	class ColliderComponent : public Component
	{
		public:
			ColliderComponent(const ComponentId& id);
			virtual ~ColliderComponent();
			inline const std::string getTag() { return mData.tag; }
		private:
			ColliderData mData;
	};
}

#endif // !COLLIDER_H
