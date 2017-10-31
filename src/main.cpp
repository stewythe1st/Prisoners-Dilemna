#include "config.h"
#include "gptree.h"
#include <chrono>
#include <iostream>

int main() {

	// Variables
	config cfg;
	gp_tree test;

	// Seed random number generator
	switch (cfg.seedType) {
	case SEED_TIME_BASED: {
			std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()); // Good lord, <chrono> types are awful aren't they??
			srand((unsigned int)time.time_since_epoch().count());
		}
		break;
	case SEED_STATIC:
		srand(cfg.seed);
		break;
	}

	// Testing
	test.randomize(3);
	test.print(std::cout);

	return 0;
}