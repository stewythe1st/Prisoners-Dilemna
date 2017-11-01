#include "game.h"

game::game() {
	for (int i = 0; i < memory_sz; i++) {
		memory.push_back((agent)(rand() % NUM_AGENTS));
	}
}

game::game(int m) {
	memory_sz = m;
	for (int i = 0; i < memory_sz; i++) {
		memory.push_back((agent)(rand() % NUM_AGENTS));
	}
}

game::~game() {

}

void game::play_round() {

	bool result;

	result = player->calc_outcome(&memory);
	std::cout << result << std::endl;

	return;
}