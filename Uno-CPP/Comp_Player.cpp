#include "Comp_Player.h"
#include <algorithm>
#include "TurnActionFactory.h"

Comp_Player::Comp_Player(const int playerID, const std::string &playerName, const sf::IntRect bounds, const bool showPlayerNameLeft, std::default_random_engine &randomEngine, const sf::Font &font)
	: Player(playerID, playerName, 1, bounds, showPlayerNameLeft, font), _randomEngine(randomEngine)
{
	resetDelayTimer();
	_consideringDelayTimer = -1;
}

PlayerUpdateResult Comp_Player::update(const float deltaTime, const Player *currentPlayer, Turn_Op *currentTurnAction, const Cards_Played_Stack *recentCards, const std::vector<Player *> &players, const RuleSet *rules)
{
	PlayerUpdateResult result = updateAntiUnoCheck(deltaTime, players);
	if (result.resultState != Player_Result_States::PlayerDidNothing)
		return result;

	result = updateJumpInCheck(deltaTime, rules, currentTurnAction, currentPlayer, recentCards);
	if (result.resultState != Player_Result_States::PlayerDidNothing)
		return result;

	// Do nothing more if this is not the current player.
	if (currentPlayer != this)
		return result; // Result will already contain PlayerDidNothing due to the checks above.

	// Delay until
	delay_Timer -= deltaTime;
	if (delay_Timer <= 0)
		resetDelayTimer();
	else
		return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};

	// If there is no turn action to deal with it means that the player is performing their regular turn
	if (currentTurnAction == nullptr)
	{
		Cards *topCard = recentCards->get_Front_Card();
		auto action = performTurn(topCard);
		// Handle special case where the player needs to call UNO simultaneously with playing their card.
		Player_Result_States state = action.second.resultState == Player_Result_States::PlayerCalledUno ? Player_Result_States::PlayerStartedTurnActionWithUno : Player_Result_States::PlayerStartedTurnAction;
		return {state, action.first, action.second.playerIDForResult, nullptr};
	}
	else
	{
		// Handle the turn action if it is necessary
		TurnDecisionAction *decisionAction = dynamic_cast<TurnDecisionAction *>(currentTurnAction);
		if (decisionAction != nullptr)
			if (decisionAction->getTimeOut())
				return handleTurnDecision(decisionAction, players, rules);
	}
	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}

PlayerUpdateResult Comp_Player::updateAntiUnoCheck(const int deltaTime, const std::vector<Player *> &players)
{
	for (auto player : players)
	{
		if (player != this && !player->isSafe() && player->getHand().size() == 1)
		{
			if (_consideringPlayerID != player->getPlayerID())
			{
				_consideringDelayTimer = (_randomEngine() % 800 + 200) / 1000.0f;
			}
			_consideringPlayerID = player->getPlayerID();
		}
	}
	if (_consideringPlayerID >= 0 && _consideringPlayerID < players.size())
	{

		if (_consideringPlayerID == -1 || players.at(_consideringPlayerID)->isSafe())
		{
			_consideringPlayerID = -1;
		}
		else
		{
			_consideringDelayTimer -= deltaTime;
			if (_consideringDelayTimer <= 0)
			{
				_consideringDelayTimer = (_randomEngine() % 1200 + 300) / 1000.0f;
				if (_randomEngine() % 100 < 30)
					return {Player_Result_States::PlayerCalledAntiUno, nullptr, _consideringPlayerID, nullptr};
			}
		}
	}
	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}

PlayerUpdateResult Comp_Player::updateJumpInCheck(const int deltaTime, const RuleSet *rules, const Turn_Op *currentTurnAction, const Player *currentPlayer, const Cards_Played_Stack *recentCards)
{
	if (rules->allowJumpInRule() && currentTurnAction == nullptr && currentPlayer != this)
	{
		Cards *topCard = recentCards->get_Front_Card();
		std::vector<Cards *> validCards;
		for (auto card : getHand())
		{
			if (card->getFaceValueID() == topCard->getFaceValueID() && card->getColourID() == topCard->getColourID())
			{
				validCards.emplace_back(card);
			}
		}

		if (!validCards.empty())
		{
			if (!_canJumpIn)
			{
				_consideringJumpIn = _randomEngine() % 100 < 80;
				_consideringJumpInTimer = (_randomEngine() % 200 + 100) / 1000.0f;
			}
			_canJumpIn = true;
		}
		else
		{
			_canJumpIn = false;
			_consideringJumpIn = false;
		}
	}
	else
	{
		_canJumpIn = false;
		_consideringJumpIn = false;
	}

	if (_consideringJumpIn)
	{
		_consideringJumpInTimer -= deltaTime;
		if (_consideringJumpInTimer <= 0)
		{
			Cards *topCard = recentCards->get_Front_Card();
			for (auto card : getHand())
			{
				if (card->getFaceValueID() == topCard->getFaceValueID() && card->getColourID() == topCard->getColourID())
				{
					return {Player_Result_States::PlayerJumpedIn, nullptr, getPlayerID(), card};
				}
			}
		}
	}
	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}

std::pair<TurnSequence<Turn_Op> *, PlayerUpdateResult> Comp_Player::performTurn(Cards *topCard)
{
	std::vector<Cards *> validMoves = getValidMoves(topCard->getFaceValueID(), topCard->getColourID());
	if (validMoves.empty())
	{
		return std::make_pair(TurnActionFactory::draw_Card_As_Action(getPlayerID()), PlayerUpdateResult{Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr});
	}
	else
	{
		Cards *cardToPlay = chooseCard(validMoves);
		PlayerUpdateResult result = checkCallUNO();
		return std::make_pair(TurnActionFactory::playCardAsAction(getPlayerID(), cardToPlay->getUniqueCardID(), cardToPlay->getFaceValueID(), cardToPlay->getColourID()), result);
	}
}

void Comp_Player::resetDelayTimer()
{
	delay_Timer = 1.5f;
}

Cards *Comp_Player::chooseCard(std::vector<Cards *> &validCards)
{
	return *std::max_element(validCards.begin(), validCards.end(), [](Cards *a, Cards *b)
							 { return a->getScoreValue() < b->getScoreValue(); });
}

PlayerUpdateResult Comp_Player::handleTurnDecision(TurnDecisionAction *decisionAction, const std::vector<Player *> &players, const RuleSet *rules)
{
	if (decisionAction->getFlagName() == "wildColour")
		return chooseWildColour(decisionAction);
	else if (decisionAction->getFlagName() == "keepOrPlay")
		return chooseKeepOrPlay(decisionAction);
	else if (decisionAction->getFlagName() == "otherPlayer")
		return choosePlayerToSwapWith(decisionAction, players);
	else if (decisionAction->getFlagName() == "isChallenging")
		return chooseChallengeOrDecline(decisionAction, rules);
	else if (decisionAction->getFlagName() == "isStacking")
		return chooseStackPlus2(decisionAction, rules);

	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}

PlayerUpdateResult Comp_Player::chooseWildColour(TurnDecisionAction *decisionAction)
{
	std::vector<Cards *> colouredHandCards;
	for (auto card : getHand())
		if (card->getColourID() != 4)
			colouredHandCards.emplace_back(card);

	// No cards, or only wilds, or rare 10% chance: randomly choose colour
	if (colouredHandCards.empty() || _randomEngine() % 100 > 90)
		decisionAction->injectProperty("colourID", _randomEngine() % 4);
	else
		decisionAction->injectProperty("colourID", colouredHandCards.at(0)->getColourID()); // Use first coloured card
	decisionAction->injectFlagProperty(1);

	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}

PlayerUpdateResult Comp_Player::chooseKeepOrPlay(TurnDecisionAction *decisionAction)
{
	PlayerUpdateResult result = checkCallUNO();
	decisionAction->injectFlagProperty(1);
	return result;
}

PlayerUpdateResult Comp_Player::choosePlayerToSwapWith(TurnDecisionAction *decisionAction, const std::vector<Player *> &players)
{
	Player *chosenPlayer = this;
	int cardCount = 9999;
	for (auto player : players)
	{
		if (player->getHand().size() < cardCount && player != this)
		{
			chosenPlayer = player;
			cardCount = chosenPlayer->getHand().size();
		}
	}
	decisionAction->injectFlagProperty(chosenPlayer->getPlayerID());

	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}

PlayerUpdateResult Comp_Player::chooseChallengeOrDecline(TurnDecisionAction *decisionAction, const RuleSet *rules)
{
	// Always stack a card if it is allowed and available.
	if (rules->canStackCards())
	{
		auto hand = getHand();
		auto validCard = std::find_if(hand.begin(), hand.end(), [](Cards *card)
									  { return card->getFaceValueID() == 13; });
		if (validCard != hand.end())
		{
			PlayerUpdateResult result = checkCallUNO();
			decisionAction->injectProperty("faceValueID", (*validCard)->getFaceValueID());
			decisionAction->injectProperty("colourID", (*validCard)->getColourID());
			decisionAction->injectProperty("cardID", (*validCard)->getUniqueCardID());
			decisionAction->injectProperty("isChaining", 1);
			decisionAction->injectFlagProperty(0);
			return result;
		}
	}
	decisionAction->injectProperty("isChaining", 0);
	// Randomly choose 50-50 whether to challenge or decline
	// Don't need to check the no bluffing rule because this method is only called if a valid choice is available
	// And the AI will ALWAYS choose to stack a card meaning this will never run the random chance of challenge in those cases.
	decisionAction->injectFlagProperty(_randomEngine() % 2);

	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}

PlayerUpdateResult Comp_Player::chooseStackPlus2(TurnDecisionAction *decisionAction, const RuleSet *rules)
{
	if (rules->canStackCards())
	{
		auto hand = getHand();
		auto validCard = std::find_if(hand.begin(), hand.end(), [](Cards *card)
									  { return card->getFaceValueID() == 10; });
		if (validCard != hand.end())
		{
			PlayerUpdateResult result = checkCallUNO();
			decisionAction->injectProperty("faceValueID", (*validCard)->getFaceValueID());
			decisionAction->injectProperty("colourID", (*validCard)->getColourID());
			decisionAction->injectProperty("cardID", (*validCard)->getUniqueCardID());
			decisionAction->injectFlagProperty(1);
			return result;
		}
	}
	decisionAction->injectFlagProperty(0);

	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}

PlayerUpdateResult Comp_Player::checkCallUNO()
{
	if (getHand().size() != 2)
		return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
	if (_randomEngine() % 100 < 70)
	{
		setUnoState(UNOState::Called);
		return {Player_Result_States::PlayerCalledUno, nullptr, getPlayerID(), nullptr};
	}
	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}
