#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

bool eval_formula(std::string str) {
  std::string symbols = "01!&|^>=";

  ASTNode *rootNode = NULL;

  std::vector<char> tokens = tokenize(str, symbols);

  rootNode = ASTNode::parseExpression(tokens);
  // if you want to print
  // rootNode->printAST();

  return rootNode->evaluate();
}

int main() {

  std::cout << eval_formula("10&") << std::endl;
  std::cout << eval_formula("10|") << std::endl;
  std::cout << eval_formula("11>") << std::endl;
  std::cout << eval_formula("10=") << std::endl;
  std::cout << eval_formula("1011||=") << std::endl;

}
