#include "UNO_Game.h"
#include <chrono>       // std::chrono::system_clock
#include <cmath>

UNO_GamePlay_Screen* UNO_Game::_currentGame = nullptr;

UNO_Game::UNO_Game(const sf::IntRect& gameBounds, const sf::Font& font)
	:	_bounds(gameBounds), _font(font)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	_randomEngine = std::default_random_engine(seed);

	End_Game = false;

	_currentGame = nullptr;
	_lobbyScr = nullptr;
	_ResultGameScr = nullptr;
	_pause_Scr = new Pause_Scr(sf::IntRect(gameBounds.width / 2 - 100, gameBounds.height / 2 - 100, 200, 200), gameBounds, font);
	_pause_Scr->setEnabled(false);

	showLobby();
}

UNO_Game::~UNO_Game()
{
	// Current game handles deleting of Player objects if they exist because that created them.
	if (_currentGame != nullptr) {
		auto players = _currentGame->getAllPlayers();
		for (auto p : players) {
			delete p;
		}

		delete _currentGame;
	} 
	if (_lobbyScr != nullptr) {
		delete _lobbyScr;
	}
	if (_ResultGameScr != nullptr) {
		delete _ResultGameScr;
	}
	if (_pause_Scr != nullptr) {
		delete _pause_Scr;
	}
}

void UNO_Game::update(const float deltaTime)
{
	if (_activeInterface != nullptr) {
		_activeInterface->update(deltaTime);

		// Move to next state if necessary
		if (_activeInterface->getResultState() != Game_Result_State::NothingState) {
			if (_activeInterface == _lobbyScr) {
				startGame();
			}
			else if (_activeInterface == _currentGame) {
				showPostGame();
			}
			else if (_activeInterface == _ResultGameScr) {
				if (_activeInterface->getResultState() == Game_Result_State::Menu) {
					showLobby();
				}
				else {
					startNextRound();
				}
			}
		}
	}
}

void UNO_Game::draw(sf::RenderWindow & renderWindow) const
{
	if (_activeInterface != nullptr) {
		_activeInterface->draw(renderWindow);
	}
	if (_pause_Scr->isEnabled()) {
		_pause_Scr->draw(renderWindow);
	}
}

void UNO_Game::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (_pause_Scr->isEnabled()) {
		_pause_Scr->handleMousePress(mousePosition, isLeft);

		// Handle a result from the pauseInterface if one was triggered.
		if (_pause_Scr->getResultState() != Game_Result_State::NothingState) {
			if (_pause_Scr->getResultState() == Game_Result_State::Finished) {
				setPauseState(false);
			} else if (_pause_Scr->getResultState() == Game_Result_State::Menu) {
				showLobby();
			} else if (_pause_Scr->getResultState() == Game_Result_State::Quit) {
				quitGame();
			}
			else
			_pause_Scr->resetResultState();
		}
	}
	if (_activeInterface != nullptr) {
		_activeInterface->handleMousePress(mousePosition, isLeft);
	}
}

void UNO_Game::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (_pause_Scr->isEnabled()) {
		_pause_Scr->handleMouseMove(mousePosition);
	}
	if (_activeInterface != nullptr) {
		_activeInterface->handleMouseMove(mousePosition);
	}
}

void UNO_Game::handleKeyInput(const sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Escape) {
		setPauseState(!_pause_Scr->isEnabled());
	}
	else {
		_activeInterface->handleKeyInput(key);
	}
}

UNO_GamePlay_Screen * UNO_Game::getCurrentGame()
{
	return _currentGame;
}

bool UNO_Game::getGameCloseRequested()
{
	return End_Game;
}

void UNO_Game::setPauseState(const bool isPaused)
{
	if (_activeInterface != nullptr) {
		_activeInterface->setEnabled(!isPaused);
	}
	_pause_Scr->setEnabled(isPaused);
}

void UNO_Game::showLobby()
{
	// Allows for transition from anywhere including itself while using the pause menu
	// Only wipe the interface and recreate it if the interface is not already active.
	if (_activeInterface != _lobbyScr || _activeInterface == nullptr) {
		if (_lobbyScr != nullptr) {
			delete _lobbyScr;
			_lobbyScr = nullptr;
		}
		
		// Create the interface and make it active.
		_lobbyScr = new Lobby_Scr(_bounds, _font, _randomEngine);
		_activeInterface = _lobbyScr;
	}

	// Hide the pause menu in case that is what brought the user here.
	setPauseState(false);
}

void UNO_Game::startGame()
{
	// Expects transition from lobby as the start of a new game
	std::vector<LobbyPlayer*> lobbyPlayers = _lobbyScr->getLobbyPlayerList();
	RuleSet* ruleSet = _lobbyScr->getRuleSet();

	// Delete any existing current game including all the references to players.
	if (_currentGame != nullptr) {
		auto players = _currentGame->getAllPlayers();
		for (auto p : players) {
			delete p;
		}

		delete _currentGame;
		_currentGame = nullptr;
	}

	// Create a new game using LobbyPlayers to let CurrentGameInterface create the Player objects.
	_currentGame = new UNO_GamePlay_Screen(_bounds, _font, lobbyPlayers, ruleSet, _randomEngine);
	_activeInterface = _currentGame;
}

void UNO_Game::startNextRound()
{
	// Expects transition from post game so get the info from there
	std::vector<Player*> players = _ResultGameScr->getPlayers();
	RuleSet* ruleSet = _ResultGameScr->getRuleSet();

	// Delete any existing current game but NOT the players
	if (_currentGame != nullptr) {
		delete _currentGame;
		_currentGame = nullptr;
	}

	// Create the new current game and make it active
	_currentGame = new UNO_GamePlay_Screen(_bounds, _font, players, ruleSet, _randomEngine);
	_activeInterface = _currentGame;
}

void UNO_Game::showPostGame()
{
	// Expects transition from current game
	std::vector<Player*> players = _currentGame->getAllPlayers();
	RuleSet* ruleSet = _currentGame->getRuleSet();

	// Delete any existing post game interface
	if (_ResultGameScr != nullptr) {
		delete _ResultGameScr;
		_ResultGameScr = nullptr;
	}

	// Create the interface and make it active
	_ResultGameScr = new Result_Game_Scr(_bounds, _font, players, ruleSet);
	_activeInterface = _ResultGameScr;
}

void UNO_Game::quitGame()
{
	End_Game = true;
}
