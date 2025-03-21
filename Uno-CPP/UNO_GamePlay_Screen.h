#pragma once
#include "Game_Scr.h"
#include "Card_Deck.h"
#include "Cards_Played_Stack.h"
#include "LobbyPlayer.h"
#include "Popup_Manager.h"
#include "Turn_sequence_anim.h"
#include "RuleSet.h"
#include "Turn_Sequence_Manager.h"
#include <vector>

class UNO_GamePlay_Screen : public Game_Scr
{
	// The rules for what card actions are set and other specific changes to how the game is played.
	RuleSet *_ruleSet;

	// The deck of cards ready to have cards drawn from it.
	Card_Deck *_deck;

	// Manages the active TurnActions and their sequences.
	Turn_Sequence_Manager *_turnActionSequenceManager;

	// A history of cards that have been played.
	Cards_Played_Stack *_recentCardPile;

	// A manager controlling the various overlays that are shown based on events during the game.
	Popup_Manager *_overlayManager;

	// All the players that are currently playing including their hands and other details.
	std::vector<Player *> _players;

	// Reference to the player who is playing the game.
	Player *_bottomPlayer;

	// The current player who is in control of actions.
	int _currentPlayerID;

	// Turn order increasing (true) means clockwise, or false would be anti-clockwise.
	bool _isIncreasing;

	// Animation to show the direction of turn order.
	Turn_sequence_anim *_playDirectionAnimation;

	// DEBUG MODE
	bool _debugModeEnabled;

	// Text shown when the debug mode is enabled.
	sf::Text _debugEnabledText;

	// Result state that changes to Finished when round ends.
	Game_Result_State _resultState;

	// Checks if there is currently a player who has won the game and initiates end game conditions once found.
	void checkForEndOfRound();

	// Sets the current player to NotSafe if they have one card or Safe otherwise, all other players are set to Safe.
	void updateUNOState();

	// Applies the specified result
	void applyPlayerUpdateResult(PlayerUpdateResult &playerUpdateResult);

	// Generates a list of players using the specified types. Requires a single Human_Player and 1 or 3 AIPlayer.
	static std::vector<Player *> createPlayersFromLobby(const std::vector<LobbyPlayer *> playerList, sf::IntRect bounds, const sf::Font &font, std::default_random_engine &randomEngine);
	// Generates bounds for where a player's cards should be placed.
	static sf::IntRect getPlayerRect(const int direction, const sf::IntRect &bounds);

public:
	/**
	 * Initialise the interface with bounds and make it enabled. Use this version when coming from the Lobby for a new set of rounds.
	 */
	UNO_GamePlay_Screen(const sf::IntRect &bounds, const sf::Font &font, const std::vector<LobbyPlayer *> playerList, RuleSet *ruleSet, std::default_random_engine &randomEngine);

	/**
	 * Initialise the interface with bounds and make it enabled. Use this version when coming from
	 * after a game has already been completed and the sequence of games is continuing.
	 */
	UNO_GamePlay_Screen(const sf::IntRect &bounds, const sf::Font &font, const std::vector<Player *> playerList, RuleSet *ruleSet, std::default_random_engine &randomEngine);
	virtual ~UNO_GamePlay_Screen();

	// Updates all the game components that need to be updated on a timer.
	virtual void update(const float deltaTime) override;

	// Draws all the game elements that are available.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override;

	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override;

	virtual void handleKeyInput(const sf::Keyboard::Key key) override;

	void jumpIn(const int playerID, const Cards *cardToPlay);

	// Used to show an overlay based on a current decision.
	void showOverlayForTurnAction();

	// Shows an overlay based on a String lookup in the overlay manager.
	void showGeneralOverlay(const std::string &overlayName);

	// Forces all hands to reveal and stay revealed.
	void revealHands();

	// Sorts the player's hand.
	void sortHand();

	Player *getBottomPlayer() const;

	/**
	 * Toggles the turn direction between clockwise and anti-clockwise.
	 * Including updating the animation direction.
	 */
	void toggleTurnDirection();

	// Moves to the next player depending on whether the direction is clockwise or anti-clockwise.
	void moveToNextPlayer();

	/**
	 * Applies the effect from being called out on not having said UNO.
	 * Flashes the message and draws two cards to that player.
	 */
	void applyAntiUno(const int playerID);

	// Gets the current direction of play.
	bool isIncreasing() const;

	// If there is a current action already active it will be queued to start asap. Otherwise the action is set up immediately.
	void setCurrentTurnAction(TurnSequence<Turn_Op> *newSequence);

	// Gets the current TurnAction if there is one.
	Turn_Op *getCurrentTurnAction() const;

	// Gets the ruleset to easily check and apply any rules.
	RuleSet *getRuleSet() const;

	// Gets the currently active player for turn order.
	Player *getCurrentPlayer() const;

	// Gets a list of all players.
	std::vector<Player *> getAllPlayers() const;

	// Looks up the player with the given ID.
	Player *getPlayerByID(const int playerID) const;

	// Gets the deck to provide access to drawing cards.
	Card_Deck *getDeck() const;

	// Gets the recently played cards.
	Cards_Played_Stack *getRecentCardPile() const;

	// Returns Nothing when interface is still in use, Finished to move to PostGame.
	Game_Result_State getResultState() const override;
};
