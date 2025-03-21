#pragma once

#include "Game_Scr.h"
#include "TurnDecision_Pop.h"
#include "Btn.h"
#include "Player.h"
#include "GameStateData.h"

class StackChoice_Pop : public Game_Scr, public TurnDecision_Pop
{
	// The decline button that simply accepts taking the card drawing.
	Btn *Cross_Button;
	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction *_currentAction;
	// Reference to the bottom player who is making the choice.
	Player *_playerReference;

public:
	//Initialise the decline button and reference to the player for tracking their cards.
	StackChoice_Pop(const sf::IntRect &bounds, const sf::Font &font, const GameStateData &gameData);

	// Does nothing.
	virtual void update(const float deltaTime) override {};

	// Draws the Decline button.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Does nothing if not enabled. Checks for a click on the decline button to handle it. And checks for the player clicking on their cards to allow stacking. If this overlay is visible it is implied that stacking is allowed.
	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override;

	// Does nothing if not enabled. Updates the hover status of the decline button.
	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override;

	// Shows the overlay.
	virtual void showOverlay(TurnDecisionAction *currentAction) override;

	virtual ~StackChoice_Pop();
};
