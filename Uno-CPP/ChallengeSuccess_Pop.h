#pragma once
#include <cmath>

#include "Game_Scr.h"
#include "General_pop_Scr.h"
#include "Polygon_Shp.h"

class ChallengeSuccess_Pop : public Game_Scr, public General_pop_Scr
{
	// Timer till the overlay is hidden again.
	float _displayTimer;

	// A tick that will be flashed using the display timer when overlay is enabled.
	Polygon_Shp *_tickShape;

public:
	// Initialise the interface with bounds and makes it ready to be enabled.
	ChallengeSuccess_Pop(const sf::IntRect &bounds);
	virtual ~ChallengeSuccess_Pop();

	// Updates the timer to hide the overlay and hides it when it hits 0.
	virtual void update(const float deltaTime) override;

	// Draws the tick flashing with showing 75% of the time.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Shows the overlay and sets a timer for how long it will appear.
	virtual void showOverlay() override;
};
