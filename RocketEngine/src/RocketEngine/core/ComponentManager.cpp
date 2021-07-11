#include "ComponentManager.h"
#include "RocketEngine/render/shader/ShaderManager.h"

namespace RKTEngine
{

	ComponentId ComponentManager::msNextMaterialComponentId = 0;
	ComponentId ComponentManager::msNextMeshComponentId = 0;
	ComponentId ComponentManager::msNextTransformComponentId = 0;

	ComponentManager::ComponentManager(uint32 maxSize, ShaderManager* shaderMan)
		: mTransformPool(maxSize, sizeof(TransformComponent))
		, mSpritePool(maxSize, sizeof(SpriteComponent))
		, mpShaderManagerHandle(shaderMan)
	{
	}

	ComponentManager::~ComponentManager()
	{
		clean();
	}

	void ComponentManager::clean()
	{
		//call destructor for all components
		for (auto& it : mTransformComponentMap)
		{
			TransformComponent* pComponent = it.second;
			pComponent->~TransformComponent();
		}
		for (auto& it : mMeshComponentMap)
		{
			SpriteComponent* pComponent = it.second;
			pComponent->~SpriteComponent();
		}
		
		//clear maps
		mTransformComponentMap.clear();
		mMeshComponentMap.clear();
		
		//reset memory pools
		mTransformPool.reset();
		mSpritePool.reset();
	}


	/******************************************************************************
	******************************************************************************

		TRANSFORM COMPONENT

	******************************************************************************
	*****************************************************************************/


	TransformComponent* ComponentManager::getTransformComponent(const ComponentId& id)
	{
		auto it = mTransformComponentMap.find(id);

		if (it != mTransformComponentMap.end())
			return it->second;
		else
			return nullptr;
	}

	ComponentId ComponentManager::allocateTransformComponent(const TransformData& data)
	{
		ComponentId newID = INVALID_COMPONENT_ID;

		RKTUtil::Byte* ptr = mTransformPool.allocateObject();
		if (ptr != nullptr)
		{
			newID = msNextTransformComponentId;
			TransformComponent* pComponent = ::new (ptr)TransformComponent(newID); //placement new https://stackoverflow.com/questions/19831298/operator-new-function-does-not-take-2-arguments
			pComponent->setData(data);
			mTransformComponentMap[newID] = pComponent;
			msNextTransformComponentId++;//increment id
		}

		return newID;
	}

	void ComponentManager::deallocateTransformComponent(const ComponentId& id)
	{
		auto it = mTransformComponentMap.find(id);

		if (it != mTransformComponentMap.end())//found it
		{
			TransformComponent* ptr = it->second;
			mTransformComponentMap.erase(it);

			ptr->~TransformComponent();
			mTransformPool.freeObject((RKTUtil::Byte*)ptr);
		}
	}

	/******************************************************************************
	******************************************************************************

		MESH COMPONENT

	******************************************************************************
	*****************************************************************************/


	SpriteComponent* ComponentManager::getMeshComponent(const ComponentId& id)
	{
		auto it = mMeshComponentMap.find(id);

		if (it != mMeshComponentMap.end())
			return it->second;
		else
			return nullptr;
	}

	//Load model and assign to gameobject
	ComponentId ComponentManager::allocateMeshComponent(const ComponentId& meshId, const SpriteComponentData& data)
	{
		ComponentId newID = INVALID_COMPONENT_ID;
		RKTUtil::Byte* ptr = mSpritePool.allocateObject();

		if (ptr != nullptr)
		{
			newID = msNextMeshComponentId;
			SpriteComponent* pComponent = ::new (ptr)SpriteComponent(newID);
			pComponent->setData(data);
			pComponent->load();			//load model mesh
			mMeshComponentMap[newID] = pComponent;
			msNextMeshComponentId++;//increment id
		}

		return newID;
	}

	void ComponentManager::deallocateMeshComponent(const ComponentId& id)
	{
		auto it = mMeshComponentMap.find(id);

		if (it != mMeshComponentMap.end())//found it
		{
			SpriteComponent* ptr = it->second;
			mMeshComponentMap.erase(it);

			ptr->~SpriteComponent();
			mSpritePool.freeObject((RKTUtil::Byte*)ptr);
		}
	}

	void ComponentManager::update(float elapsedTime)
	{
	}

	void ComponentManager::renderSprites()
	{
		SpriteComponent* tempComponent = nullptr;
		for (auto& it : mMeshComponentMap)
		{
			if (tempComponent != nullptr)
			{
				//Check if the mesh is instanced
				glm::mat4* pMatrices = tempComponent->getData()->instanceMatrices;
				if (pMatrices != nullptr)
				{
					//If the previous instanced mesh matches the current one, don't call render on the current mesh
					if (pMatrices == it.second->getData()->instanceMatrices)
					{
						continue;
					}
				}
			}

			tempComponent = it.second;
			it.second->render();
		}
	}
}