#pragma once
#include "Player.h"
#include "TurnDecisionAction.h"
#include <random>

class Comp_Player : public Player
{
	// Random engine used for making AI decisions.
	std::default_random_engine &_randomEngine;

	// Timer used to add delay between AI actions.
	float delay_Timer;

	// ID of the player being evaluated for a potential callout.
	int _consideringPlayerID;

	// Timer for deciding whether to call out another player.
	float _consideringDelayTimer;

	/** Indicates if the current game state permits a jump-in action.
	 * Transitioning from false to true triggers evaluation for jump-in opportunities. */
	bool _canJumpIn;

	// Tracks if the AI has decided to jump in, activated after a decision delay.
	bool _consideringJumpIn;

	// Timer for executing a jump-in action if it remains valid.
	float _consideringJumpInTimer;

	// Checks the current state for possible anti-UNO calls and decides whether to call out another player.
	PlayerUpdateResult updateAntiUnoCheck(const int deltaTime, const std::vector<Player *> &players);

	// Updates the AI's jump-in state if the game rules and situation allow it.
	PlayerUpdateResult updateJumpInCheck(const int deltaTime, const RuleSet *rules, const Turn_Op *currentTurnAction, const Player *currentPlayer, const Cards_Played_Stack *recentCards);

	/**
	 * Executes the AI player's turn by determining if a valid move exists.
	 * If no move is possible, a card is drawn. Otherwise, selects a valid move to play.
	 * Returns a pair containing an action sequence and an optional PlayerUpdateResult (e.g., if UNO is called during the turn).
	 */
	std::pair<TurnSequence<Turn_Op> *, PlayerUpdateResult> performTurn(Cards *topCard);

	// Resets the action delay timer to its initial state.
	void resetDelayTimer();

	// Chooses a card to play from a list of valid options using the AI's strategy.
	Cards *chooseCard(std::vector<Cards *> &validCards);

	// Handles the AI's response to a turn decision based on the provided flag and the game's current state.
	PlayerUpdateResult handleTurnDecision(TurnDecisionAction *decisionAction, const std::vector<Player *> &players, const RuleSet *rules);

	// Selects a wild card color based on the AI's strategy, considering hand composition or random choice.
	PlayerUpdateResult chooseWildColour(TurnDecisionAction *decisionAction);

	// Chooses to always play cards drawn during the current turn.
	PlayerUpdateResult chooseKeepOrPlay(TurnDecisionAction *decisionAction);

	// Identifies the player with the fewest cards (excluding itself) and indicates a preference to swap hands.
	PlayerUpdateResult choosePlayerToSwapWith(TurnDecisionAction *decisionAction, const std::vector<Player *> &players);

	// Decides randomly whether to challenge or decline a challenge. Always stacks valid cards if allowed.
	PlayerUpdateResult chooseChallengeOrDecline(TurnDecisionAction *decisionAction, const RuleSet *rules);

	// Plays a valid +2 card if stacking is allowed; otherwise, indicates no stacking action.
	PlayerUpdateResult chooseStackPlus2(TurnDecisionAction *decisionAction, const RuleSet *rules);

	// Evaluates whether the AI should call UNO to avoid being penalized.
	PlayerUpdateResult checkCallUNO();

public:
	// Constructs an AI player object with the specified attributes, enabling decision-making during gameplay.
	Comp_Player(const int playerID, const std::string &playerName, const sf::IntRect bounds, const bool showPlayerNameLeft, std::default_random_engine &randomEngine, const sf::Font &font);
	virtual ~Comp_Player() = default;

	// Processes valid actions for this player and performs them if applicable.
	PlayerUpdateResult update(const float deltaTime, const Player *currentPlayer, Turn_Op *currentTurnAction, const Cards_Played_Stack *recentCards, const std::vector<Player *> &players, const RuleSet *rules) override;
};
