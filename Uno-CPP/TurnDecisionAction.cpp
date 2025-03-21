#include "TurnDecisionAction.h"

TurnDecisionAction::TurnDecisionAction(Turn_Op * next, Turn_Op * otherNext, const bool timeOut, const std::string & flagName, TurnSequence<Turn_Op>* parentSequence, const TurnActionEffect actionID, const std::string & actionDebugText)
	: Turn_Op(next, parentSequence, actionID, actionDebugText), _otherNext(otherNext), isTimer_out(timeOut), _flagName(flagName)
{
	_hasRunOnce = false;
}

Turn_Op * TurnDecisionAction::getNext()
{
	if(getPropertyValue(_flagName) != -1) {
        return (getPropertyValue(_flagName) == 0) ? _next : _otherNext;
    }
    return this;
}

TurnActionEffect TurnDecisionAction::getTurnAction()
{
	if (_hasRunOnce) { return TurnActionEffect::Nothing; }
	_hasRunOnce = true;
	// Call the base version with to actually perform the action.
	return Turn_Op::getTurnAction();
}

void TurnDecisionAction::injectFlagProperty(const int value)
{
	injectProperty(_flagName, value);
}

bool TurnDecisionAction::requiresTimeout() const
{
	return isTimer_out;
}

std::string TurnDecisionAction::getFlagName() const
{
	return _flagName;
}

bool TurnDecisionAction::getHasRunOnce() const
{
	return _hasRunOnce;
}

bool TurnDecisionAction::getTimeOut() const
{
	return isTimer_out;
}

Turn_Op * TurnDecisionAction::getOtherNextPointer() const
{
	return _otherNext;
}
