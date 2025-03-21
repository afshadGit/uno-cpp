#include "Btn.h"

Btn::Btn(const sf::IntRect & bounds, const std::string & text, const int actionID, const sf::Font& font)
	: Inter_Rect(bounds), Btn_text(text), Btn_action_ID(actionID)
{
	Btn_isHovered = false;

	initialiseDefaultView(font);
	initialiseHoveredView(font);
}

void Btn::draw(sf::RenderWindow & renderWindow) const
{
	if (Btn_isHovered) {
		_hoveredView.draw(renderWindow);
	}
	else {
		_defaultView.draw(renderWindow);
	}
}

int Btn::getActionID() const
{
	return Btn_action_ID;
}

void Btn::setHovering(const bool isHovering)
{
	Btn_isHovered = isHovering;
}

void Btn::initialiseDefaultView(const sf::Font& font)
{
	Draw_Shp* background = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(_bounds.width-2, _bounds.height-2)), sf::Color(123, 133, 163), sf::Vector2f(1,1));
	background->setBorder(1, sf::Color::Black);
	_defaultView.add_Child(background);
	Draw_Txt* text = new Draw_Txt(Btn_text, font, 20, sf::Color::Black, sf::Text::Bold);
	text->setOffset(sf::Vector2f(_bounds.width / 2 - text->getTextWidth() / 2, _bounds.height / 2 - 20 + 8));
	_defaultView.add_Child(text);
	_defaultView.setPositionWithOffset(sf::Vector2f(_bounds.left, _bounds.top));
}

void Btn::initialiseHoveredView(const sf::Font & font)
{
	Draw_Shp* background = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(_bounds.width - 4, _bounds.height - 4)), sf::Color(123, 133, 163), sf::Vector2f(2, 2));
	background->setBorder(3, sf::Color::Blue);
	_hoveredView.add_Child(background);
	Draw_Txt* text = new Draw_Txt(Btn_text, font, 20, sf::Color::Blue, sf::Text::Bold);
	text->setOffset(sf::Vector2f(_bounds.width / 2 - text->getTextWidth() / 2, _bounds.height / 2 - 20 + 8));
	_hoveredView.add_Child(text);
	_hoveredView.setPositionWithOffset(sf::Vector2f(_bounds.left, _bounds.top));
}
