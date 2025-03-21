#include "Turn_Sequence_Manager.h"
#include <iostream>
#include "TurnDecisionAction.h"

Turn_Sequence_Manager::Turn_Sequence_Manager(const bool& debugModeEnabled)
	: _debugModeEnabled(debugModeEnabled)
{
	current_Sequence = nullptr;
	queued_Sequence = nullptr;
	_debugShowTaskActionNotes = false;
	_debugShowTreeOnNewAction = false;
}

Turn_Sequence_Manager::~Turn_Sequence_Manager()
{
	if (current_Sequence != nullptr) {
		delete queued_Sequence;
	}
	if (queued_Sequence != nullptr) {
		delete queued_Sequence;
	}
}

void Turn_Sequence_Manager::update()
{
	if (hasActiveTurnAction()) {
		// Tree Debug Output
		if (_debugModeEnabled && _debugShowTaskActionNotes) {
			TurnDecisionAction* decisionAction = dynamic_cast<TurnDecisionAction*>(getCurrentTurnAction());
			if (decisionAction != nullptr) {
				if (!decisionAction->getHasRunOnce()) {
					std::cout << getCurrentTurnAction()->getActionDebugText() << std::endl;
				}
			}
			else {
				std::cout << getCurrentTurnAction()->getActionDebugText() << std::endl;
			}
		}
		current_Sequence->iterateSequence();

		// Force swap to queued sequence if there is one
		if (queued_Sequence != nullptr) {
			delete current_Sequence;
			current_Sequence = queued_Sequence;
			queued_Sequence = nullptr;
		}
		// If the sequence ended delete the sequence
		else if (getCurrentTurnAction() == nullptr) {
			delete current_Sequence;
			current_Sequence = nullptr;
		}
	}
}

void Turn_Sequence_Manager::setSequence(TurnSequence<Turn_Op>* newSequence)
{
	if (hasActiveTurnAction()) {
		queueSequence(newSequence);

		if (_debugModeEnabled && _debugShowTreeOnNewAction) {
			std::cout << "Queued action sequence:" << std::endl;
			current_Sequence->debugOutputTurnActionTree();
		}
	}
	else {
		if (current_Sequence != nullptr) {
			delete queued_Sequence;
		}
		current_Sequence = newSequence;

		if (_debugModeEnabled && _debugShowTreeOnNewAction) {
			std::cout << "Set action sequence:" << std::endl;
			current_Sequence->debugOutputTurnActionTree();
		}
	}
}

void Turn_Sequence_Manager::queueSequence(TurnSequence<Turn_Op>* newSequence)
{
	if (queued_Sequence != nullptr) {
		delete queued_Sequence;
	}
	queued_Sequence = newSequence;
}

Turn_Op * Turn_Sequence_Manager::getCurrentTurnAction() const
{
	if (current_Sequence == nullptr) {
		return nullptr;
	}

	return current_Sequence->getCurrentAction();
}

bool Turn_Sequence_Manager::hasActiveTurnAction() const
{
	return getCurrentTurnAction() != nullptr;
}

void Turn_Sequence_Manager::toggleDebugShowTaskActionNotes()
{
	_debugShowTaskActionNotes = !_debugShowTaskActionNotes;
}

void Turn_Sequence_Manager::toggleDebugShowTreeOnNewAction()
{
	_debugShowTreeOnNewAction = !_debugShowTreeOnNewAction;
}
