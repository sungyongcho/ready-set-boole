#include <iostream>
#include <set>
#include <stack>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

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

int main() {
  std::cout << negation_normal_form("AB&!") << std::endl;
  // A!B!|
  std::cout << negation_normal_form("AB|!") << std::endl;
  // A!B!&
  std::cout << negation_normal_form("AB>") << std::endl;
  // A!B|
  std::cout << negation_normal_form("AB=") << std::endl;
  // AB&A!B!&|
  // this is given from the example but it's same as A!B|B!A|&
  std::cout << negation_normal_form("AB|C&!") << std::endl;
  // A!B!&C!|
}
