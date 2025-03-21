#include "Card.h"

Cards::Cards(const int faceValueID, const int colourID, const int cardID, const std::shared_ptr<CardBack_GroupObject> &cardBack, const sf::Font &font)
	: Inter_Rect(sf::IntRect(0, 0, CARD_WIDTH, CARD_HEIGHT)),
	  _faceValueID(faceValueID), card_colourID(colourID), _uniqueCardID(cardID), _cardBack(cardBack), _font(font)
{
	_drawColour = getColourByID(card_colourID);
	card_Label = getLabelByFaceValue(_faceValueID);
	Card_corner_Label = getCornerLabelByFaceValue(_faceValueID);
	_cardFront = std::make_unique<CardFrontObjectGroup>(faceValueID, colourID, sf::Vector2f(_bounds.left, _bounds.top), font);
}

Cards::~Cards() {}

void Cards::drawCardFront(sf::RenderWindow &renderWindow) const
{
	_cardFront->draw(renderWindow);
}

void Cards::drawCardBack(sf::RenderWindow &renderWindow) const
{
	_cardBack->drawAt(sf::Vector2f(_bounds.left, _bounds.top), renderWindow);
}

void Cards::setColour(const int colourID)
{
	card_colourID = colourID;
	_drawColour = getColourByID(card_colourID);
	// Remake the card.
	_cardFront.release();
	_cardFront = std::make_unique<CardFrontObjectGroup>(_faceValueID, colourID, sf::Vector2f(_bounds.left, _bounds.top), _font);
}

int Cards::getColourID() const { return card_colourID; }

int Cards::getFaceValueID() const { return _faceValueID; }

int Cards::getUniqueCardID() const { return _uniqueCardID; }

int Cards::getScoreValue() const
{
	if (_faceValueID < 10)
	{
		return _faceValueID;
	}
	else if (_faceValueID == 13 || _faceValueID == 14)
	{
		return 50;
	}
	else
	{
		return 20;
	}
}

sf::Color Cards::getDrawColour() const
{
	return _drawColour;
}

std::string Cards::getCardLabel() const
{
	return card_Label;
}

std::string Cards::getCornerLabel() const
{
	return Card_corner_Label;
}

sf::Color Cards::getColourByID(const int colourID)
{
	switch (colourID)
	{
	case 0:
		return sf::Color(191, 48, 48); // Red
	case 1:
		return sf::Color(36, 94, 160); // Blue
	case 2:
		return sf::Color(115, 187, 54); // Green
	case 3:
		return sf::Color(238, 188, 65); // Yellow
	default:
		return sf::Color(0, 0, 0); // Black
	};
}

std::string Cards::getLabelByFaceValue(const int faceValue)
{
	if (faceValue >= 0 && faceValue <= 9)
	{
		return std::to_string(faceValue);
	}

	switch (faceValue)
	{
	case 10:
		return "Draw Two";
	case 11:
		return "Skip";
	case 12:
		return "Reverse";
	case 13:
		return "Draw Four";
	case 14:
		return "Wild";
	default:
		return "ERROR";
	}
}

std::string Cards::getCornerLabelByFaceValue(const int faceValue)
{
	switch (faceValue)
	{
	case 10:
		return "+2";
	case 13:
		return "+4";
	case 14:
		return "";
	default:
		return getLabelByFaceValue(faceValue);
	}
}

void Cards::setPosition(const sf::Vector2i &newPosition)
{
	_bounds = sf::IntRect(newPosition.x, newPosition.y, _bounds.width, _bounds.height);
	_cardFront->setPositionWithOffset(sf::Vector2f(newPosition.x, newPosition.y));
}
