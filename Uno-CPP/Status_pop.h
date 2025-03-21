#pragma once

#include "Game_Scr.h"
#include "TurnDecision_Pop.h"
#include "Draw_Shp.h"
#include "Draw_Txt.h"
#include "GameStateData.h"

class Status_pop : public Game_Scr, public TurnDecision_Pop
{
public:
	// Initialise the interface ready to show a status.
	Status_pop(const sf::IntRect& bounds, const sf::Font& font, const GameStateData& gameData);
	virtual ~Status_pop();

	// Updates the timeOut remaining.
	virtual void update(const float deltaTime) override;
	// Draws the text for the status and timer.
	virtual void draw(sf::RenderWindow & renderWindow) const override;

	// Shows the overlay by generating a status depending on the currentAction.
	virtual void showOverlay(TurnDecisionAction * currentAction) override;

private:
	// Centre of the bounds to draw the text at.
	sf::Vector2f _centre;
	
	// Timeout representing time remaining to complete the action.
	float _timeOut;
	
	// Text to display showing the current status.
	Draw_Txt* _statusText;
	
	// String showing the number representing the time remaining.
	Draw_Txt* _timeOutText;
	
	// Shadow for the string showing the number representing the time remaining.
	Draw_Txt* _timeOutShadowText;
	
	// Background for status text
	Draw_Shp* _background;

	// Reference to the game state for checking the current player.
	const GameStateData& _gameData;

	std::string createContextString(TurnDecisionAction* currentAction);

	// Updates the label and background based on the status.
	void updateStatusLabel(const std::string& status);

	// Updates the label based on the current timeout value.
	void updateTimeOutLabel();
};

