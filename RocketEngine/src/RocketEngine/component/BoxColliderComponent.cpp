#include "BoxColliderComponent.h"
#include "TransformComponent.h"
#include "RocketEngine/render/RenderCommand.h"

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

	void BoxColliderComponent::renderOverlay()
	{
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