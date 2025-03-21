#pragma once
#include "Inter_Rect.h"
#include "Draw_Obj_Grp.h"
#include "Draw_Shp.h"
#include "Draw_Txt.h"

class Btn : public Inter_Rect
{
	//a number used to detect when button is clicked.
	int Btn_action_ID;

	bool Btn_isHovered; // check for mouse is over button

	// Text to centre in the button.
	std::string Btn_text;

	// views the button in default appearance and changes it when hovered over
	Draw_Obj_Grp _defaultView, _hoveredView;

	// Initialises the default view of the button stored in _defaultValue.
	void initialiseDefaultView(const sf::Font &font);

	// Initialises the view when the button is hovered over in _hoveredView.
	void initialiseHoveredView(const sf::Font &font);

public:
	// ets up the button ready to display and interact with.
	Btn(const sf::IntRect &bounds, const std::string &text, const int actionID, const sf::Font &font);

	// Draws the button
	void draw(sf::RenderWindow &renderWindow) const;

	int getActionID() const; // Returns the action ID of the button.

	// Updates the hovering state to the specified value.
	void setHovering(const bool isHovering);
	
	virtual ~Btn() = default; // Destructor
};
