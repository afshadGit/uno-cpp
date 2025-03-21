#include "Draw_Obj_Grp.h"

void Draw_Obj_Grp::draw(sf::RenderWindow &renderWindow) const
{
	for (auto obj : _childObjects)
		obj->draw(renderWindow);
}

void Draw_Obj_Grp::setPositionWithOffset(const sf::Vector2f &position)
{
	for (auto obj : _childObjects)
		obj->setPositionWithOffset(position);
}

void Draw_Obj_Grp::move(const sf::Vector2f &offset)
{
	for (auto obj : _childObjects)
		obj->move(offset);
}

void Draw_Obj_Grp::add_Child(Draw_Obj *child, const size_t index)
{
	if (index >= _childObjects.size())
		_childObjects.emplace_back(child);
	else
		_childObjects.insert(_childObjects.begin() + index, child);
}

void Draw_Obj_Grp::add_Child(Draw_Obj *child)
{
	add_Child(child, _childObjects.size());
}

Draw_Obj *Draw_Obj_Grp::get_Child(const size_t index)
{
	return _childObjects.at(index);
}

Draw_Obj_Grp::~Draw_Obj_Grp() {}