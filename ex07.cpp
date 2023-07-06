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
  std::cout << sat("ABC||") << std::endl;
  std::cout << sat("ABC&&") << std::endl;
  std::cout << sat("AB^") << std::endl;
}
