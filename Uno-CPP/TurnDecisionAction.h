#pragma once
#include "TurnAction.h"

class TurnDecisionAction : public Turn_Op
{
	// When true, the TurnDecisionAction has a time limit to complete it.
	bool isTimer_out;
	// The alternative Turn Action to move to if the flag variable is non-zero.
	Turn_Op *_otherNext;
	// The flag variable used to determine when the decision has been met.
	std::string _flagName;
	// A boolean variable to track whether the action has already been run.
	bool _hasRunOnce;

public:
	// Defines a TurnDecisionAction that chooses to use either the next or otherNext TurnAction based on the value stored in flagName's mapped value stored in storedData. 0 will trigger next, and 1 will trigger otherNext. getNext() will continue to return this current object until the flagName has been set to a value.
	TurnDecisionAction(Turn_Op *next, Turn_Op *otherNext, const bool timeOut, const std::string &flagName, TurnSequence<Turn_Op> *parentSequence, const TurnActionEffect actionID, const std::string &actionDebugText);
	virtual ~TurnDecisionAction() = default;

	// Checks if the flagName has been set in storedData. If it has been set the value is evaluated such that 0 returns next, or any other value returns otherNext. When it has not yet been set the method will continue to return a reference to the current class.
	Turn_Op *getNext() override;

	// Checks if the action has already been performed. Then returns the action if it is not nothing based on the definition in TurnAction.

	TurnActionEffect getTurnAction() override;

	// method to storing a value directly into the flagName associated with this TurnDecisionAction.
	void injectFlagProperty(const int value);

	// Returns true when the action requires a timeout for limiting interaction.
	bool requiresTimeout() const;

	// Gets the name used as a variable for the decision.
	std::string getFlagName() const;

	// When true this decision has run its action once already, so it should not run again
	bool getHasRunOnce() const;

	// When true, the TurnDecisionAction has a time limit to complete it.
	bool getTimeOut() const;

	// function to return the next action with no variable state.
	Turn_Op *getOtherNextPointer() const;
};
