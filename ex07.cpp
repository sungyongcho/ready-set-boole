#include <iostream>
#include <set>
#include <stack>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

int main() {
  std::cout << sat("AB|") << std::endl;
  // true
  std::cout << sat("AB&") << std::endl;
  // true
  std::cout << sat("AA!&") << std::endl;
  // false
  std::cout << sat("AA^") << std::endl;
  // false
  // std::cout << "==========" << std::endl;
  // std::cout << sat("ABC||") << std::endl;
  // std::cout << sat("ABC&&") << std::endl;
  // std::cout << sat("AB^") << std::endl;

  // std::cout << "==========" << std::endl;
  // std::cout << sat("A") << std::endl;
  // std::cout << sat("A!") << std::endl;
  // std::cout << sat("AA|") << std::endl;
  // std::cout << sat("AA&") << std::endl;
  // std::cout << sat("AA!&") << std::endl;
  // std::cout << sat("AA^") << std::endl;
  // std::cout << sat("AB^") << std::endl;
  // std::cout << sat("AB=") << std::endl;
  // std::cout << sat("AA>") << std::endl;
  // std::cout << sat("AA!>") << std::endl;
  // std::cout << "==========" << std::endl;
  // std::cout << sat("ABC||") << std::endl;
  // std::cout << sat("AB&A!B!&&") << std::endl;
  // std::cout << sat("ABCDE&&&&") << std::endl;
  // std::cout << sat("AAA^^") << std::endl;
  // std::cout << sat("ABCDE^^^^") << std::endl;

}
