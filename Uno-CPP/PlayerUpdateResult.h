#pragma once

#include "TurnAction.h"

enum Player_Result_States
{
	PlayerDidNothing,
	PlayerStartedTurnAction,
	PlayerStartedTurnActionWithUno,
	PlayerJumpedIn,
	PlayerCalledAntiUno,
	PlayerCalledUno
};

struct PlayerUpdateResult
{
	// Defines the type of result to expect.
	Player_Result_States resultState;

	// Only set if resultState is PlayerStartedTurnAction
	TurnSequence<Turn_Op> *turnActionRequest;

	// Only set if PlayerJumpedIn, PlayerCalledUno and PlayerCalledAntiUno to designate the target.
	int playerIDForResult;

	// Only set if the state is set to PlayerJumpedIn
	Cards *cardForJump;
};