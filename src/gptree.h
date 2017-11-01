#ifndef GPTREE_H
#define GPTREE_H

#include "tree/tree.h"
#include "tree/tree_util.h"
#include <iostream>
#include <vector>

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

enum agent {
	PLAYER,
	OPPONENT,
	NUM_AGENTS
};

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


class gp_tree {
private:
	tree<node> controller;
	int depth = 0;
	int memory = 5;
	int payoff = 0;
	int rounds_played = 0;

	// Private member functions
	void add_random_children(tree<node>::iterator parent, int children, int depth);
public:
	// Constructors, Destructors, Etc.
	gp_tree() { depth = 0; memory = 5; payoff = 0; };
	inline gp_tree(int d, int m) { depth = d; memory = m; payoff = 0; };
	~gp_tree() {};
	gp_tree& operator= (const gp_tree& rhs);

	// Member Functions
	void randomize();
	inline bool calc_outcome(std::vector<outcome>* memory) { return calc_outcome(controller.begin(), memory); };
	bool calc_outcome(tree<node>::iterator x, std::vector<outcome>* memory);
	inline void print(std::ostream& out) { print_tree_bracketed(controller, out); };

	// Accessors & Mutators
	inline void add_payoff(int p) { payoff += p; rounds_played++; };
	inline float get_fitness() { return ((float)payoff / (float)rounds_played); };
	
};

#endif