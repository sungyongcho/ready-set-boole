#include <iostream>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

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

  std::vector<int> result = rootNode->evaluate(variableMapping);

  return result;
}

int main() {
  std::vector<std::vector<int> > sets;
  std::vector<int> set1;
  set1.push_back(0);
  set1.push_back(1);
  set1.push_back(2);

  std::vector<int> set2;
  set2.push_back(3);
  set2.push_back(4);
  set2.push_back(5);

  std::vector<int> set3;
  set3.push_back(3);
  set3.push_back(4);
  set3.push_back(5);

  sets.push_back(set1);
  sets.push_back(set2);
  sets.push_back(set3);

  std::vector<int> result = eval_set("ABC|&", sets);
  if (result.empty()) {
    std::cout << "Empty set" << std::endl;
  } else {
    for (std::vector<int>::iterator it = result.begin(); it != result.end(); it++) {
      std::cout << *it << std::endl;
    }
  }
}
