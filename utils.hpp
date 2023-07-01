#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

std::vector<char> tokenize(std::string str, std::string symbols);

// for ex04
typedef std::unordered_map<char, int> VariableAssignments;

std::vector<char> extractVariables(std::vector<char> tokens);
std::vector<std::vector<int> > generateCombinations(const std::vector<char> &variables);
VariableAssignments createVariableAssignment(const std::vector<char> &variables, const std::vector<int> &combination);
void printTableHeader(const std::vector<char> &variables);
void printTableRows(const std::vector<std::vector<int> > &truthTable);

// for ex04
#endif  // UTILS_HPP
