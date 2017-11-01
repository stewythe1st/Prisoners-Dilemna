#include "gptree.h"
#include <string>


void gp_tree::randomize() {

	// Sanity check
	if (depth < 0)
		return;
	
	// Variables
	tree<node>::iterator root;
	int type;

	// Place root node and recursively add child nodes
	if (depth != 0) {
		type = rand() % NUM_OPERATORS;
		root = controller.insert(controller.begin(), node(false, type, 0));
		add_random_children(root, (type == NOT ? 1 : 2), 1);
	}

	// If depth is 0, then just place a single leaf node
	else {
		type = rand() % NUM_AGENTS;
		controller.insert(controller.begin(), node(true, type, (rand() % memory) + 1));
	}

	return;
}

gp_tree& gp_tree::operator= (const gp_tree& rhs) {
	controller = rhs.controller;
	depth = rhs.depth;
	memory = rhs.memory;
	payoff = rhs.payoff;
	rounds_played = rhs.rounds_played;
	return *this;
}

void gp_tree::add_random_children(tree<node>::iterator parent, int children, int cur_depth) {

	// Variables
	int type;
	tree<node>::iterator temp;
	bool leaf = (cur_depth == depth);

	// Add children (recursively if not a leaf node)
	for (int i = 0; i < children; i++) {
		if (!leaf) {
			type = rand() % NUM_OPERATORS;
			temp = controller.append_child(parent, node(leaf, type, 0));
			add_random_children(temp, (type == NOT ? 1 : 2), cur_depth + 1);
		}
		else {
			type = rand() % NUM_CHOICES;
			temp = controller.append_child(parent, node(leaf, type, (rand() % memory) + 1));
		}
	}
	
}

bool gp_tree::calc_outcome(tree<node>::iterator x, std::vector<outcome>* memory) {
	if ((*x).leaf) {
		if((*x).type == PLAYER)
			return (*memory)[(*x).count - 1].player == (bool)(*x).type;
		else // ((*x).type == OPPONENT)
			return (*memory)[(*x).count - 1].opponent == (bool)(*x).type;
	}
	else {
		bool a = false;
		bool b = false;
		tree<node>::sibling_iterator it = x.begin();
		a = calc_outcome(it, memory);
		if ((*x).type != NOT) {
			it++;
			b = calc_outcome(it, memory);
		}
		return (*x).evaluate(a, b);
	}
}


bool node::evaluate(bool arg1, bool arg2) {
	switch (type) {
	case AND:
		return arg1 & arg2;
	case OR:
		return arg1 | arg2;
	case XOR:
		return arg1 ^ arg2;
	case NOT:
		return !arg1;
	}
	return false;
}


std::ostream& operator<<(std::ostream& out, const node& n) {
	if (!n.leaf) {
		switch (n.type) {
		case AND:
			out << "AND";
			break;
		case OR:
			out << "OR";
			break;
		case XOR:
			out << "XOR";
			break;
		case NOT:
			out << "NOT";
			break;
		}
	}
	else {
		switch (n.type) {
		case PLAYER:
			out << "P";
			break;
		case OPPONENT:
			out << "O";
			break;
		}
		out << n.count;
	}
	return out;
}
