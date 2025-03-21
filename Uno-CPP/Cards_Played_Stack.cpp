#include "Cards_Played_Stack.h"

Cards_Played_Stack::Cards_Played_Stack(const sf::Vector2f centredCardPos, std::default_random_engine &randomEngine) : center_CardPos(centredCardPos), _randomEngine(randomEngine) {}

Cards_Played_Stack::~Cards_Played_Stack()
{
	for (auto p : recent_CardsList)
		delete p;
}

void Cards_Played_Stack::draw(sf::RenderWindow &renderWindow) const
{
	for (const auto &card : recent_CardsList)
		card->drawCardFront(renderWindow);
}

void Cards_Played_Stack::setTopCardColour(const int colourID)
{
	recent_CardsList.at(recent_CardsList.size() - 1)->setColour(colourID);
}

void Cards_Played_Stack::placeCard(Cards *card)
{
	card->setPosition(sf::Vector2i(center_CardPos.x + _randomEngine() % 24 - 12, center_CardPos.y + _randomEngine() % 24 - 12));
	recent_CardsList.emplace_back(card);
	int MAX_CARD_HISTORY = 10;
	if (recent_CardsList.size() > MAX_CARD_HISTORY)
	{
		Cards *oldCard = recent_CardsList.at(0);
		recent_CardsList.erase(recent_CardsList.begin());
		delete oldCard;
	}
}

void Cards_Played_Stack::forcePlayCard(Cards *card)
{
	placeCard(card);

	if (card->getFaceValueID() >= 13)
		setTopCardColour(_randomEngine() % 4);
}

Cards *Cards_Played_Stack::get_Front_Card() const { return recent_CardsList.at(recent_CardsList.size() - 1); }

Cards *Cards_Played_Stack::getCardBelowTop() const { return recent_CardsList.at(recent_CardsList.size() - 2); }
