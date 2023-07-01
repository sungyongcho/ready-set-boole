#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

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

int main() {
  print_truth_table("AB&C|");
}
