#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

void print_truth_table(std::string formula) {
  std::string symbols = "ABCDEFGHIJKLMNOPQURSTUVWXYZ!&|^>=";

  ASTNode *rootNode = NULL;

  std::vector<char> tokens = tokenize(formula, symbols);
  std::vector<char> variables = extractVariables(tokens);

  std::vector<std::vector<int> > combinations = generateCombinations(variables);
  std::vector<std::vector<int> > truthTable;
  rootNode = ASTNode::parseExpression(tokens);

  for (int i = 0; i < (int)combinations.size(); i++) {
    const std::vector<int> &combination = combinations[i];
    VariableAssignments assignment = createVariableAssignment(variables, combination);
    bool result = rootNode->evaluate(assignment);
    std::vector<int> row = combination;
    row.push_back(result);
    truthTable.push_back(row);
  }

  // if you want to print
  rootNode->printAST();
  printTableHeader(variables);
  printTableRows(truthTable);
}

int main() {
  print_truth_table("AB&C|");
}
