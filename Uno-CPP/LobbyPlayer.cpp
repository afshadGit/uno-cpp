#include "LobbyPlayer.h"
#include <iostream>
#include "StringUtils.h"

LobbyPlayer::LobbyPlayer(const std::string &playerName, const bool playerType, const sf::IntRect &bounds, const sf::Font &font) : Inter_Rect(bounds), Player_Name(playerName), _playerType(playerType), _bgHoverColour(sf::Color(173, 216, 230, 204)), _bgNotHoveredColour(135, 206, 250, 204)
{
	_isEnabled = true;
	_playerTypeStr = (playerType == 0) ? "You:" : "AI Player:";

	_drawableObject = new Draw_Obj_Grp();
	_background = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(bounds.width - 2, bounds.height - 2)), _bgNotHoveredColour, sf::Vector2f(1, 1));
	_background->setBorder(1, sf::Color::Black);
	_playerNameText = new Draw_Txt(playerName, font, 20, sf::Color::Black, sf::Text::Bold);
	_playerNameText->setOffset(sf::Vector2f(150, 30));
	Draw_Txt *playerTypeText = new Draw_Txt(_playerTypeStr, font, 20, sf::Color::Black, sf::Text::Bold);
	playerTypeText->setOffset(sf::Vector2f(20, 30));
	_drawableObject->add_Child(_background);
	_drawableObject->add_Child(_playerNameText);
	_drawableObject->add_Child(playerTypeText);

	if (playerType!=1){
		Draw_Txt *actionText = new Draw_Txt("Click to change name", font, 15, sf::Color::Black, sf::Text::Bold);
		actionText->setOffset(sf::Vector2f(300, 30));
		_drawableObject->add_Child(actionText);
	}

	

	_drawableObject->setPositionWithOffset(sf::Vector2f(bounds.left, bounds.top));
}

LobbyPlayer::~LobbyPlayer()
{
	if (_drawableObject != nullptr)
	{
		// All other objects are managed and deleted by this call.
		delete _drawableObject;
		// Cleanup so they don't point to missing objects.
		_background = nullptr;
		_playerNameText = nullptr;
		_aiStrategyText = nullptr;
	}
}

void LobbyPlayer::setPlayerName(const std::string &playerName) { Player_Name = playerName; }

std::string LobbyPlayer::get_PlayerName() const { return Player_Name; }

bool LobbyPlayer::getPlayerType() const { return _playerType; }

void LobbyPlayer::setEnabled(const bool isEnabled) { _isEnabled = isEnabled; }

bool LobbyPlayer::isEnabled() const { return _isEnabled; }

void LobbyPlayer::handleClick()
{
	if (!isEnabled())
		return;

	if (_playerType == 0)
	{
		std::cout << "Enter name name: ";
		std::string input;
		std::getline(std::cin, input);
		trim(input);
		if (input.length() > 12)
		{
			input = input.substr(0, 12);
		}
		if (input.length() > 0)
		{
			Player_Name = input;
			_playerNameText->setText(Player_Name);
		}
	}
}

void LobbyPlayer::draw(sf::RenderWindow &renderWindow) const
{
	if (!isEnabled())
		return;

	_drawableObject->draw(renderWindow);
}

void LobbyPlayer::updateHoverState(const sf::Vector2i &mousePosition)
{
	bool currentValue = _isHovered;
	_isHovered = isPositionInside(mousePosition);
	// If the state changed
	if (currentValue != _isHovered)
	{
		_background->setColour(_isHovered ? _bgHoverColour : _bgNotHoveredColour);
	}
}
