#pragma once

#include "Game_Scr.h"
#include "TurnDecision_Pop.h"
#include "Btn.h"
#include "Player.h"
#include "GameStateData.h"

class Challenge_Pop : public Game_Scr, public TurnDecision_Pop
{
	// List of buttons that can be used in the overlay. Includes a Challenge and Decline button.
	std::vector<Btn> _buttonList;

	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction *Pop_by_Act;

	// When true via the RuleSet the player's cards can be stacked if possible.
	bool _allowStacking;

	// Reference to the player to be used for card selection when stacking is allowed.
	Player *_playerReference;

public:

	Challenge_Pop(const sf::IntRect &bounds, const sf::Font &font, const GameStateData &gameData);
	virtual ~Challenge_Pop() = default;

	// Does nothing.
	void update(const float deltaTime) override {};

	// Draws the buttons.
	void draw(sf::RenderWindow &renderWindow) const override;

	void handleMouseMove(const sf::Vector2i &mousePosition) override; // Handles the mouse hovering over the buttons.

	void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override; // Handles the mouse press on the buttons.

	virtual void showOverlay(TurnDecisionAction *currentAction) override; // Shows the overlay.
};
