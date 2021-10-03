#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <time.h>

namespace RKTEngine
{
	class Random
	{
	public:
		/// <summary>
		/// Generates a random integer value between min(inclusive) and max(inclusive)
		/// </summary>
		template<typename T>
		static T range(T min, T max)
		{
			auto randomNumberGen = std::uniform_int_distribution<T>(min, max);
			return (T)randomNumberGen(mt_randomizer);
		}

		/// <summary>
		/// Generates a random floating point value between min(inclusive) and max(inclusive)
		/// </summary>
		template<typename T>
		static T range_real(T min, T max)
		{
			auto randomNumberGen = std::uniform_real_distribution<T>(min, max);
			return (T)randomNumberGen(mt_randomizer);
		}
	private:
		static std::mt19937 mt_randomizer;
	};
}

#endif // !RANDOM_H
