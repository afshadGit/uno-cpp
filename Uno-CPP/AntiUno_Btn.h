#pragma once
#include "OvalHover_Btn.h"
#include "General_pop_Scr.h"
#include "Player.h"
#include "GameStateData.h"

class AntiUno_Btn : public OvalHover_Btn, public General_pop_Scr { // AntiUno_Btn is a button that is used to call out a player for not saying "UNO" when they have only one card left.

	Player *_bottomPlayer; // Reference to the BottomPlayer.
	const GameStateData &_gameState;// Reference to the game state.

	public:
		AntiUno_Btn(const sf::Vector2f position, const sf::Font &font, const GameStateData &gameData); // initialises the button.
		virtual ~AntiUno_Btn();

		virtual void update(const float deltaTime) override; // updates to determine if there is a player vulnerable to being called out on not saying "UNO". They are vulnerable if they only have one card, are not the bottom player (because that is the one controlling it), and the player did not call UNO yet.

		virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override; // When the button is active it means there is at least one player that can be called out. This method checks for the button being pressed and determines which player needs to be called out.

		virtual void showOverlay() override; // Show the overlay.
};
