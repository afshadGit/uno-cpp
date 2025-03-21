#pragma once

#include "UNO_GamePlay_Screen.h"
#include "Lobby_Scr.h"
#include "Result_Game_Scr.h"
#include "Pause_Scr.h"

class UNO_Game
{
	const sf::IntRect _bounds;

	const sf::Font &_font;

	// Shared randomEngine used for all the randomisation.
	std::default_random_engine _randomEngine;

	// pointer to the current active interface.
	Game_Scr *_activeInterface;

	// pointer to the active current game interface or nullptr.
	static UNO_GamePlay_Screen *_currentGame;

	// pointer to the active LobbyInterface if one exists. Used for passing the LobbyPlayer objects into the CurrentGame.
	Lobby_Scr *_lobbyScr;

	// pointer to the active PostGameInterface if one exists. Used for the Post Game results.
	Result_Game_Scr *_ResultGameScr;

	// pointer to the Pause_Interface shown when the game enters a pause state.
	Pause_Scr *_pause_Scr;

	// When true, the game should end.
	bool End_Game;

	// Pauses or unpauses the game.
	void setPauseState(const bool isPaused);

	/**
	 * Sets the currently active interface to the lobby removing any existing interface.
	 * If this is triggered from the pause interface it just resumes the current interface.
	 */
	void showLobby();

	// Creates a new game
	void startGame();

	// Creates a new game with the specified list of players. Use this for coming from post-game.
	void startNextRound();

	// Sets the currently active interface to the post-game interface after a game has ended.
	void showPostGame();

	void quitGame();

public:
	// Initialises the game with the Lobby.
	UNO_Game(const sf::IntRect &gameBounds, const sf::Font &font);

	// Updates the current state of the active window.
	void update(const float deltaTime);

	// Draws the current window.
	void draw(sf::RenderWindow &renderWindow) const;

	// Handles passing events on to the active interface.
	void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft);

	// Handles passing the mouse moved event to the active interface.
	void handleMouseMove(const sf::Vector2i &mousePosition);

	// Handles passing key pressed event to the active interface.
	void handleKeyInput(const sf::Keyboard::Key key);

	// Gets a reference to the single active interface used to access all the callbacks.
	static UNO_GamePlay_Screen *getCurrentGame();

	//  When true, the game should end.
	bool getGameCloseRequested();

	virtual ~UNO_Game();
};
