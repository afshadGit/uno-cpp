#pragma once

#include "TurnSequence.h"
#include "TurnDecisionAction.h"

class TurnActionFactory
{
	// Generates a sequence of TurnActions to handle the events required when a +2 card is played. The following shows the sequence that can occur. It is constructed in reverse. MoveToNextTurn -> Increase Draw Count +2 -> hasPlus2AndResponseAllowed? -> (true) -> isStacking? -> (true) -> Begin Action Play Card -> (false) -> Draw Card * Draw Count + Reset Draw Count to 0 -> MoveToNextTurn -> (false) -> Draw Card * Draw Count + Reset Draw Count to 0 -> MoveToNextTurn
	static Turn_Op *playPlus2Action(TurnSequence<Turn_Op> *nextSequence);

	//  Generates a sequence of TurnActions to handle the events required when a +4 card is played. The following shows the sequence that can occur. It is constructed in reverse. couldPreviousPlayCard PreCheck ->  -> Set top pile colour -> MoveToNextTurn-> isChallenging? -> (true) -> couldPreviousPlayCard? -> (true) ->  -> Draw 6 cards -> MoveToNextPlayer -> Draw * Draw Count + reset-> (false) -> Increase drawCount by 4 -> Draw * Draw Count + reset draw count -> (false) -> isChaining? -> (true) -> Begin Action Play Card -> (false) -> Increase drawCount by 4 -> Draw * Draw Count + reset draw count
	static Turn_Op *playPlus4Action(TurnSequence<Turn_Op> *nextSequence);

	// Generates a sequence of TurnActions to handle the events required when a Wild card is played. The following shows the sequence that can occur. It is constructed in reverse. WildColourSelection -> Set top pile colour -> MoveToNextTurn
	static Turn_Op *playWildAction(TurnSequence<Turn_Op> *nextSequence);

	// Generates a sequence of TurnActions to handle the events required when a Skip card is played. The following shows the sequence that can occur. It is constructed in reverse. MoveToNextTurn -> Show Skip -> MoveToNextTurn
	static Turn_Op *playSkipAction(TurnSequence<Turn_Op> *nextSequence);

	// Generates a sequence of TurnActions to handle the events required when a Reverse card is played. The following shows the sequence that can occur. It is constructed in reverse. Toggle Turn Direction Order -> MoveToNextTurn
	static Turn_Op *playReverseAction(TurnSequence<Turn_Op> *nextSequence);

	// otherPlayer? Selection -> Swap Hands (current, selected) -> MoveToNextTurn	  Generates a sequence of TurnActions to handle the events required when a Swap card is played. This is for a different game mode with selecting a player to swap hands with. The following shows the sequence that can occur. It is constructed in reverse.
	static Turn_Op *playSwapAction(TurnSequence<Turn_Op> *nextSequence);

	// Generates a sequence of TurnActions to handle the events required when a Pass All card is played. This is for a different game mode with shifting all hands around based on turn order. The following shows the sequence that can occur. It is constructed in reverse. Pass All Cards -> MoveToNextTurn
	static Turn_Op *playPassAllAction(TurnSequence<Turn_Op> *nextSequence);

	// Looks up a relevant action to apply based on the faceValue of the card. If there is no matching associated action to generate a TurnAction sequence from then the default is to move to the next turn.
	static Turn_Op *cardIDToTurnAction(const int faceValueID, TurnSequence<Turn_Op> *nextSequence);

public:
	// Queues placing the specified card followed by the sequence of actions that result from the type of card that was played from calling this method.
	static TurnSequence<Turn_Op> *playCardAsAction(const int playerID, const int cardID, const int faceValueID, const int colourID);

	// This method should be used when the player is using their turn action to draw a card from the deck. The decision tree generated by this method follows the sequence shown below. It is constructed in reverse. Draw Card-> cardPlayable? -> (true) -> keepOrPlay?->Keep->MoveToNextTurn->Play->Begin Action Play Card->(false) -> drawTillCanPlay? -> (true) ->  Begin Action Draw Card-> (false)->MoveToNextTurn
	static TurnSequence<Turn_Op> *draw_Card_As_Action(const int playerID);
};
