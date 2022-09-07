#include <string>

#define MAKE_NVP(x,y) ::cereal::make_nvp(x,y)

namespace RKTEngine
{
	//Help from https://github.com/TheCherno/Hazel
	//A game engine and tutorial series developed under the Apache-2.0 license
	#define RKT_PROFILE 0
	#define RKT_RENDER_STATS 0
	#define BITSHIFT(x) (1 << x)
	#define RKT_BIND_MESSAGE_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
	#define RKT_BIND_CALLBACK_FN(funcRef) std::bind(&funcRef, this)
	#define OVERRIDE_REQUIRED =0

	typedef unsigned int uint32;
	typedef uint32 ComponentId;
	typedef uint32 GameObjectId;
	typedef std::string ShaderKey;
}