#include "ASTNode.hpp"

#include <stack>
#include <vector>

bool isBinaryOperator(char token) {
  return (token == '&' || token == '|' || token == '^' || token == '>' || token == '=');
}

ASTNode *ASTNode::parseExpression(const std::vector<char> &tokens) {
  std::stack<ASTNode *> operandStack;

  try {
    for (int i = 0; i < static_cast<int>(tokens.size()); i++) {
      char token = tokens[i];
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
        if (isalpha(token)) {
          ASTNode *operandNode = new OperandNode(token, 0);
          operandStack.push(operandNode);
        } else {
          int value = token - '0';
          ASTNode *operandNode = new OperandNode(value);
          operandStack.push(operandNode);
        }
      }
    }

    // After parsing all tokens, the top of the operand stack should contain the final parsed expression
    if (operandStack.size() != 1) {
      throw std::runtime_error("Invalid expression: Unexpected number of operands");
    }

    ASTNode *parsedExpression = operandStack.top();
    operandStack.pop();
    return parsedExpression;
  } catch (const std::exception &e) {
    // Clean up the remaining nodes in the operand stack
    while (!operandStack.empty()) {
      ASTNode *node = operandStack.top();
      operandStack.pop();
      delete node;
    }

    std::cout << e.what() << std::endl;
    return nullptr;  // Return nullptr or some error value indicating failure
  }
}

void ASTNode::printAST(const std::string &prefix, bool isLeft) const {
  std::cout << prefix;
  std::cout << (isLeft ? "├──" : "└──");

  if (const OperandNode *operandNode = dynamic_cast<const OperandNode *>(this)) {
    // Print operand node value
    if (isalpha(operandNode->getVariable()))
      std::cout << "Operand Variable: " << operandNode->getVariable() << " / Value: " << operandNode->getValue() << std::endl;
    else
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

ASTNode *ASTNode::expand() {
  if (const UnaryOperationNode *unaryNode = dynamic_cast<const UnaryOperationNode *>(this)) {
    if (unaryNode->getOperator() == '!') {
      if (const BinaryOperationNode *binaryNode = dynamic_cast<const BinaryOperationNode *>(unaryNode->getOperand())) {
        // Expand negation of a binary operation
        ASTNode *leftExpansion = new UnaryOperationNode('!', binaryNode->getLeft()->expand());
        ASTNode *rightExpansion = new UnaryOperationNode('!', binaryNode->getRight()->expand());
        return new BinaryOperationNode(binaryNode->getOperator(), leftExpansion, rightExpansion);
      } else if (const UnaryOperationNode *innerUnaryNode = dynamic_cast<const UnaryOperationNode *>(unaryNode->getOperand())) {
        // Expand double negation
        return innerUnaryNode->getOperand()->expand();
      }
    }
    // Expand negation of a variable or operand
    return new UnaryOperationNode('!', unaryNode->getOperand()->expand());
  } else if (const BinaryOperationNode *binaryNode = dynamic_cast<const BinaryOperationNode *>(this)) {
    ASTNode *leftExpansion = binaryNode->getLeft()->expand();
    ASTNode *rightExpansion = binaryNode->getRight()->expand();
    if (binaryNode->getOperator() == '&') {
      return new BinaryOperationNode('&', leftExpansion, rightExpansion);
    } else if (binaryNode->getOperator() == '|') {
      ASTNode *negatedLeft = new UnaryOperationNode('!', leftExpansion);
      ASTNode *negatedRight = new UnaryOperationNode('!', rightExpansion);
      return new BinaryOperationNode('&', negatedLeft, negatedRight);
    } else if (binaryNode->getOperator() == '^') {
      ASTNode *leftAndRight = new BinaryOperationNode('&', leftExpansion, rightExpansion);
      ASTNode *negatedLeftAndRight = new UnaryOperationNode('!', leftAndRight);
      ASTNode *leftAndNegatedRight = new BinaryOperationNode('&', leftExpansion, new UnaryOperationNode('!', rightExpansion));
      return new BinaryOperationNode('&', negatedLeftAndRight, leftAndNegatedRight);
    } else if (binaryNode->getOperator() == '>') {
      ASTNode *leftNegated = new UnaryOperationNode('!', leftExpansion);
      return new BinaryOperationNode('|', leftNegated, rightExpansion);
    } else {
      throw std::runtime_error("Invalid binary operator");
    }
  } else if (const OperandNode *operandNode = dynamic_cast<const OperandNode *>(this)) {
    // For operand nodes, just return the current node
    return new OperandNode(operandNode->getVariable(), operandNode->getValue());
  }

  throw std::runtime_error("Invalid node type");
}

ASTNode *ASTNode::expandLogicalOr(ASTNode *left, ASTNode *right) {
  ASTNode *negatedLeft = new UnaryOperationNode('!', left);
  ASTNode *negatedRight = new UnaryOperationNode('!', right);
  return new BinaryOperationNode('&', negatedLeft, negatedRight);
}

ASTNode *ASTNode::expandExclusiveOr(ASTNode *left, ASTNode *right) {
  ASTNode *leftAndRight = new BinaryOperationNode('&', left, right);
  ASTNode *negatedLeftAndRight = new UnaryOperationNode('!', leftAndRight);
  ASTNode *leftAndNegatedRight = new BinaryOperationNode('&', left, new UnaryOperationNode('!', right));
  return new BinaryOperationNode('&', negatedLeftAndRight, leftAndNegatedRight);
}

ASTNode *ASTNode::expandImplication(ASTNode *left, ASTNode *right) {
  ASTNode *leftNegated = new UnaryOperationNode('!', left);
  return new BinaryOperationNode('|', leftNegated, right);
}
