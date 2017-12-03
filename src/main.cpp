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
*	Functions
**********************************************************/
#ifdef _WIN32
extern "C" int __stdcall IsDebuggerPresent(void);
#endif


/**********************************************************
*	Main Function
**********************************************************/
int main(int argc, char *argv[]) {

	// Force Visual Studio to pause on debug exit
	#ifdef _WIN32
	if (IsDebuggerPresent()) {
		atexit([] {system("PAUSE"); });
	}
	#endif

	// Variables
	config			cfg;
	agent*			best;
	agent*			abs_best;
	float			average;
	float			abs_average;
	agent*			parent1 = nullptr;
	agent*			parent2 = nullptr;
	agent			global_best;
	pool			population;
	pool			offspring;
	int				eval;
	bool			terminate;
	std::ofstream	log;
	std::ofstream	csv;
	std::ofstream	csv2;
	std::ofstream	solution;

	// Get configuration
	if (argc > 1) {
		if (!cfg.read(argv[1])) {
			exit(1);
		}
	}

	// Check for incalid configuration
	if (cfg.rounds < cfg.memory * 3) {
		std::cout << "Error: Configuration constraint violated (l >= 3k)" << std::endl;
		exit(1);
	}
	if (cfg.survivalStrat == SURVIVALSTRAT_COMMA && cfg.mu > cfg.lambda ) {
		std::cout << "Error: For comma (generational) survival, mu cannot be greater than lambda" << std::endl;
		exit(1);
	}
	
	// Seed random number generator
	if (cfg.seedType == SEED_TIME_BASED) {
		std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()); // Good lord, <chrono> types are awful aren't they??
		cfg.seed = (unsigned int)time.time_since_epoch().count();
	}
	srand(cfg.seed);

	// Open log file
	log.open(cfg.logfile);
	if (!log.is_open()) {
		std::cout << "Error: Unable to write log file" << std::endl;
		exit(1);
	}
	cfg.print(log);
	csv.open(cfg.logfile.substr(0, cfg.logfile.size() - 4) + ".csv");
	csv2.open(cfg.logfile.substr(0, cfg.logfile.size()-4) + "_best.csv");
	if (!csv.is_open() || !csv2.is_open()) {
		std::cout << "Error: Unable to write csv log file(s)" << std::endl;
		exit(1);
	}
	csv << "Run,Evals,Average Composite Fitness,Best Composite Fitness,Average Absolute Fitness, Best Absolute Fitness" << std::endl;
	csv2 << "Run,Population Member,Composite Fitness, Absolute Fitness" << std::endl;

	// Runs
	for (int run = 0; run < cfg.runs; run++) {
		log << std::endl << "Run " << run + 1 << std::endl;
		std::cout << std::endl << "Run " << run + 1 << std::endl;

		agent local_best;

		// Generate a random start population
		// Use a 50% chance to generate leaf nodes before max depth
		// (ramped half-and-half - 50/50 grow/full)
		for (int i = 0; i < cfg.mu; i++) {
			agent temp(cfg.depth, cfg.memory);
			temp.randomize(rand() % 2);
			temp.play_rounds(cfg.rounds, cfg.rerandmem);
			temp.calc_fitness(cfg.parsimony);
			population.add(temp);
		}
				
		// Evals
		terminate = false;
		eval = cfg.mu;
		while (!terminate) {

			switch (cfg.parentSelection) {
			case PARENT_FP:
				population.calc_fp();
				break;
			case PARENT_OS:
				population.calc_os();
				break;
			}

			// Generate offspring
			for (int i = 0; i < cfg.lambda; i++) {
				switch (cfg.parentSelection) {
				case PARENT_FP:
					parent1 = population.choose_parent_fp();
					parent2 = population.choose_parent_fp();
					break;
				case PARENT_OS:
					parent1 = population.choose_parent_os();
					parent2 = population.choose_parent_os();
					break;
				}
				agent temp(parent1, parent2);
				if (GEN_RAND_DECIMAL <= cfg.mutation) {
					temp.mutate();
				}
				temp.play_rounds(cfg.rounds, cfg.rerandmem);
				temp.calc_fitness(cfg.parsimony);
				offspring.add(temp);
				eval++;
			}

			// If comma survival, remove parents from population first
			if (cfg.survivalStrat == SURVIVALSTRAT_COMMA) {
				population.empty();
			}

			// Add offspring to population
			population.copy_from(&offspring);
			offspring.clear();
			population.calc_comp_fitnesses(cfg.coevSamp);

			// Reduce population
			switch (cfg.survivalSelection) {
			case SURVIVAL_TRUNCATION:
				population.reduce_by_truncation(cfg.mu);
				break;
			case SURVIVAL_KTOURN:
				population.reduce_by_k_tourn(cfg.mu, cfg.survivalK);
				break;
			}			

			// Update local best
			best = population.get_comp_best();
			abs_best = population.get_best();
			average = population.get_comp_average();
			abs_average = population.get_average();
			if (best->get_comp_fitness() > local_best.get_comp_fitness()) {
				local_best = *best;
			}
			log << eval << "\t" << IO_FORMAT_FLOAT(3) << average << "\t" << IO_FORMAT_FLOAT(3) << local_best.get_comp_fitness() << std::endl;
			std::cout << eval << "\t" << IO_FORMAT_FLOAT(3) << average << "\t" << IO_FORMAT_FLOAT(4) << local_best.get_comp_fitness() << std::endl;
			csv << run + 1 << "," << eval << "," << IO_FORMAT_FLOAT(3) << average << "," << IO_FORMAT_FLOAT(3) << local_best.get_comp_fitness() << "," << IO_FORMAT_FLOAT(3) << abs_average << "," << IO_FORMAT_FLOAT(3) << abs_best->get_fitness() << std::endl;

			// Run termination test
			switch (cfg.termTest) {
			case TERMTEST_BEST_UNCHANGED:
				terminate = population.term_test_best_unchanged(cfg.unchanged);
				break;
			case TERMTEST_NUM_EVALS:
				terminate = (eval >= cfg.evals);
				break;
			}

		}

		// Output final absolute fitness
		std::cout << std::endl << "Absolute Fitness" << std::endl << local_best.get_fitness() << std::endl;
		log << std::endl << "Absolute Fitness" << std::endl << local_best.get_fitness() << std::endl;

		// Update global best
		if (local_best.get_comp_fitness() > global_best.get_comp_fitness()) {
			global_best = local_best;
		}


		// Log all final fitness values in population for box plot
		for (int i = 0; i < cfg.mu; i++) {
			csv2 << run + 1 << "," << i << "," << population.get(i)->get_comp_fitness() << "," << population.get(i)->get_fitness() << std::endl;
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
