#include "Draw_Path.h"

Draw_Path::Draw_Path(std::initializer_list<sf::Vector2f> vertexList)
{
	_elementCount = vertexList.size();
	line = getVertexList(vertexList);
}

void Draw_Path::draw(sf::RenderWindow &renderWindow) const
{
	renderWindow.draw(line, _elementCount, sf::Lines);
}

sf::Vertex *Draw_Path::getVertexList(std::initializer_list<sf::Vector2f> vertexList)
{
	sf::Vertex *result = new sf::Vertex[vertexList.size()]; 
	size_t insertPos = 0;
	for (const auto &pos : vertexList)
	{
		result[insertPos] = sf::Vertex(pos);
		result[insertPos].color = sf::Color::Black;
		++insertPos;
	}
	return result;
}

Draw_Path::~Draw_Path()
{
	delete[] line;
}