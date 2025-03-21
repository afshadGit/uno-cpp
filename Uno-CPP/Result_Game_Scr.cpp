#include "Result_Game_Scr.h"
#include "Draw_Path.h"

Result_Game_Scr::Result_Game_Scr(const sf::IntRect &bounds, const sf::Font &font, const std::vector<Player *> &playerList, RuleSet *ruleSet)
	: Game_Scr(bounds), _playerList(playerList)
{
	initialiseInterface(font, ruleSet);
	_resultState = Game_Result_State::NothingState;
	_rules = ruleSet;
}

void Result_Game_Scr::draw(sf::RenderWindow &renderWindow) const
{
	_staticElements->draw(renderWindow);
	for (const auto &button : _buttonList)
		button.draw(renderWindow);
}

void Result_Game_Scr::handleMousePress(const sf::Vector2i &mousePosition, bool isLeft)
{
	if (!isEnabled())
		return;
	for (const auto &button : _buttonList)
	{
		if (button.isPositionInside(mousePosition))
			handleButtonPress(button.getActionID());
	}
}

void Result_Game_Scr::handleMouseMove(const sf::Vector2i &mousePosition)
{
	if (!isEnabled())
		return;
	for (auto &button : _buttonList)
	{
		button.setHovering(button.isPositionInside(mousePosition));
	}
}

Game_Result_State Result_Game_Scr::getResultState() const { return _resultState; }

std::vector<Player *> Result_Game_Scr::getPlayers() const { return _playerList; }

RuleSet *Result_Game_Scr::getRuleSet() const { return _rules; }

void Result_Game_Scr::handleButtonPress(const int actionID)
{
	switch (actionID)
	{
	case 1:
		_resultState = Game_Result_State::Menu;
		break;
	case 3:
		for (auto player : _playerList)
			player->resetScore();
	case 2:
		_resultState = Game_Result_State::Finished;
		break;
	}
}

void Result_Game_Scr::initialiseInterface(const sf::Font &font, RuleSet *ruleSet)
{
	_staticElements = new Draw_Obj_Grp();
	Draw_Shp *background = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(_bounds.width / 2 - 2, 500 - 2)), sf::Color(205, 138, 78, 128), sf::Vector2f(_bounds.width / 4 + 1, 80 + 1));
	background->setBorder(1, sf::Color::Black);
	_staticElements->add_Child(background);

	
	Draw_Txt *uLetter = new Draw_Txt("U", font, 40, Cards::getColourByID(0), sf::Text::Bold);
	uLetter->setOffset(sf::Vector2f(_bounds.width / 2 - 40 + 2, 8));
	_staticElements->add_Child(uLetter);
	Draw_Txt *nLetter = new Draw_Txt("N", font, 40, Cards::getColourByID(1), sf::Text::Bold);
	nLetter->setOffset(sf::Vector2f(_bounds.width / 2 - 40 + 2 + 30, 8));
	_staticElements->add_Child(nLetter);
	Draw_Txt *oLetter = new Draw_Txt("O", font, 40, Cards::getColourByID(2), sf::Text::Bold);
	oLetter->setOffset(sf::Vector2f(_bounds.width / 2 - 40 + 2 + 60, 8));
	_staticElements->add_Child(oLetter);
	Draw_Txt *exclamationMark = new Draw_Txt("!", font, 40, Cards::getColourByID(3), sf::Text::Bold);
	exclamationMark->setOffset(sf::Vector2f(_bounds.width / 2 - 40 + 2 + 90, 8));
	_staticElements->add_Child(exclamationMark);

	Draw_Txt *titleText = new Draw_Txt("Post-Game Summary", font, 40, sf::Color::Black, sf::Text::Bold);
	titleText->setOffset(sf::Vector2f(_bounds.width / 2 - titleText->getTextWidth() / 2, 80));
	_staticElements->add_Child(titleText);

	Draw_Path *topLine = new Draw_Path({sf::Vector2f(_bounds.width / 4 + 10, 200), sf::Vector2f(_bounds.width * 3 / 4 - 10, 200)});
	_staticElements->add_Child(topLine);

	Draw_Txt *roundScoreTitle = new Draw_Txt("Round Score", font, 20, sf::Color::Black, sf::Text::Bold);
	roundScoreTitle->setOffset(sf::Vector2f(_bounds.width / 2 - 20, 160));
	_staticElements->add_Child(roundScoreTitle);
	Draw_Txt *totalScoreTitle = new Draw_Txt("Total Score", font, 20, sf::Color::Black, sf::Text::Bold);
	totalScoreTitle->setOffset(sf::Vector2f(_bounds.width / 2 + 165, 160));
	_staticElements->add_Child(totalScoreTitle);

	Player *winner = _playerList.at(0);
	for (int i = 0; i < _playerList.size(); i++)
	{
		if (_playerList.at(i)->getWon())
		{
			winner = _playerList.at(i);
		}

		Draw_Path *dividerLine = new Draw_Path({sf::Vector2f(_bounds.width / 4 + 10, 260 + i * 60), sf::Vector2f(_bounds.width * 3 / 4 - 10, 260 + i * 60)});
		_staticElements->add_Child(dividerLine);
		std::string playerString = (_playerList.at(i)->getPlayerType() == 0 ? "You: " : "AI: ") + _playerList.at(i)->getPlayerName();
		Draw_Txt *playerStringText = new Draw_Txt(playerString, font, 20, sf::Color::Black, sf::Text::Bold);
		playerStringText->setOffset(sf::Vector2f(_bounds.width / 4 + 50, 220 + i * 60));
		_staticElements->add_Child(playerStringText);
		Draw_Txt *currentRoundScore = new Draw_Txt(std::to_string(_playerList.at(i)->getCurrentRoundScore()), font, 20, sf::Color::Black, sf::Text::Bold);
		currentRoundScore->setOffset(sf::Vector2f(_bounds.width / 2, 220 + i * 60));
		_staticElements->add_Child(currentRoundScore);
		Draw_Txt *totalScore = new Draw_Txt(std::to_string(_playerList.at(i)->getTotalScore()), font, 20, sf::Color::Black, sf::Text::Bold);
		totalScore->setOffset(sf::Vector2f(_bounds.width / 2 + 180, 220 + i * 60));
		_staticElements->add_Child(totalScore);
	}

	Draw_Path *line1 = new Draw_Path({sf::Vector2f(_bounds.width / 4 + 10, 200), sf::Vector2f(_bounds.width / 4 + 10, 200 + _playerList.size() * 60)});
	_staticElements->add_Child(line1);
	Draw_Path *line2 = new Draw_Path({sf::Vector2f(_bounds.width / 2 - 40, 150), sf::Vector2f(_bounds.width / 2 - 40, 200 + _playerList.size() * 60)});
	_staticElements->add_Child(line2);
	Draw_Path *line3 = new Draw_Path({sf::Vector2f(_bounds.width / 2 + 130, 150), sf::Vector2f(_bounds.width / 2 + 130, 200 + _playerList.size() * 60)});
	_staticElements->add_Child(line3);
	Draw_Path *line4 = new Draw_Path({sf::Vector2f(_bounds.width * 3 / 4 - 10, 150), sf::Vector2f(_bounds.width * 3 / 4 - 10, 200 + _playerList.size() * 60)});
	_staticElements->add_Child(line4);
	Draw_Path *line5 = new Draw_Path({sf::Vector2f(_bounds.width / 2 - 40, 150), sf::Vector2f(_bounds.width * 3 / 4 - 10, 150)});
	_staticElements->add_Child(line5);

	std::string scoreLimitStr;
	bool scoreLimitReached;
	switch (ruleSet->getScoreLimitType())
	{
	case ScoreLimitType::OneRound:
		scoreLimitReached = true;
		scoreLimitStr = "Score Limit: One Round";
		break;
	case ScoreLimitType::Score200:
		scoreLimitReached = winner->getTotalScore() >= 200;
		scoreLimitStr = "Score Limit: 200 Points";
		break;
	case ScoreLimitType::Score300:
		scoreLimitReached = winner->getTotalScore() >= 300;
		scoreLimitStr = "Score Limit: 300 Points";
		break;
	case ScoreLimitType::Score500:
		scoreLimitReached = winner->getTotalScore() >= 500;
		scoreLimitStr = "Score Limit: 500 Points";
		break;
	case ScoreLimitType::Unlimited:
		scoreLimitReached = false;
		scoreLimitStr = "Score Limit: Unlimited";
		break;
	}

	Draw_Txt *winnerTitle = new Draw_Txt("Round Winner: ", font, 20, sf::Color::Black, sf::Text::Bold);
	winnerTitle->setOffset(sf::Vector2f(_bounds.width / 4 + 25, 470));
	_staticElements->add_Child(winnerTitle);
	Draw_Txt *winnerLabel = new Draw_Txt(winner->getPlayerName(), font, 20, sf::Color::Black, sf::Text::Bold);
	winnerLabel->setOffset(sf::Vector2f(_bounds.width / 4 + 175, 470));
	_staticElements->add_Child(winnerLabel);
	Draw_Txt *scoreLimitText = new Draw_Txt(scoreLimitStr, font, 20, sf::Color::Black, sf::Text::Bold);
	scoreLimitText->setOffset(sf::Vector2f(_bounds.width / 2 + 20, 470));
	_staticElements->add_Child(scoreLimitText);

	if (scoreLimitReached)
	{
		Draw_Txt *scoreLimitReachedText = new Draw_Txt("Score limit reached!", font, 20, sf::Color::Black, sf::Text::Bold);
		scoreLimitReachedText->setOffset(sf::Vector2f(_bounds.width / 2 + 40, 510));
		_staticElements->add_Child(scoreLimitReachedText);

		_buttonList.emplace_back(Btn(sf::IntRect(_bounds.width / 2 - 250 - 10, 620, 250, 40), "Return to Lobby", 1, font));
		_buttonList.emplace_back(Btn(sf::IntRect(_bounds.width / 2 + 10, 620, 270, 40), "Close Game", 3, font));
	}
	else
	{
		_buttonList.emplace_back(Btn(sf::IntRect(_bounds.width / 2 - 125 - 250 - 20, 620, 250, 40), "Return to Lobby", 1, font));
		_buttonList.emplace_back(Btn(sf::IntRect(_bounds.width / 2 - 125, 620, 250, 40), "Continue Next Round", 2, font));
		_buttonList.emplace_back(Btn(sf::IntRect(_bounds.width / 2 + 125 + 20, 620, 270, 40), "Close game", 3, font));
	}

	_staticElements->setPositionWithOffset(sf::Vector2f(0, 0));
}

Result_Game_Scr::~Result_Game_Scr()
{
	delete _staticElements;
}