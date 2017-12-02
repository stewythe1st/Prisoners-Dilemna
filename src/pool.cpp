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
#include <algorithm>
#include <cstdlib>


/**********************************************************
*	empty()
*	Empties the pool.
**********************************************************/
void pool::empty() {
	agents.clear();
	return;
}


/**********************************************************
*	pool::calc_fp()
*	Updates the fitness proportional vector. Each element
*	holds a ordered slice of (0.0-1.0) with width eqaul to
*	elementFitness/totalFitness
**********************************************************/
void pool::calc_fp() {

	// Variables
	float	totalFitness;
	float	lastProbability;

	fp.clear();

	// Get the sum of fitness values
	totalFitness = 0;
	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); ++it) {
		totalFitness += (*it).get_comp_fitness();
	}

	// Build up FP vector
	lastProbability = 0.0f;
	fp.clear();
	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); ++it) {
		fp.push_back(((*it).get_comp_fitness() / totalFitness) + lastProbability);
		lastProbability = fp.back();
	}

	return;
}


/**********************************************************
*	pool::choose_parent_fp()
*	Returns a pointer to a member agent chosen by fitness
*	proportional selection.
*	 @return pointer to the chosen agent
**********************************************************/
agent* pool::choose_parent_fp() {

	// Choose a random decimal (0.000 - 1.000)
	float value = GEN_RAND_DECIMAL;

	// Walk down the FP probability array and find the corresponding parent
	size_t i = 0;
	while (fp[i] < value && i < agents.size() - 1)
		i++;

	return &agents[i];
}


/**********************************************************
*	pool::calc_os()
*	Updates the overselection vectors. The os variable holds
*	two vectors containing pointers to the top proportion
*	and the bottom proportion, respectively.
**********************************************************/
static inline bool agentCompare(agent a, agent b) { return a.get_comp_fitness() > b.get_comp_fitness(); };
void pool::calc_os() {

	// Variables
	int i = 0;
	float top_pct;

	os[0].clear();
	os[1].clear();
	
	// Determine top tier fraction
	for (std::map<int, float>::iterator it = os_top_proportion.begin(); it != os_top_proportion.end(); it++) {
		top_pct = it->second;
		if (it->first > (int)agents.size()) {
			break;
		}
	}

	// Sort agents
	std::sort(agents.begin(), agents.end(), agentCompare);

	// Add agents to top tier or bottom tier
	while (i < (float)agents.size() * top_pct) {
		os[0].push_back(&agents[i]);
		i++;
	}
	while (i < (int)agents.size()) {
		os[1].push_back(&agents[i]);
		i++;
	}

	return;
}


/**********************************************************
*	pool::choose_parent_os()
*	Returns a pointer to a member agent chosen by overselection
*	 @return pointer to the chosen agent
**********************************************************/
agent* pool::choose_parent_os() {

	// Choose a group
	int		rand_val = rand() % 10;
	agent*	rtn;

	if (rand_val < 8) {
		rand_val = rand() % os[0].size();
		rtn = os[0][rand_val];
	}
	else {
		rand_val = rand() % os[1].size();
		rtn = os[1][rand_val];
	}

	return rtn;
}


/**********************************************************
*	pool::calc_comp_fitnesses()
*	Calculates each member's competitive fitness by averaging
*	against a random sampling of its opponents.
*	 @param opponents number of opponents to average against
**********************************************************/
void pool::calc_comp_fitnesses(int opponents) {

	// Variables
	int					num_agents;
	float				avg;
	int*				opp_idxs;

	// Sanity check
	num_agents = (int)agents.size();
	if (opponents > (num_agents - 1)) {
		opponents = num_agents - 1;
	}

	opp_idxs = new int[opponents];
	for (size_t i = 0; i < agents.size(); i++) {

		// Choose sample of unique opponents (no replacement)
		for (int j = 0; j < opponents; j++) {
			opp_idxs[j] = rand() % num_agents;
			if (opp_idxs[j] == i) {
				j--;
				continue;
			}
			for (int k = 0; k < j; k++) {
				if (opp_idxs[j] == opp_idxs[k]) {
					j--;
					break;
				}
			}
		}

		// Calculate opponents' average fitness
		avg = agents[i].get_fitness();
		for (int j = 0; j < opponents; j++) {
			avg += agents[opp_idxs[j]].get_fitness();
		}
		avg /= opponents + 1;
		agents[i].set_comp_fitness(avg);
	}

	delete[] opp_idxs;
	return;
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
		if (!highest && agents[tournament[i]].get_comp_fitness() > agents[bestIdx].get_comp_fitness())
			bestIdx = tournament[i];
		else if (highest && agents[tournament[i]].get_comp_fitness() < agents[bestIdx].get_comp_fitness())
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
			if ((*it).get_comp_fitness() < (*lowest).get_comp_fitness()) {
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
	float best = get_best()->get_comp_fitness();
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
*	pool::get_comp_best()
*	Finds the best (highest composite coevolutionary fitness)
*	agent in the pool.
*	 @return pointer to the best agent
**********************************************************/
agent* pool::get_comp_best() {

	// Variables
	std::vector<agent>::iterator highest = agents.begin();

	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); it++) {
		if ((*it).get_comp_fitness() > (*highest).get_comp_fitness()) {
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


/**********************************************************
*	pool::get_comp_average()
*	Calculates and returns the composite coevolutionary
	average fitness of all agents in the pool.
*	 @return average value of all agents in the pool
**********************************************************/
float pool::get_comp_average() {

	// Variables
	float	total = 0.0f;

	// Calculate total
	for (std::vector<agent>::iterator it = agents.begin(); it != agents.end(); it++) {
		total += (*it).get_comp_fitness();
	}

	return total / (float)agents.size();
}
