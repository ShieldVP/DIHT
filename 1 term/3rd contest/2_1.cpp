/*
Вычислить разницу глубин наивного дерева поиска и декартового дерева.
Разница может быть отрицательна, необходимо вывести модуль разности.
*/

#include <iostream>
#include <stack>
#include <cmath>
using namespace std;

template<typename T_key, typename T_priority, class Comparer_key, class Comparer_priority>
class Decart {
public:
	Decart(Comparer_key comp1, Comparer_priority comp2) : compare_key(comp1), compare_priority(comp2) {
		root = nullptr;
	}

	~Decart() {
		stack<Node*> buf, res;
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

	void insert(T_key x, T_priority y) {
		if (empty()) root = new Node(x, y);
		else if (compare_priority(root->priority, y)) {
			pair<Node*, Node*> children = root->split(x, compare_key);
			root = new Node(x, y);
			root->left = children.first;
			root->right = children.second;
		}
		else root->insert(x, y, compare_key, compare_priority);
	}

	int getHeight() {
		return root->height() - 1;
	}

private:
	struct Node {
		Node* left = nullptr, * right = nullptr;
		T_key key;
		T_priority priority;
		Node(T_key k, T_priority p) {
			key = k;
			priority = p;
		}

		void insert(T_key x, T_priority y, Comparer_key compare_key, Comparer_priority compare_priority) {
			if (compare_key(x, key)) { // going left
				if (left == nullptr) left = new Node(x, y);
				else if (compare_priority(left->priority, y)) {
					pair<Node*, Node*> children = left == nullptr ? make_pair(nullptr, nullptr) : left->split(x, compare_key);
					left = new Node(x, y);
					left->left = children.first;
					left->right = children.second;
				}
				else left->insert(x, y, compare_key, compare_priority);
			}
			else { // going right
				if (right == nullptr) right = new Node(x, y);
				else if (compare_priority(right->priority, y)) {
					pair<Node*, Node*> children = right == nullptr ? make_pair(nullptr, nullptr) : right->split(x, compare_key);
					right = new Node(x, y);
					right->left = children.first;
					right->right = children.second;
				}
				else right->insert(x, y, compare_key, compare_priority);
			}
		}

		pair<Node*, Node*> split(T_key k, Comparer_key comp) {
			if (comp(k, key)) { // going left
				pair<Node*, Node*> buf = left == nullptr ? make_pair(nullptr, nullptr) : left->split(k, comp);
				left = buf.second;
				return make_pair(buf.first, this);
			}
			else { // going right
				pair<Node*, Node*> buf = right == nullptr ? make_pair(nullptr, nullptr) : right->split(k, comp);
				right = buf.first;
				return make_pair(this, buf.second);
			}
		}

		int height() {
			int lh = left == nullptr ? 0 : left->height();
			int rh = right == nullptr ? 0 : right->height();
			return 1 + (lh > rh ? lh : rh);
		}
	};
	Node* root;
	Comparer_key compare_key;
	Comparer_priority compare_priority;
};

template<typename T, class Comparer>
class BST {
public:
	BST(Comparer comp) : compare(comp) {
		root = nullptr;
		height = 0;
	}

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
			// adding element
			int wayHeight = 0;
			auto current = root, parent = root;
			bool is_left = true;
			while (current != nullptr) {
				parent = current;
				if (compare(val, current->value)) {
					++wayHeight;
					current = current->left;
					is_left = true;
				}
				else if (compare(current->value, val)) {
					++wayHeight;
					current = current->right;
					is_left = false;
				}
				else return false; // if we have such an element in BST
			}
			if (is_left) parent->left = new node(val);
			else parent->right = new node(val);
			if (wayHeight > height) height = wayHeight;
		}
		return true;
	}

	int getHeight() const {
		return height;
	}

private:
	struct node {
		T value;
		node* left = nullptr, * right = nullptr;
		explicit node(T const& val) :value(val) {}
	};
	node* root;
	Comparer compare;
	int height;
};

class int_comparer {
public:
	bool operator()(int x, int y) {
		return x < y;
	}
};

int main() {
	int_comparer icomp;
	BST<int, int_comparer> firstTree(icomp);
	Decart<int, int, int_comparer, int_comparer> secondTree(icomp, icomp);
	int N;
	cin >> N;
	for (int i = 0; i < N; ++i) {
		int x, y;
		cin >> x >> y;
		firstTree.insert(x);
		secondTree.insert(x, y);
	}
	cout << abs(secondTree.getHeight() - firstTree.getHeight());
	return 0;
}