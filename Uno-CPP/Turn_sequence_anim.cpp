#include "Turn_sequence_anim.h"
#include <cmath>

Turn_sequence_anim::Turn_sequence_anim(const sf::Vector2f centre, const int radiusFromCentre, const int indicatorSize)
	: Center(centre), radius_From_Center(radiusFromCentre),
	  Dot_1(indicatorSize), Dot_2(indicatorSize)
{
	Angle = 0;
	_isIncreasing = true;
	Dot_1.setFillColor(sf::Color::Red);
	Dot_2.setFillColor(sf::Color::Red);
}

void Turn_sequence_anim::update(const float deltaTime)
{
	if (_isIncreasing)
		Angle += deltaTime;

	else
		Angle -= deltaTime;

	if (Angle > 3.141592654f * 2 || Angle < -3.141592654f * 2)
		Angle = 0;

	Dot_1.setPosition(sf::Vector2f(Center.x + radius_From_Center * cos(Angle), Center.y + radius_From_Center * sin(Angle)));
	Dot_2.setPosition(sf::Vector2f(Center.x - radius_From_Center * cos(Angle), Center.y - radius_From_Center * sin(Angle)));
}

void Turn_sequence_anim::draw(sf::RenderWindow &renderWindow) const
{
	renderWindow.draw(Dot_1);
	renderWindow.draw(Dot_2);
}

void Turn_sequence_anim::setIsIncreasing(bool isIncreasing)
{
	_isIncreasing = isIncreasing;
}
