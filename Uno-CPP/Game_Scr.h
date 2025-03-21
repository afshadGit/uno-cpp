#pragma once
#include <SFML/Graphics.hpp>

enum Game_Result_State
{
	NothingState,
	Finished,
	Restart,
	Menu,
	Quit
};

class Game_Scr {
// protected members for bounds and enabled state, that are protected because they are used by derived classes.
protected:
	sf::IntRect _bounds;
	bool _isEnabled;

public:
	Game_Scr(sf::IntRect bounds) : _bounds(bounds), _isEnabled(true) {};
	// Initialise the interface with bounds .

	// Update the interface elements.
	virtual void update(const float deltaTime) = 0;

	// Draw all elements to the interface.

	virtual void draw(sf::RenderWindow &renderWindow) const = 0;

	// Handle updates related to the mouse pressing at the specified position.

	virtual void handleMousePress(const sf::Vector2i &mousePosition, bool isLeft) {};

	// Handle updates related to the mouse being moved.

	virtual void handleMouseMove(const sf::Vector2i &mousePosition) {};

	// Handles the key input from a keyboard action.

	virtual void handleKeyInput(const sf::Keyboard::Key key) {};

	// Change the enabled state of this object.

	void setEnabled(bool enabled) { _isEnabled = enabled; };

	// Get the current enabled state of the object.

	bool isEnabled() const { return _isEnabled; }

	// By default this method returns Game_Result_State::Nothing unless overriden.
	virtual Game_Result_State getResultState() const { return Game_Result_State::NothingState; };
	virtual ~Game_Scr() = default;
};
