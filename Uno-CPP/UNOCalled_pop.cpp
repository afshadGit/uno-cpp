#include "UNOCalled_pop.h"
#include "Card.h"

UNOCalled_pop::UNOCalled_pop(sf::Vector2f position, const sf::Font &font)
	: PlayerFlash_Pop(position, "UNO!", sf::Color::Black, 40, font)
{
	std::string message = "UNO!";
	for (int i = 0; i < message.length(); i++)
	{
		_UNO_Txt.emplace_back(sf::Text(message.at(i), font, 40));
		_UNO_Txt.at(i).setFillColor(Cards::getColourByID(i % 4));
		_UNO_Txt.at(i).setPosition(position.x + 2 + i * 30, position.y + 2);
	}
	_shadowText.setPosition(position);
}

UNOCalled_pop::~UNOCalled_pop() {}

void UNOCalled_pop::draw(sf::RenderWindow &renderWindow) const
{
	if (fmod(_displayTimer, 0.2f) < 0.15f)
	{
		renderWindow.draw(_shadowText);
		for (const auto &letter : _UNO_Txt)

			renderWindow.draw(letter);
	}
}
