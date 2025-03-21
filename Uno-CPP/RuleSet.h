#pragma once

#include <map>

enum CardAction
{
	C_Nothing,
	Plus2,
	Plus4,
	Wild,
	Skip,
	Reverse,
	Swap,
	PassAll
}; // Defines the different unique CardActions that can be mapped to faceValueIDs.

enum ScoreLimitType
{
	OneRound,
	Score200,
	Score300,
	Score500,
	Unlimited
}; // Score limits that determine how long the game will run for.
class RuleSet
{
public:
	// Initialises a default RuleSet.
	RuleSet();

	// Sets all the rules to their default states.
	void setToDefaults();

	// Looks up the CardAction that should be activated in relation to a played card.
	CardAction getActionForCard(const int faceValueID) const;

	// Checks whether +2 and +4 cards can be played in response to other +2 and +4 cards.
	bool canStackCards() const;

	// Changes the state of card stacking.
	void setCanStackCards(const bool canStackCards);

	// Checks whether cards must be drawn till one can be played.
	bool shouldDrawnTillCanPlay() const;

	// Changes the state of drawing to a card can be played.
	void setDrawnTillCanPlay(const bool drawnTillCanPlay);

	// Gets the time in seconds that can be spent maximum for any individual action.
	int getDefaultTimeOut() const;

	// Sets the state of whether there are only two players.
	void setTwoPlayers(const bool onlyTwoPlayers);

	// Gets the current state of the two players.
	bool getOnlyTwoPlayers() const;

	// Changes the state of whether the seven-zero rule is active.
	void setSevenZeroRule(const bool sevenZeroRule);

	// Checks whether the Seven-Zero rule is active.
	bool getSevenZeroRule() const;

	// Sets the No Buffing rule.
	void setNoBuffingRule(const bool noBluffingRule);

	// Gets the current state of the no bluffing rule.
	bool getNoBluffingRule() const;

	// Sets the current state of the allowing jump in rule.
	void setAllowJumpInRule(const bool allowJumpInRule);

	// Gets the current state of the jump in rule.
	bool allowJumpInRule() const;

	// Sets the current state of the forced play rule.
	void setForcedPlayRule(const bool forcedPlayRule);

	// Gets the current state of the forced play rule.
	bool getForcedPlayRule() const;

	// Sets the score limit to wind the rounds.
	void setScoreLimitType(const ScoreLimitType scoreLimitType);

	// Gets the current score limit setting.
	ScoreLimitType getScoreLimitType() const;

	virtual ~RuleSet() = default;

private:
	// 0 to 14 mapped with CardActions to represent each of the different Uno cards.
	std::map<int, CardAction> _faceValueToActionMap;
	// True if +2 and +4 cards can be stacked in response.
	bool _canStackCards;
	// True if when drawing from the deck for turn cards must continue to be drawn till a playable card is found.
	bool _drawTillCanPlay;
	// The time player's have to make their choice during actions.
	int _defaultTimeOut;
	// Used to determine if Reverse becomes a skip when true.
	bool _onlyTwoPlayers;
	// When true the 7 becomes a swap action, and 0 becomes a pass all action.
	bool _sevenZeroRule;
	// When no bluffing is true, the +4 can not be challenged.
	bool _noBluffingRule;
	// When true, players can jump in out of turn order with cards showing the same face value.
	bool _allowJumpInRule;
	// When true, there is no keep or play choice, it is forced play.
	bool _forcedPlayRule;
	// Stores the type of score limit to use for managing the end of game.
	ScoreLimitType _scoreLimitType;
};
