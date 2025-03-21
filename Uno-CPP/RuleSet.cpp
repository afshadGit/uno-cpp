#include "RuleSet.h"

RuleSet::RuleSet()
{
	for (int i = 0; i <= 9; i++)
		_faceValueToActionMap[i] = CardAction::C_Nothing;

	_faceValueToActionMap[10] = CardAction::Plus2;
	_faceValueToActionMap[11] = CardAction::Skip;
	_faceValueToActionMap[12] = CardAction::Reverse;
	_faceValueToActionMap[13] = CardAction::Plus4;
	_faceValueToActionMap[14] = CardAction::Wild;
	_defaultTimeOut = 25;
	setToDefaults();
}

void RuleSet::setToDefaults()
{
	setCanStackCards(true);
	setDrawnTillCanPlay(false);
	setTwoPlayers(false);
	setSevenZeroRule(false);
	setForcedPlayRule(true);
	setAllowJumpInRule(false);
	setNoBuffingRule(false);
	setScoreLimitType(ScoreLimitType::OneRound);
}

CardAction RuleSet::getActionForCard(const int faceValueID) const
{
	return _faceValueToActionMap.at(faceValueID);
}

bool RuleSet::canStackCards() const
{
	return _canStackCards;
}

void RuleSet::setCanStackCards(const bool canStackCards)
{
	_canStackCards = canStackCards;
}

bool RuleSet::shouldDrawnTillCanPlay() const
{
	return _drawTillCanPlay;
}

void RuleSet::setDrawnTillCanPlay(const bool drawnTillCanPlay)
{
	_drawTillCanPlay = drawnTillCanPlay;
}

int RuleSet::getDefaultTimeOut() const
{
	return _defaultTimeOut;
}

void RuleSet::setTwoPlayers(bool onlyTwoPlayers)
{
    // Set the state for whether the game has only two players
    _onlyTwoPlayers = onlyTwoPlayers;

    // Update the action for face value 12 based on the number of players
    if (onlyTwoPlayers)
    {
        _faceValueToActionMap[12] = CardAction::Skip;
    }
    else
    {
        _faceValueToActionMap[12] = CardAction::Reverse;
    }
}


bool RuleSet::getOnlyTwoPlayers() const { return _onlyTwoPlayers; }

void RuleSet::setSevenZeroRule(bool enableSevenZeroRule)
{
    // Update the state for the seven-zero rule
    _sevenZeroRule = enableSevenZeroRule;

    // Update the action map for face values 0 and 7 based on the rule's state
    if (enableSevenZeroRule)
    {
        _faceValueToActionMap[0] = CardAction::Swap;
        _faceValueToActionMap[7] = CardAction::PassAll;
    }
    else
    {
        _faceValueToActionMap[0] = CardAction::C_Nothing;
        _faceValueToActionMap[7] = CardAction::C_Nothing;
    }
}


bool RuleSet::getSevenZeroRule() const { return _sevenZeroRule; }

void RuleSet::setNoBuffingRule(const bool noBluffingRule) { _noBluffingRule = noBluffingRule; }

bool RuleSet::getNoBluffingRule() const { return _noBluffingRule; }

void RuleSet::setAllowJumpInRule(const bool allowJumpInRule) { _allowJumpInRule = allowJumpInRule; }

bool RuleSet::allowJumpInRule() const { return _allowJumpInRule; }

void RuleSet::setForcedPlayRule(const bool forcedPlayRule) { _forcedPlayRule = forcedPlayRule; }

bool RuleSet::getForcedPlayRule() const { return _forcedPlayRule; }

void RuleSet::setScoreLimitType(const ScoreLimitType scoreLimitType) { _scoreLimitType = scoreLimitType; }

ScoreLimitType RuleSet::getScoreLimitType() const { return _scoreLimitType; }
