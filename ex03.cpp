#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "ASTNode.hpp"

// std::vector<char> tokenize(std::string str) {
//   std::vector<char> tokens;

//   try {
//     for (int i = 0; i < (int)str.size(); i++) {
//       if (symbols.find(str[i]) != std::string::npos) {
//         tokens.push_back(str[i]);
//       } else {
//         tokens.clear();  // Clear the vector
//         throw std::invalid_argument("Invalid character input - [" + std::string(1, str[i]) + "]");
//       }
//     }
//   } catch (const std::invalid_argument &e) {
//     std::cout << "Error: " << e.what() << std::endl;
//     tokens.clear();  // Clear the vector
//   }

//   return tokens;
// }

bool eval_formula(std::string str) {
  std::string symbols = "01!&|^>=";

  // std::vector<char> tokens;
  ASTNode *rootNode = NULL;

  rootNode->tokenize(str, symbols);
  rootNode->parseExpression();
  // rootNode->printAST();

  return rootNode->evaluate();
}

int main() {
  // std::vector<char> tokens = tokenize("abcde");
  // std::vector<char>::iterator it;
  // for (it = tokens.begin(); it != tokens.end(); ++it) {
  //   std::cout << *it << " ";
  // }

  // tokens = tokenize("10&");
  // ASTNode *rootNode = ASTNode::parseExpression(tokens);
  // rootNode->printAST();
  // std::cout << rootNode->evaluate() << std::endl;

  std::cout << "========" << std::endl;
  std::cout << eval_formula("10&") << std::endl;
  std::cout << "========" << std::endl;

  // tokens = tokenize("11>");
  // ASTNode *rootNodeTwo = ASTNode::parseExpression(tokens);
  // rootNodeTwo->printAST();
  // std::cout << rootNodeTwo->evaluate() << std::endl;

  // tokens = tokenize("1011||=");
  // rootNodeTwo = ASTNode::parseExpression(tokens);
  // printAST(rootNodeTwo);
  // std::cout << rootNodeTwo->evaluate() << std::endl;
  // for (it = tokens.begin(); it != tokens.end(); ++it) {
  //   std::cout << *it << " ";
  // }
}
