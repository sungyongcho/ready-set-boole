#include <iostream>
#include <set>
#include <stack>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

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

int main() {



  std::cout << conjunctive_normal_form("AB&!") <<  std::endl;
  // A!B!|
  std::cout << conjunctive_normal_form("AB|!") << std::endl;
  // A!B!&
  std::cout << conjunctive_normal_form("AB|C&") << std::endl;
  // AB|C&
  std::cout << conjunctive_normal_form("AB|C|D|") << std::endl;
  // ABCD|||
  std::cout << conjunctive_normal_form("AB&C&D&") << std::endl;
  // ABCD&&&
  std::cout << conjunctive_normal_form("AB&!C!|") << std::endl;
  // A!B!C!||
  std::cout << conjunctive_normal_form("AB|!C!&") << std::endl;
  // A!B!C!&&

  // my own
  std::cout << conjunctive_normal_form("AB^") << std::endl;
  std::cout << conjunctive_normal_form("AB=") << std::endl;

  return 0;
}
