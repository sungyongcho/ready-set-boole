
// Forward declaration of AST node classes
class ASTNode;
class OperandNode;
class UnaryOperationNode;
class BinaryOperationNode;
#include <iostream>
#include <vector>
#include <algorithm>

#include "utils.hpp"

// Base class for all AST nodes
class ASTNode {
 public:
  virtual ~ASTNode() {}

  static ASTNode *parseExpression(const std::vector<char> &tokens);
  virtual bool evaluate() = 0;
  virtual bool evaluate(const VariableAssignments &variableAssignments) = 0;
  virtual std::vector<int> evaluate(const VariableAssignmentSet &variableAssignments) const = 0;

  void printAST(const std::string &prefix = "", bool isLeft = false) const;
  // void tokenize(std::string str, std::string symbols);
  static ASTNode *NegNNF(ASTNode *phi);
  static ASTNode *BNF2NNF(ASTNode *formula);
  static void transformDisjunctionToConjunction(ASTNode *node);
  static void transformDisjunctionToConjunctionTwo(ASTNode* node);
  static void transformDisjunctionToConjunctionThree(ASTNode* node);
  static std::vector<ASTNode *> tseitinTransformation(ASTNode *node, int &counter);
  static void transformOperations(ASTNode *node);
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
  std::vector<int> evaluate(const VariableAssignmentSet &variableAssignments) const {
    char variable = getVariable();
    const std::vector<int> &values = variableAssignments.at(variable);
    return std::vector<int>(values.begin(), values.end());
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
  std::vector<int> evaluate(const VariableAssignmentSet &variableAssignments) const {
    if (op == '!') {
      return complement(operand->evaluate(variableAssignments));
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
  std::vector<int> complement(const std::vector<int> &set) const {
    std::vector<int> result;

    // Handle the case where the input set is empty
    if (set.empty()) {
      for (int i = 0; i < (int)set.size(); ++i) {
        result.push_back(i);
      }
    } else {
      for (int i = 0; i < (int) set.size() + 1; ++i) {
        if (std::find(set.begin(), set.end(), i) == set.end()) {
          result.push_back(i);
        }
      }
    }

    return result;
  }
};

// AST node for binary operations
class BinaryOperationNode : public ASTNode {
 public:
  BinaryOperationNode(char op, ASTNode *left, ASTNode *right)
      : op(op), left(left), right(right) {}

  char getOperator() const { return op; }
  ASTNode *getLeft() const { return left; }
  ASTNode *getRight() const { return right; }
  void setOperator(char newOp) {
    op = newOp;
  }

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
  std::vector<int> evaluate(const VariableAssignmentSet &variableAssignments) const {
    std::vector<int> leftValue = left->evaluate(variableAssignments);
    std::vector<int> rightValue = right->evaluate(variableAssignments);

    if (op == '&') {
      return setIntersection(leftValue, rightValue);
    } else if (op == '|') {
      return setUnion(leftValue, rightValue);
    } else if (op == '^') {
      return setDifference(setUnion(leftValue, rightValue), setIntersection(leftValue, rightValue));
    } else if (op == '>') {
      return setUnion(rightValue, complement(leftValue, rightValue));
    } else if (op == '=') {
      return setIntersection(setUnion(leftValue, complement(leftValue, rightValue)), setUnion(rightValue, complement(rightValue, leftValue)));
    } else {
      throw std::runtime_error("Invalid binary operator");
    }
  }
  void setLeft(ASTNode *node) { left = node; }
  void setRight(ASTNode *node) { right = node; }
  std::vector<int> setIntersection(const std::vector<int> &set1, const std::vector<int> &set2) const {
    std::vector<int> result;
    for (std::size_t i = 0; i < set1.size(); ++i) {
      int element = set1[i];
      bool found = false;
      for (std::size_t j = 0; j < set2.size(); ++j) {
        if (set2[j] == element) {
          found = true;
          break;
        }
      }
      if (found) {
        result.push_back(element);
      }
    }
    return result;
  }

  std::vector<int> setUnion(const std::vector<int> &set1, const std::vector<int> &set2) const {
    std::vector<int> result;

    for (std::size_t i = 0; i < set1.size(); ++i) {
      result.push_back(set1[i]);
    }

    // Copy elements from set2 to result if not already present
    for (std::size_t i = 0; i < set2.size(); ++i) {
      int element = set2[i];
      bool found = false;
      for (std::size_t j = 0; j < result.size(); ++j) {
        if (result[j] == element) {
          found = true;
          break;
        }
      }
      if (!found) {
        result.push_back(element);
      }
    }
    return result;
  }

  std::vector<int> setDifference(const std::vector<int> &set1, const std::vector<int> &set2) const {
    std::vector<int> result;
    for (std::size_t i = 0; i < set1.size(); ++i) {
      int element = set1[i];
      bool found = false;
      for (std::size_t j = 0; j < set2.size(); ++j) {
        if (set2[j] == element) {
          found = true;
          break;
        }
      }
      if (!found) {
        result.push_back(element);
      }
    }
    for (std::size_t i = 0; i < set2.size(); ++i) {
      int element = set2[i];
      bool found = false;
      for (std::size_t j = 0; j < set1.size(); ++j) {
        if (set1[j] == element) {
          found = true;
          break;
        }
      }
      if (!found) {
        result.push_back(element);
      }
    }
    return result;
  }
  std::vector<int> complement(const std::vector<int> &set1, const std::vector<int> &set2) const {
    std::vector<int> result;
    for (std::size_t i = 0; i < set2.size(); ++i) {
      int element = set2[i];
      bool found = false;
      for (std::size_t j = 0; j < set1.size(); ++j) {
        if (set1[j] == element) {
          found = true;
          break;
        }
      }
      if (!found) {
        result.push_back(element);
      }
    }

    return result;
  }

 private:
  char op;
  ASTNode *left;
  ASTNode *right;
};
