#include <iostream>
#include <set>
#include <stack>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"



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
  std::cout << negation_normal_form("A") << std::endl;
  std::cout << negation_normal_form("A!") << std::endl;
  std::cout << negation_normal_form("AB&!") << std::endl;
  std::cout << negation_normal_form("AB>!") << std::endl;
  std::cout << negation_normal_form("AB=!") << std::endl;
}
