#include "game.h"

game::game() {
	for (int i = 0; i < memory_sz; i++) {
		outcome temp = { (bool)(rand() % 2), (bool)(rand() % 2) };
		memory.push_back(temp);
	}
}

game::game(int m) {
	memory_sz = m;
	for (int i = 0; i < memory_sz; i++) {
		outcome temp = { (bool)(rand() % 2), (bool)(rand() % 2) };
		memory.push_back(temp);
	}
}

game::~game() {

}

void game::play_round() {

	// Variables
	bool player_choice;
	bool opponent_choice;

	// Get agent outcomes
	player_choice = player->calc_outcome(&memory);
	opponent_choice = opponent->calc_outcome(&memory);
	
	// Assign payoffs
	if (player_choice == DEFECT && opponent_choice == DEFECT) {
		player->add_payoff(5 - 4);
		opponent->add_payoff(5 - 4);
	}
	else if (player_choice == DEFECT && opponent_choice == COOPERATE) {
		player->add_payoff(5 - 0);
		opponent->add_payoff(5 - 5);
	}
	else if (player_choice == COOPERATE && opponent_choice == DEFECT) {
		player->add_payoff(5 - 5);
		opponent->add_payoff(5 - 0);
	}
	else if (player_choice == COOPERATE && opponent_choice == COOPERATE) {
		player->add_payoff(5 - 2);
		opponent->add_payoff(5 - 2);
	}

	return;
}