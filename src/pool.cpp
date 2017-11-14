/******************************************************************************
*	pool.cpp
*
*	Definitions for the pool class for the Iterated Prisoner's Dilemna Problem
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/


/**********************************************************
*	Headers
**********************************************************/
#include "game.h"
#include "pool.h"
#include <cstdlib>


/**********************************************************
*	pool::choose_parent_fp()
*	Returns a pointer to a member agent chosen by fitness
*	proportional selection.
*	 @return pointer to the chosen agent
**********************************************************/
agent* pool::choose_parent_fp() {

	// Variables
	float	total = 0.0f;
	float	temp = 0.0f;
	agent*	rtn = &agents.front();

	// Calculate total
	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); it++) {
		total += (*it).get_fitness();
	}

	// Choose a random decimal (0.000 - 1.000)
	float rand = (float)(std::rand() % 1001) / 1000.0f;

	// Keep summing up fitness proportional until we get to our random number
	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); it++) {
		temp += ((*it).get_fitness() / total);
		if (temp > rand) {
			rtn = &(*it);
			break;
		}
	}

	return rtn;
}


/**********************************************************
*	pool::void pool::reduce_by_truncation()
*	Repeatedly removes an agent from the pool until it has
*	been shrunk to the passed size.
*	 @param int	desired size of the pool
**********************************************************/
void pool::reduce_by_truncation(int size) {

	// Variables
	std::vector<agent>::iterator lowest;

	// Repeat until desired size is reached
	while (agents.size() > size) {
		lowest = agents.begin();

		// Find element with lowest fitness and remove it
		for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); it++) {
			if ((*it).get_fitness() < (*lowest).get_fitness()) {
				lowest = it;
			}
		}
		agents.erase(lowest);
	}

	return;
}

void pool::copy_from(pool* p) {
	for (std::vector<agent>::iterator it = p->agents.begin(); it != p->agents.end(); it++) {
		agents.push_back(*it);
	}
	return;
}


agent* pool::get_best() {

	// Variables
	std::vector<agent>::iterator highest = agents.begin();

	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); it++) {
		if ((*it).get_fitness() > (*highest).get_fitness()) {
			highest = it;
		}
	}

	return &(*highest);
}

float pool::get_average() {

	// Variables
	float	total = 0.0f;
	
	// Calculate total
	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); it++) {
		total += (*it).get_fitness();
	}

	return total / (float)agents.size();
}
