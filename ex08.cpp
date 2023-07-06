#include <iostream>
#include <vector>

#include "utils.hpp"

void showSubsets(const std::vector<std::vector<int> >& subsets) {
    std::cout << "Subsets:\n";
    for (std::size_t i = 0; i < subsets.size(); ++i) {
        const std::vector<int>& subset = subsets[i];
        std::cout << "{ ";
        for (std::size_t j = 0; j < subset.size(); ++j) {
            const int& element = subset[j];
            std::cout << element << " ";
        }
        std::cout << "}\n";
    }
}


int main() {
  std::vector<int> set;
  set.push_back(0);
  set.push_back(1);
  set.push_back(2);
  set.push_back(3);
  showSubsets(powerset(set));
}
