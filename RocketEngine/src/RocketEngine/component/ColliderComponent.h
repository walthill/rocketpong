#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

namespace RKTEngine
{
	struct ColliderData
	{
		//std::string tag;
		//
		//ColliderData(const std::string& t = "") : tag(t) {};
	};

	class ColliderComponent : public Component
	{
		public:
			ColliderComponent(const ComponentId& id);
			virtual ~ColliderComponent();
			virtual inline const std::string& getTag() OVERRIDE_REQUIRED;
			virtual inline void setTag(const std::string& tag) OVERRIDE_REQUIRED;
			virtual inline bool isEnabled() override { return true; }
			virtual inline void setEnabled(bool enabled) override { }

		private:
			ColliderData mData;
	};
}

#endif // !COLLIDER_H
