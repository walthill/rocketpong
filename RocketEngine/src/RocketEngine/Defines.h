
namespace RKTEngine
{
	//Help from https://github.com/TheCherno/Hazel
	//A game engine and tutorial series developed under the Apache-2.0 license
	#define BITSHIFT(x) (1 << x)
	#define RKT_BIND_MESSAGE_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

	#define OVERRIDE_REQUIRED =0

	typedef unsigned int uint32;
}