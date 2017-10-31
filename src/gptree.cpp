#include "gptree.h"
#include "tree/tree_util.h"
#include <string>


void gp_tree::randomize(int depth) {
	
	// Variables
	tree<node>::iterator a, b, c, d, e;

	// Place root node
	a = controller.insert(controller.begin(), node(false, rand() % NUM_OPERATORS, 0));

	// Append level 2 nodes
	b = controller.append_child(a, node(false, rand() % NUM_OPERATORS, 0));
	c = controller.append_child(a, node(false, rand() % NUM_OPERATORS, 0));

	// Append leve 3 leaf nodes
	d = controller.append_child(b, node(true, rand() % NUM_AGENTS, 2));
	e = controller.append_child(b, node(true, rand() % NUM_AGENTS, 2));
	d = controller.append_child(c, node(true, rand() % NUM_AGENTS, 2));
	e = controller.append_child(c, node(true, rand() % NUM_AGENTS, 2));

	return;
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
