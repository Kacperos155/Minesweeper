#include "Minesweeper.hpp"
#include <random>

int randomNumberGenerator(int bottom_range, int upper_range, int seed)
{
	static std::mt19937 engine;
	if (seed == 0)
		engine.seed(std::random_device()() + seed);
	else
		engine.seed(seed);
	std::uniform_int_distribution<int> randomizing(bottom_range, upper_range);
	int random_number = randomizing(engine);
	return random_number;
}
