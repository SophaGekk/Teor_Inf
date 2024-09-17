#ifndef HENNON_H
#define HENNON_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

struct Symbol {
  char character;
  double frequency;
};

std::map<char, std::string> shannonFano(std::vector<Symbol> symbols);
std::map<char, std::string> main_Phanno();
double calculateLong(const std::vector<Symbol>& symbols, std::map<char, std::string> codes);
double Longprint();
#endif 