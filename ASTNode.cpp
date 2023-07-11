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

ASTNode *ASTNode::NegNNF(ASTNode *phi) {
  if (phi == nullptr) {
    return nullptr;
  }

  if (const OperandNode *operandNode = dynamic_cast<const OperandNode *>(phi)) {
    // Negate the operand
    char variable = operandNode->getVariable();
    int value = operandNode->getValue();
    return new UnaryOperationNode('!', new OperandNode(variable, value));
  } else if (const UnaryOperationNode *unaryNode = dynamic_cast<const UnaryOperationNode *>(phi)) {
    if (unaryNode->getOperator() == '!') {
      ASTNode *operand = unaryNode->getOperand();
      return NegNNF(operand);  // Recursively apply negation to the operand
    }
  } else if (const BinaryOperationNode *binaryNode = dynamic_cast<const BinaryOperationNode *>(phi)) {
    char op = binaryNode->getOperator();
    ASTNode *left = binaryNode->getLeft();
    ASTNode *right = binaryNode->getRight();

    ASTNode *negNNFLeft = NegNNF(left);
    ASTNode *negNNFRight = NegNNF(right);

    if (op == '|') {
      return new BinaryOperationNode('&', negNNFLeft, negNNFRight);
    } else if (op == '&') {
      return new BinaryOperationNode('|', negNNFLeft, negNNFRight);
    } else if (op == '>') {
      return new BinaryOperationNode('|', NegNNF(negNNFLeft), negNNFRight);
    } else if (op == '=') {
      return new BinaryOperationNode('&', new BinaryOperationNode('|', negNNFLeft, NegNNF(negNNFRight)),
                                     new BinaryOperationNode('|', NegNNF(negNNFLeft), negNNFRight));
    } else if (op == '^') {
      return new BinaryOperationNode('|', new BinaryOperationNode('&', negNNFLeft, NegNNF(negNNFRight)),
                                     new BinaryOperationNode('&', NegNNF(negNNFLeft), negNNFRight));
    }
  }

  throw std::runtime_error("Invalid formula in NNF");
}

ASTNode *ASTNode::BNF2NNF(ASTNode *formula) {
  if (const OperandNode *operandNode = dynamic_cast<const OperandNode *>(formula)) {
    // Base case: formula is an operand (variable)
    return new OperandNode(operandNode->getVariable(), operandNode->getValue());
  } else if (const UnaryOperationNode *unaryNode = dynamic_cast<const UnaryOperationNode *>(formula)) {
    // Unary operation (negation)
    if (unaryNode->getOperator() == '!') {
      return NegNNF(BNF2NNF(unaryNode->getOperand()));
    } else {
      throw std::runtime_error("Invalid unary operator");
    }
  } else if (const BinaryOperationNode *binaryNode = dynamic_cast<const BinaryOperationNode *>(formula)) {
    // Binary operation
    ASTNode *left = BNF2NNF(binaryNode->getLeft());
    ASTNode *right = BNF2NNF(binaryNode->getRight());
    if (binaryNode->getOperator() == '|') {
      return new BinaryOperationNode('|', left, right);
    } else if (binaryNode->getOperator() == '&') {
      return new BinaryOperationNode('&', left, right);
    } else if (binaryNode->getOperator() == '>') {
      return new BinaryOperationNode('|', NegNNF(left), right);
    } else if (binaryNode->getOperator() == '=') {
      return new BinaryOperationNode('&', new BinaryOperationNode('|', left, NegNNF(right)),
                                     new BinaryOperationNode('|', NegNNF(left), right));
    } else if (binaryNode->getOperator() == '^') {
      return new BinaryOperationNode('|', new BinaryOperationNode('&', left, NegNNF(right)),
                                     new BinaryOperationNode('&', NegNNF(left), right));
    } else {
      throw std::runtime_error("Invalid binary operator");
    }
  } else {
    throw std::runtime_error("Invalid node type");
  }
}

void ASTNode::transformDisjunctionToConjunction(ASTNode* node) {
  if (node == nullptr)
    return;

  if (BinaryOperationNode* binaryNode = dynamic_cast<BinaryOperationNode*>(node)) {
    transformDisjunctionToConjunction(binaryNode->getLeft());
    transformDisjunctionToConjunction(binaryNode->getRight());

    if (binaryNode->getOperator() == '|') {
      ASTNode* leftChild = binaryNode->getLeft();
      ASTNode* rightChild = binaryNode->getRight();

      if (leftChild != nullptr && dynamic_cast<BinaryOperationNode*>(leftChild) != nullptr &&
          dynamic_cast<BinaryOperationNode*>(leftChild)->getOperator() == '|') {

        BinaryOperationNode* leftChildBinaryNode = dynamic_cast<BinaryOperationNode*>(leftChild);
        ASTNode* grandchild = leftChildBinaryNode->getRight();

        // Perform right shift
        binaryNode->setLeft(leftChildBinaryNode->getLeft());
        binaryNode->setRight(new BinaryOperationNode('&', grandchild, rightChild));
        delete leftChildBinaryNode;

        // Call the function recursively on the updated binaryNode
        transformDisjunctionToConjunction(binaryNode);
      }
    }
  }
}

void ASTNode::transformDisjunctionToConjunctionTwo(ASTNode* node) {
  if (node == nullptr)
    return;

  if (BinaryOperationNode* binaryNode = dynamic_cast<BinaryOperationNode*>(node)) {
    transformDisjunctionToConjunction(binaryNode->getLeft());
    transformDisjunctionToConjunction(binaryNode->getRight());

    if (binaryNode->getOperator() == '|') {
      ASTNode* leftChild = binaryNode->getLeft();
      ASTNode* rightChild = binaryNode->getRight();

      if (leftChild != nullptr && rightChild != nullptr &&
          dynamic_cast<UnaryOperationNode*>(leftChild) != nullptr &&
          dynamic_cast<UnaryOperationNode*>(rightChild) != nullptr &&
          dynamic_cast<UnaryOperationNode*>(leftChild)->getOperator() == '!' &&
          dynamic_cast<UnaryOperationNode*>(rightChild)->getOperator() == '!') {

        ASTNode* negatedLeftChild = dynamic_cast<UnaryOperationNode*>(leftChild)->getOperand();
        ASTNode* negatedRightChild = dynamic_cast<UnaryOperationNode*>(rightChild)->getOperand();

        // Create a new conjunction node with the negated child nodes
        // ASTNode* conjunctionNode = new BinaryOperationNode('&', negatedLeftChild, negatedRightChild);

        // Replace the root node with the new conjunction node
        binaryNode->setOperator('&');
        binaryNode->setLeft(negatedLeftChild);
        binaryNode->setRight(negatedRightChild);
        // // Replace the root node with the new conjunction node
        // *binaryNode = *conjunctionNode;

        // // Clean up the negation nodes
        // delete conjunctionNode;
        // delete leftChild;
        // delete rightChild;
      }
    }
  }
}

void ASTNode::transformDisjunctionToConjunctionThree(ASTNode* node) {
  if (node == nullptr)
    return;

  if (BinaryOperationNode* binaryNode = dynamic_cast<BinaryOperationNode*>(node)) {
    transformDisjunctionToConjunctionThree(binaryNode->getLeft());
    transformDisjunctionToConjunctionThree(binaryNode->getRight());

    if (binaryNode->getOperator() == '|') {
      ASTNode* leftChild = binaryNode->getLeft();
      ASTNode* rightChild = binaryNode->getRight();

      if (rightChild != nullptr && dynamic_cast<BinaryOperationNode*>(rightChild) != nullptr &&
          dynamic_cast<BinaryOperationNode*>(rightChild)->getOperator() == '&') {

        // Apply distribute low rule
        BinaryOperationNode* rightBinaryNode = dynamic_cast<BinaryOperationNode*>(rightChild);

        ASTNode* leftGrandchild = rightBinaryNode->getLeft();
        ASTNode* rightGrandchild = rightBinaryNode->getRight();

        // Create new disjunction and conjunction nodes
        ASTNode* disjunctionNode = new BinaryOperationNode('&', leftChild, leftGrandchild);
        ASTNode* conjunctionNode = new BinaryOperationNode('&', leftChild, rightGrandchild);

        // Replace the root node with the new disjunction node
        binaryNode->setOperator('|');
        binaryNode->setLeft(disjunctionNode);
        binaryNode->setRight(conjunctionNode);

        // Clean up the original right child node and conjunction node
        // delete rightChild;
        // delete conjunctionNode;
      }
    }
  }
}

void ASTNode::transformOperations(ASTNode* node) {
  if (node == nullptr)
    return;

  if (BinaryOperationNode* binaryNode = dynamic_cast<BinaryOperationNode*>(node)) {
    transformOperations(binaryNode->getLeft());
    transformOperations(binaryNode->getRight());

    if (binaryNode->getOperator() == '&' || binaryNode->getOperator() == '|') {
      ASTNode* leftChild = binaryNode->getLeft();
      ASTNode* rightChild = binaryNode->getRight();

      if (leftChild != nullptr && dynamic_cast<BinaryOperationNode*>(leftChild) != nullptr &&
          dynamic_cast<BinaryOperationNode*>(leftChild)->getOperator() == binaryNode->getOperator()) {

        BinaryOperationNode* leftChildBinaryNode = dynamic_cast<BinaryOperationNode*>(leftChild);
        ASTNode* grandchild = leftChildBinaryNode->getRight();

        // Perform right shift
        binaryNode->setLeft(leftChildBinaryNode->getLeft());
        binaryNode->setRight(new BinaryOperationNode(binaryNode->getOperator(), grandchild, rightChild));
        delete leftChildBinaryNode;

        // Call the function recursively on the updated binaryNode
        transformOperations(binaryNode);
      }
    }
  }
}
