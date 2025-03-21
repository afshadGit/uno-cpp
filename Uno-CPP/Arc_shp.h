#pragma once
#include <SFML/Graphics.hpp>

class Arc_shp : public sf::Shape {
	sf::Vector2f _radius; // Radius of the ellipse

	float startAngle; // Start angle in radians.

	// end angle in radians.
	float endAngle;

	// number of points used to represent the shape.
	std::size_t pointCount;

public:
	// Defines the shape with angles in degrees and radius of the partial ellipse.
	explicit Arc_shp(const float startAngle, const float endAngle, const sf::Vector2f &radius);
	virtual ~Arc_shp() = default;

	// Sets the radius of the ellipse shape that is only partially visible.
	void setRadius(const sf::Vector2f &radius);

	// returns the radius of the ellipse that is fully visible.
	const sf::Vector2f &getRadius() const;

	// returns the number of points used to represent the shape.
	virtual std::size_t getPointCount() const;

	// generates the point at a given index.
	virtual sf::Vector2f getPoint(std::size_t index) const; 
}; 
