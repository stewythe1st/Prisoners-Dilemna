#ifndef GAME_H
#define GAME_H

#include "gptree.h"
#include <vector>

class game {
private:
	std::vector<outcome>memory;
	gp_tree*			player;
	gp_tree*			opponent;
	int					memory_sz = 5;
	int					round = 1;
public:
	game();
	game(int m);
	~game();

	void play_round();

	inline void set_player(gp_tree* p) { player = p; };
	inline void set_opponent(gp_tree* o) { opponent = o; };
	inline gp_tree* get_player() { return player; };
	inline gp_tree* get_opponent() { return opponent; };
};

#endif