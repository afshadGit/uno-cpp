#pragma once

#include "Draw_Obj.h"

class Draw_Path : public Draw_Obj
{
	// Number of points
	std::size_t _elementCount;

	// Points to draw
	sf::Vertex *line;

	// Initialises the line by packaging vertexList into sf::Vertex objects.
	sf::Vertex *getVertexList(std::initializer_list<sf::Vector2f> vertexList);

public:
	// Creates a line made up of the vextexList points. This line can not be moved.
	Draw_Path(std::initializer_list<sf::Vector2f> vertexList);
	virtual ~Draw_Path();

	// Draw the line.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	virtual void setPositionWithOffset(const sf::Vector2f &position) override {}

	virtual void move(const sf::Vector2f &offset) override {}
};
