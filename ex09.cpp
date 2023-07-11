#include <iostream>
#include <vector>

#include "ASTNode.hpp"
#include "utils.hpp"

int main() {
  std::vector<std::vector<int> > sets;
  std::vector<int> set1;
  set1.push_back(0);
  // set1.push_back(1);
  // set1.push_back(2);

  std::vector<int> set2;
  set2.push_back(0);
  // set2.push_back(1);
  // set2.push_back(2);

  std::vector<int> set3;
  // set3.push_back(3);
  set3.push_back(0);
  // set3.push_back(5);

  sets.push_back(set1);
  sets.push_back(set2);
  sets.push_back(set3);

  std::vector<int> result = eval_set("ABC^^", sets);

  if (result.empty()) {
    std::cout << "Empty set" << std::endl;
  } else {
    for (std::vector<int>::iterator it = result.begin(); it != result.end(); it++) {
      std::cout << *it << std::endl;
    }
  }
}
