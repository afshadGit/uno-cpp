#pragma once

#include "Game_Scr.h"
#include "TurnDecision_Pop.h"
#include "Btn.h"
#include "Player.h"

class PlayerSelection_pop : public Game_Scr, public TurnDecision_Pop
{
	// List of buttons that can be used in the overlay. Includes a Challenge and Decline button.
	std::vector<Btn> _buttonList;

	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction *_currentAction;

public:
	PlayerSelection_pop(const sf::IntRect &bounds, std::vector<Player *> playerList, const sf::Font &font);

	// Does nothing
	virtual void update(const float deltaTime) override {}

	// Draws all the buttons.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override;

	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override;

	virtual void showOverlay(TurnDecisionAction *currentAction) override;

	virtual ~PlayerSelection_pop();
};
