#pragma once

#include "Game_Scr.h"
#include "Player.h"
#include "RuleSet.h"
#include "Btn.h"
#include "Draw_Obj_Grp.h"

class Result_Game_Scr : public Game_Scr
{
	// List of buttons visible on the interface.
	std::vector<Btn> _buttonList;

	// Reference to the players that are being shown for scores.
	std::vector<Player *> _playerList;

	// Static interface elements
	Draw_Obj_Grp *_staticElements;

	// Result state that changes to Finished for Continue, Menu for Lobby
	Game_Result_State _resultState;

	// Reference to the rules.
	RuleSet *_rules;

	// Looks up the action to apply based on an actionID for a button.
	void handleButtonPress(const int actionID);

	// Initialises all the visual elements of the interface.
	void initialiseInterface(const sf::Font &font, RuleSet *ruleSet);

public:
	// Initialise the interface with bounds and make it enabled.
	Result_Game_Scr(const sf::IntRect &bounds, const sf::Font &font, const std::vector<Player *> &playerList, RuleSet *ruleSet);
	virtual ~Result_Game_Scr();

	// Does nothing.
	virtual void update(const float deltaTime) override {}

	// Draws all the elements of the interface.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Does nothing if not enabled. Checks all the buttons for presses and handles actions if necessary.
	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override;

	// Does nothing if not enabled. Updates the hover state of all buttons.
	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override;

	// Returns Nothing when interface is still in use, Finished for Continue, Menu for Lobby
	Game_Result_State getResultState() const override;

	// Gets references to all the players.
	std::vector<Player *> getPlayers() const;

	// Gets a reference to the rules.
	RuleSet *getRuleSet() const;
};
