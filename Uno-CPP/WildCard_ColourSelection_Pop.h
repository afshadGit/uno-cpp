#pragma once

#include "Game_Scr.h"
#include "TurnDecision_Pop.h"
#include "Inter_Rect.h"
#include "Draw_Obj_Grp.h"
#include "Draw_Shp.h"

class WildCard_ColourSelection_Pop : public Game_Scr, public TurnDecision_Pop
{
	// Tracking of the area hovered with the current grid position and region number.
	int _hoveredRegion, _hoverX, _hoverY;

	// Reference to the TurnAction that triggered the display of this overlay.
	TurnDecisionAction *_currentAction;

	// Region where hovering is managed.
	Inter_Rect _interactionRect;

	// Collection of background graphics for the overlay.
	Draw_Obj_Grp *_background;

	// Arcs to show only when hovering.
	std::vector<Draw_Shp *> _hoverArcs;

public:
	// Initialises the overlay using the specified region.
	WildCard_ColourSelection_Pop(const sf::IntRect &bounds, const sf::Font &font);
	virtual ~WildCard_ColourSelection_Pop();

	// Does nothing.
	virtual void update(const float deltaTime) override {}
	// Draws a selection interface to choose a colour.
	virtual void draw(sf::RenderWindow &renderWindow) const override;

	// Shows the overlay.
	virtual void showOverlay(TurnDecisionAction *currentAction) override;

	// Checks if the region clicked is valid for a colour selection and applies the colour as an action if appropriate.
	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) override;

	// Updates the currently hovered region in the colour wheel.
	virtual void handleMouseMove(const sf::Vector2i &mousePosition) override;
};
