#pragma once

#include "Game_Scr.h"
#include "General_pop_Scr.h"

class PlayerFlash_Pop : public Game_Scr, public General_pop_Scr
{
protected:
	// The text that is drawn by this overlay.
	sf::Text _flashingText;
	// Same text, but black for a shadow.
	sf::Text _shadowText;
	// Centre of where the text should be.
	sf::Vector2f _position;
	// Timer till the overlay is hidden again.
	float _displayTimer;

public:
	PlayerFlash_Pop(sf::Vector2f position, const std::string &message, const sf::Color &colour, const int fontSize, const sf::Font &font);
	virtual ~PlayerFlash_Pop();

	void update(const float deltaTime) override;

	// Draws the message.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Shows the overlay and sets a timer for how long it will appear.
	void showOverlay() override;

	void setMessage(const std::string &message);
};
