#include <iostream>
#include <vector>

#include "utils.hpp"

void showSubsets(const std::vector<std::vector<int> >& subsets) {
  std::cout << "Subsets:\n";

  // Add an empty subset at the beginning
  std::cout << "{ }\n";

  for (std::size_t i = 0; i < subsets.size(); i++) {
    const std::vector<int>& subset = subsets[i];
    for (std::size_t j = 0; j < subset.size(); j++) {
      std::cout << "{ ";
      for (std::size_t k = 0; k <= j; k++) {
        std::cout << subset[k];
        if (k < j) {
          std::cout << " ";
        }
      }
      std::cout << " }\n";
    }
  }
}

int main() {
  std::vector<int> set;
  set.push_back(0);
  set.push_back(1);
  // set.push_back(2);
  // set.push_back(3);
  showSubsets(powerset(set));
}
