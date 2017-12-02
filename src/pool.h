/******************************************************************************
*	pool.h
*
*	Declarations for the pool class for the Iterated Prisoner's	Dilemna Problem
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/
#ifndef POOL_H
#define POOL_H


/**********************************************************
*	Headers
**********************************************************/
#include  "game.h"
#include <map>
#include <vector>


/**********************************************************
*	Compiler Constants
**********************************************************/
#define GEN_RAND_DECIMAL	((float)(std::rand() % 1001) / 1000.0f)


/**********************************************************
*	Variables
**********************************************************/
static std::map<int, float> os_top_proportion = {
	{ 1000, 0.32f },
	{ 2000, 0.16f },
	{ 4000, 0.8f },
	{ 8000, 0.4f },
};


/**********************************************************
*	Types, Etc.
**********************************************************/


/**********************************************************
*	Pool Class
**********************************************************/
class pool {

private:

	std::vector<agent>	agents;
	std::vector<float>	fp;
	std::vector<agent*>	os[2];
	int					gens_unchanged = 0;
	float				last_best = FLT_MIN;

	// Private Member Functions
	int k_tourn(int k, bool best, bool replacement);

public:

	// Constructors, Destructors, Etc.
	pool() {};
	~pool() {};

	// Member Functions
	void empty();
	void calc_fp();
	agent* choose_parent_fp();
	void calc_os();
	agent* choose_parent_os();
	void calc_comp_fitnesses(int opponents);
	void reduce_by_truncation(int size);
	void reduce_by_k_tourn(size_t size, int k);
	void copy_from(pool* p);
	inline void clear() { agents.clear(); };
	agent* get_best();
	agent* get_comp_best();
	float get_average();
	float get_comp_average();
	bool term_test_best_unchanged(int target);

	// Accessors & Mutators
	agent* get(int i) { return &agents[i]; };
	void add(agent a) { return agents.push_back(a); };

};


#endif
