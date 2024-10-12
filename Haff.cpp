#include <iostream>   
#include <queue>   
#include <vector>   
#include <unordered_map>  
#include <cmath> // для log2   
#include <algorithm> // для sort  
#include "Haff.h"  
#include <fstream>
#include <sstream>
#include <bitset>   
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
        Node_huff* parent = new Node_huff("0", left->frequency + right->frequency); 
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

// Рекурсивная функция для обхода дерева Хаффмана и генерации кодов
void printCodes_huff(Node_huff* node, std::string code, std::unordered_map<std::string, std::string>& huffmanCodes) { 
    if (node == nullptr) { 
        return; 
    } 
    if (node->left == nullptr && node->right == nullptr) { 

        huffmanCodes[std::string(node->symbol)] = code; 
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

// Функция для записи заголовка в файл
void writeHeader(std::ofstream& encodedFile, int blockSize, const std::unordered_map<std::string, std::string>& codeTable) {
    // Запись размера блока
    encodedFile << blockSize << std::endl;

    // Запись таблицы кодов
    for (const auto& pair : codeTable) {
        encodedFile << pair.first << " " << pair.second << std::endl;
    }
}

// Функция для кодирования текста
std::string encodeText(const std::vector<std::string>& blocks, const std::unordered_map<std::string, std::string>& codeTable) {
    std::string encodedText;
    for (const auto& block : blocks) {
        encodedText += codeTable.at(block); // Получаем код из таблицы
    }
    return encodedText;
}

// // Функция для записи закодированного текста в файл
// void writeEncodedText(std::ofstream& encodedFile, const std::string& encodedText) {
//     // Преобразование кодов в биты
//     std::bitset<8> bits;
//     std::string binaryText;
//     for (char c : encodedText) {
//         bits = std::bitset<8>(c);
//         binaryText += bits.to_string();
//     }

//     // Запись в файл
//     encodedFile << binaryText << std::endl;
// }
void average_(std::ofstream& encodedFile_haff, std::unordered_map<std::string, std::string> huffmanCodes, std::map<std::string, double> frequencies)
{
  encodedFile_haff<< "Средняя длина кодовой комбинации: ";
    // std::cout << "Средняя длина кодовой комбинации: " << std::endl; //длина кода*вероятнсть
    // Вывод средней длины кода  
    double averageLengt = 0;  
    for (const auto& [character, code] : huffmanCodes) {  
        for (auto& symbols : frequencies) {  
            if (symbols.first == character) {  
                averageLengt += symbols.second * code.length();  
            }  
        }  
    } 
  encodedFile_haff<<averageLengt << std::endl; 
}
int main_haff(std::map<std::string, double> frequencies_block, std::string text, std::vector<std::string> blocks) {
    std::ofstream encodedFile_haff("C:/Users/Sopha/Desktop/teor inf/lab_02/encoded_haff.txt");

    // Преобразуем map в вектор пар (символ, частота) 
    std::vector<std::pair<std::string, double>> frequencyPairs; 
    for (const auto& pair : frequencies_block) { 
        frequencyPairs.push_back(pair); 
    } 

    // Сортируем вектор пар по частоте в порядке возрастания
    std::sort(frequencyPairs.begin(), frequencyPairs.end(), [](const auto& a, const auto& b) { 
        return a.second > b.second; // Сортировка по возрастанию частоты 
    }); 

    // Преобразуем вектор пар в вектор узлов Node_huff 
    std::vector<Node_huff> frequencies; 
    for (const auto& pair : frequencyPairs) { 
        frequencies.push_back(Node_huff(pair.first, pair.second)); 
    } 

    Node_huff* root_symbol = buildHuffmanTree(frequencies);
    std::unordered_map<std::string, std::string> huffmanCodes;

    printCodes_huff(root_symbol, "", huffmanCodes);
    
    // std::string encodedTextHuffman;
    // for (char ch : text) {
    //     encodedTextHuffman += huffmanCodes[std::string(1, ch)];
    // }

    // // Запись в файл encodedFile_haff кодированной информации   
    // for (const auto& pair : huffmanCodes) {
    //     encodedFile_haff << pair.first << ": " << pair.second << std::endl;
    // }

    // Запись заголовка в файл
    writeHeader(encodedFile_haff, 3, huffmanCodes);
    // Кодирование текста
    std::string encodedTextHuffman = encodeText(blocks, huffmanCodes);
    // Запись закодированного текста в файл
    // writeEncodedText(encodedFile_haff, encodedTextHuffman);

    encodedFile_haff<< std::endl;
    encodedFile_haff << encodedTextHuffman << std::endl;
    average_(encodedFile_haff, huffmanCodes, frequencies_block);
    encodedFile_haff.close();

//////////////////////////
    // Декодирование файла
    // Открытие закодированного файла
    std::ifstream encodedFile("C:/Users/Sopha/Desktop/teor inf/lab_02/encoded_haff.txt");
    std::ofstream decodedFile_haff("C:/Users/Sopha/Desktop/teor inf/lab_02/decoded_haff.txt");

    std::unordered_map<std::string, std::string> decodeHuffmanCodes;
    readHeader(encodedFile, decodeHuffmanCodes);
    // Чтение закодированного текста из файла
    std::string encodedTextFromFile = readEncodedText(encodedFile);

    std::string decodedTextHuffman = decodeText(encodedTextFromFile,decodeHuffmanCodes);
    decodedFile_haff << decodedTextHuffman << std::endl;
    decodedFile_haff.close();

    return 0;
}

