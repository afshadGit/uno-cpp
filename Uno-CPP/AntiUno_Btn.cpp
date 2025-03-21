#include "AntiUno_Btn.h"

AntiUno_Btn::AntiUno_Btn(const sf::Vector2f position, const sf::Font &font, const GameStateData &gameData)
	: OvalHover_Btn(sf::IntRect(position.x, position.y, 80, 60), font, "!", sf::Color(147, 44, 44)), _gameState(gameData) { // initialises the button
	setEnabled(true);
	_bottomPlayer = _gameState.bottomPlayer;
}

AntiUno_Btn::~AntiUno_Btn() {}

void AntiUno_Btn::update(const float deltaTime) { // updates the button to determine if there is a player vulnerable to being called out on not saying "UNO"
	_isActive = false;
	for (const auto &player : *_gameState.players) {
		if (player != _bottomPlayer && !player->isSafe() && player->getHand().size() == 1)
			_isActive = true;
	}
}

void AntiUno_Btn::handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) { // handles the mouse press on the button for calling out a player
	if (_isActive && _bounds.contains(mousePosition)) {
		for (const auto &player : *_gameState.players) {
			if (player != _bottomPlayer && !player->isSafe() && player->getHand().size() == 1) {
				_isTriggered = true;
				break;
			}
		}
	}
}

void AntiUno_Btn::showOverlay() { // shows the overlay for calling out a player
	setEnabled(true);
}
