#pragma once

#include <SFML/Graphics.hpp>

class Inter_Rect
{
protected:
	// The bounds of the rectangle used for isPositionInside().
	sf::IntRect _bounds;

public:
	// Passes the bounds for the object for use with isPositionInside().
	explicit Inter_Rect(const sf::IntRect &bounds) : _bounds(bounds) {}
	virtual ~Inter_Rect() = default;

	// returns true if the position is inside this object's bounds.
	bool isPositionInside(const sf::Vector2i &mousePosition) const
	{
		return _bounds.contains(mousePosition);
	}
};
