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

// Функция для подсчета частоты символов в строке
std::map<std::string, int> countNodeShannonFanoFrequencies(const std::string& text) {
    std::map<std::string, int> frequencis; 
    for (size_t i = 0; i < text.size(); ++i) {
        std::string symbol = std::string(1, text[i]); 
        if (frequencis.find(symbol) == frequencis.end()) {
            frequencis[symbol] = 1; 
        } else {
            frequencis[symbol]++; 
        }
    }
    return frequencis; 
}


bool compareNodeShannonFanos(const NodeShannonFano& a, const NodeShannonFano& b) {
  return a.frequency > b.frequency;
}

// Функция для поиска оптимального индекса `mid`
int findOptimalMid(const std::vector<NodeShannonFano>& NodeShannonFanos) {
  int mid = NodeShannonFanos.size() / 2; 
  int minDiff = INT_MAX; 
  for (int i = 1; i < NodeShannonFanos.size() - 1; ++i) { 
    int leftSum = 0, rightSum = 0;
    for (int j = 0; j < i; ++j) {
      leftSum += NodeShannonFanos[j].frequency;
    }
    for (int j = i; j < NodeShannonFanos.size(); ++j) {
      rightSum += NodeShannonFanos[j].frequency;
    }
    int diff = abs(leftSum - rightSum);
    if (diff < minDiff) {
      minDiff = diff;
      mid = i; 
    }
  }
  return mid; 
}

std::map<std::string, std::string> shannonFano(std::vector<NodeShannonFano> NodeShannonFanos) {
  std::map<std::string, std::string> codes; 

  // Сортировка символов по убыванию частоты  
  sort(NodeShannonFanos.begin(), NodeShannonFanos.end(), compareNodeShannonFanos); 

  // Рекурсивная функция для построения кодов  
  std::function<void(std::vector<NodeShannonFano>, std::string)> encode = [&](std::vector<NodeShannonFano> NodeShannonFanos, std::string prefix) {
    if (NodeShannonFanos.size() == 1) {
      codes[NodeShannonFanos[0].symbol] = prefix;
      return;
    }

    // Нахождение оптимального индекса `mid` 
    int mid = findOptimalMid(NodeShannonFanos);  

    // Деление символов на две группы с максимально близкими суммами частот  
    int leftSum = 0, rightSum = 0;  
    for (int i = 0; i < mid; i++) {
      leftSum += NodeShannonFanos[i].frequency;
    }  
    for (int i = mid; i < NodeShannonFanos.size(); i++) {
      rightSum += NodeShannonFanos[i].frequency;
    }  

    // Рекурсивный вызов для левой и правой групп  
    encode(std::vector<NodeShannonFano>(NodeShannonFanos.begin(), NodeShannonFanos.begin() + mid), prefix + "1");  
    encode(std::vector<NodeShannonFano>(NodeShannonFanos.begin() + mid, NodeShannonFanos.end()), prefix + "0");  
  };

  // Запуск рекурсивной функции  
  encode(NodeShannonFanos, ""); 

  return codes;
}
// Функция для чтения заголовка из файла
void readHeaderShannonFano(std::ifstream& inputFile, std::unordered_map<std::string, std::string>& codeTable) {
    std::string line;
    // getline(inputFile, line); // Сброс оставшейся части строки
    while (getline(inputFile, line)) {
        if (line.empty()) {
            break;
        }
        size_t pos = line.find(':');
        std::string block = line.substr(0, pos);
        std::string code = line.substr(pos + 2);
        codeTable[block] = code;
    }
}
// Функция для чтения закодированного текста из файла
std::string readEncodedTextShannonFano(std::ifstream& inputFile) {
    std::string encodedText;
    inputFile >> encodedText;
    return encodedText;
}
// Функция для декодирования текста блоками с использованием таблицы кодирования 
std::string decodeTextShannonFano(const std::string& encodedText, std::unordered_map<std::string, std::string>& decodeHuffmanCodes) { 
    std::string decodedText; 
    std::string currentCode; 
    for (char ch : encodedText) { 
        currentCode += ch; 
 
        // Проверяем, есть ли текущий код в значениях словаря
        for (auto const& [NodeShannonFano, code] : decodeHuffmanCodes) {
            if (code == currentCode) {
                decodedText += NodeShannonFano; 
                currentCode = ""; 
                break; // Выходим из цикла, так как символ найден
            }
        }
    }  
    return decodedText;  
}

int main_hennon_phano(std::string text) { 
    system("chcp 866"); 
    std::ofstream encodedFile_hen("C:/Users/Sopha/Desktop/teor inf/ghj/encoded_hen.txt", std::ios::binary); 
    // Подсчитываем частоту символов
    std::map<std::string, int> frequencis = countNodeShannonFanoFrequencies(text); 
    // Преобразуем map в вектор пар (символ, частота) 
    std::vector<std::pair<std::string, int>> frequencyPairs; 
    for (const auto& pair : frequencis) { 
        frequencyPairs.push_back(pair); 
    } 
    // Преобразуем вектор пар в вектор узлов NodeShannonFano 
    std::vector<NodeShannonFano> frequencies; 
    for (const auto& pair : frequencyPairs) { 
        frequencies.push_back(NodeShannonFano(pair.first, pair.second)); 
    } 
    // Кодируем Шенноном-Фано
    std::map<std::string, std::string> ShannonFanoCodes = shannonFano(frequencies);

    
    std::string encodedTextPhano;
    for (char ch : text) {
        encodedTextPhano += ShannonFanoCodes[std::string(1, ch)];
    }

    // Запись в файл encodedFile_haff кодированной информации   
    for (const auto& pair : ShannonFanoCodes) {
        encodedFile_hen << pair.first << ": " << pair.second << std::endl;
    }
    encodedFile_hen<< std::endl;
    encodedFile_hen << encodedTextPhano << std::endl;
    encodedFile_hen.close();

////////////////////////// 
    // Декодирование файла
    // Открытие закодированного файла
    std::ifstream encodedFile("C:/Users/Sopha/Desktop/teor inf/ghj/encoded_hen.txt", std::ios::binary);
    std::ofstream decodedFile_hen("C:/Users/Sopha/Desktop/teor inf/ghj/decoded_hen.txt");

    std::unordered_map<std::string, std::string> decodePhanonCodes;
    readHeaderShannonFano(encodedFile, decodePhanonCodes);
    // Чтение закодированного текста из файла
    std::string encodedTextFromFile = readEncodedTextShannonFano(encodedFile);

    std::string decodedTextPhano = decodeTextShannonFano(encodedTextFromFile, decodePhanonCodes);
    decodedFile_hen << decodedTextPhano << std::endl;

    decodedFile_hen.close();
    return 0;
}
