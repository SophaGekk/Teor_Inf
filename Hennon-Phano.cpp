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
// Функция для записи заголовка в файл
void writeHeader_(std::ofstream& encodedFile, int blockSize, const std::map<std::string, std::string>& codeTable) {
    // Запись размера блока
    encodedFile << blockSize << std::endl;

    // Запись таблицы кодов
    for (const auto& pair : codeTable) {
        encodedFile << pair.first << " " << pair.second << std::endl;
    }
}

// Функция для кодирования текста
std::string encodeText_(const std::vector<std::string>& blocks, const std::map<std::string, std::string>& codeTable) {
    std::string encodedText;
    for (const auto& block : blocks) {
        encodedText += codeTable.at(block); // Получаем код из таблицы
    }
    return encodedText;
}
void average(std::ofstream& encodedFile_hen, std::map<std::string, std::string> ShannonFanoCodes, std::map<std::string, double> frequencies_)
{
  encodedFile_hen<< "Средняя длина кодовой комбинации: ";
    // std::cout << "Средняя длина кодовой комбинации: " << std::endl; //длина кода*вероятнсть
    // Вывод средней длины кода  
    double averageLengt = 0;  
    for (const auto& [character, code] : ShannonFanoCodes) {  
        for (auto& symbols : frequencies_) {  
            if (symbols.first == character) {  
                averageLengt += symbols.second * code.length();  
            }  
        }  
    } 
  encodedFile_hen<<averageLengt << std::endl; 
}
int main_hennon_phano(std::map<std::string, double> frequencies_, std::string text, std::vector<std::string> blocks) { 
    system("chcp 866"); 
    std::ofstream encodedFile_hen("C:/Users/Sopha/Desktop/teor inf/lab_02/encoded_hen.txt", std::ios::binary); 
    // Преобразуем map в вектор пар (символ, частота) 
    std::vector<std::pair<std::string, double>> frequencyPairs; 
    for (const auto& pair : frequencies_) { 
        frequencyPairs.push_back(pair); 
    } 

    // Сортируем вектор пар по частоте в порядке возрастания
    std::sort(frequencyPairs.begin(), frequencyPairs.end(), [](const auto& a, const auto& b) { 
        return a.second > b.second; // Сортировка по возрастанию частоты 
    }); 

    // Преобразуем вектор пар в вектор узлов Node_huff 
    std::vector<NodeShannonFano> frequencies; 
    for (const auto& pair : frequencyPairs) { 
        frequencies.push_back(NodeShannonFano(pair.first, pair.second)); 
    } 
    // Кодируем Шенноном-Фано
    std::map<std::string, std::string> ShannonFanoCodes = shannonFano(frequencies);

    // Запись заголовка в файл
    writeHeader_(encodedFile_hen, 3, ShannonFanoCodes);
    // Кодирование текста
    std::string encodedTextPhano = encodeText_(blocks, ShannonFanoCodes);

    encodedFile_hen<< std::endl;
    encodedFile_hen << encodedTextPhano << std::endl;
    average(encodedFile_hen, ShannonFanoCodes, frequencies_);
    encodedFile_hen.close();

////////////////////////// 
    // // Декодирование файла
    // // Открытие закодированного файла
    // std::ifstream encodedFile("C:/Users/Sopha/Desktop/teor inf/lab_02/encoded_hen.txt", std::ios::binary);
    // std::ofstream decodedFile_hen("C:/Users/Sopha/Desktop/teor inf/lab_02/decoded_hen.txt");

    // std::unordered_map<std::string, std::string> decodePhanonCodes;
    // readHeaderShannonFano(encodedFile, decodePhanonCodes);
    // // Чтение закодированного текста из файла
    // std::string encodedTextFromFile = readEncodedTextShannonFano(encodedFile);

    // std::string decodedTextPhano = decodeTextShannonFano(encodedTextFromFile, decodePhanonCodes);
    // decodedFile_hen << decodedTextPhano << std::endl;

    // decodedFile_hen.close();
    return 0;
}
