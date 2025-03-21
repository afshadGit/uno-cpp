#pragma once
#include "TurnSequence.h"
#include "TurnAction.h"

class Turn_Sequence_Manager
{
	// The current sequence of TurnActions.
	TurnSequence<Turn_Op> *current_Sequence;

	// The queued sequence of TurnActions ready for when the current one ends.
	TurnSequence<Turn_Op> *queued_Sequence;

	// Reference to the debug state
	const bool &_debugModeEnabled;

	// When GamePanel.DEBUG_MODE and this are true, output is shown for each transition in the TaskAction sequence.
	bool _debugShowTaskActionNotes;

	// When GamePanel.DEBUG_MODE and this are true, output is shown with the whole tree TaskAction sequence when setCurrentTurnAction is used.
	bool _debugShowTreeOnNewAction;

public:
	// Initialises the manager object ready to set a sequence.
	explicit Turn_Sequence_Manager(const bool &debugModeEnabled);

	// Updates the current turn action state by performing the action and then iterating to the next one if possible.
	void update();

	void setSequence(TurnSequence<Turn_Op> *newSequence);

	// Queues the specified sequence to begin when the current sequence has safely ended.
	void queueSequence(TurnSequence<Turn_Op> *newSequence);

	// Gets the current turn action for the current sequence.
	Turn_Op *getCurrentTurnAction() const;

	// Returns true when there is a sequence actively being run.
	bool hasActiveTurnAction() const;

	// [DEBUG ONLY] Toggle the state of showing task action notes
	void toggleDebugShowTaskActionNotes();

	// [DEBUG ONLY] Toggle the showing of a state on a new action
	void toggleDebugShowTreeOnNewAction();

	virtual ~Turn_Sequence_Manager();
};
