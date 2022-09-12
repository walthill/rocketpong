#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "ColliderComponent.h"
#include <map>

namespace RKTEngine
{
	class TransformComponent;

	struct BoxColliderData : ComponentData
	{
		int width;
		int height;
		std::string tag;

		BoxColliderData() : ComponentData(), width(0), height(0), tag("") {};
		BoxColliderData(int w, int h, const std::string& t = "", bool enabled = true) : ComponentData(enabled), width(w), height(h), tag(t) {};

		template<class Archive>
		void save(Archive& archive) const
		{
			if(this != nullptr)
				archive(CEREAL_NVP(isEnabled), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(tag));
		}

		template<class Archive>
		void load(Archive& archive)
		{
			try
			{
				archive(CEREAL_NVP(isEnabled), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(tag));
			}
			catch (cereal::Exception&)
			{
				archive.setNextName(nullptr);
				// Loading a key that doesn't exist results in an exception
				// Since "Not Found" is a valid condition for us, we swallow
				// this exception and the archive will not load anything
			}
		}
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
			inline BoxColliderData* getData() { return &mBoxColliderData; }

			virtual inline const std::string& getTag() override { return mBoxColliderData.tag; };
			virtual inline void setTag(const std::string& tag) override { mBoxColliderData.tag = tag; };

			inline int getWidth() { return mBoxColliderData.width; }
			inline int getHeight() { return mBoxColliderData.height; }
			inline TransformComponent* getTransform() { return mpParentTransform; }
			virtual inline bool isEnabled() override { return mBoxColliderData.isEnabled; }
			virtual inline void setEnabled(bool enabled) override { mBoxColliderData.isEnabled = enabled; }

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
