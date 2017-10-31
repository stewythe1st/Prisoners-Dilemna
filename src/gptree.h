#ifndef GPTREE_H
#define GPTREE_H

#include "tree/tree.h"
#include "tree/tree_util.h"
#include <iostream>

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
private:
	bool	leaf;
	int		type;
	int		count;
public:
	inline node() { leaf = 0, type = 0; count = 0; };
	inline node(bool l, int t, int c) { leaf = l; type = t; count = c; };
	friend std::ostream& operator<<(std::ostream& out, const node& n);
};


class gp_tree {
private:
	tree<node> controller;
	int depth;
	int memory;

	// Private member functions
	void add_random_children(tree<node>::iterator parent, int children, int depth);
public:
	// Constructors, Destructors, Etc.
	gp_tree() { depth = 0; memory = 5; };
	inline gp_tree(int d, int m) { depth = d; memory = m; };
	~gp_tree() {};

	// Member Functions
	void randomize();
	inline void print(std::ostream& out) { print_tree_bracketed(controller, std::cout); };

	// Accessors & Mutators
	
};

#endif