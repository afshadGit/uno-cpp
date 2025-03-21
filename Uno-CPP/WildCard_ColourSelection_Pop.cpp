#include "WildCard_ColourSelection_Pop.h"
#include "Draw_Txt.h"
#include "Arc_shp.h"
#include "Card.h"

WildCard_ColourSelection_Pop::WildCard_ColourSelection_Pop(const sf::IntRect &bounds, const sf::Font &font) : Game_Scr(bounds), _interactionRect(bounds)
{
	setEnabled(false);

	_background = new Draw_Obj_Grp();
	Draw_Shp *backgroundRect = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(bounds.width + 40 - 2, bounds.height + 60 - 2)), sf::Color::Black, sf::Vector2f(bounds.left - 20 + 1, bounds.top - 40 + 1));
	backgroundRect->setBorder(1, sf::Color::White);
	_background->add_Child(backgroundRect);
	Draw_Txt *title = new Draw_Txt("Choose Colour", font, 20, sf::Color::White, sf::Text::Bold);
	title->setOffset(sf::Vector2f(bounds.left + bounds.width / 2 - title->getTextWidth() / 2, bounds.top - 35));
	_background->add_Child(title);

	for (int i = 0; i < 4; i++)
	{
		Arc_shp *arcShape = new Arc_shp(90 * i, 90 * (i + 1), sf::Vector2f(bounds.width / 2, bounds.height / 2));
		Draw_Shp *arc = new Draw_Shp(arcShape, Cards::getColourByID(i), sf::Vector2f(bounds.left, bounds.top));
		_background->add_Child(arc);
	}
	for (int i = 0; i < 4; i++)
	{
		Arc_shp *arcShape = new Arc_shp(90 * i, 90 * (i + 1), sf::Vector2f(bounds.width / 2 + 10, bounds.height / 2 + 10));
		_hoverArcs.emplace_back(new Draw_Shp(arcShape, Cards::getColourByID(i), sf::Vector2f(bounds.left - 10, bounds.top - 10)));
		_hoverArcs.at(_hoverArcs.size() - 1)->setPositionWithOffset(sf::Vector2f(0, 0));
	}

	_background->setPositionWithOffset(sf::Vector2f(0, 0));
}

WildCard_ColourSelection_Pop::~WildCard_ColourSelection_Pop()
{
	delete _background;
	for (auto p : _hoverArcs)
	{
		delete p;
	}
}

void WildCard_ColourSelection_Pop::draw(sf::RenderWindow &renderWindow) const
{
	_background->draw(renderWindow);
	if (_hoveredRegion != -1)
	{
		_hoverArcs.at(_hoveredRegion)->draw(renderWindow);
	}
}

void WildCard_ColourSelection_Pop::showOverlay(TurnDecisionAction *currentAction)
{
	_currentAction = currentAction;
	setEnabled(true);
}

void WildCard_ColourSelection_Pop::handleMousePress(const sf::Vector2i &mousePosition, bool isLeft)
{
	handleMouseMove(mousePosition);
	if (_hoveredRegion != -1)
	{
		_currentAction->injectProperty("colourID", _hoveredRegion);
		_currentAction->injectFlagProperty(1);
		setEnabled(false);
	}
}

void WildCard_ColourSelection_Pop::handleMouseMove(const sf::Vector2i &mousePosition)
{
	_hoveredRegion = -1;
	if (_interactionRect.isPositionInside(mousePosition))
	{
		_hoverX = (mousePosition.x - _bounds.left) / (_bounds.width / 2);
		_hoverY = (mousePosition.y - _bounds.top) / (_bounds.height / 2);
		if (_hoverX == 0 && _hoverY == 0)
			_hoveredRegion = 2;
		else if (_hoverX == 1 && _hoverY == 0)
			_hoveredRegion = 3;
		else if (_hoverX == 1 && _hoverY == 1)
			_hoveredRegion = 0;
		else if (_hoverX == 0 && _hoverY == 1)
			_hoveredRegion = 1;
	}
}
