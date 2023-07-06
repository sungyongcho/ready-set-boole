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
void printTableRow(const std::vector<int> &row);
// for ex04

// ex06
std::string conjunctive_normal_form(std::string format);
// ex07
bool sat(const std::string &formula);
// ex08
std::vector<std::vector<int> > powerset(const std::vector<int>& set);
std::vector<std::vector<int> > powerset_iter(const std::vector<int>& set);
#endif  // UTILS_HPP
