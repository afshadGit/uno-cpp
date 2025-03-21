#include "Card_Deck.h"
#include <algorithm> // std::shuffle

// constructor including the position of the deck, the font used for the deck title, and the random engine used for shuffling
Card_Deck::Card_Deck(const sf::Vector2f position, const sf::Font &font, std::default_random_engine &randomEngine)
	: _position(position), Inter_Rect(sf::IntRect(static_cast<int>(position.x), static_cast<int>(position.y), CARD_WIDTH, CARD_HEIGHT)),
	  _deckTitle("DECK", font, 20, sf::Color::Blue, sf::Text::Bold),
	  _randomEngine(randomEngine), _font(font)
{
	_nextCardID = 0;
	_cardBack = std::make_shared<CardBack_GroupObject>(sf::Vector2f(0, 0), font);
	_deckTitle.setPositionWithOffset(sf::Vector2f(position.x + CARD_WIDTH / 2 - _deckTitle.getTextWidth() / 2, position.y - 4 - 20));
}

Card_Deck::~Card_Deck()
{
	for (auto p : _deck)
		delete p;
}

// draws the back of a card to represent the deck position, with the word "DECK" appearing above it
void Card_Deck::draw(sf::RenderWindow &renderWindow) const
{
	_cardBack->drawAt(_position, renderWindow);
	_deckTitle.draw(renderWindow);
}

Cards *Card_Deck::drawCard()
{
	if (_deck.empty())
		fillDeck();

	if (_deck.empty()){
        std::cerr << "Error: Deck is still empty after refill. Cannot draw a card!" << std::endl;
        return nullptr; // Return nullptr to signal failure
    }
	// Cards *drawnCard = _deck.at(_deck.size() - 1);
	Cards *drawnCard = _deck.back();
	_deck.pop_back();
	return drawnCard;
	
}

void Card_Deck::fillDeck()
{
	// Delete all elements
	for (auto p : _deck)
		delete p;
	_deck.clear();

	// for each colour
	for (int colourID = 0; colourID < 4; colourID++)
	{
		// Only 1x"0"
		addCard(0, colourID);
		// Two of 1 to 9, Draw Two, Skip, and Reverse
		for (int faceValue = 1; faceValue <= 12; faceValue++)
		{
			addCard(faceValue, colourID);
			addCard(faceValue, colourID);
		}
	}
	// Four of each Wild and Draw 4 Wild.
	for (int i = 0; i < 4; i++)
	{
		addCard(13, 4);
		addCard(14, 4);
	}

	std::shuffle(_deck.begin(), _deck.end(), _randomEngine);
}

void Card_Deck::addCard(const int faceValueID, const int colourID)
{
	_deck.emplace_back(new Cards(faceValueID, colourID, _nextCardID, _cardBack, _font));
	++_nextCardID;
}
