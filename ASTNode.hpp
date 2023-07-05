
// Forward declaration of AST node classes
class ASTNode;
class OperandNode;
class UnaryOperationNode;
class BinaryOperationNode;
#include <iostream>
#include <vector>

#include "utils.hpp"

// Base class for all AST nodes
class ASTNode {
 public:
  virtual ~ASTNode() {}

  static ASTNode *parseExpression(const std::vector<char> &tokens);
  virtual bool evaluate() = 0;
  virtual bool evaluate(const VariableAssignments &variableAssignments) = 0;
  void printAST(const std::string &prefix = "", bool isLeft = false) const;
  // void tokenize(std::string str, std::string symbols);
  static ASTNode *NegNNF(ASTNode *phi);
  static ASTNode *BNF2NNF(ASTNode *formula);
  static void transformOperations(ASTNode* node);
  // New function to get the postfix form as a string
  virtual std::string getPostfix() const = 0;



 private:
  std::vector<char> tokens;
};

// AST node for operands (numeric literals or variables)
class OperandNode : public ASTNode {
 public:
  explicit OperandNode(int value) : value(value) {}
  explicit OperandNode(char variable, int value) : variable(variable), value(value) {}
  int getValue() const {
    return value;
  }
  char getVariable() const {
    return variable;
  }
  void setValue(int newValue) {
    value = newValue;
  }
  bool evaluate() {
    return static_cast<bool>(value);
  }
  bool evaluate(const VariableAssignments &variableAssignments) {
    char variable = getVariable();
    int value = variableAssignments.at(variable);
    return static_cast<bool>(value);
  }
  // Get the operand in postfix form as a string
  std::string getPostfix() const {
    return std::string(1, getVariable());
  }

  // Get the inorder postfix form as a string (same as the postfix form)


 private:
  char variable;
  int value;
};

// AST node for unary operations
class UnaryOperationNode : public ASTNode {
 public:
  UnaryOperationNode(char op, ASTNode *operand) : op(op), operand(operand) {}

  char getOperator() const { return op; }
  ASTNode *getOperand() const { return operand; }

  bool evaluate() {
    if (op == '!') {
      return !operand->evaluate();
    } else {
      throw std::runtime_error("Invalid unary operator");
    }
  }
  bool evaluate(const VariableAssignments &variableAssignments) {
    if (op == '!') {
      return !operand->evaluate(variableAssignments);
    } else {
      throw std::runtime_error("Invalid unary operator");
    }
  }
  // Get the unary operation in postfix form as a string
  std::string getPostfix() const {
    return operand->getPostfix() + getOperator();
  }

    // Get the inorder postfix form as a string

 private:
  char op;
  ASTNode *operand;
};

// AST node for binary operations
class BinaryOperationNode : public ASTNode {
 public:
  BinaryOperationNode(char op, ASTNode *left, ASTNode *right)
      : op(op), left(left), right(right) {}

  char getOperator() const { return op; }
  ASTNode *getLeft() const { return left; }
  ASTNode *getRight() const { return right; }

  bool evaluate() {
    bool leftValue = left->evaluate();
    bool rightValue = right->evaluate();

    if (op == '&') {
      return leftValue && rightValue;
    } else if (op == '|') {
      return leftValue || rightValue;
    } else if (op == '^') {
      return leftValue != rightValue;
    } else if (op == '>') {
      return !leftValue || rightValue;
    } else if (op == '=') {
      return leftValue == rightValue;
    } else {
      throw std::runtime_error("Invalid binary operator");
    }
  }
  bool evaluate(const VariableAssignments &variableAssignments) {
    bool leftValue = left->evaluate(variableAssignments);
    bool rightValue = right->evaluate(variableAssignments);

    if (op == '&') {
      return leftValue && rightValue;
    } else if (op == '|') {
      return leftValue || rightValue;
    } else if (op == '^') {
      return leftValue != rightValue;
    } else if (op == '>') {
      return !leftValue || rightValue;
    } else if (op == '=') {
      return leftValue == rightValue;
    } else {
      throw std::runtime_error("Invalid binary operator");
    }
  }
  std::string getPostfix() const {
    return left->getPostfix() + right->getPostfix() + getOperator();
  }
  void setLeft(ASTNode* node) { left = node; }
  void setRight(ASTNode* node) { right = node; }
 private:
  char op;
  ASTNode *left;
  ASTNode *right;
};
