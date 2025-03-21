#include "Challenge_Pop.h"
#include <iostream>
Challenge_Pop::Challenge_Pop(const sf::IntRect &bounds, const sf::Font &font, const GameStateData &gameData)
	: Game_Scr(bounds)
{
	setEnabled(false);
	sf::Vector2f centre(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
	// Only add the Challenge button if bluffing is allowed.
	if (!gameData.ruleSet->getNoBluffingRule())
	{
		_buttonList.emplace_back(Btn(sf::IntRect(centre.x - 150, centre.y + 100, 100, 40), "Challenge", 1, font));
	}
	_buttonList.emplace_back(Btn(sf::IntRect(centre.x + 50, centre.y + 100, 100, 40), "Decline", 0, font));

	_allowStacking = gameData.ruleSet->canStackCards();
	_playerReference = gameData.bottomPlayer;
}

void Challenge_Pop::draw(sf::RenderWindow &renderWindow) const
{
	for (const auto &button : _buttonList)
		button.draw(renderWindow);
}

void Challenge_Pop::handleMouseMove(const sf::Vector2i &mousePosition)
{
	if (!isEnabled())
		return;

	for (auto &button : _buttonList)
		button.setHovering(button.isPositionInside(mousePosition));
}

void Challenge_Pop::handleMousePress(const sf::Vector2i &mousePosition, bool isLeft)
{
	// printf("In ChallengeOverlay::handleMousePress");
	if (!isEnabled())
		return;

	// Test if a button was clicked and return a non-chaining result.
	for (const auto &button : _buttonList)
	{
		if (button.isPositionInside(mousePosition))
		{
			Pop_by_Act->injectProperty("isChaining", 0);
			Pop_by_Act->injectFlagProperty(button.getActionID());
			setEnabled(false);
			return;
		}
	}

	// If stacking is allowed and a draw 4 card has been clicked then inject properties to force a chain.
	if (_allowStacking)
	{
		auto clickedCard = _playerReference->chooseCardFromClick(mousePosition);
		if (clickedCard == nullptr)
		{
			std::cerr << "Error: clickedCard is nullptr!" << std::endl;
			return;
		}
		if (clickedCard != nullptr && clickedCard->getFaceValueID() == 13)
		{
			Pop_by_Act->injectProperty("faceValueID", clickedCard->getFaceValueID());
			Pop_by_Act->injectProperty("colourID", clickedCard->getColourID());
			Pop_by_Act->injectProperty("cardID", clickedCard->getUniqueCardID());
			Pop_by_Act->injectProperty("isChaining", 1);
			Pop_by_Act->injectFlagProperty(0);
			setEnabled(false);
		}
	}
}

void Challenge_Pop::showOverlay(TurnDecisionAction *currentAction)
{
	Pop_by_Act = currentAction;
	setEnabled(true);
}
