#include "RandHandler.h"
#include <array>

RandHandler::RandHandler()
	:
	rng(PrimeGenerator())
{
}

unsigned RandHandler::Generate(unsigned base, unsigned range)
{
	if (range > 0) {
		std::uniform_int_distribution<unsigned> Dist(0, range);
		return base + Dist(rng);
	}
	else {
		return base;
	}
}

std::mt19937 RandHandler::PrimeGenerator(void)
{
	std::random_device rd;
	std::array<std::random_device::result_type, std::mt19937::state_size> seed_data;
	std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

	return std::mt19937(seq);
}