#pragma once

#include <map>
#include "TurnDecisionAction.h"
#include "Game_Scr.h"
#include "Player.h"
#include "GameStateData.h"
#include "Uno_Btn.h"
#include "AntiUno_Btn.h"

class Popup_Manager : public Game_Scr
{
	// Interfaces mapped to unique strings.
	std::map<std::string, Game_Scr *> _overlays;

	// Current action for an active TurnDecisionAction.
	TurnDecisionAction *_overlayAction;

	// Reference to core game variables.
	GameStateData _gameState;

	// Reference to the Uno button for getUnoButtonsState()
	Uno_Btn *_unoButton;

	// Reference to the AntiUno button for getUnoButtonState()
	AntiUno_Btn *_antiUnoButton;

public:
	// Initialise the interfaces all ready for any that needs to be made visible.
	Popup_Manager(const sf::IntRect &bounds, std::vector<Player *> playerList, const sf::Font &font, GameStateData gameData);
	virtual ~Popup_Manager();

	// Updates all the active overlays and hides all the decision overlays if the TurnAction changed.
	virtual void update(const float deltaTime, const Turn_Op *currentTurnAction);

	// Do not call the default WndInterface update method for this.
	virtual void update(const float deltaTime) override {};

	// Draws all enabled overlays.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Passes the mousePress event on to all enabled overlays.
	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override;

	// Passes the mouseMove event on to all enabled overlays.
	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override;

	void showDecisionOverlay(TurnDecisionAction *currentAction);

	// Finds the matching interface and makes it visible if possible.
	void showGeneralOverlay(const std::string &overlayName);

	// Hides all the decision overlays automatically called when the TurnAction changes in update().
	void hideAllDecisionOverlays();

	// Gets any trigger state of the Uno or AntiUno buttons handled during handleMousePress
	PlayerUpdateResult getUnoButtonsState();
};
