#pragma once
#include "Draw_Obj_Grp.h"
#include <SFML/Graphics.hpp>

class CardFrontObjectGroup : public Draw_Obj_Grp
{
public:
	// Generates the visual representation based on specified faceValue and colourID.
	CardFrontObjectGroup(const int faceValue, const int colourID, const sf::Vector2f &initialPosition, const sf::Font &font);
	virtual ~CardFrontObjectGroup() = default; // Destructor
};
