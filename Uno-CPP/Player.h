
/**
 * Safe indicates the player is not vulnerable to counter calls.
 * Called indicates the Player called before ending their turn with a card.
 * NotSafe indicates that players can counter call the player's uno and make them draw cards.
 */
enum UNOState
{
	Safe,
	Called,
	NotSafe
};
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Card.h"
#include "Draw_Txt.h"
#include "Cards_Played_Stack.h"
#include "TurnAction.h"
#include "PlayerUpdateResult.h"

class Player
{
	// The unique ID for this player.
	const int _playerID;

	// The name for this player.
	std::string _playerName;

	// The type of player. (Human_Player, or AIPlayer).
	bool _playerType;

	// The region for drawing the player's cards.
	const sf::IntRect _bounds;

	// The collection of cards contained in the player's hand.
	std::vector<Cards *> _hand;

	// The card that the player is currently hovering their mouse over.
	Cards *_hoveredCard;

	// When true the cards for this player are revealed face-up.
	bool _showCards;

	// The total score between multiple rounds for this player.
	int _totalScore;

	// The score for a single round for this player.
	int _currentRoundScore;

	// When true this player won the current round. ecessary to store this because a score could be 0 is all other players only have 0s in their hands.
	bool _wonRound;

	// When true, the player's name is centred to the left side of the bounds, otherwise it is centred on the top.
	const bool _showPlayerNameLeft;

	// The current UNOState that can be Safe, Called, or NotSafe.
	UNOState _unoState;

	// Name tag
	Draw_Obj_Grp *_nameTag;

	// Reference to text to change its colour based on current player ID (handled via _nameTag for rendering).
	Draw_Txt *_playerNameText;

	/**
	 * Recalculates positions for all cards by calculating numbers of
	 * rows and columns then centring inside the region and applying
	 * positions to all cards in the hand.
	 */
	void recalculateCardPositions();

public:
	Player(const int playerID, const std::string &playerName, const bool playerType, const sf::IntRect bounds, const bool showPlayerNameLeft, const sf::Font &font);
	virtual ~Player();

	virtual PlayerUpdateResult update(const float deltaTime, const Player *currentPlayer, Turn_Op *currentTurnAction, const Cards_Played_Stack *recentCards, const std::vector<Player *> &players, const RuleSet *rules)
	{
		return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
	}

	// Draws the player's cards with either card backs or fronts. Then draws the player's name nearby.
	void draw(sf::RenderWindow &renderWindow, const Player *currentPlayer) const;

	void addCardToHand(Cards *card);

	// Empties the hand. Warning! This deletes all the cards in the hand.
	void emptyHand();

	void revealHand(const bool reveal);

	// Gets the type of Player.
	bool getPlayerType() const;

	// Gets the unique player ID.
	int getPlayerID() const;

	std::vector<Cards *> getValidMoves(const int curFaceValue, const int curColourValue) const;

	/* Sorts the hand and recalculates the positions of all cards.
	 * Cards are sorted first by colour and then by face values. */
	void sortHand();

	void updateHover(const sf::Vector2i &mousePosition);

	Cards *removeCard(const int uniqueCardID);

	const Cards *getCardByID(const int cardID);

	const Cards *chooseCardFromClick(const sf::Vector2i &mousePosition);

	// Gets all the cards in the player's hand.
	const std::vector<Cards *> &getHand() const;

	// Empties the hand and returns a vector with the cards
	std::vector<Cards *> takeAllHand();

	// Gets the player name.
	const std::string &getPlayerName() const;

	int getHandTotalScore() const;

	// Gets the centre of the player's region.
	sf::Vector2f getCentreOfBounds() const;

	void setCurrentRoundScore(const int newCurrentRoundScore);

	// Sets the won state to true.
	void setWon();

	bool getWon() const;

	int getTotalScore() const;

	int getCurrentRoundScore() const;

	// Reset the current round and total scores, the won state, and UNO State to Safe for the player.
	void resetScore();

	bool isSafe() const;

	void setUnoState(const UNOState unoState);

	UNOState getUnoState() const;
};
