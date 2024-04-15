#pragma once

#include <random>

namespace Core
{

	class Random
	{
	public:
		static void Init()
		{
			_random.seed(std::random_device()());
		}

		static float Float()
		{
			return static_cast<float>(_distribution(_random)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
		}

	private:
		static std::mt19937 _random;
		static std::uniform_int_distribution<std::mt19937::result_type> _distribution;
	};

}