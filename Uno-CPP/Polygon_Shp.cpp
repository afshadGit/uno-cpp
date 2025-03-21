#include "Polygon_Shp.h"

Polygon_Shp::Polygon_Shp(const std::vector<sf::Vector2f> &pointList) : points_list(pointList)
{
	update();
}

std::size_t Polygon_Shp::getPointCount() const { return points_list.size(); }

sf::Vector2f Polygon_Shp::getPoint(std::size_t index) const { return points_list.at(index); }
