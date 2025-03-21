#pragma once
#include "Draw_Obj.h"

class Draw_Obj_Grp : public Draw_Obj
{
	// Collection of child DrawableObjects that are drawn and managed as a group by this object.
	std::vector<Draw_Obj *> _childObjects;
public:
	// Creates an empty group of child objects.
	Draw_Obj_Grp() {}

	// Draws all objects in the order they were added as children.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Sets the position of all objects by adding the object's offset to the specified position.
	virtual void setPositionWithOffset(const sf::Vector2f &position) override;

	// Moves all the objects by the specified offset without consideration for any other factors.
	virtual void move(const sf::Vector2f &offset) override;

	// Adds a child at the specified index. If the index is out of bounds it is forced to the back by default.
	void add_Child(Draw_Obj *child, const size_t index);

	// Adds a child to the back of the list (drawn on top of preceeding children).
	void add_Child(Draw_Obj *child);

	// Gets a reference to the child by the specified index.
	Draw_Obj *get_Child(const size_t index);
	
	virtual ~Draw_Obj_Grp(); // Destructor
};
