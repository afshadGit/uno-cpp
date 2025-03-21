#include "CardFrontObjectGroup.h"
#include "Draw_Shp.h"
#include "Draw_Txt.h"
#include "Round_shp.h"
#include "Arc_shp.h"
#include "Card.h"

CardFrontObjectGroup::CardFrontObjectGroup(const int faceValue, const int colourID, const sf::Vector2f& initialPosition, const sf::Font& font)
{
	// Black background with white border
	Draw_Shp* background = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(CARD_WIDTH - 4, CARD_HEIGHT - 4)), 
													Cards::getColourByID(colourID), sf::Vector2f(2, 2));
	background->setBorder(2, sf::Color::White);
	add_Child(background);

	if (colourID != 4) {
		// White oval in centre of card
		Draw_Shp* centralEllipse = new Draw_Shp(new Round_shp(sf::Vector2f((CARD_WIDTH - 8) / 2, (CARD_WIDTH - 8) / 4)), 
												sf::Color::White, sf::Vector2f(4, CARD_HEIGHT / 2 - ((CARD_WIDTH - 8) / 4)));
		add_Child(centralEllipse);
	}
	else {
		// Red, blue, green, yellow segments for any wild card in the middle.
		for (int i = 0; i < 4; i++) {
			Arc_shp* arcShape = new Arc_shp(90 * i, 90 * (i+1), sf::Vector2f((CARD_WIDTH - 8) / 2, ((CARD_WIDTH - 8) / 4)/2+10));
			Draw_Shp* arc = new Draw_Shp(arcShape, Cards::getColourByID(i), sf::Vector2f(4, CARD_HEIGHT / 2 - ((CARD_WIDTH - 8) / 4)));
			add_Child(arc);
		}
	}

	std::string label = Cards::getLabelByFaceValue(faceValue);
	std::string cornerLabel = Cards::getCornerLabelByFaceValue(faceValue);

	// Shadow behind central text (only shows for wild and short text)
	if (colourID == 4 || label.length() <= 4) {
		size_t fontHeight = (label.length() > 4) ? 10 : 20;
		Draw_Txt* shadowText = new Draw_Txt(label, font, fontHeight, sf::Color::Black, sf::Text::Bold);
		shadowText->setOffset(sf::Vector2f(CARD_WIDTH/2-shadowText->getTextWidth()/2-1, CARD_HEIGHT/2-fontHeight/2-1));
		add_Child(shadowText);
	}

	// Foreground text that changes dependent on shadow and card colour.
	sf::Color textColour = sf::Color::White;
	if (colourID != 4) {
		textColour = label.length() <= 4 ? Cards::getColourByID(colourID) : sf::Color::Black;
	}
	size_t fontHeight = (label.length() > 4) ? 10 : 20;
	Draw_Txt* foregroundText = new Draw_Txt(label, font, fontHeight, textColour, sf::Text::Bold);
	foregroundText->setOffset(sf::Vector2f(CARD_WIDTH / 2 - foregroundText->getTextWidth() / 2, CARD_HEIGHT / 2 - fontHeight / 2));
	add_Child(foregroundText);

	// Labels in each of the corners.	
	fontHeight = (cornerLabel.length() > 2) ? 10 : 20;
	Draw_Txt* topLeftCorner = new Draw_Txt(cornerLabel, font, fontHeight, sf::Color::White, sf::Text::Bold);
	topLeftCorner->setOffset(sf::Vector2f(5, 5));
	add_Child(topLeftCorner);
	Draw_Txt* bottomRightCorner = new Draw_Txt(cornerLabel, font, fontHeight, sf::Color::White, sf::Text::Bold);
	bottomRightCorner->setOffset(sf::Vector2f(CARD_WIDTH-bottomRightCorner->getTextWidth()-5, CARD_HEIGHT-fontHeight-5));
	add_Child(bottomRightCorner);

	// ESSENTIAL! This line updates all elements to apply the offset based on a relative position. 
	setPositionWithOffset(initialPosition);
}
