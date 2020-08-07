/*
Дано число N ≤ 10^4 и последовательность целых чисел из [-2^31, 2^31] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки. 
Выведите элементы в порядке post-order (снизу вверх).
Рекурсия запрещена.
*/

#include <iostream>
#include <stack>
using namespace std;

template<typename T, class Comparer>
class BST {
public:
	BST(Comparer compare) : compare(compare) {}

	~BST() {
		stack<node*> buf, res;
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

	bool empty() const {
		return root == nullptr;
	}

	bool insert(T const& val) {
		if (empty()) root = new node(val);
		else {
			auto current = root, parent = root;
			bool is_left = true;
			while (current != nullptr) {
				parent = current;
				if (compare(val, current->value)) {
					current = current->left;
					is_left = true;
				}
				else if (compare(current->value, val)) {
					current = current->right;
					is_left = false;
				}
				else return false;
			}
			if (is_left) parent->left = new node(val);
			else parent->right = new node(val);
		}
		return true;
	}

	void print() const {
		stack<node*> buf, res;
		buf.push(root);
		while (!buf.empty()) {
			res.push(buf.top());
			buf.pop();
			if (res.top()->left != nullptr) buf.push(res.top()->left);
			if (res.top()->right != nullptr) buf.push(res.top()->right);
		}
		while (!res.empty()) {
			cout << res.top()->value << ' ';
			res.pop();
		}
	}

private:
	struct node {
		T value;
		node* left = nullptr;
		node* right = nullptr;
		explicit node(T const& val) :value(val) {}
	};
	node* root = nullptr;
	Comparer compare;
};

class int_comparer {
public:
	bool operator()(int x, int y) {
		return x < y;
	}
};

int main() {
	int_comparer icomp;
	BST<int, int_comparer> tree(icomp);
	int N;
	cin >> N;
	for (int i = 0; i < N; ++i) {
		int tmp;
		cin >> tmp;
		tree.insert(tmp);
	}
	tree.print();
}