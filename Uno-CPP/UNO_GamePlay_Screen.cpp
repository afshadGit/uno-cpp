#include "UNO_GamePlay_Screen.h"
#include "TurnActionFactory.h"
#include <iostream>
#include "GameStateData.h"

UNO_GamePlay_Screen::UNO_GamePlay_Screen(const sf::IntRect &bounds, const sf::Font &font, const std::vector<LobbyPlayer *> playerList, RuleSet *ruleSet, std::default_random_engine &randomEngine)
	: UNO_GamePlay_Screen(bounds, font, createPlayersFromLobby(playerList, bounds, font, randomEngine), ruleSet, randomEngine)
{
}

UNO_GamePlay_Screen::UNO_GamePlay_Screen(const sf::IntRect &bounds, const sf::Font &font, const std::vector<Player *> playerList, RuleSet *ruleSet, std::default_random_engine &randomEngine)
	: Game_Scr(bounds), _recentCardPile(new Cards_Played_Stack(sf::Vector2f(bounds.width / 2 - 30, bounds.height / 2 - 45), randomEngine))
{
	_ruleSet = ruleSet;
	_deck = new Card_Deck(sf::Vector2f(bounds.width / 2 - 30 - 160, bounds.height / 2 - 45), font, randomEngine);
	for (auto player : playerList)
	{
		_players.emplace_back(player);
		if (player->getPlayerType() == 0)
		{
			_bottomPlayer = player;
		}
		player->emptyHand();
		for (int i = 0; i < 7; i++)
		{
			player->addCardToHand(_deck->drawCard());
		}
	}
	_currentPlayerID = randomEngine() % playerList.size();
	_isIncreasing = randomEngine() % 100 < 50;
	_playDirectionAnimation = new Turn_sequence_anim(sf::Vector2f(bounds.width / 2, bounds.height / 2), 120, 5);
	_playDirectionAnimation->setIsIncreasing(_isIncreasing);

	GameStateData gameStateData = {_ruleSet, &_players, _bottomPlayer, &_isIncreasing, _deck, &_currentPlayerID};
	_overlayManager = new Popup_Manager(bounds, playerList, font, gameStateData);
	_recentCardPile->forcePlayCard(_deck->drawCard());
	_debugModeEnabled = false;

	_turnActionSequenceManager = new Turn_Sequence_Manager(_debugModeEnabled);

	_resultState = Game_Result_State::NothingState;
	_debugEnabledText = sf::Text("DEBUG MODE ENABLED", font, 30);
	_debugEnabledText.setPosition(sf::Vector2f(5, 5));
	_debugEnabledText.setFillColor(sf::Color::Black);
}

UNO_GamePlay_Screen::~UNO_GamePlay_Screen()
{
	delete _overlayManager;
	delete _playDirectionAnimation;
	delete _turnActionSequenceManager;
	delete _recentCardPile;
}

void UNO_GamePlay_Screen::update(const float deltaTime)
{
	if (!isEnabled())
		return;

	_playDirectionAnimation->update(deltaTime);
	_overlayManager->update(deltaTime, getCurrentTurnAction());
	_turnActionSequenceManager->update();
	for (auto player : _players)
	{
		PlayerUpdateResult result = player->update(deltaTime, getCurrentPlayer(), getCurrentTurnAction(), getRecentCardPile(), getAllPlayers(), getRuleSet());
		applyPlayerUpdateResult(result);
	}
	checkForEndOfRound();
}

void UNO_GamePlay_Screen::draw(sf::RenderWindow &renderWindow) const
{
	_deck->draw(renderWindow);
	_recentCardPile->draw(renderWindow);
	for (const auto player : _players)
	{
		player->draw(renderWindow, getCurrentPlayer());
	}
	_overlayManager->draw(renderWindow);
	_playDirectionAnimation->draw(renderWindow);

	if (_debugModeEnabled)
	{
		renderWindow.draw(_debugEnabledText);
	}
}

void UNO_GamePlay_Screen::handleMousePress(const sf::Vector2i &mousePosition, bool isLeft)
{
	if (!isEnabled())
		return;

	_overlayManager->handleMousePress(mousePosition, isLeft);
	PlayerUpdateResult buttonState = _overlayManager->getUnoButtonsState();
	applyPlayerUpdateResult(buttonState);

	// Handle player turn actions
	if (!_turnActionSequenceManager->hasActiveTurnAction())
	{
		if (_currentPlayerID == _bottomPlayer->getPlayerID())
		{
			if (_deck->isPositionInside(mousePosition))
			{
				_turnActionSequenceManager->setSequence(TurnActionFactory::draw_Card_As_Action(_currentPlayerID));
			}
			else
			{
				auto cardToPlay = _bottomPlayer->chooseCardFromClick(mousePosition);
				auto topCard = _recentCardPile->get_Front_Card();
				// Play if a valid move
				std::vector<Cards *> validMoves = _bottomPlayer->getValidMoves(topCard->getFaceValueID(), topCard->getColourID());
				if (std::find(validMoves.begin(), validMoves.end(), cardToPlay) != validMoves.end())
				{
					_turnActionSequenceManager->setSequence(TurnActionFactory::playCardAsAction(
						_currentPlayerID, cardToPlay->getUniqueCardID(),
						cardToPlay->getFaceValueID(), cardToPlay->getColourID()));
				}
			}
		}
		else if (_ruleSet->allowJumpInRule())
		{
			auto cardToPlay = _bottomPlayer->chooseCardFromClick(mousePosition);
			if (cardToPlay != nullptr)
			{
				jumpIn(_bottomPlayer->getPlayerID(), cardToPlay);
			}
		}
	}
}

void UNO_GamePlay_Screen::handleMouseMove(const sf::Vector2i &mousePosition)
{
	if (!isEnabled())
		return;

	_overlayManager->handleMouseMove(mousePosition);
	_bottomPlayer->updateHover(mousePosition);
}

void UNO_GamePlay_Screen::handleKeyInput(const sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Q)
	{
		sortHand();
	}
	else if (key == sf::Keyboard::Key::Num0)
	{
		_debugModeEnabled = !_debugModeEnabled;
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num9)
	{
		revealHands();
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num8)
	{
		toggleTurnDirection();
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num7)
	{
		_bottomPlayer->emptyHand();
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num6)
	{
		Cards *removedCard = _bottomPlayer->removeCard(_bottomPlayer->getHand().at(0)->getUniqueCardID());
		delete removedCard;
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num5)
	{
		_turnActionSequenceManager->toggleDebugShowTreeOnNewAction();
	}
	else if (_debugModeEnabled && key == sf::Keyboard::Key::Num4)
	{
		_turnActionSequenceManager->toggleDebugShowTaskActionNotes();
	}
	else
	{
		// Note: The OverlayManager does not actually use the keys at all, but could be used in future.
		_overlayManager->handleKeyInput(key);
	}
}

void UNO_GamePlay_Screen::jumpIn(const int playerID, const Cards *cardToPlay)
{
	auto topCard = _recentCardPile->get_Front_Card();
	if (!_turnActionSequenceManager->hasActiveTurnAction() && _currentPlayerID != playerID && topCard->getFaceValueID() == cardToPlay->getFaceValueID() && topCard->getColourID() == cardToPlay->getColourID())
	{
		_currentPlayerID = playerID;
		showGeneralOverlay("JumpIn" + std::to_string(playerID));
		_turnActionSequenceManager->setSequence(TurnActionFactory::playCardAsAction(_currentPlayerID,
																					cardToPlay->getUniqueCardID(), cardToPlay->getFaceValueID(), cardToPlay->getColourID()));
	}
}

void UNO_GamePlay_Screen::showOverlayForTurnAction()
{
	TurnDecisionAction *action = dynamic_cast<TurnDecisionAction *>(_turnActionSequenceManager->getCurrentTurnAction());
	if (action != nullptr)
	{
		_overlayManager->showDecisionOverlay(action);
	}
}

void UNO_GamePlay_Screen::showGeneralOverlay(const std::string &overlayName)
{
	_overlayManager->showGeneralOverlay(overlayName);
}

void UNO_GamePlay_Screen::revealHands()
{
	for (auto player : _players)
	{
		player->revealHand(true);
	}
}

void UNO_GamePlay_Screen::sortHand()
{
	_bottomPlayer->sortHand();
}

Player *UNO_GamePlay_Screen::getBottomPlayer() const
{
	return _bottomPlayer;
}

void UNO_GamePlay_Screen::toggleTurnDirection()
{
	_isIncreasing = !_isIncreasing;
	_playDirectionAnimation->setIsIncreasing(_isIncreasing);
}

void UNO_GamePlay_Screen::moveToNextPlayer()
{
	updateUNOState();
	if (_isIncreasing)
	{
		_currentPlayerID++;
		if (_currentPlayerID >= _players.size())
		{
			_currentPlayerID = 0;
		}
	}
	else
	{
		_currentPlayerID--;
		if (_currentPlayerID < 0)
		{
			_currentPlayerID = _players.size() - 1;
		}
	}
}

void UNO_GamePlay_Screen::applyAntiUno(const int playerID)
{
	showGeneralOverlay("AntiUnoCalled" + std::to_string(playerID));
	// Set to safe to prevent multiple anti-uno callouts.
	_players.at(playerID)->setUnoState(UNOState::Safe);
	_players.at(playerID)->addCardToHand(_deck->drawCard());
	_players.at(playerID)->addCardToHand(_deck->drawCard());
}

bool UNO_GamePlay_Screen::isIncreasing() const
{
	return _isIncreasing;
}

void UNO_GamePlay_Screen::setCurrentTurnAction(TurnSequence<Turn_Op> *newSequence)
{
	_turnActionSequenceManager->setSequence(newSequence);
}

Turn_Op *UNO_GamePlay_Screen::getCurrentTurnAction() const
{
	return _turnActionSequenceManager->getCurrentTurnAction();
}

RuleSet *UNO_GamePlay_Screen::getRuleSet() const
{
	return _ruleSet;
}

Player *UNO_GamePlay_Screen::getCurrentPlayer() const
{
	return _players.at(_currentPlayerID);
}

std::vector<Player *> UNO_GamePlay_Screen::getAllPlayers() const
{
	return _players;
}

Player *UNO_GamePlay_Screen::getPlayerByID(const int playerID) const
{
	return _players.at(playerID);
}

Card_Deck *UNO_GamePlay_Screen::getDeck() const
{
	return _deck;
}

Cards_Played_Stack *UNO_GamePlay_Screen::getRecentCardPile() const
{
	return _recentCardPile;
}

Game_Result_State UNO_GamePlay_Screen::getResultState() const
{
	return _resultState;
}

void UNO_GamePlay_Screen::checkForEndOfRound()
{
	for (auto player : _players)
	{
		if (player->getHand().size() == 0)
		{
			int totalScore = 0;
			for (auto playerForValue : _players)
			{
				if (playerForValue != player)
				{
					playerForValue->setCurrentRoundScore(0);
					totalScore += playerForValue->getHandTotalScore();
				}
			}
			player->setCurrentRoundScore(totalScore);
			player->setWon();
			_resultState = Game_Result_State::Finished;
			return;
		}
	}
}

void UNO_GamePlay_Screen::updateUNOState()
{
	_players.at(_currentPlayerID)->setUnoState(_players.at(_currentPlayerID)->getHand().size() == 1 ? UNOState::NotSafe : UNOState::Safe);
	for (auto player : _players)
	{
		if (player->getPlayerID() != _currentPlayerID)
		{
			player->setUnoState(UNOState::Safe);
		}
	}
}

void UNO_GamePlay_Screen::applyPlayerUpdateResult(PlayerUpdateResult &playerUpdateResult)
{
	switch (playerUpdateResult.resultState)
	{
	case Player_Result_States::PlayerCalledAntiUno:
		applyAntiUno(playerUpdateResult.playerIDForResult);
		break;
	case Player_Result_States::PlayerCalledUno:
		showGeneralOverlay("UNOCalled" + std::to_string(playerUpdateResult.playerIDForResult));
		break;
	case Player_Result_States::PlayerJumpedIn:
		jumpIn(playerUpdateResult.playerIDForResult, playerUpdateResult.cardForJump);
		break;
	case Player_Result_States::PlayerStartedTurnAction:
		setCurrentTurnAction(playerUpdateResult.turnActionRequest);
		break;
	case Player_Result_States::PlayerStartedTurnActionWithUno:
		showGeneralOverlay("UNOCalled" + std::to_string(playerUpdateResult.playerIDForResult));
		setCurrentTurnAction(playerUpdateResult.turnActionRequest);
		break;
	}
}

std::vector<Player *> UNO_GamePlay_Screen::createPlayersFromLobby(const std::vector<LobbyPlayer *> playerList, sf::IntRect bounds, const sf::Font &font, std::default_random_engine &randomEngine)
{
	std::vector<Player *> result;
	std::vector<LobbyPlayer *> playersToAdd;
	for (auto lobbyPlayer : playerList)
	{
		if (lobbyPlayer->isEnabled())
		{
			playersToAdd.emplace_back(lobbyPlayer);
		}
	}

	if (playersToAdd.size() != 2 && playersToAdd.size() != 4)
	{
		std::cerr << "Critical Error. Only combinations of 2 or 4 players are allowed" << std::endl;
		return result;
	}
	int thisPlayerIndex = -1;
	for (int i = 0; i < playersToAdd.size(); i++)
	{
		if (playersToAdd.at(i)->getPlayerType() == 0)
		{
			if (thisPlayerIndex == -1)
			{
				thisPlayerIndex = i;
			}
			else
			{
				std::cerr << "Critical Error. Only one Human_Player is allowed." << std::endl;
				return result;
			}
		}
	}
	if (thisPlayerIndex == -1)
	{
		std::cerr << "Critical Error. One Human_Player is required!" << std::endl;
		return result;
	}

	for (int i = 0; i < playersToAdd.size(); i++)
	{
		sf::IntRect playerRegion;
		bool showNameLeft;
		if (playersToAdd.size() == 4)
		{
			playerRegion = getPlayerRect((i + 4 - thisPlayerIndex) % 4, bounds);
			showNameLeft = (i + 4 - thisPlayerIndex) % 2 == 0;
		}
		else
		{
			playerRegion = getPlayerRect(playersToAdd.at(i)->getPlayerType() == 0 ? 0 : 2, bounds);
			showNameLeft = true;
		}
		if (playersToAdd.at(i)->getPlayerType() == 1)
		{
			result.emplace_back(new Comp_Player(i, playersToAdd.at(i)->get_PlayerName(), playerRegion, showNameLeft, randomEngine, font));
		}
		else
		{
			result.emplace_back(new Player(i, playersToAdd.at(i)->get_PlayerName(), playersToAdd.at(i)->getPlayerType(), playerRegion, showNameLeft, font));
		}
	}
	return result;
}

sf::IntRect UNO_GamePlay_Screen::getPlayerRect(const int direction, const sf::IntRect &bounds)
{
	switch (direction)
	{
	case 1:
		return sf::IntRect(bounds.left, bounds.top + bounds.height / 2 - 150, (CARD_WIDTH + 4) * 6, bounds.height / 2 - 100 - 10);
	case 2:
		return sf::IntRect(bounds.left + bounds.width / 2 - (CARD_WIDTH + 4) * 10 / 2, bounds.top - 30,
						   (CARD_WIDTH + 4) * 10, bounds.height / 2 - 100 - 10);
	case 3:
		return sf::IntRect(bounds.left + bounds.width - ((CARD_WIDTH + 4) * 6 + 50) + 50,
						   bounds.top + bounds.height / 2 - 150, (CARD_WIDTH + 4) * 6, bounds.height / 2 - 100 - 10);
	default:
		return sf::IntRect(bounds.left + bounds.width / 2 - (CARD_WIDTH + 4) * 10 / 2,
						   bounds.top + bounds.height / 2 + 130, (CARD_WIDTH + 4) * 10, bounds.height / 2 - 100 - 10);
	}
}
