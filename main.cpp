#include <fstream>
#include <cmath>
#include <bitset>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include "Hennon-Phano.h"
#include "Haff.h"
// Структура узла дерева
struct Node {
    std::string block;
    Node* left;
    Node* right;

    Node(const std::string& block) : block(block), left(nullptr), right(nullptr) {}
};

// Функция для создания дерева кодирования
Node* createCodingTree(const std::vector<std::string>& blocks) {
    // Инициализация очереди узлов
    std::queue<Node*> nodes;

    // Создание корневого узла (пустой)
    Node* root = new Node("");

    // Добавление всех блоков в очередь
    for (const std::string& block : blocks) {
        nodes.push(new Node(block));
    }

    // Строим дерево
    while (nodes.size() > 1) {
        // Извлекаем два узла из очереди
        Node* left = nodes.front();
        nodes.pop();
        Node* right = nodes.front();
        nodes.pop();

        // Создаем новый узел с пустым блоком и добавляем два извлеченных узла как потомков
        Node* parent = new Node("");
        parent->left = left;
        parent->right = right;

        // Добавляем новый узел обратно в очередь
        nodes.push(parent);
    }

    // Возвращаем корневой узел дерева
    return nodes.front();
}

std::map<std::string, std::string> createCodeTable(Node* root, int codeLength) {
    std::map<std::string, std::string> codeTable;
    std::queue<std::pair<Node*, std::string>> queue;
    queue.push(std::make_pair(root, ""));

    while (!queue.empty()) 
    {
      Node* node = queue.front().first;
      std::string code = queue.front().second;
      queue.pop();

      // Если узел - лист (имеет блок), добавляем его в таблицу кодирования.
      if (node->block != "") {
          codeTable[node->block] = code;
      } else { 
          if (node->left) {
              std::string newCode = code + "0";
              queue.push(make_pair(node->left, newCode)); 
          }
          if (node->right) {
              std::string newCode = code + "1";
              queue.push(make_pair(node->right, newCode)); 
          }
      }
      
    } 
  
    if (codeLength > 0) 
    { 
      for (auto it = codeTable.begin(); it != codeTable.end(); it++) 
      { 
          while (it->second.length() < codeLength) 
          { 
              it->second += "0"; 
          } 
          if (it->second.length() > codeLength) 
          { 
              it->second = it->second.substr(0, codeLength); 
          } 
      } 
    }
    return codeTable;
}
// Функция для подсчета частоты символов в тексте
std::map<std::string , int> countSymbolFrequencies(const std::vector<std::string>& text) {
    std::map<std::string , int> frequencies;
    for (std::string c : text) {
        frequencies[c]++;
    }
    return frequencies;
}

// Функция для вычисления длины кода с одинаковой длиной для всех символов
int calculateCodeLength(const std::map<std::string, int>& frequencies) {
    int totalFrequency = 0;
    for (auto it : frequencies) {
        totalFrequency += it.second;
    }
    return ceil(log2(totalFrequency));
}

// Функция для записи заголовка в файл
void writeHeader(std::ofstream& outputFile, int blockSize, int codeLength, const std::map<std::string, std::string> & codeTable) {
    outputFile << blockSize << std::endl;
    outputFile << codeLength << std::endl;
    for (auto it : codeTable) {
        outputFile << it.first << ": " << it.second << std::endl;
    }
}

// Функция для чтения заголовка из файла
std::pair<int, int> readHeader(std::ifstream& inputFile, std::map<std::string, std::string>& codeTable) {
    int blockSize;
    int codeLength;

    inputFile >> blockSize;
    inputFile >> codeLength;
    std::string line;
    getline(inputFile, line); // Сброс оставшейся части строки

    while (getline(inputFile, line)) {
        if (line.empty()) {
            break;
        }
        size_t pos = line.find(':');
        std::string block = line.substr(0, pos);
        std::string code = line.substr(pos + 2);
        codeTable[block] = code;
    }

    return std::make_pair(blockSize, codeLength);
}

// Функция для кодирования блоками
std::vector<std::string> encodeBlocks(const std::string& text, int blockSize) {
    std::vector<std::string> blocks;
    for (size_t i = 0; i < text.size(); i += blockSize) {
        blocks.push_back(text.substr(i, blockSize));
    }
    return blocks;
}

// Функция для декодирования блоками
std::string decodeBlocks(const std::vector<std::string>& blocks) {
    std::string decodedText;
    for (const std::string& block : blocks) {
        decodedText += block;
    }
    return decodedText;
}

// Функция для кодирования текста блоками с использованием таблицы кодирования
std::string encodeText(const std::vector<std::string>& blocks, const std::map<std::string, std::string>& codeTable) {
    std::string encodedText;
    for (const std::string& block : blocks) {
        encodedText += codeTable.at(block);
    }
    return encodedText;
}

// Функция для декодирования текста блоками с использованием таблицы кодирования
std::string decodeText(const std::string& encodedText, const std::map<std::string, std::string>& codeTable, int codeLength, int blockSize) {
    std::string decodedText;
    for (size_t i = 0; i < encodedText.size(); i += codeLength) {
        std::string blockCode = encodedText.substr(i, codeLength);
        for (auto it : codeTable) {
            if (it.second == blockCode) {
                decodedText += it.first;
                break;
            }
        }
    }
    return decodedText;
}
// Функция для записи закодированного текста в файл
void writeEncodedText(std::ofstream& outputFile, const std::string& encodedText) {
    outputFile <<std::endl;
    outputFile << encodedText;
}

// Функция для чтения закодированного текста из файла
std::string readEncodedTex(std::ifstream& inputFile) {
    std::string encodedText;
    inputFile >> encodedText;
    return encodedText;
}

int main() {
    // Создание файлов
    std::ofstream outputFile("C:/Users/Sopha/Desktop/teor inf/ghj/encoded.txt");
    std::ifstream inputFile("C:/Users/Sopha/Desktop/teor inf/ghj/text.txt");
    
    // Чтение текста из файла
    std::string text;
    getline(inputFile, text, '0');
    inputFile.close();

    // Кодирование блоками
    int blockSize = 3; // Размер блока
    std::vector<std::string> blocks = encodeBlocks(text, blockSize);

    // Подсчет частоты символов
    std::map<std::string, int> frequencies = countSymbolFrequencies(blocks);

    // Вычисление длины кода
    int codeLength = calculateCodeLength(frequencies);

    // Создаем дерево кодирования
    Node* root = createCodingTree(blocks);

    // Создаем таблицу кодирования
    std::map<std::string, std::string> codeTable = createCodeTable(root, codeLength);
    
    // Запись заголовка в файл
    writeHeader(outputFile, blockSize, codeLength, codeTable);

    // Кодирование текста
    std::string encodedText = encodeText(blocks, codeTable);

    // Запись закодированного текста в файл
    writeEncodedText(outputFile, encodedText);

    // Закрытие файла
    outputFile.close();

    // =======================================================================

    // Декодирование файла
    // Открытие закодированного файла
    std::ifstream encodedFile("C:/Users/Sopha/Desktop/teor inf/ghj/encoded.txt");

    // Чтение заголовка из файла
    std::map<std::string, std::string> decodedCodeTable;
    std::pair<int, int> headerInfo = readHeader(encodedFile, decodedCodeTable);
    int blockSize_d = headerInfo.first;
    int codeLength_d = headerInfo.second;

    // Чтение закодированного текста из файла
    std::string encodedTextFromFile = readEncodedTex(encodedFile);

    // Декодирование текста
    std::string decodedText = decodeText(encodedTextFromFile, decodedCodeTable, codeLength_d, blockSize_d);

    // Декодирование блоками
    std::vector<std::string> decodedBlocks;
    for (size_t i = 0; i < decodedText.size(); i += codeLength_d) {
        decodedBlocks.push_back(decodedText.substr(i, codeLength_d));
    }
    std::string finalDecodedText = decodeBlocks(decodedBlocks);

    // Запись декодированного текста в файл
    std::ofstream outputDecodedFile("C:/Users/Sopha/Desktop/teor inf/ghj/decoded.txt");
    outputDecodedFile << finalDecodedText;
    outputDecodedFile.close();

    // Закрытие файлов
    encodedFile.close();
    main_haff(frequencies, text);
    main_hennon_phano(text);
    return 0;
}