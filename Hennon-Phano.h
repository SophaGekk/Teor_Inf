#ifndef HENNON_H
#define HENNON_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

class NodeShannonFano {
public:
  std::string symbol;
  int frequency;
  std::string code;

  NodeShannonFano(std::string symbol, int frequency)
    : symbol(symbol), frequency(frequency) {}
};

std::map<std::string, std::string> shannonFano(std::vector<NodeShannonFano> NodeShannonFanos);
void printCodesShannonFano(std::vector<NodeShannonFano>& nodes);
int main_hennon_phano(std::string text);
#endif 