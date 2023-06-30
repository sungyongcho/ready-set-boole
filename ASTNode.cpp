#include "ASTNode.hpp"

#include <stack>
#include <vector>

void ASTNode::tokenize(std::string str, std::string symbols) {
  try {
    for (int i = 0; i < (int)str.size(); i++) {
      if (symbols.find(str[i]) != std::string::npos) {
        this->tokens.push_back(str[i]);
      } else {
        this->tokens.clear();  // Clear the vector
        throw std::invalid_argument("Invalid character input - [" + std::string(1, str[i]) + "]");
      }
    }
  } catch (const std::invalid_argument &e) {
    std::cout << "Error: " << e.what() << std::endl;
    this->tokens.clear();  // Clear the vector
  }
}

bool isBinaryOperator(char token) {
  return (token == '&' || token == '|' || token == '^' || token == '>' || token == '=');
}

void ASTNode::parseExpression(void) {
  std::stack<ASTNode *> operandStack;

  try {
    for (int i = 0; i < static_cast<int>(this->tokens.size()); i++) {
      char token = this->tokens[i];
      if (isBinaryOperator(token)) {
        // Binary operation
        if (operandStack.size() < 2) {
          throw std::runtime_error("Invalid expression: Insufficient operands for binary operation");
        }

        ASTNode *rightOperand = operandStack.top();
        operandStack.pop();
        ASTNode *leftOperand = operandStack.top();
        operandStack.pop();
        ASTNode *binaryOperationNode = new BinaryOperationNode(token, leftOperand, rightOperand);
        operandStack.push(binaryOperationNode);
      } else if (token == '!') {
        // Unary operation
        if (operandStack.empty()) {
          throw std::runtime_error("Invalid expression: Insufficient operand for unary operation");
        }

        ASTNode *operand = operandStack.top();
        operandStack.pop();
        ASTNode *unaryOperationNode = new UnaryOperationNode(token, operand);
        operandStack.push(unaryOperationNode);
      } else {
        // Operand (numeric literal)
        if (!isdigit(token)) {
          throw std::runtime_error("Invalid expression: Invalid operand token");
        }

        int value = token - '0';
        ASTNode *operandNode = new OperandNode(value);
        operandStack.push(operandNode);
      }
    }

    // After parsing all tokens, the top of the operand stack should contain the final parsed expression
    if (operandStack.size() != 1) {
      throw std::runtime_error("Invalid expression: Unexpected number of operands");
    }

    ASTNode *parsedExpression = operandStack.top();
    operandStack.pop();
    // return parsedExpression;
  } catch (const std::exception &e) {
    // Clean up the remaining nodes in the operand stack
    while (!operandStack.empty()) {
      ASTNode *node = operandStack.top();
      operandStack.pop();
      delete node;
    }

    std::cout << e.what() << std::endl;
    // return nullptr;  // Return nullptr or some error value indicating failure
  }
}

void ASTNode::printAST(const std::string &prefix, bool isLeft) const {
  std::cout << prefix;
  std::cout << (isLeft ? "├──" : "└──");

  if (const OperandNode *operandNode = dynamic_cast<const OperandNode *>(this)) {
    // Print operand node value
    std::cout << "Operand: " << operandNode->getValue() << std::endl;
  } else if (const UnaryOperationNode *unaryNode = dynamic_cast<const UnaryOperationNode *>(this)) {
    std::cout << "Unary Operation: " << unaryNode->getOperator() << std::endl;
    // Print left child
    unaryNode->getOperand()->printAST(prefix + (isLeft ? "│   " : "    "), true);

  } else if (const BinaryOperationNode *binaryNode = dynamic_cast<const BinaryOperationNode *>(this)) {
    // Print operator symbol
    std::cout << "Binary Operation: " << binaryNode->getOperator() << std::endl;
    // Print left child
    binaryNode->getLeft()->printAST(prefix + (isLeft ? "│   " : "    "), true);

    // Print right child
    binaryNode->getRight()->printAST(prefix + (isLeft ? "│   " : "    "), false);
  }
  // Add conditions for other types of nodes if needed
}
