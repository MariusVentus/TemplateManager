#include "RandHandler.h"

RandHandler::RandHandler()
	:
	rng(rd())
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
