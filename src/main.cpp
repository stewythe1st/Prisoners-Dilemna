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
int main() {

	// Variables
	config cfg;
	game g;
	agent* temp;
	agent local_best, global_best;
	std::ofstream log;

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

	// Open log file
	log.open("./log/default.txt");
	if (!log.is_open()) {
		std::cout << "Error: Unable to write log file" << std::endl;
		exit(1);
	}
	log << "Result Log" << std::endl;

	// Create random player and opponent
	temp = new agent(cfg.depth, cfg.memory);
	temp->randomize();
	g.set_player(temp);
	temp = new agent(cfg.depth, cfg.memory);
	temp->randomize();
	g.set_opponent(temp);

	// Run for 100 rounds
	for (int i = 0; i < 1000; i++) {

		g.play_round();		

		// Check for best fitness
		if (g.get_player()->get_fitness() > g.get_opponent()->get_fitness())
			local_best = *g.get_player();
		else
			local_best = *g.get_opponent();
		if (local_best.get_fitness() > global_best.get_fitness() || i == 0) {
			global_best = local_best;
			log << i + 1 << "\t" << IO_FORMAT_FLOAT(2) << global_best.get_fitness() << std::endl;
			std::cout << i + 1 << "\t" << IO_FORMAT_FLOAT(2) << global_best.get_fitness() << std::endl;
		}

		// Play tit-for-tat (i.e. current player becomes next opponent, then generate a new random player)
		delete g.get_opponent();
		g.set_opponent(g.get_player());
		temp = new agent(cfg.depth, cfg.memory);
		temp->randomize();
		g.set_player(temp);		
	}

	// Clean up
	delete g.get_player();
	delete g.get_opponent();

	return 0;
}
