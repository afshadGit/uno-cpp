#include "OvalHover_Btn.h"

#include "Round_shp.h"
#include "Draw_Txt.h"

OvalHover_Btn::OvalHover_Btn(const sf::IntRect & bounds, const sf::Font & font, const std::string & message, const sf::Color & bgColour)
	: Game_Scr(bounds), _bgColour(bgColour)
{
	_isHovered = false;
	_isActive = true;

	Draw_Txt* _foreground = new Draw_Txt(message, font, 30, sf::Color(226, 173, 67), sf::Text::Bold);
	int strWidth = _foreground->getTextWidth();
	_foreground->setPositionWithOffset(sf::Vector2f(bounds.left + _bounds.width / 2 - strWidth / 2, bounds.top + _bounds.height / 2 - 20));
	Draw_Txt* _shadow = new Draw_Txt(message, font, 30, sf::Color::Black, sf::Text::Bold);
	_shadow->setPositionWithOffset(sf::Vector2f(bounds.left + _bounds.width / 2 - strWidth / 2 - 2, bounds.top + _bounds.height / 2 - 20 + 2));
	_text = new Draw_Obj_Grp();
	_text->add_Child(_shadow);
	_text->add_Child(_foreground);

	_hoverShape = new Draw_Shp(new Round_shp(sf::Vector2f(_bounds.width / 2, _bounds.height / 2)), _bgColour);
	_hoverShape->setPositionWithOffset(sf::Vector2f(bounds.left, bounds.top));
}

OvalHover_Btn::~OvalHover_Btn()
{
	delete _hoverShape;
	delete _text;
}

void OvalHover_Btn::handleMouseMove(const sf::Vector2i & mousePosition)
{
	bool currentValue = _isHovered;
	_isHovered = _bounds.contains(mousePosition);
	// If the state changed
	if (currentValue != _isHovered) {
		_hoverShape->setBorder(_isHovered ? 10.0f : 0.0f, _bgColour);
	}
}

void OvalHover_Btn::draw(sf::RenderWindow & renderWindow) const
{
	if (!_isActive) return;

	_hoverShape->draw(renderWindow);
	_text->draw(renderWindow);
}

bool OvalHover_Btn::isTriggeredReset()
{
	bool result = _isTriggered;
	_isTriggered = false;
	return result;
}

int OvalHover_Btn::getActionID() const
{
	return _actionID;
}
