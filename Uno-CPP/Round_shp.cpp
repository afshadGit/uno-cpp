#include "Round_shp.h"
#include <cmath>

Round_shp::Round_shp(const sf::Vector2f &radius) : _radius(radius)
{
	update();
}

void Round_shp::setRadius(const sf::Vector2f &radius)
{
	_radius = radius;
	update();
}

const sf::Vector2f &Round_shp::getRadius() const
{
	return _radius;
}

std::size_t Round_shp::getPointCount() const
{
	return 30;
}

sf::Vector2f Round_shp::getPoint(std::size_t index) const
{
	static const float pi = 3.141592654f;

	float angle = index * 2 * pi / getPointCount() - pi / 2;
	float x = std::cos(angle) * _radius.x;
	float y = std::sin(angle) * _radius.y;

	return sf::Vector2f(_radius.x + x, _radius.y + y);
}

Round_shp::~Round_shp() {}