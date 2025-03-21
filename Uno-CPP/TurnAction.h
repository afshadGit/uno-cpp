#pragma once

#include "TurnSequence.h"
#include <string>
#include "TurnActionEffect.h"

class Turn_Op
{
protected:
	// Reference to the parent for accessing shared data for the sequence and executing the actions.
	TurnSequence<Turn_Op> *_parent;

	// Reference to the action to be executed by this TurnAction during performAction().
	const TurnActionEffect _actionID;

	// Reference to the next TurnAction in the linked list sequence. This can be null to indicate the end.
	Turn_Op *_next;

	// Text to be used to describe the current state in debug output.
	std::string _actionDebugText;

public:
	// Stores the properties specified ready to use.
	Turn_Op(Turn_Op *next, TurnSequence<Turn_Op> *parentSequence, const TurnActionEffect actionID, const std::string &actionDebugText);
	virtual ~Turn_Op() = default;

	// Gets the action associated with this TurnAction.
	virtual TurnActionEffect getTurnAction();

	// Gets the reference to the next turn action after this one, can be null if at the end.
	virtual Turn_Op *getNext();

	// Stores the specified data into the storedData map to be used for future iterations.
	void injectProperty(const std::string &key, const int value);

	// Gets the value at the specified position, returns -1 if the key could not be found.
	int getPropertyValue(const std::string &key) const;

	// Gets the debug text describing this TurnAction
	std::string getActionDebugText() const;

	// Method to expose the next action with no variable state.
	Turn_Op *getNextPointer() const;
};
