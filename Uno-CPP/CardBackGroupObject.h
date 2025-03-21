#pragma once
#include <memory>

#include "Draw_Obj_Grp.h"

class CardBack_GroupObject : public Draw_Obj_Grp
{
public:
	// initialises the generic Card back visual.
	CardBack_GroupObject(const sf::Vector2f &initialPosition, const sf::Font &font);

	// moves the object to the specified position and then draws it. Use this for drawing instead because the CardBackGroupObject is generic and shared between instances.
	void drawAt(const sf::Vector2f &position, sf::RenderWindow &renderWindow);

	virtual ~CardBack_GroupObject() = default; 	// Destructor
};
