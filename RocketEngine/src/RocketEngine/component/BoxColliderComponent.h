#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "ColliderComponent.h"

namespace RKTEngine
{
	class TransformComponent;

	struct BoxColliderData : ColliderData
	{
		int width;
		int height;

		BoxColliderData() : ColliderData(""), width(0), height(0) {};
		BoxColliderData(int w, int h, const std::string& t = "") : ColliderData(t), width(w), height(h) {};
	};

	const BoxColliderData ZERO_BOX_COLLIDER_DATA;

	class BoxColliderComponent : public ColliderComponent
	{
		friend class GameObjectManager;
		friend class ComponentManager;

		public:
			BoxColliderComponent(const ComponentId& id);

			///Default deconstructor
			~BoxColliderComponent();
			
			void cleanup();

			void renderOverlay();
			bool checkCollision(BoxColliderComponent* collider);

			void setData(const BoxColliderData& data);

			///Access the component data
			inline BoxColliderData getData() { return mBoxColliderData; }

			virtual inline const std::string& getTag() override { return mBoxColliderData.tag; };

			inline int getWidth() { return mBoxColliderData.width; }
			inline int getHeight() { return mBoxColliderData.height; }

		private:
			TransformComponent* mpParentTransform = nullptr;
			BoxColliderData mBoxColliderData;
			
			bool mLastFrameCollided = false;
			bool mIsColliding = false;

			void setTransformParent(TransformComponent* tr);
	};
}

#endif // !COLLIDER_H
