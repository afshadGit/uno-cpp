#pragma once

#include <SFML/Graphics.hpp>

class Polygon_Shp : public sf::Shape
{
	// The list of points drawn for this polygon.
	std::vector<sf::Vector2f> points_list;

public:
	// Sets up the shape with a point list of all points used to draw the polygon.
	Polygon_Shp(const std::vector<sf::Vector2f> &pointList);

	// Gets the point at a given index in the polygon (required by sf::Shape)
	virtual sf::Vector2f getPoint(std::size_t index) const;

	// Gets the number of points (required by sf::Shape)
	virtual std::size_t getPointCount() const;

	virtual ~Polygon_Shp() = default;
};
