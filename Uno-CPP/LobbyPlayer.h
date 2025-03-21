#pragma once
#include "Inter_Rect.h"
#include "Comp_Player.h"
#include "Draw_Obj_Grp.h"
#include "Draw_Shp.h"
#include "Draw_Txt.h"

class LobbyPlayer : public Inter_Rect
{
	// The name shown for the player.
	std::string Player_Name;

	// The type of player(Human_Player, AIPlayer).
	bool  _playerType;

	// Visible and included in the collection of players when true.
	bool _isEnabled;

	// True when the mouse is over the player.
	bool _isHovered;

	// String representing the type of player.
	std::string _playerTypeStr;

	// All drawable elements
	Draw_Obj_Grp *_drawableObject;

	// Background Rect referenced for changing colour on hover.
	Draw_Shp *_background;

	// Background colour when hovered
	sf::Color _bgHoverColour;

	// Background colour when not hovered
	sf::Color _bgNotHoveredColour;

	// Player Name Element
	Draw_Txt *_playerNameText;

	// AI Strategy Element
	Draw_Txt *_aiStrategyText;

public:
	// Initialises the object ready to show information about the player.
	LobbyPlayer(const std::string &playerName, const bool playerType, const sf::IntRect &bounds, const sf::Font &font);

	void setPlayerName(const std::string &playerName);

	std::string get_PlayerName() const;

	bool getPlayerType() const;

	void setEnabled(const bool isEnabled);

	bool isEnabled() const;

	void handleClick();

	void draw(sf::RenderWindow &renderWindow) const;

	void updateHoverState(const sf::Vector2i &mousePosition);

	virtual ~LobbyPlayer();
};
