#pragma once
#include <random>

class RandHandler {
public:
	RandHandler();
	unsigned Generate(unsigned base, unsigned range);


private:
	//Vars
	std::random_device rd;
	std::mt19937 rng;
};