#include "Arc_shp.h"
#include <cmath>
Arc_shp::Arc_shp(const float startAngle, const float endAngle, const sf::Vector2f& radius = sf::Vector2f(0.f, 0.f))
	: _radius(radius), startAngle(startAngle * 3.141592654f / 180), endAngle(endAngle * 3.141592654f / 180) {
	pointCount = 30;
	update();
}

void Arc_shp::setRadius(const sf::Vector2f & radius) {
	_radius = radius;
	update();
}

const sf::Vector2f & Arc_shp::getRadius() const {
	return _radius;
}

std::size_t Arc_shp::getPointCount() const {
	return pointCount;
}

sf::Vector2f Arc_shp::getPoint(std::size_t index) const {
	static const float pi = 3.141592654f;

	if (index == pointCount-1) {
		return sf::Vector2f(_radius.x, _radius.y);
	}

	float angle = index * 2 * pi / (getPointCount()-1) + startAngle;
	if (angle > endAngle) {
		angle = endAngle;
	}
	
	float x = std::cos(angle) * _radius.x;
	float y = std::sin(angle) * _radius.y;
	return sf::Vector2f(_radius.x + x, _radius.y + y);
}
