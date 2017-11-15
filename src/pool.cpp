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
*	k_tourn(int k, int type)
*	Randomly picks k agents and returns the index of the
*	agent with either the highest or lowest fitness bassed
*	on which type was passed.
*	 @param k number of agents chosen in in the tournament
*	 @param highest true for highest, false for lowest
*	 @param replacement true if replacement is allowed
*	 @return index of the chosen state
**********************************************************/
int pool::k_tourn(int k, bool highest, bool replacement) {

	// Variables
	int*	tournament = new int[k];
	int		bestIdx;
	bool	inTournament;

	// Randomly pick members for the tournament, with or without replacement
	for (int i = 0; i < k; i++) {
		tournament[i] = rand() % agents.size();
		if (!replacement) {
			inTournament = false;
			for (int j = 0; j < i && !inTournament; j++) {
				if (tournament[j] == tournament[i]) {
					i--;
					break;
				}
			}
		}
	}

	// Run tournament, looking for best or worst fitness	
	bestIdx = tournament[0];
	for (int i = 1; i < k; i++) {
		if (!highest && agents[tournament[i]].get_fitness() > agents[bestIdx].get_fitness())
			bestIdx = tournament[i];
		else if (highest && agents[tournament[i]].get_fitness() < agents[bestIdx].get_fitness())
			bestIdx = tournament[i];
	}

	// Clean up
	delete[] tournament;
	return bestIdx;
}


/**********************************************************
*	reduce_by_k_tourn()
*	Reduces the pool to the passed size by running k-tournaments.
*	 @param size desired size of the pool after removal
*	 @param k number of states chosen in each tournament
**********************************************************/
void pool::reduce_by_k_tourn(size_t size, int k) {

	// Sanity check
	if (agents.size() < size)
		return;

	// Variables
	int idx;

	// Run tournaments until we've shrunk to desired size
	while (agents.size() > size) {

		// Since we're not allowing replacement, need to make sure k isn't bigger than size
		if (k > (int)agents.size())
			k = (int)agents.size();

		// Choose the lowest of k agents and remove it
		idx = k_tourn(k, true, false);
		agents.erase(agents.begin() + idx);
	}

	return;
}


/**********************************************************
*	pool::reduce_by_truncation()
*	Repeatedly removes an agent from the pool until it has
*	been shrunk to the passed size.
*	 @param int	desired size of the pool
**********************************************************/
void pool::reduce_by_truncation(int size) {

	// Variables
	std::vector<agent>::iterator lowest;

	// Repeat until desired size is reached
	while (agents.size() > (size_t)size) {
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


/**********************************************************
*	pool::term_test_best_unchanged()
*	Termination test. Checks if best agent in pool has not
*	changed for the passed number of generations.
*	 @param target Target number of generations unchanged
*	 @return boolean for termination
**********************************************************/
bool pool::term_test_best_unchanged(int target) {
	float best = get_best()->get_fitness();
	if (last_best == best) {
		gens_unchanged++;
	}
	else {
		gens_unchanged = 0;
		last_best = best;
	}
	return(gens_unchanged >= target);
}


/**********************************************************
*	pool::copy_from()
*	Copies all the agents in the passed pool and inserts
*	them into the current pool.
*	 @param p Target pool to copy from
**********************************************************/
void pool::copy_from(pool* p) {
	for (std::vector<agent>::iterator it = p->agents.begin(); it != p->agents.end(); it++) {
		agents.push_back(*it);
	}
	return;
}


/**********************************************************
*	pool::get_best()
*	Finds the best (highest fitness) agent in the pool.
*	 @return pointer to the best agent
**********************************************************/
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


/**********************************************************
*	pool::get_average()
*	Calculates and returns the average fitness of all agents
*	in the pool.
*	 @return average value of all agents in the pool
**********************************************************/
float pool::get_average() {

	// Variables
	float	total = 0.0f;
	
	// Calculate total
	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); it++) {
		total += (*it).get_fitness();
	}

	return total / (float)agents.size();
}
