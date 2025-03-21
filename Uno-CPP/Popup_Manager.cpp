#include "Popup_Manager.h"

#include "WildCard_ColourSelection_Pop.h"
#include "Keep_Or_Play_Pop.h"
#include "PlayerFlash_Pop.h"
#include "Challenge_Pop.h"
#include "Status_pop.h"
#include "PlayerSelection_pop.h"
#include "StackChoice_Pop.h"
#include "ChallengeSuccess_Pop.h"
#include "Challenge_Fail_Pop.h"
#include "UNOCalled_pop.h"
#include "Uno_Btn.h"
#include "AntiUno_Btn.h"
#include "TurnDecision_Pop.h"
#include "General_pop_Scr.h"
#include <sstream>

Popup_Manager::Popup_Manager(const sf::IntRect &bounds, std::vector<Player *> playerList, const sf::Font &font, GameStateData gameData)
	: Game_Scr(bounds), _gameState(gameData)
{
	setEnabled(true);
	WildCard_ColourSelection_Pop *wildColourSelectionOverlay = new WildCard_ColourSelection_Pop(sf::IntRect(bounds.width / 2 - 100, bounds.height / 2 - 30, 200, 200), font);
	Keep_Or_Play_Pop *keepOrPlayOverlay = new Keep_Or_Play_Pop(bounds, font);
	PlayerSelection_pop *playerSelectionOverlay = new PlayerSelection_pop(bounds, playerList, font);
	Status_pop *statusOverlay = new Status_pop(bounds, font, _gameState);
	Challenge_Pop *challengeOverlay = new Challenge_Pop(bounds, font, _gameState);
	StackChoice_Pop *stackChoiceOverlay = new StackChoice_Pop(bounds, font, _gameState);
	_overlays["wildColour"] = wildColourSelectionOverlay;
	_overlays["keepOrPlay"] = keepOrPlayOverlay;
	_overlays["otherPlayer"] = playerSelectionOverlay;
	_overlays["statusOverlay"] = statusOverlay;
	_overlays["isChallenging"] = challengeOverlay;
	_overlays["isStacking"] = stackChoiceOverlay;

	_unoButton = new Uno_Btn(sf::Vector2f(bounds.left + bounds.width - 80 - 40, bounds.top + bounds.height - 60 - 40), font, _gameState);
	_antiUnoButton = new AntiUno_Btn(sf::Vector2f(bounds.left + bounds.width - 80 - 40 - 100, bounds.top + bounds.height - 60 - 40), font, _gameState);
	for (int i = 0; i < playerList.size(); i++)
	{
		sf::Vector2f playerCentre = playerList.at(i)->getCentreOfBounds();
		PlayerFlash_Pop *skipVisualOverlay = new PlayerFlash_Pop(playerCentre, "TURN SKIPPED", sf::Color::Red, 40, font);
		_overlays["SkipVisual" + std::to_string(i)] = skipVisualOverlay;
		PlayerFlash_Pop *drawNMessageOverlay = new PlayerFlash_Pop(playerCentre, "", sf::Color::Red, 40, font);
		_overlays["DrawN" + std::to_string(i)] = drawNMessageOverlay;
		ChallengeSuccess_Pop *challengeSuccessOverlay = new ChallengeSuccess_Pop(sf::IntRect(playerCentre.x, playerCentre.y, 100, 100));
		_overlays["ChallengeSuccess" + std::to_string(i)] = challengeSuccessOverlay;
		Challenge_Fail_Pop *challengeFailedOverlay = new Challenge_Fail_Pop(sf::IntRect(playerCentre.x, playerCentre.y, 100, 100));
		_overlays["ChallengeFailed" + std::to_string(i)] = challengeFailedOverlay;
		UNOCalled_pop *unoCalledOverlay = new UNOCalled_pop(sf::Vector2f(playerCentre.x, playerCentre.y + 20), font);
		_overlays["UNOCalled" + std::to_string(i)] = unoCalledOverlay;
		PlayerFlash_Pop *antiUnoOverlay = new PlayerFlash_Pop(sf::Vector2f(playerCentre.x, playerCentre.y + 20), "!", sf::Color(226, 173, 67), 50, font);
		_overlays["AntiUnoCalled" + std::to_string(i)] = antiUnoOverlay;
		PlayerFlash_Pop *jumpInOverlay = new PlayerFlash_Pop(sf::Vector2f(playerCentre.x, playerCentre.y + 20), "JUMPED IN", sf::Color(255, 215, 0), 40, font);
		_overlays["JumpIn" + std::to_string(i)] = jumpInOverlay;
	}
	_overlays["UnoButton"] = _unoButton;
	_overlays["antiUnoButton"] = _antiUnoButton;
}

Popup_Manager::~Popup_Manager()
{
	for (auto &[key, overlay] : _overlays)
	{
		delete overlay;
	}
}

void Popup_Manager::update(const float deltaTime, const Turn_Op *currentTurnAction)
{
	if (_overlayAction != currentTurnAction)
	{
		_overlayAction = nullptr;
		hideAllDecisionOverlays();
	}

	for (auto &[key, overlay] : _overlays)
	{
		if (overlay != nullptr && overlay->isEnabled())
		{
			overlay->update(deltaTime);
		}
	}
}

void Popup_Manager::draw(sf::RenderWindow &renderWindow) const
{
	for (auto const &[key, overlay] : _overlays)
	{
		if (overlay != nullptr && overlay->isEnabled())
		{
			overlay->draw(renderWindow);
		}
	}
}

void Popup_Manager::handleMousePress(const sf::Vector2i &mousePosition, bool isLeft)
{
	for (auto &[key, overlay] : _overlays)
	{
		if (overlay == nullptr) {
            // printf("Skipping null overlay.\n");
            continue;
        }
		// printf("\nhandling mouse press");
		if (overlay != nullptr && overlay->isEnabled())
		{
			// printf("\nFound match");
			overlay->handleMousePress(mousePosition, isLeft);
		}
	}
}

void Popup_Manager::handleMouseMove(const sf::Vector2i &mousePosition)
{
	for (auto &[key, overlay] : _overlays)
	{
		// printf("\nhandling mouse Move");

		if (overlay != nullptr && overlay->isEnabled())
		{
			// printf("\nFound mouse Move match");
			overlay->handleMouseMove(mousePosition);
		}
	}
}

void Popup_Manager::showDecisionOverlay(TurnDecisionAction *currentAction)
{
	if (currentAction->requiresTimeout())
	{
		setEnabled(true);
		if (_gameState.players->at(*_gameState.currentPlayerID)->getPlayerType() == 0)
		{
			TurnDecision_Pop *overlayToShow = dynamic_cast<TurnDecision_Pop *>(_overlays[currentAction->getFlagName()]);
			if (overlayToShow != nullptr)
			{
				overlayToShow->showOverlay(currentAction);
			}
		}
		_overlayAction = currentAction;
		(dynamic_cast<TurnDecision_Pop *>(_overlays["statusOverlay"]))->showOverlay(currentAction);
	}
}

void Popup_Manager::showGeneralOverlay(const std::string &overlayName)
{
	// Split to allow for parameter inputs separated by ;
	std::vector<std::string> splitOverlayName{};
	std::stringstream sstream(overlayName);
	std::string part;
	while (std::getline(sstream, part, ';'))
	{
		splitOverlayName.push_back(part);
	}
	General_pop_Scr *overlayToShow = dynamic_cast<General_pop_Scr *>(_overlays[splitOverlayName.at(0)]);
	if (overlayToShow != nullptr)
	{
		overlayToShow->showOverlay();
		if (splitOverlayName.at(0).rfind("DrawN", 0) == 0)
		{
			(dynamic_cast<PlayerFlash_Pop *>(_overlays[splitOverlayName.at(0)]))->setMessage("+" + splitOverlayName.at(1));
		}
	}
}

void Popup_Manager::hideAllDecisionOverlays()
{
	for (auto &[key, overlay] : _overlays)
	{
		if (dynamic_cast<TurnDecision_Pop *>(overlay) != nullptr)
		{
			overlay->setEnabled(false);
		}
	}
	setEnabled(false);
}

PlayerUpdateResult Popup_Manager::getUnoButtonsState()
{
	bool buttonTriggered = _unoButton->isTriggeredReset();
	if (buttonTriggered)
	{
		return {Player_Result_States::PlayerCalledUno, nullptr, _unoButton->getActionID(), nullptr};
	}
	buttonTriggered = _antiUnoButton->isTriggeredReset();
	if (buttonTriggered)
	{
		for (const auto &player : *_gameState.players)
		{
			if (player != _gameState.bottomPlayer && !player->isSafe() && player->getHand().size() == 1)
			{
				return {Player_Result_States::PlayerCalledAntiUno, nullptr, player->getPlayerID(), nullptr};
			}
		}
	}

	return {Player_Result_States::PlayerDidNothing, nullptr, -1, nullptr};
}
