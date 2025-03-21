#pragma once

#include <SFML/Graphics.hpp>

class Turn_sequence_anim
{
	// Centre of the orbit.
	const sf::Vector2f Center;
	// Distance the orbs are spaced out from the centre.
	const int radius_From_Center;
	// The current angle of the orbit.
	float Angle;
	// Direction of the orbit.
	bool _isIncreasing;
	// Object visuals
	sf::CircleShape Dot_1, Dot_2;

public:
	
	// Initialises the ovals to represent showing play direction.
	Turn_sequence_anim(const sf::Vector2f centre, const int radiusFromCentre, const int indicatorSize);
	
	// Moves the two ovals around in a circle motion around the centre.	
	void update(const float deltaTime);

	// Draws the two ovals representing play direction.
	void draw(sf::RenderWindow &renderWindow) const;
	
	// Changes the direction of the visual.
	void setIsIncreasing(bool isIncreasing);

	virtual ~Turn_sequence_anim() = default;
};
