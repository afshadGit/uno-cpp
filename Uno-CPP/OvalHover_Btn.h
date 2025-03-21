#pragma once

#include "Game_Scr.h"
#include "Draw_Shp.h"
#include "Draw_Obj_Grp.h"

class OvalHover_Btn : public Game_Scr
{
	// Background colour
	sf::Color _bgColour;

	// Shape that increases in size when hovered.
	Draw_Shp *_hoverShape;

	// Text shown on top of the oval.
	Draw_Obj_Grp *_text;

protected:
	// Current hover status of the button.
	bool _isHovered;

	// When isActive is active the button can be interacted with and is visible.
	bool _isActive;

	// The triggered state to know when the button has been interacted with.
	bool _isTriggered;

	// The action id associated with the triggering
	int _actionID;

public:
	/*
		Initialises the button with bounds, font, message, and colour.
	*/
	OvalHover_Btn(const sf::IntRect &bounds, const sf::Font &font, const std::string &message, const sf::Color &bgColour);
	virtual ~OvalHover_Btn();

	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override;

	// Draws the button with an expanding oval on hover with the text in the middle.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Gets the current state of isTriggered and forces it to reset to false.
	bool isTriggeredReset();

	// Gets the actionID for this button.
	int getActionID() const;
};
