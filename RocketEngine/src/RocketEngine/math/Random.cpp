#include "Random.h"

namespace RKTEngine
{
	std::mt19937 Random::mt_randomizer = std::mt19937(std::random_device()());
}