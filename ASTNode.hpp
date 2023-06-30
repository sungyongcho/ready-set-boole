
// Forward declaration of AST node classes
class ASTNode;
class OperandNode;
class UnaryOperationNode;
class BinaryOperationNode;
#include <iostream>
#include <vector>

// Base class for all AST nodes
class ASTNode {
 public:
  virtual ~ASTNode() {}

  static ASTNode *parseExpression(const std::vector<char> &tokens);
  virtual bool evaluate() = 0;
  void printAST(const std::string &prefix = "", bool isLeft = false) const;
  // void tokenize(std::string str, std::string symbols);

 private:
  std::vector<char> tokens;
};

// AST node for operands (numeric literals or variables)
class OperandNode : public ASTNode {
 public:
  explicit OperandNode(int value) : value(value) {}
  int getValue() const {
    return value;
  }

  bool evaluate() {
    return static_cast<bool>(value);
  }

 private:
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

 private:
  char op;
  ASTNode *left;
  ASTNode *right;
};
