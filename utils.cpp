#include "utils.hpp"

#include <iostream>
#include <vector>

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

void printTableRows(const std::vector<std::vector<int> > &truthTable) {
  for (int i = 0; i < (int)truthTable.size(); i++) {
    const std::vector<int> &row = truthTable[i];
    std::cout << "| ";
    for (int j = 0; j < (int)row.size(); j++) {
      std::cout << row[j] << " | ";
    }
    std::cout << std::endl;
  }
}
