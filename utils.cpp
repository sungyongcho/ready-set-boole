#include "utils.hpp"

#include <iostream>
#include <stack>
#include <vector>

#include "ASTNode.hpp"

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
//ex04
std::vector<char> extractVariables(std::vector<char> tokens) {
  std::vector<char> variables;
  for (std::vector<char>::iterator it = tokens.begin(); it != tokens.end(); it++) {
    if (isalpha(*it) && std::find(variables.begin(), variables.end(), *it) == variables.end()) {
      variables.push_back(*it);
    }
  }
  return variables;
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

// ex04
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

// ex04
void printTableRow(const std::vector<int> &row) {
  std::cout << "| ";
  for (int i = 0; i < (int)row.size(); i++) {
    if (i + 1 == (int)row.size())
      std::cout << row[i] << " |";
    else
      std::cout << row[i] << " | ";
  }
  std::cout << std::endl;
}

// ex04
void print_truth_table(std::string formula) {
  std::string symbols = "ABCDEFGHIJKLMNOPQURSTUVWXYZ!&|^>=";

  ASTNode *rootNode = nullptr;

  std::vector<char> tokens = tokenize(formula, symbols);
  std::vector<char> variables = extractVariables(tokens);

  rootNode = ASTNode::parseExpression(tokens);
  // if you want to print
  // rootNode->printAST();

  printTableHeader(variables);
  std::vector<int> combination(variables.size());
  while (true) {
    VariableAssignments assignment = createVariableAssignment(variables, combination);
    bool result = rootNode->evaluate(assignment);
    std::vector<int> row = combination;
    row.push_back(result);
    printTableRow(row);

    // Generate the next combination
    bool carry = true;
    for (int i = combination.size() - 1; i >= 0; i--) {
      if (carry) {
        combination[i] = (combination[i] + 1) % 2;
        carry = (combination[i] == 0);
      }
    }

    if (carry) {
      break;  // All combinations generated
    }
  }
}

// ex05
// ref:
// https://logic4free.informatik.uni-kiel.de/llocs/Negation_normal_form
// (this is gold)

std::string negation_normal_form(std::string format) {
  std::string symbols = "ABCDEFGHIJKLMNOPQURSTUVWXYZ!&|^>=";

  ASTNode *rootNode = nullptr;

  std::vector<char> tokens = tokenize(format, symbols);

  rootNode = ASTNode::parseExpression(tokens);
  // if you want to see
  // rootNode->printAST();

  ASTNode *nnfRoot = ASTNode::BNF2NNF(rootNode);

  // also if you want to see
  // nnfRoot->printAST();

  return nnfRoot->getPostfix();
}

// ex06

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
  ASTNode::transformDisjunctionToConjunctionTwo(nnfRoot);
  ASTNode::transformDisjunctionToConjunctionThree(nnfRoot);

  // also if you want to see
  // nnfRoot->printAST();

  // nnfRoot->inorderTraversal();

  return nnfRoot->getPostfix();
}

bool sat(const std::string &formula) {
  std::string symbols = "ABCDEFGHIJKLMNOPQURSTUVWXYZ!&|^>=";

  ASTNode *rootNode = nullptr;

  std::vector<char> tokens = tokenize(formula, symbols);
  std::vector<char> variables = extractVariables(tokens);

  rootNode = ASTNode::parseExpression(tokens);
  // if you want to print
  // rootNode->printAST();

  std::vector<int> combination(variables.size());
  while (true) {
    VariableAssignments assignment = createVariableAssignment(variables, combination);
    bool result = rootNode->evaluate(assignment);
    if (result) {
      return true;  // The formula is satisfiable
    }

    // Generate the next combination
    bool carry = true;
    for (int i = combination.size() - 1; i >= 0; i--) {
      if (carry) {
        combination[i] = (combination[i] + 1) % 2;
        carry = (combination[i] == 0);
      }
    }

    if (carry) {
      break;  // All combinations generated
    }
  }

  return false;  // The formula is unsatisfiable
}

std::vector<std::vector<int> > powerset(const std::vector<int> &set) {
  if (set.empty()) {
    // Base case: empty set, return the empty set as the only subset
    std::vector<std::vector<int> > subsets;
    subsets.push_back(std::vector<int>());
    return subsets;
  }

  int element = set.back();                              // Get the last element of the set
  std::vector<int> setCopy(set.begin(), set.end() - 1);  // Copy the set without the last element

  std::vector<std::vector<int> > subsets = powerset(setCopy);

  // Create new subsets by including the current element
  std::vector<std::vector<int> > newSubsets;
  for (std::size_t i = 0; i < subsets.size(); ++i) {
    std::vector<int> newSubset = subsets[i];
    newSubset.push_back(element);
    newSubsets.push_back(newSubset);
  }

  // Merge the new subsets with the existing subsets
  for (std::size_t i = 0; i < newSubsets.size(); ++i) {
    subsets.push_back(newSubsets[i]);
  }

  return subsets;
}

std::vector<std::vector<int> > powerset_iter(const std::vector<int> &set) {
  std::vector<std::vector<int> > result;
  int n = set.size();
  int numSubsets = 1 << n;  // Calculate the total number of subsets (2^n)

  for (int i = 0; i < numSubsets; ++i) {
    std::vector<int> subset;
    for (int j = 0; j < n; ++j) {
      if ((i & (1 << j)) != 0) {
        subset.push_back(set[j]);
      }
    }
    result.push_back(subset);
  }

  return result;
}

VariableAssignmentSet createVariableMapping(const std::vector<std::vector<int> > &sets, const std::vector<char> &variables) {
  VariableAssignmentSet variableMapping;
  if (sets.size() != variables.size()) {
    throw std::runtime_error("Number of sets and number of variables mismatch");
  }
  for (std::size_t i = 0; i < variables.size(); ++i) {
    char variable = variables[i];
    const std::vector<int> &set = sets[i];
    variableMapping[variable] = set;
  }

  return variableMapping;
}

void printVariableMapping(VariableAssignmentSet variableMapping) {
  std::cout << "Variable Mapping:\n";
  for (VariableAssignmentSet::const_iterator it = variableMapping.begin(); it != variableMapping.end(); it++) {
    char variable = it->first;
    const std::vector<int> &set = it->second;

    std::cout << variable << ": ";
    for (std::size_t i = 0; i < set.size(); ++i) {
      std::cout << set[i] << " ";
    }
    std::cout << "\n";
  }
}

std::vector<int> eval_set(std::string formula, std::vector<std::vector<int> > sets) {
  ASTNode *rootNode = nullptr;
  std::vector<char> tokens = tokenize(formula, "ABCDEFGHIJKLMNOPQRSTUVWXYZ!&|^>=");
  std::vector<char> variables = extractVariables(tokens);
  VariableAssignmentSet variableMapping = createVariableMapping(sets, variables);

  // if you want to print
  printVariableMapping(variableMapping);

  rootNode = ASTNode::parseExpression(tokens);

  // // also if you want to print
  // rootNode->printAST();

  std::vector<int> result = rootNode->evaluate(variableMapping);

  return result;
}

double map(uint16_t x, uint16_t y) {
  const uint64_t maxValue = (1ULL << 16) - 1;
  try {
    if (x > maxValue || y > maxValue) {
      throw std::out_of_range("Input out of range.");
    }

    uint64_t interleaved = (static_cast<unsigned long long>(x) << 16) | y;
    return static_cast<double>(interleaved) / (maxValue * maxValue);
  } catch (const std::out_of_range &e) {
    std::cout << "Error: " << e.what() << std::endl;
    return 0.0;
  }
}

void reverse_map(double n) {
  const uint64_t maxValue = (1ULL << 16) - 1;
  uint64_t encodedValue = static_cast<uint64_t>(n * (maxValue * maxValue));

  uint16_t y = encodedValue & maxValue;
  uint16_t x = (encodedValue >> 16) & maxValue;

  std::cout << "Decoded value: (" << x << ", " << y << ")" << std::endl;
}
