#pragma once
#include "Draw_Obj.h"

class Draw_Txt : public Draw_Obj
{
	// Reference to the object wrapped by this class.
	sf::Text _textObject;

public:
	// Sets the initial properties applied to the sf::Text object.
	Draw_Txt(const std::string &str, const sf::Font &font, const size_t fontSize, const sf::Color textColour, const sf::Uint32 style);
	virtual ~Draw_Txt() = default;

	// Draws the text to the renderWindow.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Sets the position by adding the object's offset to the specified position.
	virtual void setPositionWithOffset(const sf::Vector2f &position) override;

	// Moves the object by the specified offset without consideration for any other factors.
	virtual void move(const sf::Vector2f &offset) override;

	// Gets the width of the text based on the font and other settings.
	float getTextWidth() const;

	// Sets the text to the specified value.
	void setText(const std::string &text);

	// Sets the text colour to the specified value.
	void setColour(const sf::Color &colour);
};
