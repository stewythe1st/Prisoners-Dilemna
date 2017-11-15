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
#include <vector>


/**********************************************************
*	Compiler Constants
**********************************************************/


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
	int					gens_unchanged = 0;
	float				last_best;

public:

	// Constructors, Destructors, Etc.
	pool() {};
	~pool() {};

	// Member Functions
	agent* choose_parent_fp();
	void reduce_by_truncation(int size);
	void copy_from(pool* p);
	inline void clear() { agents.clear(); };
	agent* get_best();
	float get_average();
	bool term_test_best_unchanged(int target);

	// Accessors & Mutators
	agent* get(int i) { return &agents[i]; };
	void add(agent a) { return agents.push_back(a); };

};


#endif
