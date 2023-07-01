#include <vector>
#include <iostream>
#include <stack>
#include <set>
#include "ASTNode.hpp"
#include "utils.hpp"

// std::string negation_normal_form(ASTNode* rootNode) {
//     std::set<char> negatedVariables;
//     std::string nnfExpression;

//     std::stack<ASTNode*> stack;
//     stack.push(rootNode);

//     while (!stack.empty()) {
//         ASTNode* node = stack.top();
//         stack.pop();

//         if (OperandNode* operandNode = dynamic_cast<OperandNode*>(node)) {
//             char variable = operandNode->getVariable();
//             if (negatedVariables.count(variable) > 0) {
//                 nnfExpression += '!';
//             }
//             nnfExpression += variable;
//         } else if (UnaryOperationNode* unaryNode = dynamic_cast<UnaryOperationNode*>(node)) {
//             if (unaryNode->getOperator() == '!') {
//                 ASTNode* operand = unaryNode->getOperand();
//                 if (OperandNode* operandNode = dynamic_cast<OperandNode*>(operand)) {
//                     char variable = operandNode->getVariable();
//                     negatedVariables.insert(variable);
//                 }
//                 stack.push(operand);
//             }
//         } else if (BinaryOperationNode* binaryNode = dynamic_cast<BinaryOperationNode*>(node)) {
//             stack.push(binaryNode->getRight());
//             stack.push(binaryNode->getLeft());
//         }
//     }

//     for (std::set<char>::iterator it = negatedVariables.begin(); it != negatedVariables.end(); ++it) {
//         char variable = *it;
//         nnfExpression += '!'; // Add '!' for each negated variable
//         nnfExpression += variable;
//     }

//     return nnfExpression;
// }


int main() {
    std::string formula = "AB^!";
    std::string symbols = "ABCDEFGHIJKLMNOPQURSTUVWXYZ!&|^>=";

    ASTNode* rootNode = nullptr;

    std::vector<char> tokens = tokenize(formula, symbols);
    std::vector<char> variables = extractVariables(tokens);

    rootNode = ASTNode::parseExpression(tokens);

    ASTNode* expandedRoot = rootNode->expand();

    rootNode->printAST();

    expandedRoot->printAST();

    // std::string nnf = negation_normal_form(rootNode);
    // std::cout << nnf << std::endl;
    return 0;
}
