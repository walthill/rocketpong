#include "BoxColliderComponent.h"
#include "TransformComponent.h"

namespace RKTEngine
{

	BoxColliderComponent::BoxColliderComponent(const ComponentId& id) :
		ColliderComponent(id)
	{
	}

	BoxColliderComponent::~BoxColliderComponent()
	{
		cleanup();
	}

	void BoxColliderComponent::cleanup()
	{
		mpParentTransform = nullptr;
	}

	bool BoxColliderComponent::checkCollision(BoxColliderComponent* collider)
	{
		//calculate sides of a
		float mLeftA = mpParentTransform->getPosition().x;
		float mRightA = mLeftA + getWidth();
		float mTopA = mpParentTransform->getPosition().y;
		float mBottomA = mTopA + getHeight();

		//calculate sides of collider
		float mLeftB = collider->mpParentTransform->getPosition().x;
		float mRightB = mLeftB + collider->getWidth();
		float mTopB = collider->mpParentTransform->getPosition().y;
		float mBottomB = mTopB + collider->getHeight();

		bool collided = true;
		if (mBottomA <= mTopB)
		{
			collided = false;
		}
		else if (mTopA >= mBottomB)
		{
			collided = false;
		}
		else if (mRightA <= mLeftB)
		{
			collided = false;
		}
		else if (mLeftA >= mRightB)
		{
			collided = false;
		}

		return collided;
	}

	void BoxColliderComponent::setData(const BoxColliderData& data)
	{
		mBoxColliderData.width = data.width;
		mBoxColliderData.height = data.height;
		mBoxColliderData.tag = data.tag;
	}

	void BoxColliderComponent::setTransformParent(TransformComponent* tr)
	{
		mpParentTransform = tr;
	}
}