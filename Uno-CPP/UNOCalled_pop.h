#pragma once
#include <cmath>

#include "PlayerFlash_Pop.h"

class UNOCalled_pop : public PlayerFlash_Pop
{
	std::vector<sf::Text> _UNO_Txt;
public:
	// Sets up the overlay ready to show.
	UNOCalled_pop(sf::Vector2f position, const sf::Font& font);

	// Draws the UNO! text flashing with showing 75% of the time.
	void draw(sf::RenderWindow & renderWindow) const override;

	virtual ~UNOCalled_pop();
};

