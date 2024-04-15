#include "Random.h"

namespace Core
{
	std::mt19937 Random::_random;
	std::uniform_int_distribution<std::mt19937::result_type> Random::_distribution;
}