#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <cmath> // для log2
#include "Hennon-Phano.h"
#include <map>

bool compareSymbols(const Symbol& a, const Symbol& b) {
  return a.frequency > b.frequency;
}

// Функция для поиска оптимального индекса `mid`
int findOptimalMid(const std::vector<Symbol>& symbols) {
  int mid = symbols.size() / 2; 
  int minDiff = INT_MAX; 
  for (int i = 1; i < symbols.size() - 1; ++i) { 
    int leftSum = 0, rightSum = 0;
    for (int j = 0; j < i; ++j) {
      leftSum += symbols[j].frequency;
    }
    for (int j = i; j < symbols.size(); ++j) {
      rightSum += symbols[j].frequency;
    }
    int diff = abs(leftSum - rightSum);
    if (diff < minDiff) {
      minDiff = diff;
      mid = i; 
    }
  }
  return mid; 
}
std::map<char, std::string> shannonFano(std::vector<Symbol> symbols) {
  std::map<char, std::string> codes; 

  // Сортировка символов по убыванию частоты  
  sort(symbols.begin(), symbols.end(), compareSymbols); 

  // Рекурсивная функция для построения кодов  
  std::function<void(std::vector<Symbol>, std::string)> encode = [&](std::vector<Symbol> symbols, std::string prefix) {
    if (symbols.size() == 1) {
      codes[symbols[0].character] = prefix;
      return;
    }

    // Нахождение оптимального индекса `mid` 
    int mid = findOptimalMid(symbols);  

    // Деление символов на две группы с максимально близкими суммами частот  
    int leftSum = 0, rightSum = 0;  
    for (int i = 0; i < mid; i++) {
      leftSum += symbols[i].frequency;
    }  
    for (int i = mid; i < symbols.size(); i++) {
      rightSum += symbols[i].frequency;
    }  

    // Рекурсивный вызов для левой и правой групп  
    encode(std::vector<Symbol>(symbols.begin(), symbols.begin() + mid), prefix + "1");  
    encode(std::vector<Symbol>(symbols.begin() + mid, symbols.end()), prefix + "0");  
  };

  // Запуск рекурсивной функции  
  encode(symbols, ""); 

  return codes;
}

// Функция для вычисления энтропии 
double calculateEntropy(const std::vector<Symbol>& frequencies) { 
    double entropy = 0; 
    for (auto& frequency : frequencies) { 
        if (frequency.frequency > 0) { // Проверка на ненулевую частоту 
            entropy -= frequency.frequency * log2(frequency.frequency);  
        } 
    } 
    return entropy; 
} 
double averageLength = 0;
double calculateLong(const std::vector<Symbol>& symbols, std::map<char, std::string> codes){
  for (const auto& [character, code] : codes) { 
    for (const auto& symbol : symbols) {
      if (symbol.character == character) { 
        averageLength += symbol.frequency * code.length();
      }
    }
  } 
  return averageLength;
}
double Longprint()
{
  return averageLength;
}
std::map<char, std::string> main_Phanno() {
    std::vector<Symbol> symbols = { //Задание 2
        {'a', 0.21}, 
        {'b', 0.19}, 
        {'c', 0.15}, 
        {'d', 0.13}, 
        {'e', 0.12}, 
        {'f', 0.09}, 
        {'g', 0.06}, 
        {'h', 0.05} 
    }; 
    // std::vector<Symbol> symbols = { //Пример
    //     {'A', 0.4}, 
    //     {'B', 0.3}, 
    //     {'C', 0.15}, 
    //     {'D', 0.1}, 
    //     {'E', 0.05}, 
    // }; 

    // std::vector<Symbol> symbols = { //Задание 1
    //     {'a', 0.7},
    //     {'b', 0.3}
    // };
  std::map<char, std::string> codes = shannonFano(symbols);

  // Вывод результатов
  for (const auto& [character, code] : codes) {
    std::cout << character << ": " << code << std::endl;
  }

  // std::cout<<std::endl; 
  std::wcout << L"Средняя Энтропия:" << std::endl; //P*log2(P)
  std::wcerr<<calculateEntropy(symbols)<<L" бит"<<std::endl; 

  int Hmax = log2(8); //максимальная энтропия Hmax = log2 (кол-во символов)
  std::wcout << L"Избыточность:" << std::endl; 
  std::cout<<1-(calculateEntropy(symbols)/Hmax)<<std::endl;//1-(H/Hmax) 
  // std::cout << std::endl;

  // Вывод средней длины кодовой комбинации для Шеннона-Фано 
  std::wcout << L"Средняя длина кодовой комбинации: " << std::endl; //длина кода*вероятнсть
  averageLength = calculateLong(symbols, codes); 
  std::wcout << averageLength <<L" бит"<< std::endl; 

  // // Генерация блоков по три знака
  // std::wcout << L"Блоки по три знака:" << std::endl;
  // for (int i = 0; i < 3; i++) {
  //     for (int j = 0; j < 3; j++) {
  //         for (int k = 0; k < 3; k++) {
  //             std::string block = "";
  //             block += (i == 0 ? 'a' : 'b');
  //             block += (j == 0 ? 'a' : 'b');
  //             block += (k == 0 ? 'a' : 'b');
  //             std::cout << block << ": ";

  //             //  Кодирование блока
  //             std::string encodedBlock = "";
  //             for (char symbol : block) {
  //                 encodedBlock += codes[symbol];
  //             }
  //             std::cout << encodedBlock << std::endl;
  //         }
  //     }
  // }
  return codes;
}