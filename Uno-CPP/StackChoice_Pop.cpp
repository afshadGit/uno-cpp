#include "StackChoice_Pop.h"

StackChoice_Pop::StackChoice_Pop(const sf::IntRect & bounds, const sf::Font & font, const GameStateData& gameData)
	: Game_Scr(bounds)
{
	setEnabled(false);
	Cross_Button = new Btn(sf::IntRect(bounds.left + bounds.width / 2 - 50, bounds.top + bounds.height / 2 + 100, 100, 40), "Decline", 0, font);

	_playerReference = gameData.bottomPlayer;
}

StackChoice_Pop::~StackChoice_Pop()
{
	delete Cross_Button;
}

void StackChoice_Pop::draw(sf::RenderWindow & renderWindow) const
{
	Cross_Button->draw(renderWindow);
}

void StackChoice_Pop::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	if (Cross_Button->isPositionInside(mousePosition)) {
		_currentAction->injectFlagProperty(0);
		setEnabled(false);
		return;
	}

	auto clickedCard = _playerReference->chooseCardFromClick(mousePosition);
	if (clickedCard != nullptr && clickedCard->getFaceValueID() == 10) {
		_currentAction->injectProperty("faceValueID", clickedCard->getFaceValueID());
		_currentAction->injectProperty("colourID", clickedCard->getColourID());
		_currentAction->injectProperty("cardID", clickedCard->getUniqueCardID());
		_currentAction->injectFlagProperty(1);
		setEnabled(false);
	}
}

void StackChoice_Pop::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	Cross_Button->setHovering(Cross_Button->isPositionInside(mousePosition));
}

void StackChoice_Pop::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;
	setEnabled(true);
}
