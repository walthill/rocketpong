#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "RocketEngine/render/RenderCommand.h"

namespace RKTEngine
{

	BoxColliderComponent::BoxColliderComponent(const ComponentId& id) :
		ColliderComponent(id)
	{
		mCollisionMap = std::map<ComponentId, CollisionData>();
	}

	BoxColliderComponent::~BoxColliderComponent()
	{
		cleanup();
	}

	void BoxColliderComponent::cleanup()
	{
		mpParentTransform = nullptr;
	}

	void BoxColliderComponent::renderOverlay()
	{
		if (!mBoxColliderData.isEnabled)
			return;

		auto scale = mpParentTransform->getScale();
		scale = glm::vec2(scale.x * getWidth(), scale.y * getHeight());

		if (mpParentTransform->getRotation().angle != 0)
		{
			RenderCommand::drawRotatedQuad(mpParentTransform->getPosition(), scale, mpParentTransform->getRotation().angle, Color::green);
		}
		else
		{
			RenderCommand::drawQuad({mpParentTransform->getPosition(), 0.0f}, scale, Color::green.getColorAlpha01());
		}
	}

	CollisionType BoxColliderComponent::checkCollision(BoxColliderComponent* collider)
	{
		if (!mBoxColliderData.isEnabled || mpParentTransform == nullptr)
			return CollisionType::NONE;

		auto posA = mpParentTransform->getPosition();
		auto posB = collider->mpParentTransform->getPosition();
		bool collided = false;

		//AABB check that uses center bounding box coordinates by ZorbaTHut
		//https://gamedev.stackexchange.com/questions/586/what-is-the-fastest-way-to-work-out-2d-bounding-box-intersection 
		if((abs(posA.x - posB.x) * 2 < (getWidth() + collider->getWidth())) &&
			(abs(posA.y - posB.y) * 2 < (getHeight() + collider->getHeight())))
		{
			collided = true;
		}

		auto otherColliderId = collider->getId();
		auto& collisionData = mCollisionMap.find(otherColliderId);
		if (collisionData == mCollisionMap.end() && collided)
		{
			mCollisionMap[otherColliderId] = { false, collided }; //lastFrameCollided, isColliding
		}
		else if (collisionData != mCollisionMap.end())
		{
			mCollisionMap[otherColliderId] = { mCollisionMap[otherColliderId].isColliding, collided };
		}

		return getCollisionType(mCollisionMap[otherColliderId]);
	}

	void BoxColliderComponent::setData(const BoxColliderData& data)
	{
		mBoxColliderData.width = data.width;
		mBoxColliderData.height = data.height;
		mBoxColliderData.tag = data.tag;
		mBoxColliderData.isEnabled = data.isEnabled;
	}

	CollisionType BoxColliderComponent::getCollisionType(const CollisionData& data)
	{
		CollisionType type = CollisionType::NONE;
		if (data.isColliding && !data.lastFrameCollided)
		{
			type = CollisionType::ENTER;
		}
		else if (data.isColliding && data.lastFrameCollided)
		{
			type = CollisionType::STAY;
		}
		else if (!data.isColliding && data.lastFrameCollided)
		{
			type = CollisionType::EXIT;
		}

		return type;
	}

	void BoxColliderComponent::setTransformParent(TransformComponent* tr)
	{
		mpParentTransform = tr;
	}
}