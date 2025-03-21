#pragma once
#include "Draw_Obj.h"
#include <SFML/Graphics.hpp>

class Draw_Shp : public Draw_Obj
{
	// Reference the managed shape that this class wraps.
	sf::Shape *_shape;

public:
	// Initialises the wrapper by passing in a shape with defined colour and optional offset.
	Draw_Shp(sf::Shape *shape, const sf::Color &colour, const sf::Vector2f &offset = sf::Vector2f(0.0f, 0.0f))
		: Draw_Obj(offset), _shape(shape)
	{
		_shape->setFillColor(colour);
	};
	virtual ~Draw_Shp();

	// Draws the shape to the renderWindow.
	void draw(sf::RenderWindow &renderWindow) const override;

	void setPositionWithOffset(const sf::Vector2f &position) override; // Sets the position of the shape with the offset applied.

	void move(const sf::Vector2f &moveOffset) override; // Moves the shape by the specified offset.

	void setColour(const sf::Color &colour); // Sets the fill colour of the shape.

	void setBorder(const float thickness, const sf::Color colour); // Sets the border of the shape.

	void setSize(const sf::Vector2f size); // Sets the size of the shape.
};
