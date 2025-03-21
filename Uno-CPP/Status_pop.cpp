#include "Status_pop.h"
#include <iomanip>
#include <sstream>

Status_pop::Status_pop(const sf::IntRect & bounds, const sf::Font & font, const GameStateData& gameData)
	: Game_Scr(bounds), _gameData(gameData), _centre(sf::Vector2f(bounds.left+bounds.width/2, bounds.top+bounds.height/2-100))
{
	setEnabled(false);
	_background = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(400, 60)), sf::Color(184, 154, 143, 204));
	_background->setPositionWithOffset(sf::Vector2f(_centre.x - 200, _centre.y - 45));
	_statusText = new Draw_Txt("NOTSET", font, 20, sf::Color::Black, sf::Text::Bold);
	_timeOutText = new Draw_Txt("NOTSET", font, 20, sf::Color::Black, sf::Text::Bold);
	_timeOutShadowText = new Draw_Txt("NOTSET", font, 20, sf::Color::Black, sf::Text::Bold);
}

Status_pop::~Status_pop()
{
	delete _statusText;
	delete _timeOutText;
	delete _background;
	delete _timeOutShadowText;
}

void Status_pop::update(const float deltaTime)
{
	_timeOut -= deltaTime;
	updateTimeOutLabel();
}

void Status_pop::draw(sf::RenderWindow & renderWindow) const
{
	_background->draw(renderWindow);
	_statusText->draw(renderWindow);
	_timeOutShadowText->draw(renderWindow);
	_timeOutText->draw(renderWindow);
}

void Status_pop::showOverlay(TurnDecisionAction * currentAction)
{
	setEnabled(true);
	std::string statusText = createContextString(currentAction);
	updateStatusLabel(statusText);
	_timeOut = _gameData.ruleSet->getDefaultTimeOut();
	updateTimeOutLabel();
}

std::string Status_pop::createContextString(TurnDecisionAction * currentAction)
{
	std::string result;

	if(currentAction->getFlagName() == "keepOrPlay") 
		result = "choosing to Keep or Play.";
	else if (currentAction->getFlagName() == "wildColour") 
		result = "choosing wild Colour.";
	else if (currentAction->getFlagName() == "isChallenging") 
		result = "choosing Response to +4.";
	else if (currentAction->getFlagName() == "otherPlayer")
		result = "choosing other Player to Swap With.";
	else 
		result = "Player choosing card...";

	if (_gameData.players->at(*_gameData.currentPlayerID)->getPlayerType() == 0) {
		return "You are " + result;
	}
	return _gameData.players->at(*_gameData.currentPlayerID)->getPlayerName() + " is " + result;
}

void Status_pop::updateStatusLabel(const std::string& status)
{
	_statusText->setText(status);
	int strWidth = _statusText->getTextWidth();
	_statusText->setPositionWithOffset(sf::Vector2f(_centre.x - strWidth / 2, _centre.y-40));
	
}

void Status_pop::updateTimeOutLabel()
{
	if (_timeOut < 0) {
		_timeOut = 0;
	}
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << _timeOut;
	std::string time = ss.str() + "s";

	_timeOutText->setColour(_timeOut < 6 ? sf::Color::Red : sf::Color::Green);

	_timeOutText->setText(time);
	_timeOutShadowText->setText(time);
	int strWidth = _timeOutText->getTextWidth();
	_timeOutText->setPositionWithOffset(sf::Vector2f(_centre.x - strWidth / 2, _centre.y - 20));
	_timeOutShadowText->setPositionWithOffset(sf::Vector2f(_centre.x - strWidth / 2 - 2, _centre.y - 20 + 2));
}
