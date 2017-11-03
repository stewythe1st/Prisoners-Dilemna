/******************************************************************************
*	main.cpp
*
*	Main function for the config class for the Iterated Prisoner's Dilemna Problem
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/


/**********************************************************
*	Headers
**********************************************************/
#include "config.h"
#include "game.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>


/**********************************************************
*	Compiler Constants
**********************************************************/
#define IO_FORMAT_FLOAT(x)	std::fixed << std::setprecision(x) << std::setfill('0')


/**********************************************************
*	Main Function
**********************************************************/
int main(int argc, char *argv[]) {

	// Variables
	config			cfg;
	game			g;
	agent*			temp;
	agent			local_best;
	agent			global_best;
	std::ofstream	log;
	std::ofstream	solution;

	// Get configuration
	if (argc > 1) {
		cfg.read(argv[1]);
	}
	
	// Seed random number generator
	if (cfg.seedType == SEED_TIME_BASED) {
		std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()); // Good lord, <chrono> types are awful aren't they??
		cfg.seed = (unsigned int)time.time_since_epoch().count();
	}
	srand(cfg.seed);

	// Open log file
	log.open("./log/default.txt");
	if (!log.is_open()) {
		std::cout << "Error: Unable to write log file" << std::endl;
		exit(1);
	}
	log << "Result Log" << std::endl;

	// Run rounds
	for (int run = 0; run < cfg.runs; run++) {

		log << std::endl << "Run: " << run << std::endl;
		std::cout << std::endl << "Run: " << run << std::endl;

		// Create random player and opponent
		temp = new agent(cfg.depth, cfg.memory);
		temp->randomize();
		g.set_player(temp);

		// Run iterations
		for (int iteration = 0; iteration < cfg.iterations; iteration++) {

			g.play_round_tit_for_tat();

			// Check for best fitness
			if ((g.get_player()->get_fitness() > local_best.get_fitness()) || iteration == 0) {
				local_best = *g.get_player();
				log << iteration + 1 << "\t" << IO_FORMAT_FLOAT(4) << local_best.get_fitness() << std::endl;
				std::cout << iteration + 1 << "\t" << IO_FORMAT_FLOAT(4) << local_best.get_fitness() << std::endl;
			}
		}

		// Check for global best
		if ((local_best.get_fitness() > global_best.get_fitness()) || run == 0) {
			global_best = local_best;
		}

		// Clean up
		delete g.get_player();
	}

	// Print global best to solution file
	solution.open(cfg.solutionfile);
	if (!solution.is_open()) {
		std::cout << "Error: Unable to write solution file" << std::endl;
		exit(1);
	}
	global_best.print(solution);

	return 0;
}
