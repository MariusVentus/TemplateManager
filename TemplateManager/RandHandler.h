#pragma once
#include <random>

class RandHandler {
public:
	RandHandler();
	unsigned Generate(unsigned base, unsigned range);
	std::mt19937 PrimeGenerator(void);

private:
	//Vars
	std::mt19937 rng;
};