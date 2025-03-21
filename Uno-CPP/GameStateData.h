#pragma once

#include "RuleSet.h"
#include "Player.h"
#include "Card_Deck.h"

// Struct to hold all the data needed for the game state, such as the rule set, players, the bottom player, the direction of play, the deck, and the current player ID.
struct GameStateData
{
	RuleSet *ruleSet;
	std::vector<Player *> *players;
	Player *bottomPlayer;
	bool *isIncreasing;
	Card_Deck *deck;
	int *currentPlayerID;
};