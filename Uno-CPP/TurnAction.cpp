#include "TurnAction.h"

Turn_Op::Turn_Op(Turn_Op *next, TurnSequence<Turn_Op> *parentSequence, const TurnActionEffect actionID, const std::string &actionDebugText)
	: _next(next), _actionID(actionID), _actionDebugText(actionDebugText)
{
	_parent = parentSequence;
}

TurnActionEffect Turn_Op::getTurnAction() { return _actionID; }

Turn_Op *Turn_Op::getNext() { return _next; }

void Turn_Op::injectProperty(const std::string &key, const int value)
{
	_parent->injectProperty(key, value);
}

int Turn_Op::getPropertyValue(const std::string &key) const
{
	return _parent->getPropertyValue(key);
}

std::string Turn_Op::getActionDebugText() const
{
	return _actionDebugText;
}

Turn_Op *Turn_Op::getNextPointer() const
{
	return _next;
}
