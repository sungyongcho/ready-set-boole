#include <vector>
#include <iostream>

std::vector<char> tokenize(std::string str, std::string symbols) {
  std::vector<char> tokens;
  try {
    for (int i = 0; i < (int)str.size(); i++) {
      if (symbols.find(str[i]) != std::string::npos) {
        tokens.push_back(str[i]);
      } else {
        tokens.clear();  // Clear the vector
        throw std::invalid_argument("Invalid character input - [" + std::string(1, str[i]) + "]");
      }
    }
  } catch (const std::invalid_argument &e) {
    std::cout << "Error: " << e.what() << std::endl;
    tokens.clear();  // Clear the vector
  }
  return tokens;
}
