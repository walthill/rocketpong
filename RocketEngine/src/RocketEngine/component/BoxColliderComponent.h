#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "ColliderComponent.h"
#include <map>

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

	struct CollisionData
	{
		bool lastFrameCollided = false;
		bool isColliding = false;
	};

	enum class CollisionType { NONE = -1, ENTER = 0, EXIT, STAY};

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
			
			void setData(const BoxColliderData& data);

			///Access the component data
			inline BoxColliderData getData() { return mBoxColliderData; }

			virtual inline const std::string& getTag() override { return mBoxColliderData.tag; };

			inline int getWidth() { return mBoxColliderData.width; }
			inline int getHeight() { return mBoxColliderData.height; }

		private:
			TransformComponent* mpParentTransform = nullptr;
			BoxColliderData mBoxColliderData;
			
			//Collection of collision events used to allow multiple collisions per-obj
			std::map<ComponentId, CollisionData> mCollisionMap;

			bool mLastFrameCollided = false;
			bool mIsColliding = false;

			CollisionType checkCollision(BoxColliderComponent* collider);
			CollisionType getCollisionType(const CollisionData& otherCollider);
			void setTransformParent(TransformComponent* tr);
	};
}

#endif // !COLLIDER_H
