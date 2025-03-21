#include "TurnActionFactory.h"
#include "TurnActionEffect.h"
#include "UNO_Game.h"

TurnSequence<Turn_Op>* TurnActionFactory::playCardAsAction(const int playerID, const int cardID, const int faceValueID, const int colourID)
{
	TurnSequence<Turn_Op>* nextSequence = new TurnSequence<Turn_Op>();
	nextSequence->injectProperty("playerID", playerID);
	nextSequence->injectProperty("cardID", cardID);
	nextSequence->injectProperty("faceValueID", faceValueID);
	nextSequence->injectProperty("colourID", colourID);
	Turn_Op* cardAction = cardIDToTurnAction(faceValueID, nextSequence);
	Turn_Op* placeCardAction = new Turn_Op(cardAction, nextSequence, TurnActionEffect::PlaceCard, "Place Card");

	// Link sequence in by setting the start.
	nextSequence->setStartOfSequence(placeCardAction);
	return nextSequence;
}

TurnSequence<Turn_Op>* TurnActionFactory::draw_Card_As_Action(const int playerID)
{
	TurnSequence<Turn_Op>* nextSequence = new TurnSequence<Turn_Op>();
	nextSequence->injectProperty("playerID", playerID);
	Turn_Op* moveToNextTurn = new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	Turn_Op* playCard = new Turn_Op(nullptr, nextSequence, TurnActionEffect::PlayCardAsActionFromData, "Play the DrawnCard");
	TurnDecisionAction* keepOrPlay = new TurnDecisionAction(moveToNextTurn, playCard, true, "keepOrPlay", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Keep Or Play Choice");
	TurnDecisionAction* isForcedPlay = new TurnDecisionAction(keepOrPlay, playCard, false, "isForcedPlay", nextSequence, TurnActionEffect::CheckForcedPlayRule, "Check if the Forced Play is enabled and force the play if so.");
	Turn_Op* keepDrawing = new Turn_Op(nullptr, nextSequence, TurnActionEffect::DrawCardAsActionFromData, "Draw Another Card (Recursive Tree)");
	TurnDecisionAction* drawTillCanPlay = new TurnDecisionAction(moveToNextTurn, keepDrawing, false, "drawTillCanPlay?", nextSequence, TurnActionEffect::CheckDrawTillCanPlayRule, "Check Draw Till Can Play Rule");
	TurnDecisionAction* canPlayCard = new TurnDecisionAction(drawTillCanPlay, isForcedPlay, false, "cardPlayable", nextSequence, TurnActionEffect::IsCardPlayable, "Check is the Card Playable");
	Turn_Op* drawCard = new Turn_Op(canPlayCard, nextSequence, TurnActionEffect::DrawCard, "Draw a Card");
	
	// Link sequence in by setting the start.
	nextSequence->setStartOfSequence(drawCard);
	return nextSequence;
}

Turn_Op * TurnActionFactory::playPlus2Action(TurnSequence<Turn_Op>* nextSequence)
{
	Turn_Op* moveToNextTurn = new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	Turn_Op* dealPenalty = new Turn_Op(moveToNextTurn, nextSequence, TurnActionEffect::DrawNCards, "Draw N Number Cards");
	Turn_Op* playCard = new Turn_Op(nullptr, nextSequence, TurnActionEffect::PlayCardAsActionFromData, "Play another +2 (Recursive)");
	TurnDecisionAction* waitForPlay2OrCancel = new TurnDecisionAction(dealPenalty, playCard, true,
		"isStacking", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Check for +2 or Cancel Choice");
	TurnDecisionAction* checkCanRespond = new TurnDecisionAction(dealPenalty, waitForPlay2OrCancel, false,
		"hasPlus2AndResponseAllowed", nextSequence, TurnActionEffect::HasPlus2AndResponseAllowed, "Can Stack and has a +2");
	Turn_Op* increaseDrawCount = new Turn_Op(checkCanRespond, nextSequence, TurnActionEffect::IncreaseDrawCountBy2, "Increase N (drawCount) by 2");
	return new Turn_Op(increaseDrawCount, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
}

Turn_Op * TurnActionFactory::playPlus4Action(TurnSequence<Turn_Op>* nextSequence)
{
	Turn_Op* moveToNextSkipDamagedPlayer = new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	Turn_Op* drawNCards = new Turn_Op(moveToNextSkipDamagedPlayer, nextSequence, TurnActionEffect::DrawNCards, "Draw N Number Cards");
	Turn_Op* increaseDrawBy4 = new Turn_Op(drawNCards, nextSequence, TurnActionEffect::IncreaseDrawCountBy4, "Increase N (drawCount) by 4");
	Turn_Op* playCardAsResponse = new Turn_Op(nullptr, nextSequence, TurnActionEffect::PlayCardAsActionFromData, "Stack +4 on Previous (Recursive)");
	Turn_Op* increaseDrawBy4ThenStack = new Turn_Op(playCardAsResponse, nextSequence, TurnActionEffect::IncreaseDrawCountBy4, "Increase N (drawCount) by 4");
	TurnDecisionAction* isChainingCard = new TurnDecisionAction(increaseDrawBy4, increaseDrawBy4ThenStack,
		false, "isChaining", nextSequence, TurnActionEffect::Nothing, "No Action");
	Turn_Op* drawNCardsAndDoNothing = new Turn_Op(nullptr, nextSequence, TurnActionEffect::DrawNCards, "Draw N Number Cards");
	Turn_Op* moveBackToNext = new Turn_Op(drawNCardsAndDoNothing, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	Turn_Op* applyPenalty = new Turn_Op(moveBackToNext, nextSequence, TurnActionEffect::Draw4ChallengeSuccess, "Apply penalty (+4) to Player");
	Turn_Op* moveToPreviousPlayer = new Turn_Op(applyPenalty, nextSequence, TurnActionEffect::MovePrevious, "Move to Previous Player");
	Turn_Op* increaseDrawBy2 = new Turn_Op(increaseDrawBy4, nextSequence, TurnActionEffect::IncreaseDrawCountBy2, "Increase N (drawCount) by 2");
	TurnDecisionAction* couldPreviousPlayCard = new TurnDecisionAction(increaseDrawBy2, moveToPreviousPlayer,
		false, "couldPreviousPlayCard", nextSequence, TurnActionEffect::ShowChallengeResult, "Could the Previous Player Have played a Card? (No Action)");
	TurnDecisionAction* isChallenging = new TurnDecisionAction(isChainingCard, couldPreviousPlayCard, true,
		"isChallenging", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Ask if the player wants to Challenge, Stack, or Do Nothing");
	TurnDecisionAction* canChallengeOrStack = new TurnDecisionAction(increaseDrawBy4, isChallenging, false,
		"canChallenge", nextSequence, TurnActionEffect::CheckNoBluffingRule, "Check if a Challenge is allowed or if there is a card to Stack");
	Turn_Op* moveToNextTurn = new Turn_Op(canChallengeOrStack, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	Turn_Op* setTopOfPileColour = new Turn_Op(moveToNextTurn, nextSequence, TurnActionEffect::SetTopPileColour, "Change the Colour on Top of Pile");
	TurnDecisionAction* chooseWildColour = new TurnDecisionAction(setTopOfPileColour, setTopOfPileColour,
		true, "wildColour", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Ask player for a Colour Choice");
	return new Turn_Op(chooseWildColour, nextSequence, TurnActionEffect::CheckCouldPlayCard, "Check if a Card Could have been Played");
}

Turn_Op * TurnActionFactory::playWildAction(TurnSequence<Turn_Op>* nextSequence)
{
	Turn_Op* moveToNextTurn = new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	Turn_Op* setTopOfPileColour = new Turn_Op(moveToNextTurn, nextSequence, TurnActionEffect::SetTopPileColour, "Change the Colour on Top of Pile");
	return new TurnDecisionAction(setTopOfPileColour, setTopOfPileColour,
		true, "wildColour", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Ask player for a Colour Choice");
}

Turn_Op * TurnActionFactory::playSkipAction(TurnSequence<Turn_Op>* nextSequence)
{
	Turn_Op* moveToNextTurnAtEnd = new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	Turn_Op* showSkip = new Turn_Op(moveToNextTurnAtEnd, nextSequence, TurnActionEffect::ShowSkip, "Show a Skip Icon Over Player");
	return new Turn_Op(showSkip, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
}

Turn_Op * TurnActionFactory::playReverseAction(TurnSequence<Turn_Op>* nextSequence)
{
	Turn_Op* moveToNextTurn = new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	return new Turn_Op(moveToNextTurn, nextSequence, TurnActionEffect::TogglePlayDirection, "Toggle Direction of Play");
}

Turn_Op * TurnActionFactory::playSwapAction(TurnSequence<Turn_Op>* nextSequence)
{
	Turn_Op* moveToNextTurn = new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	Turn_Op* swapHands = new Turn_Op(moveToNextTurn, nextSequence, TurnActionEffect::SwapHandWithOther, "Swap Hands with Selected Player");
	return new TurnDecisionAction(swapHands, swapHands, true,
		"otherPlayer", nextSequence, TurnActionEffect::BeginChoiceOverlay, "Choose Other Player to Swap With");
}

Turn_Op * TurnActionFactory::playPassAllAction(TurnSequence<Turn_Op>* nextSequence)
{
	Turn_Op* moveToNextTurn = new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	return new Turn_Op(moveToNextTurn, nextSequence, TurnActionEffect::PassAllHands, "Pass All Hands");
}

Turn_Op * TurnActionFactory::cardIDToTurnAction(const int faceValueID, TurnSequence<Turn_Op>* nextSequence)
{
	switch (UNO_Game::getCurrentGame()->getRuleSet()->getActionForCard(faceValueID)) {
		case CardAction::Plus2: return playPlus2Action(nextSequence);
		case CardAction::Plus4: return playPlus4Action(nextSequence);
		case CardAction::Wild: return playWildAction(nextSequence);
		case CardAction::Skip: return playSkipAction(nextSequence);
		case CardAction::Reverse: return playReverseAction(nextSequence);
		case CardAction::Swap: return playSwapAction(nextSequence);
		case CardAction::PassAll: return playPassAllAction(nextSequence);
		default: return new Turn_Op(nullptr, nextSequence, TurnActionEffect::MoveNextTurn, "Move to Next Turn");
	};
}
