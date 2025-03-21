#pragma once

#include "Game_Scr.h"
#include "TurnDecision_Pop.h"
#include "Btn.h"
#include "CardFrontObjectGroup.h"

class Keep_Or_Play_Pop : public Game_Scr, public TurnDecision_Pop
{
	// Reference to the font.
	const sf::Font &_font;

	// List of buttons consisting of the Keep and Play buttons.
	std::vector<Btn> _buttonList;

	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction *_currentAction;

	// Reference to the dummy card that the overlay is making a decision about.
	CardFrontObjectGroup *_cardForChoice;

	// Position where the card in question is to be placed.
	sf::Vector2f _cardPosition;

public:
	// Initialise the Keep and Play buttons and the location where the dummy card has to be placed.
	Keep_Or_Play_Pop(const sf::IntRect &bounds, const sf::Font &font);
	virtual ~Keep_Or_Play_Pop();

	// Not used.
	virtual void update(const float deltaTime) override {}

	// Draws all the buttons and the card related to the choice.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	virtual void showOverlay(TurnDecisionAction *currentAction) override; // Shows the overlay.

	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override; // Handles the mouse press on the buttons.

	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override; // Handles the mouse hovering over the buttons.
};
