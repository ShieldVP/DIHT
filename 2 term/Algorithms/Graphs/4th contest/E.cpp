/*
Реализуйте структуру данных “массив строк” на основе декартового дерева по неявному ключу со следующими методами: 
// Добавление строки в позицию position.\\  
// Все последующие строки сдвигаются на одну позицию вперед.\\  
void InsertAt( int position, const std::string& value );\\  
// Удаление строки из позиции position.\\  
// Все последующие строки сдвигаются на одну позицию назад.\\  
void DeleteAt( int position );\\  
// Получение строки из позиции position.\\  
std::string GetAt( int position );
Все методы должны работать за O(log n) в среднем, где n – текущее количество строк в массиве.
Команда “+ 10 hello”
означает добавление строки hello в позицию 10.
Команда “- 14 16”
означает удаление строки от позиции 14 до позиции 16.
Команда “? 33”
означает запрос на вывод строки из массива в позиции 33.
*/

#include <iostream>
#include <string>
#include <stack>
#include <ctime>
#include <cassert>
using namespace std;

#define ll long long

template <typename T>
class SmartArray {
public:
    SmartArray() {
        root = nullptr;
        srand(time(nullptr));
    }

    ~SmartArray() {
        stack<Node*> buf, res;
        buf.push(root);
        while (!buf.empty()) {
            res.push(buf.top());
            buf.pop();
            if (res.top()) {
                if (res.top()->left != nullptr) buf.push(res.top()->left);
                if (res.top()->right != nullptr) buf.push(res.top()->right);
            }
        }
        while (!res.empty()) {
            delete res.top();
            res.pop();
        }
    }

    bool empty() const {
        return root == nullptr;
    }

    size_t size() const {
        return size_of(root);
    }

    void InsertAt(int position, const T &value) {
        auto halves = split(root, position);
        auto inserted = new Node(value);
        auto right = merge(inserted, halves.second);
        root = merge(halves.first, right);
    }

    void DeleteAt(int position) {
        assert(!empty());
        auto halves = split(root, position);
        auto deleting = split(halves.second, 1);
        root = merge(halves.first, deleting.second);
    }

    T GetAt(int position) {
        assert(!empty());
        auto node = root;
        for (ll left = size_of(node->left); left != position; left = size_of(node->left)) {
            if (position < left) node = node->left;
            else {
                node = node->right;
                position -= left + 1;
            }
        }
        return node->value;
    }

private:
    struct Node {
        Node *left, *right;
        size_t size;
        ll priority;
        T value;

        explicit Node(T val) {
            size = 1, priority = rand(), value = val;
            left = nullptr, right = nullptr;
        }
    };

    Node *root;

    size_t size_of(Node* const &target) const {
        return target ? target->size : 0;
    }

    void update(Node *target) {
        if (target) target->size = size_of(target->left) + size_of(target->right) + 1;
    }

    Node* merge(Node *left, Node *right) {
        if (!left) return right;
        if (!right) return left;
        if (left->priority > right->priority) { // going right
            left->right = merge(left->right, right);
            update(left);
            return left;
        }
        else { // going left
            right->left = merge(left, right->left);
            update(right);
            return right;
        }
    }

    pair<Node*, Node*> split(Node *target, ll separator) {
        if (!target) return make_pair(nullptr, nullptr);
        if (size_of(target->left) >= separator) {
            auto halves = split(target->left, separator);
            target->left = halves.second;
            update(target);
            return make_pair(halves.first, target);
        }
        else {
            auto halves = split(target->right, separator - size_of(target->left) - 1);
            target->right = halves.first;
            update(target);
            return make_pair(target, halves.second);
        }
    }
};

int main() {
    SmartArray<string> array;
    size_t n;
    cin >> n;
    for (size_t i = 0; i < n; ++i) {
        char command;
        cin >> command;
        if (command == '+') {
            int position;
            string value;
            cin >> position >> value;
            array.InsertAt(position, value);
        }
        else if (command == '?') {
            int position;
            cin >> position;
            cout << array.GetAt(position) << endl;
        }
        else {
            int left, right;
            cin >> left >> right;
            for (size_t j = 0; j < right - left + 1; ++j)
                array.DeleteAt(left);
        }
    }
    return 0;
}