#include "Keep_Or_Play_Pop.h"
#include "Card.h"

Keep_Or_Play_Pop::Keep_Or_Play_Pop(const sf::IntRect &bounds, const sf::Font &font) : Game_Scr(bounds), _font(font)
{
	setEnabled(false);
	sf::Vector2f centre = sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
	_buttonList.emplace_back(Btn(sf::IntRect(centre.x - 150, centre.y + 100, 100, 40), "Keep", 0, font));
	_buttonList.emplace_back(Btn(sf::IntRect(centre.x + 50, centre.y + 100, 100, 40), "Play", 1, font));
	_cardPosition = sf::Vector2f(centre.x - CARD_WIDTH / 2, centre.y + 100 + 20 - CARD_HEIGHT / 2);
}

Keep_Or_Play_Pop::~Keep_Or_Play_Pop()
{
	if (_cardForChoice != nullptr)
	{
		delete _cardForChoice;
	}
}

void Keep_Or_Play_Pop::draw(sf::RenderWindow &renderWindow) const
{
	for (const auto &button : _buttonList)
	{
		button.draw(renderWindow);
	}
	_cardForChoice->draw(renderWindow);
}

void Keep_Or_Play_Pop::showOverlay(TurnDecisionAction *currentAction)
{
	_currentAction = currentAction;

	if (_cardForChoice != nullptr)
	{
		delete _cardForChoice;
	}
	_cardForChoice = new CardFrontObjectGroup(_currentAction->getPropertyValue("faceValueID"),
											  _currentAction->getPropertyValue("colourID"), _cardPosition, _font);
	setEnabled(true);
}

void Keep_Or_Play_Pop::handleMousePress(const sf::Vector2i &mousePosition, bool isLeft)
{
	if (!isEnabled())
		return;

	for (auto &button : _buttonList)
	{
		if (button.isPositionInside(mousePosition))
		{
			setEnabled(false);
			_currentAction->injectFlagProperty(button.getActionID());
			break;
		}
	}
}

void Keep_Or_Play_Pop::handleMouseMove(const sf::Vector2i &mousePosition)
{
	if (!isEnabled())
		return;

	for (auto &button : _buttonList)
	{
		button.setHovering(button.isPositionInside(mousePosition));
	}
}
