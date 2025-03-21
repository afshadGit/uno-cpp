#include "Draw_Shp.h"


void Draw_Shp::draw(sf::RenderWindow& renderWindow) const
{
	renderWindow.draw(*_shape);
}

void Draw_Shp::setPositionWithOffset(const sf::Vector2f & position)
{
	_shape->setPosition(sf::Vector2f(position.x + _offset.x, position.y + _offset.y));
}

void Draw_Shp::move(const sf::Vector2f & moveOffset)
{
	_shape->move(moveOffset);
}

void Draw_Shp::setColour(const sf::Color & colour)
{
	_shape->setFillColor(colour);
}

void Draw_Shp::setBorder(const float thickness, const sf::Color colour)
{
	_shape->setOutlineThickness(thickness);
	_shape->setOutlineColor(colour);
}

void Draw_Shp::setSize(const sf::Vector2f size)
{
	auto rect = _shape->getTextureRect();
	rect.height += size.y;
	rect.width += size.x;
	_shape->setTextureRect(rect);
}

Draw_Shp::~Draw_Shp()
{
	delete _shape;
}