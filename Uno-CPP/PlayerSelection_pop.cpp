#include "PlayerSelection_pop.h"

PlayerSelection_pop::PlayerSelection_pop(const sf::IntRect & bounds, std::vector<Player*> playerList, const sf::Font & font)
	: Game_Scr(bounds)
{
	setEnabled(false);
	for (int i = 0; i < playerList.size(); i++) {
		if (playerList.at(i)->getPlayerType() != 0) {
			sf::Vector2f centre = playerList.at(i)->getCentreOfBounds();
			_buttonList.emplace_back(Btn(sf::IntRect(centre.x - 100, centre.y - 20, 200, 40), "Choose Player", i, font));
		}
	}
}

PlayerSelection_pop::~PlayerSelection_pop()
{
}

void PlayerSelection_pop::draw(sf::RenderWindow & renderWindow) const
{
	for (const auto& button : _buttonList) {
		button.draw(renderWindow);
	}
}

void PlayerSelection_pop::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		if (button.isPositionInside(mousePosition)) {
			setEnabled(false);
			_currentAction->injectFlagProperty(button.getActionID());
			break;
		}
	}
}

void PlayerSelection_pop::handleMouseMove(const sf::Vector2i & mousePosition)
{
	printf("In PlayerSelectionOverlay");
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		button.setHovering(button.isPositionInside(mousePosition));
	}
}

void PlayerSelection_pop::showOverlay(TurnDecisionAction * currentAction)
{
	_currentAction = currentAction;
	setEnabled(true);
}
