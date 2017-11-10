/******************************************************************************
*	game.cpp
*
*	Definitions for the game and agent classes for the Iterated Prisoner's
*	Dilemna Problem
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/


/**********************************************************
*	Headers
**********************************************************/
#include "game.h"
#include <string>


/******************************************************************************
*
*	Node Class
*
******************************************************************************/


/**********************************************************
*	node::evaluate()
*	Evalute this node's boolean equation.
*	 @param arg1 first argument to evaluate
*	 @param arg2 second argument to evaluate
*	 @return boolean result
**********************************************************/
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


/**********************************************************
*	(friend node::)operator<<
*	Outputs the node to the passed output stream.
*	 @param out the output stream to print to
*	 @param n the node to be printed
*	 @return the calling output stream (for chaining)
**********************************************************/
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


/******************************************************************************
*
*	Agent Class
*
******************************************************************************/


/**********************************************************
*	agent::agent()
*	Crossover contructor. Creates a new agent that is a 
*	random recombination of the two passed parents.
*	 @param parent1 pointer to the first parent
*	 @param parent2 pointer to the second parent
**********************************************************/
agent::agent(agent* parent1, agent* parent2) {

	// Variables
	int rand_node;
	tree<node>::iterator cross1;
	tree<node>::iterator cross2;

	// Copy over info
	gp_tree = parent1->gp_tree;
	depth = parent1->depth;
	memory = parent1->depth;
	payoff = parent1->depth;
	rounds_played = parent1->depth;

	// Choose crossover points
	rand_node = rand() % gp_tree.size();
	cross1 = gp_tree.begin();
	for (int i = 0; i < rand_node; i++) {
		cross1++;
	}
	rand_node = rand() % parent2->gp_tree.size();
	cross2 = parent2->gp_tree.begin();
	for (int i = 0; i < rand_node; i++) {
		cross2++;
	}

	// Delete after cross1 and insert the subtree from cross2
	gp_tree.erase_children(cross1);	
	gp_tree.insert_subtree_after(cross1, cross2);
	gp_tree.erase(cross1);

	return;
}


/**********************************************************
*	agent::randomize()
*	Create a new random tree for the agent to the assigned
*	depth
**********************************************************/
void agent::randomize() {

	// Sanity check
	if (depth < 0)
		return;
	
	// Variables
	tree<node>::iterator root;
	int type;

	// Place root node and recursively add child nodes
	if (depth != 0) {
		type = rand() % NUM_OPERATORS;
		root = gp_tree.insert(gp_tree.begin(), node(false, type, 0));
		add_random_children(root, (type == NOT ? 1 : 2), 1);
	}

	// If depth is 0, then just place a single leaf node
	else {
		type = rand() % NUM_AGENT_TYPES;
		gp_tree.insert(gp_tree.begin(), node(true, type, (rand() % memory) + 1));
	}

	return;
}


/**********************************************************
*	agent::operator=
*	Assignment operator for the agent class. Simply copies
*	all elements over. Performs a deep copy of the tree.
*	 @parem rhs agent being copied
*	 @return pointer to the calling object (for chaining)
**********************************************************/
agent& agent::operator= (const agent& rhs) {
	gp_tree = rhs.gp_tree;
	depth = rhs.depth;
	memory = rhs.memory;
	payoff = rhs.payoff;
	rounds_played = rhs.rounds_played;
	return *this;
}


/**********************************************************
*	agent::add_random_children()
*	Adds a random child at a point in the tree. To be used 
*	in conjunction with randomize(). This must be seperate
*	because placing the root node in randomize() requires
*	a different procedure. This function is called 
*	recursively.
*	 @param parent parent node in the tree
*	 @param children number of children to add
*	 @param cur_depth current recurive depth
**********************************************************/
void agent::add_random_children(tree<node>::iterator parent, int children, int cur_depth) {

	// Variables
	int type;
	tree<node>::iterator temp;
	bool leaf = (cur_depth == depth);

	// Add children (recursively if not a leaf node)
	for (int i = 0; i < children; i++) {
		if (!leaf) {
			type = rand() % NUM_OPERATORS;
			temp = gp_tree.append_child(parent, node(leaf, type, 0));
			add_random_children(temp, (type == NOT ? 1 : 2), cur_depth + 1);
		}
		else {
			type = rand() % NUM_CHOICES;
			temp = gp_tree.append_child(parent, node(leaf, type, (rand() % memory) + 1));
		}
	}
	
}


/**********************************************************
*	agent::calc_outcome()
*	Calculate the outcome of an agent's tree. This function
*	is recursive.
*	 @param x current node in the tree
*	 @param memory pointer to the game's memory structure
*	 @return boolean result
**********************************************************/
bool agent::calc_outcome(tree<node>::iterator x, std::vector<outcome>* memory) {

	// If this node is a leaf, evaluate the argument against the memory structure
	if ((*x).leaf) {
		if((*x).type == PLAYER)
			return (*memory)[(*x).count - 1].player == (bool)(*x).type;
		else // ((*x).type == OPPONENT)
			return (*memory)[(*x).count - 1].opponent == (bool)(*x).type;
	}

	// Otherwise, recurse down the tree
	// Evalute the boolean equation upon return
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


/**********************************************************
*	agent::print()
*	Prints the agent's tree in pre-order format to the passed
*	output stream.
*	 @param out the output stream to print to
*	 @return the calling output stream (for chaining)
**********************************************************/
void agent::print(std::ostream& out) {
	for (tree<node>::pre_order_iterator it = gp_tree.begin(); it != gp_tree.end(); it++) {
		out << (*it) << " ";
	}
}


/******************************************************************************
*
*	Game Class
*
******************************************************************************/


/**********************************************************
*	game::set_memory()
*	Custom constructor for the game class. Overrides the 
*	default memory length, then fills the memory structure.
**********************************************************/
void game::set_memory(int m) {
	memory.clear();
	memory_sz = m;
	for (int i = 0; i < memory_sz; i++) {
		outcome temp = { (bool)(rand() % 2), (bool)(rand() % 2) };
		memory.push_back(temp);
	}
}

void game::play_round() {
	opponent = player;
	agent test(player, opponent);
}


/**********************************************************
*	game::play_round()
*	Plays a round of the game by determining each player's
*	outcome and then updating their accumulated payoffs.
**********************************************************/
void game::play_round_tit_for_tat() {

	// Variables
	outcome out;

	// Get agent outcomes
	out.player = player->calc_outcome(&memory);
	out.opponent = memory[0].player;

	// Update payoff total
	if (out.player == DEFECT && out.opponent == DEFECT)
		player->add_payoff(5 - 4);
	else if (out.player == DEFECT && out.opponent == COOPERATE)
		player->add_payoff(5 - 0);
	else if (out.player == COOPERATE && out.opponent == DEFECT)
		player->add_payoff(5 - 5);
	else if (out.player == COOPERATE && out.opponent == COOPERATE)
		player->add_payoff(5 - 2);

	// Update memory
	memory.insert(memory.begin(), out);
	memory.pop_back();

	return;
}
