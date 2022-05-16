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

		mLastFrameCollided = mIsColliding;
		mIsColliding = collided;
		
		return mIsColliding;
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