#pragma once
#include "UNO_Game.h"

class MainGame
{
	sf::RenderWindow _window; 
	sf::Font _font; // game font .

	UNO_Game _game; // UNO Game object

	sf::Font loadFont();

public:
	MainGame(); // Initialises the game ready to start.

	// Continues running the game until
	void game_Loop();

	virtual ~MainGame() = default;
};
