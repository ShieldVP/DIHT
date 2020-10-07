/*
 Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”.
 Найти позиции всех вхождений шаблона в тексте длины n.
 Каждое вхождение шаблона предполагает, что все обычные символы совпадают с соответствующими из текста,
 а вместо символа “?” в тексте встречается произвольный символ. Гарантируется, что сам “?” в тексте не встречается.
 Время работы - O(n + m + Z), где Z - общее число вхождений подстрок шаблона “между вопросиками” в исходном тексте.
 m ≤ 5000, n ≤ 2000000.
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Trie {
public:
    Trie(const vector<pair<string, int>>& base) {
        root = new Node;
        for (auto& str : base)
            add(str.first, str.second);
    }

    ~Trie() {
        flush(root);
    }

    void add(const string& str, int shift) {
        auto currentState = root;
        for (char s : str) {
            if (currentState->children.find(s) == currentState->children.end()) {
                currentState->children[s] = new Node;
                currentState->children.at(s)->symbol = s;
                currentState->children.at(s)->parent = currentState;
            }
            currentState = currentState->children.at(s);
        }
        currentState->shifts.push_back(shift);
    }

    vector<int> findSubstrings(const string& str) const {
        vector<int> partsCounter(str.size(), 0);
        auto currentState = root;
        for (int i = 0; i < str.size(); ++i) {
            currentState = go(currentState, str[i]);
            auto tempState = currentState;
            while (tempState != root) {
                for (auto shift : tempState->shifts)
                    if (i >= shift) ++partsCounter[i - shift];
                tempState = getNextTerminalSuffixLink(tempState);
            }
        }
        return partsCounter;
    }

private:
    struct Node {
        char symbol{};
        vector<int> shifts; // shifts for terminal nodes
        Node *parent = nullptr, *suffixLink = nullptr, *nextTerminalSuffixLink = nullptr;
        unordered_map<char, Node*> children;
    };

    Node* root;

    Node* go(Node* node, char symbol) const {
        while (node->children.find(symbol) == node->children.end() && node != root)
            node = getSuffixLink(node);
        if (node == root && node->children.find(symbol) == node->children.end())
            return root;
        return node->children.at(symbol);
    }

    Node* getSuffixLink(Node* node) const {
        if (node->suffixLink == nullptr) {
            if (node == root || node->parent == root)
                node->suffixLink = root;
            else
                node->suffixLink = go(getSuffixLink(node->parent), node->symbol);
        }
        return node->suffixLink;
    }

    Node* getNextTerminalSuffixLink(Node* node) const {
        if (node->nextTerminalSuffixLink == nullptr) {
            auto nextNode = getSuffixLink(node);
            if (nextNode == root)
                node->nextTerminalSuffixLink = root;
            else
                node->nextTerminalSuffixLink = nextNode->shifts.empty() ? getNextTerminalSuffixLink(nextNode) : nextNode;
        }
        return node->nextTerminalSuffixLink;
    }

    void flush(Node* node) {
        if (node) {
            for (auto child : node->children)
                flush(child.second);
            delete node;
        }
    }
};

vector<int> findMask(const string& text, const string& mask) {
    // separate mask on substrings without masks
    vector<pair<string, int>> parts;
    int lastNoMaskStart = 0;
    for (int i = 0; i < mask.size(); ++i)
        if (mask[i] == '?') {  // mask symbol
            if (lastNoMaskStart != i)
                parts.emplace_back(mask.substr(lastNoMaskStart, i - lastNoMaskStart), i - 1);
            lastNoMaskStart = i + 1;
        }
    if (lastNoMaskStart != mask.size())
        parts.emplace_back(mask.substr(lastNoMaskStart, mask.size() - lastNoMaskStart), mask.size() - 1);

    // counting parts by positions
    Trie editor = parts;
    auto partsCounter = editor.findSubstrings(text);

    // looking for positions where all parts can be
    vector<int> result;
    for (int i = 0; i < partsCounter.size(); ++i)
        if (partsCounter[i] == parts.size() && i + mask.size() <= text.size())
            result.push_back(i);

    return result;
}

int main() {
    string mask, text;
    cin >> mask >> text;
    for (auto position : findMask(text, mask))
        cout << position << ' ';
    return 0;
}