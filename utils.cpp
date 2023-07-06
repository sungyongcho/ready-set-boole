#include "utils.hpp"
#include "ASTNode.hpp"

#include <iostream>
#include <vector>
#include <stack>

std::vector<char> tokenize(std::string str, std::string symbols) {
  std::vector<char> tokens;
  try {
    for (int i = 0; i < (int)str.size(); i++) {
      if (symbols.find(str[i]) != std::string::npos) {
        tokens.push_back(str[i]);
      } else {
        tokens.clear();  // Clear the vector
        throw std::invalid_argument("Invalid character input - [" + std::string(1, str[i]) + "]");
      }
    }
  } catch (const std::invalid_argument &e) {
    std::cout << "Error: " << e.what() << std::endl;
    tokens.clear();  // Clear the vector
  }
  return tokens;
}

std::vector<char> extractVariables(std::vector<char> tokens) {
  std::vector<char> variables;
  for (std::vector<char>::iterator it = tokens.begin(); it != tokens.end(); it++) {
    if (isalpha(*it)) {
      variables.push_back(*it);
    }
  }
  return variables;
}

std::vector<std::vector<int> > generateCombinations(const std::vector<char> &variables) {
  std::vector<std::vector<int> > combinations;
  int numVariables = variables.size();

  int numCombinations = 1 << numVariables;  // 2^numVariables

  for (int i = 0; i < numCombinations; i++) {
    std::vector<int> combination(numVariables);

    // Assign truth values based on binary representation of i
    for (int j = 0; j < numVariables; j++) {
      combination[j] = (i >> (numVariables - 1 - j)) & 1;
    }

    combinations.push_back(combination);
  }

  return combinations;
}

VariableAssignments createVariableAssignment(const std::vector<char> &variables, const std::vector<int> &combination) {
  VariableAssignments assignment;
  int numVariables = variables.size();

  for (int i = 0; i < numVariables; i++) {
    char variable = variables[i];
    int value = combination[i];
    assignment[variable] = value;
  }

  return assignment;
}

void printTableHeader(const std::vector<char> &variables) {
  std::cout << "| ";
  for (int i = 0; i < (int)variables.size(); i++) {
    std::cout << variables[i] << " | ";
  }
  std::cout << "= |" << std::endl;

  std::cout << "|";
  for (int i = 0; i <= (int)variables.size(); i++) {
    std::cout << "---|";
  }
  std::cout << std::endl;
}

void printTableRow(const std::vector<int> &row) {
  std::cout << "| ";
  for (int i = 0; i < (int)row.size(); i++) {
    std::cout << row[i] << " | ";
  }
  std::cout << std::endl;
}

std::string conjunctive_normal_form(std::string format) {
  std::string symbols = "ABCDEFGHIJKLMNOPQURSTUVWXYZ!&|^>=";

  ASTNode *rootNode = nullptr;

  std::vector<char> tokens = tokenize(format, symbols);

  rootNode = ASTNode::parseExpression(tokens);
  // if you want to see
  // rootNode->printAST();

  ASTNode *nnfRoot = ASTNode::BNF2NNF(rootNode);
  // nnfRoot->printAST();
  ASTNode::transformOperations(nnfRoot);
  ASTNode::transformDisjunctionToConjunction(nnfRoot);


  // also if you want to see
  // nnfRoot->printAST();

  // nnfRoot->inorderTraversal();

  return nnfRoot->getPostfix();
}

bool sat(const std::string &formula) {
  std::string cnfTrasnformed = conjunctive_normal_form(formula);
  std::stack<bool> stack;

  for (std::size_t i = 0; i < cnfTrasnformed.size(); ++i) {
    char token = cnfTrasnformed[i];
    if (isalpha(token)) {
      stack.push(true);
    } else if (token == '!') {
      // Unary operator (negation)
      bool operand = stack.top();
      stack.pop();
      stack.push(!operand);
    } else if (token == '&') {
      // Conjunction (AND)
      bool operand2 = stack.top();
      stack.pop();
      bool operand1 = stack.top();
      stack.pop();
      stack.push(operand1 && operand2);
    } else if (token == '|') {
      // Disjunction (OR)
      bool operand2 = stack.top();
      stack.pop();
      bool operand1 = stack.top();
      stack.pop();
      stack.push(operand1 || operand2);
    }
  }

  return stack.top();  // The final result of the formula
}
