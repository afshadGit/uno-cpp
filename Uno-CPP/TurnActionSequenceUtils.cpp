// #include "TurnActionSequenceUtils.h"

// #include "TurnDecisionAction.h"
// #include <iostream>

// void debugRecursiveNodeOutput(Turn_Op *currentNode, int indentLevel)
// {
// 	if (currentNode == nullptr)
// 		return;
// 	for (int i = 0; i < indentLevel; i++)
// 		std::cout << "\t";

// 	TurnDecisionAction *currentSplitNode = dynamic_cast<TurnDecisionAction *>(currentNode);
// 	if (currentSplitNode != nullptr)
// 	{
// 		std::cout << "? " << (indentLevel + 1) << ". " << currentSplitNode->getFlagName()
// 				  << " Timeout: " << currentSplitNode->getTimeOut() << " " << currentSplitNode->getActionDebugText() << std::endl;
// 		debugRecursiveNodeOutput(currentSplitNode->getNextPointer(), indentLevel + 1);
// 		if (currentSplitNode->getNextPointer() != currentSplitNode->getOtherNextPointer())
// 		{
// 			debugRecursiveNodeOutput(currentSplitNode->getOtherNextPointer(), indentLevel + 1);
// 		}
// 	}
// 	else
// 	{
// 		std::cout << "- " << (indentLevel + 1) << ". " << currentNode->getActionDebugText() << std::endl;
// 		debugRecursiveNodeOutput(currentNode->getNextPointer(), indentLevel + 1);
// 	}
// }

#include "TurnActionSequenceUtils.h"
#include "TurnDecisionAction.h"
#include <iostream>
#include <stack>

void debugIterativeNodeOutput(Turn_Op* startNode)
{
    if (startNode == nullptr)
        return;

    // Stack to manage nodes for iterative traversal
    std::stack<std::pair<Turn_Op*, int>> nodeStack;
    nodeStack.push({ startNode, 0 });  // Push the starting node with indent level 0

    while (!nodeStack.empty()) {
        // Get the current node and its indent level
        auto [currentNode, indentLevel] = nodeStack.top();
        nodeStack.pop();

        // Print the indentation for the current level
        for (int i = 0; i < indentLevel; i++) {
            std::cout << "\t";
        }

        // Check if the current node is a TurnDecisionAction
        TurnDecisionAction* currentSplitNode = dynamic_cast<TurnDecisionAction*>(currentNode);
        if (currentSplitNode != nullptr) {
            // If it's a decision node, print its information
            std::cout << "? " << (indentLevel + 1) << ". " << currentSplitNode->getFlagName()
                      << " Timeout: " << currentSplitNode->getTimeOut() << " "
                      << currentSplitNode->getActionDebugText() << std::endl;

            // Push next nodes to the stack for further traversal
            if (currentSplitNode->getNextPointer() != nullptr) {
                nodeStack.push({ currentSplitNode->getNextPointer(), indentLevel + 1 });
            }
            if (currentSplitNode->getOtherNextPointer() != nullptr &&
                currentSplitNode->getNextPointer() != currentSplitNode->getOtherNextPointer()) {
                nodeStack.push({ currentSplitNode->getOtherNextPointer(), indentLevel + 1 });
            }
        } else {
            // If it's a regular node, print its action text
            std::cout << "- " << (indentLevel + 1) << ". " << currentNode->getActionDebugText() << std::endl;

            // Push the next node to the stack if it exists
            if (currentNode->getNextPointer() != nullptr) {
                nodeStack.push({ currentNode->getNextPointer(), indentLevel + 1 });
            }
        }
    }
}

