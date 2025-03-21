#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random> // std::default_random_engine
#include "Card.h"
#include "CardBackGroupObject.h"
#include "Draw_Txt.h"
#include "Inter_Rect.h"

class Card_Deck : public Inter_Rect
{
	// random engine used for shuffling the deck.
	std::default_random_engine &_randomEngine;

	// position where the card in deck is located. Used for drawing the card back.
	sf::Vector2f _position;

	// The list of cards currently in the deck.
	std::vector<Cards *> _deck;

	// Shared reference to the card back.
	std::shared_ptr<CardBack_GroupObject> _cardBack;

	// Used to give every card a unique cardID.
	int _nextCardID;

	Draw_Txt _deckTitle;

	const sf::Font &_font;

	void fillDeck(); // Fills the deck with a new deck of cards.

	// Adds a card matching the faceValueID and colourID to the _deck.
	void addCard(const int faceValueID, const int colourID);

public:
	// initialises the deck with 0 cards initially.
	Card_Deck(const sf::Vector2f position, const sf::Font &font, std::default_random_engine &randomEngine);
	virtual ~Card_Deck();

	// Draws the back of a card to represent the deck position with the word "DECK" appearing above it.
	void draw(sf::RenderWindow &renderWindow) const;

	// If the deck is empty it is populated with a new deck. Then a card is removed from the deck and returned.
	Cards *drawCard();
};
