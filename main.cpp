#include <iostream>   
#include <queue>   
#include <vector>   
#include <unordered_map>  
#include <cmath> // для log2   
#include <algorithm> // для sort  
#include "Hennon-Phano.h"  
#include <fstream>
// Структура для узла дерева Хаффмана   
struct Node {   
    char symbol; // Символ, который представляет узел   
    double frequency; // Частота символа   
    Node* left; // Левый дочерний узел   
    Node* right; // Правый дочерний узел   
  
    // Конструктор для создания узла   
    Node(char symbol, double frequency) : symbol(symbol), frequency(frequency), left(nullptr), right(nullptr) {}   
};   
 Node* buildHuffmanTree(std::vector<Node>& frequencies) {
    // Сортировка вектора пар по частоте 
    std::sort(frequencies.begin(), frequencies.end(), [](const auto& a, const auto& b) {
        return a.frequency < b.frequency; // Сортируем в порядке возрастания частоты
    }); 

    // Создаем новый вектор узлов 
    std::vector<Node*> nodes;
    for (const auto& pair : frequencies) {
        nodes.push_back(new Node(pair.symbol, pair.frequency));
    }

    // Пока в векторе больше одного узла 
    while (nodes.size() > 1) {
        // Извлечение двух узлов с наименьшими частотами
        Node* left = nodes[0]; 
        nodes.erase(nodes.begin());
        Node* right = nodes[0]; 
        nodes.erase(nodes.begin()); 

        // Создание нового узла-родителя с суммой частот  
        Node* parent = new Node('0', left->frequency + right->frequency); 
        parent->left = left; 
        parent->right = right; 

        // Вставка родительского узла в правильную позицию в векторе
        // (важно, чтобы он был вставлен так, чтобы вектор оставался отсортированным)
        auto it = std::lower_bound(nodes.begin(), nodes.end(), parent, [](const Node* a, const Node* b) {
            return a->frequency < b->frequency;
        });
        nodes.insert(it, parent);
    }

    // Корень дерева - последний узел в векторе
    return nodes.back(); 
}   
  
// Функция для рекурсивного вывода кодов Хаффмана   
void printCodes(Node* node, std::string code, std::map<char, std::string>& codes) {
    if (node == nullptr) {
        return;
    }

    // Если узел - лист (символ), сохраняем его код
    if (node->symbol != '0') {        
        codes[node->symbol] = code;
    }

    // Рекурсивный вызов для левого и правого узлов
    printCodes(node->left, code + "0", codes);
    printCodes(node->right, code + "1", codes);
}

// Функция для вычисления энтропии 
double calculateEntropy(const std::vector<Node>& frequencies) { 
    double entropy = 0; 
    for (auto& frequency : frequencies) { 
        if (frequency.frequency > 0) { // Проверка на ненулевую частоту 
            entropy -= frequency.frequency * log2(frequency.frequency);  
        } 
    } 
    return entropy; 
} 

int main() { 
    setlocale(LC_ALL, "Russian"); // Установка кодировки для русского языка 
    // Заданные частоты символов 
    std::vector<Node> frequencies = { 
        {'a', 0.21}, 
        {'b', 0.19}, 
        {'c', 0.15}, 
        {'d', 0.13}, 
        {'e', 0.12}, 
        {'f', 0.09}, 
        {'g', 0.06}, 
        {'h', 0.05} 
    }; 

    // Построение дерева Хаффмана 
    Node* root = buildHuffmanTree(frequencies); 

    // Вывод кодов Хаффмана 
    std::wstring text = L"Коды Хаффмана:";// Не работает сетлокал 
    std::wcout << text << std::endl; 

    // Создание карты для хранения кодов
    std::map<char, std::string> codes;
    // Получение кодов
    printCodes(root, "", codes);
    // Вывод кодов из карты 
    for (const auto& pair : codes) { 
        std::cout << pair.first << ": " << pair.second << std::endl; 
    }
    std::cout<<std::endl; 
    std::wcout << L"Средняя Энтропия:" << std::endl; //P*log2(P)
    double Entropy = calculateEntropy(frequencies);
    std::wcerr<< Entropy <<L" бит"<<std::endl; 

    int Hmax = log2(8); //максимальная энтропия Hmax = log2 (кол-во символов)
    std::wcout << L"Избыточность:" << std::endl; 
    std::cout<<1-(Entropy/Hmax)<<std::endl;//1-(H/Hmax) 
    std::cout << std::endl;

    
    // Вывод средней длины кодовой комбинации для Хаффмана
    std::wcout << L"Средняя длина кодовой комбинации: " << std::endl; //длина кода*вероятнсть

    // Вывод средней длины кода  
    double averageLengt = 0;  
    for (const auto& [character, code] : codes) {  
        for (const auto& symbols : frequencies) {  
            if (symbols.symbol == character) {  
                averageLengt += symbols.frequency * code.length();  
            }  
        }  
    }  
    std::wcout << averageLengt <<L" бит"<< std::endl; 
    std::cout << std::endl;

    // Вывод кодов Шеннона-Фано
    std::wcout << L"Коды Шеннона-Фано:" << std::endl;
    std::map<char, std::string> codik = main_Phanno();
    
    // Запись данных в файл
    std::ofstream outputFile("shannon_fano_codes.txt");
    if (outputFile.is_open()) {
        outputFile << "Коды Хаффмана:" <<std::endl;
        for (const auto& [character, code] : codes) {
            outputFile << character << ": " << code << std::endl;
        }
        outputFile << "Коды Шеннона-Фано:" <<std::endl;
        for (const auto& [charact, cod] : codik) {
            outputFile << charact << ": " << cod << std::endl;
        }
        outputFile << "Средняя Энтропия: " <<Entropy<<std::endl;
        outputFile << "Избыточность: " <<1-(Entropy/Hmax)<<std::endl;
        outputFile << "Средняя длина кодовой комбинации Хаффмана: " <<averageLengt<<std::endl;
        outputFile << "Средняя длина кодовой комбинации Шеннона-Фано: " <<Longprint()<<std::endl;
        outputFile.close();
        std::wcout << L"Данные успешно записаны в файл shannon_fano_codes.txt" << std::endl;
    } else {
        std::cerr << "Ошибка открытия файла!" << std::endl;
    }
    return 0; 
}