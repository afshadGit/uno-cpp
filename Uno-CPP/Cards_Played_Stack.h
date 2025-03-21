#pragma once

#include "Card.h"
#include <random>

class Cards_Played_Stack
{
	// A history of cards that have been played.
	std::vector<Cards *> recent_CardsList;

	// Centre point used to randomly spread cards around.
	sf::Vector2f center_CardPos;

	// Reference to the random engine for randomising card colours and positions.
	std::default_random_engine &_randomEngine;

public:
	// Initialises the empty pile at the specified position.
	Cards_Played_Stack(const sf::Vector2f centredCardPos, std::default_random_engine &randomEngine);

	// Draw the cards on the pile.
	void draw(sf::RenderWindow &renderWindow) const;

	// Changes the top card colour. Used for changing the colour of the wild and +4 cards.
	void setTopCardColour(const int colourID);

	void placeCard(Cards *card);

	void forcePlayCard(Cards *card);

	// Gets the most recently played recentCard.
	Cards *get_Front_Card() const;

	// Gets the card below the top card.
	Cards *getCardBelowTop() const;

	virtual ~Cards_Played_Stack(); // Destructor
}; 
