#include "Challenge_Fail_Pop.h"
#include <cmath>

Challenge_Fail_Pop::Challenge_Fail_Pop(const sf::IntRect & bounds)
	: Game_Scr(bounds)
{
	setEnabled(false);
	int x = bounds.left;
	int y = bounds.top;
	int sValX = bounds.width / 8;
	int sValY = bounds.height / 8;

	std::vector<sf::Vector2f> pointList;
	pointList.emplace_back(sf::Vector2f(x + sValX, y + 2 * sValY));
	pointList.emplace_back(sf::Vector2f(x + 2 * sValX, y + sValY));
	pointList.emplace_back(sf::Vector2f(x + 4 * sValX, y + 3 * sValY));
	pointList.emplace_back(sf::Vector2f(x + 6 * sValX, y + sValY));
	pointList.emplace_back(sf::Vector2f(x + 7 * sValX, y + 2 * sValY));
	pointList.emplace_back(sf::Vector2f(x + 5 * sValX, y + 4 * sValY));
	pointList.emplace_back(sf::Vector2f(x + 7 * sValX, y + 6 * sValY));
	pointList.emplace_back(sf::Vector2f(x + 6 * sValX, y + 7 * sValY));
	pointList.emplace_back(sf::Vector2f(x + 4 * sValX, y + 5 * sValY));
	pointList.emplace_back(sf::Vector2f(x + 2 * sValX, y + 7 * sValY));
	pointList.emplace_back(sf::Vector2f(x + sValX, y + 6 * sValY));
	pointList.emplace_back(sf::Vector2f(x + 3 * sValX, y + 4 * sValY));

	_crossShape = new Polygon_Shp(pointList);
	_crossShape->setFillColor(sf::Color(179, 50, 38));
	_crossShape->setOutlineThickness(1);
	_crossShape->setOutlineColor(sf::Color::Black);
}

Challenge_Fail_Pop::~Challenge_Fail_Pop()
{
	delete _crossShape;
}

void Challenge_Fail_Pop::update(const float deltaTime)
{
	_displayTimer -= deltaTime;
	if (_displayTimer <= 0) {
		setEnabled(false);
	}
}

void Challenge_Fail_Pop::draw(sf::RenderWindow & renderWindow) const
{
	if (fmod(_displayTimer, 0.2f) < 0.15f) {
		renderWindow.draw(*_crossShape);
	}
}

void Challenge_Fail_Pop::showOverlay()
{
	setEnabled(true);
	_displayTimer = 2.0f;
}
