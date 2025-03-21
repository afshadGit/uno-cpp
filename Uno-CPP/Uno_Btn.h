#pragma once

#include "General_pop_Scr.h"
#include "Player.h"
#include "OvalHover_Btn.h"
#include "GameStateData.h"

class Uno_Btn : public OvalHover_Btn, public General_pop_Scr
{
	// Object of BottomPlayer.
	Player *_bottomPlayer;

	// Reference to the game state for checking current player
	const GameStateData &_gameState;

public:
	// Initialises the UnoButton.
	Uno_Btn(const sf::Vector2f position, const sf::Font &font, const GameStateData &gameData);

	// Enables the button when it should be available.
	virtual void update(const float deltaTime) override;

	// Shows the overlay.
	virtual void showOverlay() override;

	// When the button is available and is clicked the player is flagged as having called and the called signal is flashed.
	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override;
	
	virtual ~Uno_Btn() = default;
};
