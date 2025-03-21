#pragma once

#include "TurnDecisionAction.h"

class TurnDecision_Pop {
public:

	virtual void showOverlay(TurnDecisionAction* currentAction) = 0;
};