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
	memory = parent1->memory;

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
*	Create a new random gp_tree for the agent. If leaf_before_max
*	is set to true, grow type trees (terminal nodes allowed
*	at any depth) and false for full type trees (terminal
*	nodes only at max depth).
*	 @param leaf_before_max allow leaf nodes before max depth?
**********************************************************/
void agent::randomize(bool leaf_before_max) {

	// Sanity check
	if (depth < 0)
		return;
	
	// Variables
	tree<node>::iterator root;
	int type;
	bool leaf;

	// Decide if this should be a leaf node
	if (!leaf_before_max) {
		leaf = (depth == 0);
		if (!leaf) {
			type = rand() % NUM_OPERATORS;
		}
	}
	else {
		type = rand() % (NUM_OPERATORS + NUM_AGENT_TYPES);
		if (type >= NUM_OPERATORS) {
			type -= NUM_OPERATORS;
			leaf = true;
		}
		else {
			leaf = false;
		}
	}

	// Place root node
	if (!leaf) {
		root = gp_tree.insert(gp_tree.begin(), node(leaf, type, 0));
		add_random_children(root, (type == NOT ? 1 : 2), 1, leaf_before_max);
	}
	else {
		gp_tree.insert(gp_tree.begin(), node(leaf, type, (rand() % memory) + 1));
	}

	return;
}


/**********************************************************
*	agent::mutate()
*	Mutation operator. Contructs a random subtree of the
*	passed depth and inserts it at a random node, replacing
*	the original contents at and below the node.
*	 @param max_depth maximum depth of the generated subtree
**********************************************************/
void agent::mutate(int max_depth) {

	// Variables
	int rand_node;
	int depth;
	tree<node>::iterator mutate_pt;

	// Choose a mutation point
	depth = rand() % max_depth;
	rand_node = rand() % gp_tree.size();
	mutate_pt = gp_tree.begin();
	for (int i = 0; i < rand_node; i++) {
		mutate_pt++;
	}

	// Add a new subtree and delete the original one
	agent temp(max_depth, memory);
	temp.randomize(true);
	gp_tree.insert_subtree_after(mutate_pt, temp.gp_tree.begin());
	gp_tree.erase(mutate_pt);

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
	fitness = rhs.fitness;
	rounds_played = rhs.rounds_played;
	return *this;
}


/**********************************************************
*	agent::add_random_children()
*	Adds a random child at a point in the tree. To be used 
*	in conjunction with randomize(). This must be seperate
*	because placing the root node in randomize() requires
*	a different procedure. leaf_before_max is true for grow
*	type trees and false for full type trees. This function
*	is called recursively.
*	 @param parent parent node in the tree
*	 @param children number of children to add
*	 @param cur_depth current recurive depth
*	 @param leaf_before_max allow leaf nodes before max depth?
**********************************************************/
void agent::add_random_children(tree<node>::iterator parent, int children, int cur_depth, bool leaf_before_max) {

	// Variables
	int type;
	tree<node>::iterator temp;
	bool leaf;
	
	// Decide if this should be a leaf node
	if (cur_depth >= depth) {
		leaf = true;
		type = rand() % NUM_OPERATORS;
	}
	else if (!leaf_before_max) {
		leaf = (cur_depth == depth);
		if (!leaf) {
			type = rand() % NUM_OPERATORS;
		}
	}
	else {
		type = rand() % (NUM_OPERATORS + NUM_AGENT_TYPES);
		if (type >= NUM_OPERATORS) {
			type -= NUM_OPERATORS;
			leaf = true;
		}
		else {
			leaf = false;
		}
		
	}

	// Add children (recursively if not a leaf node)
	for (int i = 0; i < children; i++) {
		if (!leaf) {
			temp = gp_tree.append_child(parent, node(leaf, type, 0));
			add_random_children(temp, (type == NOT ? 1 : 2), cur_depth + 1, leaf_before_max);
		}
		else {
			temp = gp_tree.append_child(parent, node(leaf, type, (rand() % memory) + 1));
		}
	}

	return;
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
*	agent::play_rounds()
*	Sets up a game and plays the passed number of rounds.
*	 @param rounds number of rounds to play
**********************************************************/
void agent::play_rounds(int rounds) {
	game g(memory);
	g.set_player(this);
	for (int i = 0; i < rounds; i++) {
		g.play_round_tit_for_tat(rounds_played >= (2 * memory));
		rounds_played++;
	}
	return;
}


/**********************************************************
*	agent::calc_fitness()
*	Calculates fitness and update the fitness member variable
**********************************************************/
void agent::calc_fitness() {
	// If insufficient rounds played, fitness is not yet valid
	if (rounds_played < (2 * memory)) {
		fitness = -1.0f;
	}
	else {
		
		// Fitness = payoffs / rounds 
		// for all valid rounds (after 2 * memory rounds)
		fitness = (float)payoff / (float)(rounds_played - (2 * memory));

		// Discourage trees from optimizing purely based on size
		// Without this term, trees of depth 1 become prevalent in the population
		size_t size = gp_tree.size();
		if (size > pow(1.5, depth)) {
			fitness -= size * PARSIMONY_PENALTY;
		}
	}

	return;
};


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


/**********************************************************
*	game::play_round()
*	Plays a round of the game by determining each player's
*	outcome and then updating their accumulated payoffs.
**********************************************************/
void game::play_round_tit_for_tat(bool dry_run) {

	// Variables
	outcome out;

	// Get agent outcomes
	out.player = player->calc_outcome(&memory);
	out.opponent = memory[0].player;

	// Update payoff total
	if (!dry_run) {
		if (out.player == DEFECT && out.opponent == DEFECT)
			player->add_payoff(5 - 4);
		else if (out.player == DEFECT && out.opponent == COOPERATE)
			player->add_payoff(5 - 0);
		else if (out.player == COOPERATE && out.opponent == DEFECT)
			player->add_payoff(5 - 5);
		else if (out.player == COOPERATE && out.opponent == COOPERATE)
			player->add_payoff(5 - 2);
	}

	// Update memory
	memory.insert(memory.begin(), out);
	memory.pop_back();

	return;
}
