#include "ChallengeSuccess_Pop.h"

ChallengeSuccess_Pop::ChallengeSuccess_Pop(const sf::IntRect & bounds)
	: Game_Scr(bounds)
{
	setEnabled(false);
	int widthDiv6 = bounds.width / 6;
	int x = bounds.left;
	int y = bounds.top;
	int heightDiv6 = bounds.height / 6;

	std::vector<sf::Vector2f> pointList;
	pointList.emplace_back(sf::Vector2f(x, y + heightDiv6 * 4));
	pointList.emplace_back(sf::Vector2f(x + widthDiv6, y + heightDiv6 * 3));
	pointList.emplace_back(sf::Vector2f(x + widthDiv6 * 2, y + heightDiv6 * 4));
	pointList.emplace_back(sf::Vector2f(x + widthDiv6 * 5, y + heightDiv6 * 2));
	pointList.emplace_back(sf::Vector2f(x + bounds.width, y + heightDiv6 * 3));
	pointList.emplace_back(sf::Vector2f(x + widthDiv6 * 2, y + bounds.height));

	_tickShape = new Polygon_Shp(pointList);
	_tickShape->setFillColor(sf::Color(106, 163, 22));
	_tickShape->setOutlineThickness(1);
	_tickShape->setOutlineColor(sf::Color::Black);
}

ChallengeSuccess_Pop::~ChallengeSuccess_Pop()
{
	delete _tickShape;
}

void ChallengeSuccess_Pop::update(const float deltaTime)
{
	_displayTimer -= deltaTime;
	if (_displayTimer <= 0) {
		setEnabled(false);
	}
}

void ChallengeSuccess_Pop::draw(sf::RenderWindow & renderWindow) const
{
	if (fmod(_displayTimer, 0.2f) < 0.15f) {
		renderWindow.draw(*_tickShape);
	}
}

void ChallengeSuccess_Pop::showOverlay()
{
	setEnabled(true);
	_displayTimer = 2.0f;
}
