#ifndef HAFF_H
#define HAFF_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>


struct Node_huff {   
    char symbol; // Символ, который представляет узел   
    double frequency; // Частота символа   
    Node_huff* left; // Левый дочерний узел   
    Node_huff* right; // Правый дочерний узел   
  
    // Конструктор для создания узла   
    Node_huff(char symbol, int frequency) : symbol(symbol), frequency(frequency), left(nullptr), right(nullptr) {}   
}; 
int main_haff(std::map<std::string, int> frequencies, std::string text);
#endif 