#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Inter_Rect.h"
#include "CardFrontObjectGroup.h"
#include "CardBackGroupObject.h"

// Constant definition of the width of a card to be used for calculations.
#define CARD_WIDTH 60
// Constant definition of the height of a card to be used for calculations.
#define CARD_HEIGHT 90

class Cards : public Inter_Rect
{
	// The label in the centre of the card.
	std::string card_Label;

	// The label in both corners of the card.
	std::string Card_corner_Label;

	/* The faceValue to represent what type of number or other visual appearance the card has.
		0 to 9 are numbers, 10 = Draw Two, 11 = Skip, 12 = Reverse, 13 = Draw Four, 14 = Wild
		Face value is used when the object is created to initialise the labels.	*/
	const int _faceValueID;

	// The unique ID based on order drawn from the deck.
	const int _uniqueCardID;

	// The ID used to determine which of the four colours it is (or 4 if the card is a wild without colour set yet).
	int card_colourID;

	// The colour used for drawing based on the colourID.
	sf::Color _drawColour;

	// Shared reference to the card back.
	const std::shared_ptr<CardBack_GroupObject> &_cardBack;

	// Card back for when the card is visible.
	std::unique_ptr<CardFrontObjectGroup> _cardFront;

	// Reference to the font for regenerating the card front if the colour is changed.
	const sf::Font &_font;

public:
	// Creates a card with the specified properties.
	Cards(const int faceValueID, const int colourID, const int cardID, const std::shared_ptr<CardBack_GroupObject> &cardBack, const sf::Font &font);

	// Draws the card face up based on properties and type of card.
	void drawCardFront(sf::RenderWindow &renderWindow) const;

	// Uses a shared card back to draw the card's generic back side.
	void drawCardBack(sf::RenderWindow &renderWindow) const;

	// Sets the colour and the colour used to draw.
	void setColour(const int colourID);

	// Gets the current colour on the card. 0=Red, 1=Blue, 2=Green, 3=Yellow, 4=Wild
	int getColourID() const;

	// Gets the faceValueID of the card. 0 to 9 are numbers, 10 = Draw Two, 11 = Skip, 12 = Reverse, 13 = Draw Four, 14 = Wild
	int getFaceValueID() const;

	// Gets the unique number that represents only this card.
	int getUniqueCardID() const;

	// Gets the score based on the faceValue of the card.
	// Numbered cards are their face value, wild and +4 are worth 50,
	// and others are worth 20.
	int getScoreValue() const;

	sf::Color getDrawColour() const;

	std::string getCardLabel() const;

	std::string getCornerLabel() const;

	// Gets a mapped colour for the colourID or Black.
	static sf::Color getColourByID(const int colourID);

	/* Generates a string to be used for the central label on the card based on faceValue.
		"0" to "9", "Draw Two", "Skip", "Reverse", "Draw Four", "Wild", else "ERROR"*/
	static std::string getLabelByFaceValue(const int faceValue);

	/* Generates a string to be used for the outer corners on the card based on faceValue.
	"+2", "+4" "" if wild, or the text from getLabelByFaceValue().*/
	static std::string getCornerLabelByFaceValue(const int faceValue);

	// Sets the position to the position specified.
	void setPosition(const sf::Vector2i &newPosition);

	~Cards();
};
