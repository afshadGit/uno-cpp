#pragma once

#include <SFML/Graphics.hpp>

class Round_shp : public sf::Shape
{
	// Current radius of the shape.
	sf::Vector2f _radius;
public:
	// Initialises the EllipseShape with either a specified radius or (0.0,0.0) radius.
	explicit Round_shp(const sf::Vector2f &radius = sf::Vector2f(0.f, 0.f));
	~Round_shp();

	// Sets the radius to the specified values and updates the shape.
	void setRadius(const sf::Vector2f &radius);

	// Gets the current radius for the shape.
	const sf::Vector2f &getRadius() const;

	// Gets the number of points (required by sf::Shape)
	virtual std::size_t getPointCount() const;

	// Calculates the position of a point equally spaces around the ellipse based on index (required by sf::Shape)
	virtual sf::Vector2f getPoint(std::size_t index) const;
};
