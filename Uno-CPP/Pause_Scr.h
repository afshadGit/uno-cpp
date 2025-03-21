#pragma once
#include "Game_Scr.h"
#include "Draw_Obj_Grp.h"
#include "Btn.h"

class Pause_Scr : public Game_Scr
{
	// The static background elements of the interface initialised by initialiseInterface().
	Draw_Obj_Grp *_interfaceBackground;

	// A list of all the buttons in the interface.
	std::vector<Btn> _buttonList;

	// The current state of the interface: WndResult::Nothing if no state change required, Finished represents the menu should be hidden, Quit represents the game should be terminated, and Menu should return to the lobby.
	Game_Result_State _resultState;

	// Initialises all the visual elements in the interface.
	void initialiseInterface(const sf::IntRect &bounds, const sf::IntRect &gameBounds, const sf::Font &font);

	// Handles the actionID by mapping each ID to an action related to the button.
	void handleButtonAction(const int actionID);

public:
	// Initialise the interface with bounds makes it ready to use.
	Pause_Scr(const sf::IntRect &bounds, const sf::IntRect &gameBounds, const sf::Font &font);
	virtual ~Pause_Scr();

	// Do nothing during update.
	virtual void update(const float deltaTime) override {}

	// Draws the background elements and buttons on top.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Does nothing if not enabled. Checks if a button has been clicked and responds to it.
	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override;

	// Does nothing if not enabled. Updates hover states of all buttons.
	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override;

	// By default this method returns WndResult::Nothing, Finished represents the menu should be hidden, Quit represents the game should be terminated, and Menu should return to the lobby.
	Game_Result_State getResultState() const override;

	// Resets the result state back to nothing after the result state has been handled.
	void resetResultState();
};
