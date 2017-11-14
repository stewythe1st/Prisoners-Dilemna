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
#include "pool.h"
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
	agent*			best;
	agent*			parent1;
	agent*			parent2;
	agent			global_best;
	pool			population;
	pool			offspring;
	std::ofstream	log;
	std::ofstream	solution;

	// Get configuration
	if (argc > 1) {
		if (!cfg.read(argv[1])) {
			exit(1);
		}
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
	log << "\tAgent memory k = " << cfg.memory << std::endl;
	log << "\tMax tree depth d = " << cfg.depth << std::endl;
	log << "\tSeed s = " << cfg.seed << std::endl;

	// Runs
	for (int run = 0; run < cfg.runs; run++) {
		log << std::endl << "Run " << run + 1 << std::endl;
		std::cout << std::endl << "Run " << run + 1 << std::endl;

		agent local_best;

		// Generate a random start population
		for (int i = 0; i < cfg.mu; i++) {
			agent temp(cfg.depth, cfg.memory);
			temp.randomize();
			temp.play_rounds(cfg.rounds);
			temp.calc_fitness();
			population.add(temp);
		}
				
		// Evals
		for (int eval = cfg.mu; eval < cfg.evals;) {

			// Generate offspring
			for (int i = 0; i < cfg.lambda; i++) {
				parent1 = population.choose_parent_fp();
				parent2 = population.choose_parent_fp();
				agent temp(parent1, parent2);
				temp.play_rounds(cfg.rounds);
				temp.calc_fitness();
				offspring.add(temp);
				eval++;
			}

			// Add offspring to population
			population.copy_from(&offspring);
			offspring.clear();

			// Reduce population
			population.reduce_by_truncation(cfg.mu);

			// Update local best
			best = population.get_best();
			if (best->get_fitness() > local_best.get_fitness()) {
				local_best = *best;
				log << eval << "\t" << IO_FORMAT_FLOAT(3) << local_best.get_fitness() << std::endl;
				std::cout << eval << "\t" << IO_FORMAT_FLOAT(3) << population.get_average() << "\t" << IO_FORMAT_FLOAT(4) << local_best.get_fitness() << std::endl;
			}

		}

		// Update global best
		if (local_best.get_fitness() > global_best.get_fitness()) {
			global_best = local_best;
		}

		population.clear();
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
