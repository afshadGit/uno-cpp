#include "Pause_Scr.h"

Pause_Scr::Pause_Scr(const sf::IntRect & bounds, const sf::IntRect& gameBounds, const sf::Font & font)
	: Game_Scr(bounds)
{
	initialiseInterface(bounds, gameBounds, font);
	_resultState = Game_Result_State::NothingState;
}

Pause_Scr::~Pause_Scr()
{
	delete _interfaceBackground;
}

void Pause_Scr::draw(sf::RenderWindow & renderWindow) const
{
	_interfaceBackground->draw(renderWindow);
	for (const auto& button : _buttonList) {
		button.draw(renderWindow);
	}
}

void Pause_Scr::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	for (const auto& button : _buttonList) {
		if (button.isPositionInside(mousePosition)) {
			handleButtonAction(button.getActionID());
			break;
		}
	}
}

void Pause_Scr::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		button.setHovering(button.isPositionInside(mousePosition));
	}
}

Game_Result_State Pause_Scr::getResultState() const
{
	return _resultState;
}

void Pause_Scr::resetResultState()
{
	_resultState = Game_Result_State::NothingState;
}

void Pause_Scr::initialiseInterface(const sf::IntRect & bounds, const sf::IntRect& gameBounds, const sf::Font & font)
{
	_interfaceBackground = new Draw_Obj_Grp();
	Draw_Shp* transparentBackground = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(gameBounds.width, gameBounds.height)), sf::Color(144, 143, 143, 204));
	_interfaceBackground->add_Child(transparentBackground);
	Draw_Shp* mainBackground = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(bounds.width - 2, bounds.height - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(bounds.left + 1, bounds.top + 1));
	mainBackground->setBorder(1, sf::Color::Black);
	_interfaceBackground->add_Child(mainBackground);
	Draw_Shp* leftBackground = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(160 - 2, 90 - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(170 + 1, 300 + 1));
	_interfaceBackground->add_Child(leftBackground);
	leftBackground->setBorder(1, sf::Color::Black);
	Draw_Shp* rightBackground = new Draw_Shp(new sf::RectangleShape(sf::Vector2f(420 - 2, 270 - 2)), sf::Color(165, 177, 94, 205), sf::Vector2f(790+1, 220+1));
	_interfaceBackground->add_Child(rightBackground);
	rightBackground->setBorder(1, sf::Color::Black);

	Draw_Txt* titleText = new Draw_Txt("Paused", font, 40, sf::Color::Black, sf::Text::Bold);
	titleText->setOffset(sf::Vector2f(bounds.left + bounds.width / 2 - titleText->getTextWidth() / 2, bounds.top));
	_interfaceBackground->add_Child(titleText);

	// LEFT SIZE TEXT
	Draw_Txt* leftTextTitle = new Draw_Txt("Controls", font, 20, sf::Color::Black, sf::Text::Bold);
	leftTextTitle->setOffset(sf::Vector2f(210, 300));
	_interfaceBackground->add_Child(leftTextTitle);

	std::vector<std::string> leftTextArray;
	leftTextArray.push_back("Escape: Pause");
	leftTextArray.push_back("Q: Sort hand");

	for (int y = 0; y < leftTextArray.size(); y++) {
		Draw_Txt* leftText = new Draw_Txt(leftTextArray.at(y), font, 20, sf::Color::Black, sf::Text::Bold);
		leftText->setOffset(sf::Vector2f(180, 325+y*30));
		_interfaceBackground->add_Child(leftText);
	}

	// RIGHT SIDE TEXT
	Draw_Txt* rightTextTitle = new Draw_Txt("Debug Controls (0 first)", font, 20, sf::Color::Black, sf::Text::Bold);
	rightTextTitle->setOffset(sf::Vector2f(880, 230));
	_interfaceBackground->add_Child(rightTextTitle);

	std::vector<std::string> rightTextArray;
	rightTextArray.push_back("0: Turn on Debug");
	rightTextArray.push_back("9: Reveal All Hands");
	rightTextArray.push_back("8: Toggle Turn Direction");
	rightTextArray.push_back("7: Empty Player Hand");
	rightTextArray.push_back("6: Remove Left Player Card");
	rightTextArray.push_back("5: Toggle Show Turn Action Sequence");
	rightTextArray.push_back("4: Toggle Show Turn Action Tree On Set");

	for (int y = 0; y < rightTextArray.size(); y++) {
		Draw_Txt* rightText = new Draw_Txt(rightTextArray.at(y), font, 20, sf::Color::Black, sf::Text::Bold);
		rightText->setOffset(sf::Vector2f(800, 270 + y * 30));
		_interfaceBackground->add_Child(rightText);
	}

	_buttonList.emplace_back(Btn(sf::IntRect(bounds.left + 6, bounds.top + 6 + 60, bounds.width - 12, 30), "Resume", 1, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.left + 6, bounds.top + 6 + 30+6+60, bounds.width - 12, 30), "Return to Lobby", 3, font));
	_buttonList.emplace_back(Btn(sf::IntRect(bounds.left + 6, bounds.top + 6 + (30+6)*2 + 60, bounds.width - 12, 30), "Quit Game", 2, font));

	// Force all components to update to their correct positions.
	_interfaceBackground->setPositionWithOffset(sf::Vector2f(gameBounds.left, gameBounds.top));
}

void Pause_Scr::handleButtonAction(const int actionID)
{
	switch (actionID) {
		case 1: _resultState = Game_Result_State::Finished; break; // gamePanel.setPauseState(false);
		case 2: _resultState = Game_Result_State::Quit; break; // _gamePanel.quitGame();
		case 3: _resultState = Game_Result_State::Menu; break; // gamePanel.showLobby();
	}
}
