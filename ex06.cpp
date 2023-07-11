#include <iostream>
#include <set>
#include <stack>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

int main() {
  std::cout << conjunctive_normal_form("AB&!") << std::endl;
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
  std::cout << "======" <<std::endl;
  std::cout << conjunctive_normal_form("A") << std::endl;
  std::cout << conjunctive_normal_form("A!") << std::endl;
  std::cout << conjunctive_normal_form("AB&!") << std::endl;
  std::cout << conjunctive_normal_form("AB>!") << std::endl;
  std::cout << conjunctive_normal_form("AB=!") << std::endl;
  std::cout << "======" <<std::endl;
  std::cout << conjunctive_normal_form("ABC||") << std::endl;
  std::cout << conjunctive_normal_form("ABC||!") << std::endl;
  std::cout << conjunctive_normal_form("ABC|&") << std::endl;
  std::cout << conjunctive_normal_form("ABC&|") << std::endl;
  std::cout << conjunctive_normal_form("ABC&|!") << std::endl;
  std::cout << conjunctive_normal_form("ABC^^") << std::endl;
  std::cout << conjunctive_normal_form("ABC>>") << std::endl;

}
