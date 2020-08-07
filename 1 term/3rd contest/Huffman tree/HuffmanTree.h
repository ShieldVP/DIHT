#include "heap.h"
#include <unordered_map>
#include <stack>

#include <string>
#include <fstream>
#include <sstream>
#include "bitio.h"

// Structure

class HuffmanTree {
public:
    ~HuffmanTree();

    void encodeFile(const std::string& originalSRC, const std::string& resultSRC);
    void decodeFile(const std::string& originalSRC, const std::string& resultSRC);

private:
    // Nodes

    struct Node {
        char symbol{};
        size_t count = 0;
        Node *left = nullptr, *right = nullptr;

        explicit Node(char symbol = char{}, size_t count = 0); // for real nodes
        Node(Node *left, Node *right); // for fake nodes
        bool operator<(const Node& other) const; // count is priority
    };

    bool isRealNode(Node *node) const;

    // Private values

    Node *root = nullptr;
    std::unordered_map<char, std::string> encodingTable;

    // Functions

    void buildTree(const std::string& text);
    size_t extractTree(const std::string& code);
    void clearTree();

    std::string encode(const std::string& text) const;
    std::string decode(const std::string& text, size_t startPosition = 0) const;

    void countSymbols(const std::string& text, std::unordered_map<char, size_t>& symbolsFrequency) const;
    void encodeSymbols(Node *node, const std::string& prefix); // filling encoding table
    std::string computeDictionary() const;
    void outlineDFS(Node *node, std::string& code) const;
    void buildDFS(Node *node, const std::string& code, size_t& position);

    std::string toChars(const std::string& code) const;
    std::string toBits(const std::string& code) const;

    std::string binary(size_t number, size_t size = 0) const;
    size_t decimal(const std::string& number, size_t start, size_t end) const;
    char ASCIIcheck(char symbol) const;
};

// Implementations

// Node

HuffmanTree::Node::Node(char symbol, size_t count) : symbol(symbol), count(count) {}

HuffmanTree::Node::Node(HuffmanTree::Node *left, HuffmanTree::Node *right) : left(left), right(right) {
    count = left->count + right->count;
}

bool HuffmanTree::Node::operator<(const HuffmanTree::Node &other) const {
    return count < other.count;
}

bool HuffmanTree::isRealNode(HuffmanTree::Node *node) const {
    return node && !node->left; // node exists and hasn't got children
}

// Huffman tree

// Tree structure

HuffmanTree::~HuffmanTree() {
    clearTree();
    encodingTable.clear();
}

void HuffmanTree::buildTree(const std::string& text) {
    std::unordered_map<char, size_t> symbolsFrequency;
    countSymbols(text, symbolsFrequency);
    Heap<Node*> bricks;
    for (auto& brick : symbolsFrequency)
        bricks.insert(new Node(brick.first, brick.second));
    while (bricks.size() > 1) {
        auto firstNode = bricks.top();
        bricks.removeTop();
        auto secondNode = bricks.top();
        bricks.removeTop();
        bricks.insert(new Node(firstNode, secondNode));
    }
    clearTree();
    root = bricks.top();
    encodingTable.clear();
    encodeSymbols(root, "");
}

size_t HuffmanTree::extractTree(const std::string& code) {
    encodingTable.clear(), clearTree();
    root = new Node;
    size_t symbolStart = 0;
    buildDFS(root, code, symbolStart);
    encodeSymbols(root, "");
    return symbolStart;
}

void HuffmanTree::clearTree() {
    if (root) {
        std::stack<Node*> buf, res;
        buf.push(root);
        while (!buf.empty()) {
            res.push(buf.top());
            buf.pop();
            if (res.top()->left != nullptr) buf.push(res.top()->left);
            if (res.top()->right != nullptr) buf.push(res.top()->right);
        }
        while (!res.empty()) {
            delete res.top();
            res.pop();
        }
    }
}

// Work with files

void HuffmanTree::encodeFile(const std::string& originalSRC, const std::string& resultSRC) {
    std::ifstream fin(originalSRC, std::ios::binary);
    std::ostringstream oss;
    oss << fin.rdbuf();
    fin.close();
    auto text = oss.str();

    buildTree(text);
    auto code = computeDictionary() + encode(text);
    auto lastSymbolLength = code.size() % 7;
    std::ofstream fout(resultSRC, std::ios::binary);
    fout << lastSymbolLength << toChars(code);
    fout.close();
}

void HuffmanTree::decodeFile(const std::string& originalSRC, const std::string& resultSRC) {
    std::ifstream fin(originalSRC, std::ios::binary);
    std::ostringstream oss;
    oss << fin.rdbuf();
    fin.close();
    auto code = toBits(oss.str());

    auto startPosition = extractTree(code);
    auto text = decode(code, startPosition);
    std::ofstream fout(resultSRC, std::ios::binary);
    fout.write(text.c_str(), text.size());
    fout.close();
}

// Encoding and decoding

std::string HuffmanTree::encode(const std::string& text) const {
    std::string encodedResult;
    for (char symbol : text) encodedResult += encodingTable.at(ASCIIcheck(symbol));
    return encodedResult;
}

std::string HuffmanTree::decode(const std::string& text, size_t startPosition) const {
    std::string decodedResult;
    Node *currentNode = root;
    for (size_t pointer = startPosition; pointer < text.size(); ++pointer) {
        if (isRealNode(currentNode)) {
            decodedResult += currentNode->symbol;
            currentNode = text[pointer] == '0' ? root->left : root->right;
        }
        else currentNode = text[pointer] == '0' ? currentNode->left : currentNode->right;
    }
    return decodedResult + currentNode->symbol;
}

// Important help functions

void HuffmanTree::countSymbols(const std::string& text, std::unordered_map<char, size_t>& symbolsFrequency) const {
    for (char symbol : text)
        ++symbolsFrequency[ASCIIcheck(symbol)];
}

void HuffmanTree::encodeSymbols(HuffmanTree::Node *node, const std::string& prefix) {
    if (isRealNode(node)) encodingTable[node->symbol] = prefix;
    else {
        encodeSymbols(node->left, prefix + "0");
        encodeSymbols(node->right, prefix + "1");
    }
}

std::string HuffmanTree::computeDictionary() const {
    // TODO: https://neerc.ifmo.ru/wiki/index.php?title=%D0%9E%D0%BF%D1%82%D0%B8%D0%BC%D0%B0%D0%BB%D1%8C%D0%BD%D0%BE%D0%B5_%D1%85%D1%80%D0%B0%D0%BD%D0%B5%D0%BD%D0%B8%D0%B5_%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D1%80%D1%8F_%D0%B2_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%B5_%D0%A5%D0%B0%D1%84%D1%84%D0%BC%D0%B0%D0%BD%D0%B0
    std::string dictionary;
    outlineDFS(root, dictionary);
    return dictionary;
}

void HuffmanTree::outlineDFS(HuffmanTree::Node *node, std::string& code) const {
    if (!isRealNode(node)) {
        code += "00"; // Going Left
        outlineDFS(node->left, code);
        code += "01"; // Going Right
        outlineDFS(node->right, code);
    }
    else code += "10" + binary(int(node->symbol), 7); // Writing symbol's ASCII code and going Up
}

void HuffmanTree::buildDFS(HuffmanTree::Node *node, const std::string& code, size_t& position) {
    if (code[position] == '1') {
        // Code is Up
        position += 2;
        node->symbol = decimal(code, position, position + 7);
        position += 7;
    }
    else if (code[position + 1] == '0') {
        // Code is Left
        node->left = new Node;
        position += 2;
        buildDFS(node->left, code, position);
        buildDFS(node, code, position);
    }
    else {
        // Code is Right
        node->right = new Node;
        position += 2;
        buildDFS(node->right, code, position);
    }
}

std::string HuffmanTree::toChars(const std::string& code) const {
    size_t lastSymbolStart = code.size() - (code.size() % 7);
    std::string chars;
    for (size_t start = 0; start < lastSymbolStart; start += 7)
        chars += char(decimal(code, start, start + 7));
    if (lastSymbolStart < code.size())
        chars += char(decimal(code, lastSymbolStart, code.size()));
    return chars;
}

std::string HuffmanTree::toBits(const std::string& code) const {
    auto lastSymbolLength = int(code[0]) - 48; // int('0') = 48
    std::string bits;
    for (size_t pointer = 1; pointer < code.size() - 1; ++pointer)
        bits += binary(int(code[pointer]), 7);
    auto lastSymbol = binary(int(code.back()), 7);
    for (int i = 7 - lastSymbolLength; i < 7; ++i)
        bits += lastSymbol[i];
    return bits;
}

// Simple help functions

std::string HuffmanTree::binary(size_t number, size_t size) const {
    std::string result;
    do {
        result = std::to_string(number % 2) + result;
        number /= 2;
    } while (number > 0);

    std::string leadingZeroes;
    if (size != 0)
        for (short i = 0; i < short(size - result.size()); ++i)
            leadingZeroes += '0';

    return leadingZeroes + result;
}

size_t HuffmanTree::decimal(const std::string& number, size_t start, size_t end) const {
    size_t result = 0, pow = 1;
    for (size_t digit = end - 1; digit != start; --digit) {
        result += (number[digit] - 48) * pow;
        pow <<= unsigned(1);
    }
    return result + (number[start] - 48) * pow;
}

char HuffmanTree::ASCIIcheck(char symbol) const {
    if (int(symbol) < 0 || int(symbol) > 127)
        return ' ';
    else return symbol;
}