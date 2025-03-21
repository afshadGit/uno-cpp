#include "Uno_Btn.h"

Uno_Btn::Uno_Btn(const sf::Vector2f position, const sf::Font& font, const GameStateData& gameData)
	: OvalHover_Btn(sf::IntRect(position.x, position.y, 80, 60), font, "UNO", sf::Color(147, 44, 44)), _gameState(gameData)
{
	setEnabled(true);
	_bottomPlayer = _gameState.bottomPlayer;
}

void Uno_Btn::update(const float deltaTime)
{
	_isActive = _bottomPlayer->getUnoState() == UNOState::NotSafe
		|| (_bottomPlayer->getUnoState() == UNOState::Safe
			&& _gameState.players->at(*_gameState.currentPlayerID) == _bottomPlayer
			&& _bottomPlayer->getHand().size() == 2);
}

void Uno_Btn::showOverlay()
{
	setEnabled(true);
}

void Uno_Btn::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (_isActive && _bounds.contains(mousePosition)) {
		_bottomPlayer->setUnoState(UNOState::Called);
		_isTriggered = true;
	}
}