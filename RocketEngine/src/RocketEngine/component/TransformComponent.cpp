#include "TransformComponent.h"

namespace RKTEngine
{

	TransformComponent::TransformComponent(const ComponentId& id) :
		Component(id)
	{
	}

	TransformComponent::~TransformComponent()
	{
	}

	void TransformComponent::setPosition(glm::vec2 pos)
	{
		mTransformData.mPosition = pos;
		mDataChanged = true;
	}

	void TransformComponent::setData(const TransformData& data)
	{
		mTransformData.mPosition = data.mPosition;
		mTransformData.mRotation.angle = data.mRotation.angle;
		mTransformData.mRotation.rotationAxis = data.mRotation.rotationAxis;
		mTransformData.mScale = data.mScale;
		mDataChanged = true;
	}

	void TransformComponent::setScale(glm::vec2 mScale)
	{
		mTransformData.mScale = mScale;
		mDataChanged = true;
	}

	void TransformComponent::setRotation(glm::vec3 rotationAxis, float angle)
	{
		mTransformData.mRotation.angle = angle;
		mTransformData.mRotation.rotationAxis = rotationAxis;
		mDataChanged = true;
	}
}