#include "CardBackGroupObject.h"

#include "Card.h"
#include "Draw_Shp.h"
#include "Round_shp.h"
#include "Draw_Txt.h"

CardBack_GroupObject::CardBack_GroupObject(const sf::Vector2f &initialPosition, const sf::Font &font)
{
	// Black background with white border
	Draw_Shp *background = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(CARD_WIDTH - 4, CARD_HEIGHT - 4)), sf::Color::Black, sf::Vector2f(2, 2));
	background->setBorder(2, sf::Color::White);
	add_Child(background);

	Draw_Shp *centreOval = new Draw_Shp(new Round_shp(sf::Vector2f((CARD_WIDTH - 8) / 2, (CARD_WIDTH - 8) / 4 + 1)), sf::Color(147, 44, 44), sf::Vector2f(4, CARD_HEIGHT / 2 - (CARD_WIDTH - 8) / 4));
	add_Child(centreOval);

	Draw_Txt *shadowText = new Draw_Txt("UNO", font, 20, sf::Color::Black, sf::Text::Bold);
	shadowText->setOffset(sf::Vector2f(CARD_WIDTH / 2 - shadowText->getTextWidth() / 2 - 2, CARD_HEIGHT / 2 - 20 / 2 - 2));
	add_Child(shadowText);

	Draw_Txt *mainText = new Draw_Txt("UNO", font, 20, sf::Color(226, 173, 67), sf::Text::Bold);
	mainText->setOffset(sf::Vector2f(CARD_WIDTH / 2 - mainText->getTextWidth() / 2, CARD_HEIGHT / 2 - 20 / 2));
	add_Child(mainText);

	// This line is essential updates all elements to apply the offset based on a relative position.
	setPositionWithOffset(initialPosition);
}

void CardBack_GroupObject::drawAt(const sf::Vector2f &position, sf::RenderWindow &renderWindow)
{
	setPositionWithOffset(position);
	draw(renderWindow);
}
