//Built with the help of Cherno's open source Hazel engine. Under the Apache 2.0 License
//https://github.com/TheCherno/Hazel/blob/master/LICENSE

#ifndef NATIVE_SCRIPT_COMP_H
#define NATIVE_SCRIPT_COMP_H

#include "Component.h"

namespace RKTEngine
{
	class Actor;

	class NativeScriptComponent : public Component
	{
		public:
		
		/**********************************************************************//**
		* Creates component with the given id value.
		*
		* @param id ComponentId value assigned to this instance of the Component class
		*************************************************************************/
			NativeScriptComponent(const ComponentId& id) :
				Component(id) {};

			///Default deconstructor
			~NativeScriptComponent() { destroyScript(this); };

			Actor* pInstance = nullptr;

			Actor* (*instantiateScript)() = nullptr;
			void (*destroyScript)(NativeScriptComponent*) = nullptr;

			template<typename T>
			void bind(uint32 id)
			{		
				instantiateScript = []() { return static_cast<Actor*>(new T()); };
				destroyScript = [](NativeScriptComponent* nsc) { delete nsc->pInstance; nsc->pInstance = nullptr; };

				pInstance = instantiateScript();
				pInstance->gameObjectId = id;
				pInstance->onCreate();
			}

			template<typename T>
			T* get()
			{
				return static_cast<T*>(pInstance);
			}

		private:
	};

}
#endif // !NATIVE_SCRIPT_COMP_H
