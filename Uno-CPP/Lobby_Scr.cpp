#include "Lobby_Scr.h"
#include <fstream>
#include <iostream>

Lobby_Scr::Lobby_Scr(const sf::IntRect &bounds, const sf::Font &font, std::default_random_engine &randomEngine)
	: Game_Scr(bounds)
{
	_ruleSet = new RuleSet();

	std::vector<std::string> aiNames = getRandomAINameList(randomEngine);
	_playerList.emplace_back(new LobbyPlayer("Player", 0, sf::IntRect(20, 100, bounds.width / 2, 100), font));
	_playerList.emplace_back(new LobbyPlayer(aiNames.at(0), 1, sf::IntRect(20, 100 + 120, bounds.width / 2, 100), font));
	_playerList.emplace_back(new LobbyPlayer(aiNames.at(1), 1, sf::IntRect(20, 100 + 120 * 2, bounds.width / 2, 100), font));
	_playerList.emplace_back(new LobbyPlayer(aiNames.at(2), 1, sf::IntRect(20, 100 + 120 * 3, bounds.width / 2, 100), font));

	initialiseBackground(bounds, font);
	initialiseRuleOptions(bounds, font);
	// Default to Nothing and Finished when StartGame is pressed.
	_resultState = Game_Result_State::NothingState;
}

Lobby_Scr::~Lobby_Scr()
{
	delete _background;
	for (auto &p : _playerList)
	{
		delete p;
	}
	delete _ruleSet;
}

void Lobby_Scr::draw(sf::RenderWindow &renderWindow) const
{
	_background->draw(renderWindow);

	for (const auto &button : _buttonList)
	{
		button.draw(renderWindow);
	}
	for (const auto &lobbyPlayer : _playerList)
	{
		lobbyPlayer->draw(renderWindow);
	}

	// Draw all dynamic strings for the rules.
	for (const auto &[key, ruleString] : _ruleStrings)
	{
		renderWindow.draw(ruleString);
	}
}

void Lobby_Scr::handleMousePress(const sf::Vector2i &mousePosition, bool isLeft)
{
	if (!isEnabled())
		return;

	for (auto &button : _buttonList)
	{
		if (button.isPositionInside(mousePosition))
		{
			handleButtonPress(button.getActionID());
		}
	}
	for (auto &lobbyPlayer : _playerList)
	{
		if (lobbyPlayer->isPositionInside(mousePosition))
		{
			lobbyPlayer->handleClick();
		}
	}
}

void Lobby_Scr::handleMouseMove(const sf::Vector2i &mousePosition)
{
	if (!isEnabled())
		return;

	for (auto &button : _buttonList)
	{
		button.setHovering(button.isPositionInside(mousePosition));
	}
	for (auto &lobbyPlayer : _playerList)
	{
		lobbyPlayer->updateHoverState(mousePosition);
	}
}

RuleSet *Lobby_Scr::getRuleSet() const
{
	return _ruleSet;
}

std::vector<LobbyPlayer *> Lobby_Scr::getLobbyPlayerList() const
{
	return _playerList;
}

Game_Result_State Lobby_Scr::getResultState() const
{
	return _resultState;
}

void Lobby_Scr::handleButtonPress(const int actionID)
{
	switch (actionID)
	{
	case 1:
		toggleNumberOfPlayers();
		break;
	case 2:
		_resultState = Game_Result_State::Finished;
		break;
	case 3:
		toggleStackRule();
		break;
	case 4:
		toggleDrawTillCanPlayRule();
		break;
	case 5:
		toggleSevenZeroRule();
		break;
	case 6:
		toggleJumpInRule();
		break;
	case 7:
		toggleForcedPlayRule();
		break;
	case 8:
		toggleNoBluffingRule();
		break;
	case 9:
		cycleScoreLimit();
		break;
	case 10:
		resetRulesToDefault();
		break;
	}
}

void Lobby_Scr::toggleNumberOfPlayers()
{
	_ruleSet->setTwoPlayers(!_ruleSet->getOnlyTwoPlayers());
	updatePlayerNumberStatus();
}

void Lobby_Scr::updatePlayerNumberStatus()
{
	_playerList.at(2)->setEnabled(!_ruleSet->getOnlyTwoPlayers());
	_playerList.at(3)->setEnabled(!_ruleSet->getOnlyTwoPlayers());

	_ruleStrings["twoPlayerPrefix"].setString(_ruleSet->getOnlyTwoPlayers() ? "Two Players:" : "Four Players:");
	_ruleStrings["twoPlayerRule"].setString(_ruleSet->getOnlyTwoPlayers() ? "Reverse is Skip" : "Normal Rules for Reverse");
}

void Lobby_Scr::toggleStackRule()
{
	_ruleSet->setCanStackCards(!_ruleSet->canStackCards());
	updateStackRuleLabel();
}

void Lobby_Scr::updateStackRuleLabel()
{
	_ruleStrings["stackRule"].setString(_ruleSet->canStackCards() ? "Stacking +2/+4: On" : "Stacking +2/+4: Off");
}

void Lobby_Scr::toggleDrawTillCanPlayRule()
{
	_ruleSet->setDrawnTillCanPlay(!_ruleSet->shouldDrawnTillCanPlay());
	updateDrawTillCanPlayRuleLabel();
}

void Lobby_Scr::updateDrawTillCanPlayRuleLabel()
{
	_ruleStrings["drawTillCanPlayRule"].setString(_ruleSet->shouldDrawnTillCanPlay() ? "Draw Till Can Play: On" : "Draw Till Can Play: Off");
}

void Lobby_Scr::toggleSevenZeroRule()
{
	_ruleSet->setSevenZeroRule(!_ruleSet->getSevenZeroRule());
	updateSevenZeroRuleLabel();
}

void Lobby_Scr::updateSevenZeroRuleLabel()
{
	_ruleStrings["sevenZeroRule"].setString(_ruleSet->getSevenZeroRule() ? "Seven-0: On (7=Swap, 0=Pass All)" : "Seven-0: Off");
}

void Lobby_Scr::toggleForcedPlayRule()
{
	_ruleSet->setForcedPlayRule(!_ruleSet->getForcedPlayRule());
	updateForcedPlayRuleLabel();
}

void Lobby_Scr::updateForcedPlayRuleLabel()
{
	_ruleStrings["forcedPlayRule"].setString(_ruleSet->getForcedPlayRule() ? "Forced Play: On" : "Forced Play: Off");
}

void Lobby_Scr::toggleJumpInRule()
{
	_ruleSet->setAllowJumpInRule(!_ruleSet->allowJumpInRule());
	updateJumpInRuleLabel();
}

void Lobby_Scr::updateJumpInRuleLabel()
{
	_ruleStrings["jumpInRule"].setString(_ruleSet->allowJumpInRule() ? "Jump In: On" : "Jump In: Off");
}

void Lobby_Scr::toggleNoBluffingRule()
{
	_ruleSet->setNoBuffingRule(!_ruleSet->getNoBluffingRule());
	updateNoBluffingRuleLabel();
}

void Lobby_Scr::updateNoBluffingRuleLabel()
{
	_ruleStrings["noBluffingRule"].setString(_ruleSet->getNoBluffingRule() ? "No Bluffing: On" : "No Bluffing: Off");
}

void Lobby_Scr::cycleScoreLimit()
{
	switch (_ruleSet->getScoreLimitType())
	{
	case ScoreLimitType::OneRound:
		_ruleSet->setScoreLimitType(ScoreLimitType::Score200);
		break;
	case ScoreLimitType::Score200:
		_ruleSet->setScoreLimitType(ScoreLimitType::Score300);
		break;
	case ScoreLimitType::Score300:
		_ruleSet->setScoreLimitType(ScoreLimitType::Score500);
		break;
	case ScoreLimitType::Score500:
		_ruleSet->setScoreLimitType(ScoreLimitType::Unlimited);
		break;
	case ScoreLimitType::Unlimited:
		_ruleSet->setScoreLimitType(ScoreLimitType::OneRound);
		break;
	}
	updateScoreLimitLabel();
}

void Lobby_Scr::updateScoreLimitLabel()
{
	switch (_ruleSet->getScoreLimitType())
	{
	case ScoreLimitType::OneRound:
		_ruleStrings["scoreLimit"].setString("Score Limit: One Round");
		break;
	case ScoreLimitType::Score200:
		_ruleStrings["scoreLimit"].setString("Score Limit: 200 Points");
		break;
	case ScoreLimitType::Score300:
		_ruleStrings["scoreLimit"].setString("Score Limit: 300 Points");
		break;
	case ScoreLimitType::Score500:
		_ruleStrings["scoreLimit"].setString("Score Limit: 500 Points");
		break;
	case ScoreLimitType::Unlimited:
		_ruleStrings["scoreLimit"].setString("Score Limit: Unlimited");
		break;
	}
}

void Lobby_Scr::resetRulesToDefault()
{
	_ruleSet->setToDefaults();
	updateAllRuleLabels();
}

void Lobby_Scr::updateAllRuleLabels()
{
	updateStackRuleLabel();
	updateDrawTillCanPlayRuleLabel();
	updatePlayerNumberStatus();
	updateSevenZeroRuleLabel();
	updateJumpInRuleLabel();
	updateForcedPlayRuleLabel();
	updateNoBluffingRuleLabel();
	updateScoreLimitLabel();
}

std::vector<std::string> Lobby_Scr::getRandomAINameList(std::default_random_engine &randomEngine)
{
	std::vector<std::string> names;
	// std::ifstream file("../AINameList.txt");
	std::ifstream file("AINameList.txt");
	if (!file.is_open())
	{
		std::cerr << "ERROR: Failed to load the AINameList.txt file." << std::endl;
		names.push_back("LOADERROR");
		names.push_back("LOADERROR");
		names.push_back("LOADERROR");
		return names;
	}
	std::string name;
	while (std::getline(file, name))
	{
		names.push_back(name);
	}
	file.close();

	if (names.size() < 3)
	{
		std::cerr << "ERROR: AINameList.txt has less than 3 names in it." << std::endl;
		names.push_back("LOADERROR");
		names.push_back("LOADERROR");
		names.push_back("LOADERROR");
		return names;
	}

	std::shuffle(names.begin(), names.end(), randomEngine);
	return names;
}

void Lobby_Scr::initialiseRuleOptions(const sf::IntRect &bounds, const sf::Font &font)
{
	_ruleStrings["twoPlayerPrefix"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["twoPlayerPrefix"].setPosition(sf::Vector2f(bounds.width / 2 + 140, 270 - 20));
	_ruleStrings["twoPlayerRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["twoPlayerRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 270 - 20));
	_ruleStrings["stackRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["stackRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 175 - 20));
	_ruleStrings["drawTillCanPlayRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["drawTillCanPlayRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 225 - 20));
	_ruleStrings["sevenZeroRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["sevenZeroRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 315 - 20));
	_ruleStrings["forcedPlayRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["forcedPlayRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 415 - 20));
	_ruleStrings["jumpInRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["jumpInRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 365 - 20));
	_ruleStrings["noBluffingRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["noBluffingRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 465 - 20));
	_ruleStrings["scoreLimit"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["scoreLimit"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 515 - 20));

	for (auto &[key, textElement] : _ruleStrings)
	{
		textElement.setStyle(sf::Text::Bold);
		textElement.setFillColor(sf::Color::Black);
	}

	updateAllRuleLabels();
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width / 4 - 150, bounds.height - 100, 300, 60), "Toggle Number of Players", 1, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width * 3 / 4 - 150, bounds.height - 100, 300, 60), "Start Game", 2, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width / 2 + 120, 145, 150, 40), "Toggle Rule", 3, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width / 2 + 120, 195, 150, 40), "Toggle Rule", 4, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width / 2 + 120, 285, 150, 40), "Toggle Rule", 5, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width / 2 + 120, 335, 150, 40), "Toggle Rule", 6, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width / 2 + 120, 385, 150, 40), "Toggle Rule", 7, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width / 2 + 120, 435, 150, 40), "Toggle Rule", 8, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width / 2 + 120, 485, 150, 40), "Cycle Limit", 9, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.width * 3 / 4 - 100, 535, 200, 40), "Reset To Default", 10, font));
}

void Lobby_Scr::initialiseBackground(const sf::IntRect &bounds, const sf::Font &font)
{
	_background = new Draw_Obj_Grp();
	Draw_Shp *leftRegion = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(bounds.width / 2 + 20 - 2, 500 - 2)), sf::Color(105, 105, 105, 128), sf::Vector2f(10 + 1, 80 + 1));
	leftRegion->setBorder(1, sf::Color::Black);
	_background->add_Child(leftRegion);
	Draw_Shp *rightRegion = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(bounds.width / 2 - 60 - 2, 500 - 2)), sf::Color(105, 105, 105, 128), sf::Vector2f(bounds.width / 2 + 40 + 1, 80 + 1));
	rightRegion->setBorder(1, sf::Color::Black);
	_background->add_Child(rightRegion);

	Draw_Txt *rulesTitle = new Draw_Txt("Rules", font, 30, sf::Color::Black, sf::Text::Bold);
	rulesTitle->setOffset(sf::Vector2f(bounds.width / 2 + 280, 120 - 30));
	_background->add_Child(rulesTitle);

	// UNO Text with shadow
	Draw_Txt *unoShadow = new Draw_Txt("UNO", font, 40, sf::Color::Black, sf::Text::Bold);
	unoShadow->setOffset(sf::Vector2f(bounds.width / 2 - 40, 50 - 40));
	_background->add_Child(unoShadow);
	Draw_Txt *uLetter = new Draw_Txt("U", font, 40, sf::Color::Yellow, sf::Text::Bold);
	uLetter->setOffset(sf::Vector2f(bounds.width / 2 - 40 + 2, 48 - 40));
	_background->add_Child(uLetter);
	Draw_Txt *nLetter = new Draw_Txt("N", font, 40, sf::Color::Yellow, sf::Text::Bold);
	nLetter->setOffset(sf::Vector2f(bounds.width / 2 - 40 + 2 + 30, 48 - 40));
	_background->add_Child(nLetter);
	Draw_Txt *oLetter = new Draw_Txt("O", font, 40, sf::Color::Yellow, sf::Text::Bold);
	oLetter->setOffset(sf::Vector2f(bounds.width / 2 - 40 + 2 + 60, 48 - 40));
	_background->add_Child(oLetter);


	// Credits
	Draw_Txt *credits = new Draw_Txt("By Afshad, Moosa and Moiz", font, 10, sf::Color::Black, sf::Text::Bold);
	credits->setOffset(sf::Vector2f(bounds.width / 2 - 85, 65 - 10));
	_background->add_Child(credits);

	// Force everything to position itself correctly if it was not.
	_background->setPositionWithOffset(sf::Vector2f(bounds.left, bounds.top));
}
