#include <iostream>   
#include <queue>   
#include <vector>   
#include <unordered_map>  
#include <cmath> // для log2   
#include <algorithm> // для sort  
#include "Haff.h"  
#include <fstream>
#include <sstream>

// Функция для подсчета частоты символов в тексте
std::map<char, int> countSymbolFrequencies(const std::string& text) {
    std::map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }
    return frequencies;
}
  
Node_huff* buildHuffmanTree(std::vector<Node_huff>& frequencies) {
    // Сортировка вектора пар по частоте 
    std::sort(frequencies.begin(), frequencies.end(), [](const auto& a, const auto& b) {
        return a.frequency < b.frequency; // Сортируем в порядке возрастания частоты
    }); 

    // Создаем новый вектор узлов 
    std::vector<Node_huff*> nodes;
    for (const auto& pair : frequencies) {
        nodes.push_back(new Node_huff(pair.symbol, pair.frequency));
    }

    // Пока в векторе больше одного узла 
    while (nodes.size() > 1) {
        // Извлечение двух узлов с наименьшими частотами
        Node_huff* left = nodes[0]; 
        nodes.erase(nodes.begin());
        Node_huff* right = nodes[0]; 
        nodes.erase(nodes.begin()); 

        // Создание нового узла-родителя с суммой частот  
        Node_huff* parent = new Node_huff('0', left->frequency + right->frequency); 
        parent->left = left; 
        parent->right = right; 

        // Вставка родительского узла в правильную позицию в векторе
        // (важно, чтобы он был вставлен так, чтобы вектор оставался отсортированным)
        auto it = std::lower_bound(nodes.begin(), nodes.end(), parent, [](const Node_huff* a, const Node_huff* b) {
            return a->frequency < b->frequency;
        });
        nodes.insert(it, parent);
    }

    // Корень дерева - последний узел в векторе
    return nodes.back(); 
}

void deleteNodesHuffman(Node_huff* node) {
  if (node == nullptr) {
    return;
  }
  deleteNodesHuffman(node->left);
  deleteNodesHuffman(node->right);
  delete node;
}


// Рекурсивная функция для обхода дерева Хаффмана и генерации кодов
void printCodes_huff(Node_huff* node, std::string code, std::unordered_map<std::string, std::string>& huffmanCodes) {
    if (node == nullptr) {
        return;
    }
    if (node->left == nullptr && node->right == nullptr) {
        huffmanCodes[std::string(1, node->symbol)] = code;
        return;
    }
    printCodes_huff(node->left, code + "0", huffmanCodes);
    printCodes_huff(node->right, code + "1", huffmanCodes);
}

// Функция для чтения заголовка из файла
void readHeader(std::ifstream& inputFile, std::unordered_map<std::string, std::string>& codeTable) {
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
std::string readEncodedText(std::ifstream& inputFile) {
    std::string encodedText;
    inputFile >> encodedText;
    return encodedText;
}
// Функция для декодирования текста блоками с использованием таблицы кодирования 
std::string decodeText(const std::string& encodedText, std::unordered_map<std::string, std::string>& decodeHuffmanCodes) { 
    std::string decodedText; 
    std::string currentCode; 
    for (char ch : encodedText) { 
        currentCode += ch; 
 
        // Проверяем, есть ли текущий код в значениях словаря
        for (auto const& [symbol, code] : decodeHuffmanCodes) {
            if (code == currentCode) {
                decodedText += symbol; 
                currentCode = ""; 
                break; // Выходим из цикла, так как символ найден
            }
        }
    }  
    return decodedText;  
}
int main_haff(std::map<std::string, int> frequencies_blok, std::string text) {
    system("chcp 866");
    std::ofstream encodedFile_haff("C:/Users/Sopha/Desktop/teor inf/ghj/encoded_haff.txt");
    std::map<char, int> frequencis= countSymbolFrequencies(text); //для каждой буквы
    // Преобразуем map в вектор пар (символ, частота)
    std::vector<std::pair<char, int>> frequencyPairs;
    for (const auto& pair : frequencis) {
        frequencyPairs.push_back(pair);
    }
    // Преобразуем вектор пар в вектор узлов Node_huff
    std::vector<Node_huff> frequencies;
    for (const auto& pair : frequencyPairs) {
        frequencies.push_back(Node_huff(pair.first, pair.second));
    }
    Node_huff* root_symbol = buildHuffmanTree(frequencies);
    std::unordered_map<std::string, std::string> huffmanCodes;

    printCodes_huff(root_symbol, "", huffmanCodes);
    
    std::string encodedTextHuffman;
    for (char ch : text) {
        encodedTextHuffman += huffmanCodes[std::string(1, ch)];
    }

    // Запись в файл encodedFile_haff кодированной информации   
    for (const auto& pair : huffmanCodes) {
        encodedFile_haff << pair.first << ": " << pair.second << std::endl;
    }
    encodedFile_haff<< std::endl;
    encodedFile_haff << encodedTextHuffman << std::endl;
    encodedFile_haff.close();

//////////////////////////
    // Декодирование файла
    // Открытие закодированного файла
    std::ifstream encodedFile("C:/Users/Sopha/Desktop/teor inf/ghj/encoded_haff.txt");
    std::ofstream decodedFile_haff("C:/Users/Sopha/Desktop/teor inf/ghj/decoded_haff.txt");

    std::unordered_map<std::string, std::string> decodeHuffmanCodes;
    readHeader(encodedFile, decodeHuffmanCodes);
    // Чтение закодированного текста из файла
    std::string encodedTextFromFile = readEncodedText(encodedFile);

    std::string decodedTextHuffman = decodeText(encodedTextFromFile,decodeHuffmanCodes);
    decodedFile_haff << decodedTextHuffman << std::endl;
    // deleteNodesHuffman(root_symbol);

    decodedFile_haff.close();
    return 0;
}

