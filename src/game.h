/******************************************************************************
*	game.h
*
*	Declarations for the game and agent classes for the Iterated Prisoner's
*	Dilemna Problem
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/
#ifndef GAME_H
#define GAME_H


/**********************************************************
*	Headers
**********************************************************/
#include "tree/tree.h"
#include "tree/tree_util.h"
#include <climits>
#include <iostream>
#include <vector>


/**********************************************************
*	Compiler Constants
**********************************************************/


/**********************************************************
*	Types, Etc.
**********************************************************/
struct outcome {
	bool player;
	bool opponent;
};

enum choice {
	DEFECT,
	COOPERATE,
	NUM_CHOICES
};

enum operators {
	AND,
	OR,
	XOR,
	NOT,
	NUM_OPERATORS
};

enum agent_type {
	PLAYER,
	OPPONENT,
	NUM_AGENT_TYPES
};


/**********************************************************
*	Node Class
**********************************************************/
class node {
public:
	bool	leaf;
	int		type;
	int		count;
	inline node() { leaf = 0, type = 0; count = 0; };
	inline node(bool l, int t, int c) { leaf = l; type = t; count = c; };
	bool evaluate(bool arg1, bool arg2);
	friend std::ostream& operator<<(std::ostream& out, const node& n);
};


/**********************************************************
*	Agent Class
**********************************************************/
class agent {

private:

	tree<node> gp_tree;
	int depth = 0;
	int memory = 5;
	int payoff = 0;
	int rounds_played = 0;
	float fitness = FLT_MIN;

	// Private member functions
	void add_random_children(tree<node>::iterator parent, int children, int depth, bool leaf_before_max);

public:

	// Constructors, Destructors, Etc.
	agent() { depth = 0; memory = 5; payoff = 0; };
	inline agent(int d, int m) { depth = d; memory = m; payoff = 0; };
	agent(agent* parent1, agent* parent2);
	~agent() {};
	agent& operator= (const agent& rhs);

	// Member Functions
	void randomize(bool leaf_before_max);
	void mutate(int max_depth);
	inline bool calc_outcome(std::vector<outcome>* memory) { return calc_outcome(gp_tree.begin(), memory); };
	bool calc_outcome(tree<node>::iterator x, std::vector<outcome>* memory);
	inline void quick_print(std::ostream& out) { print_tree_bracketed(gp_tree, out); };
	void print(std::ostream& out);
	void play_rounds(int rounds, int rerandmem);
	void calc_fitness(float parsimony);

	// Accessors & Mutators
	inline void add_payoff(int p) { payoff += p; };
	inline float get_fitness() { return fitness; };
	
};


/**********************************************************
*	Game Class
**********************************************************/
class game {

private:

	std::vector<outcome>
				memory;
	agent*		player;
	agent*		opponent;
	int			memory_sz = 5;

public:

	// Constructors, Destructors, Etc.
	game() { set_memory(memory_sz); };
	game(int m) { set_memory(m); };
	~game() {};

	// Member Functions
	void play_round_tit_for_tat(bool dry_run);

	// Accessors & Mutators
	void set_memory(int m);
	inline void set_player(agent* p) { player = p; };
	inline void set_opponent(agent* o) { opponent = o; };
	inline agent* get_player() { return player; };
	inline agent* get_opponent() { return opponent; };
};


#endif
