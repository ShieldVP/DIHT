/*
В одной военной части решили построить в одну шеренгу по росту.
Cолдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие.
Все солдаты в части разного роста.
Для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.
N – количество команд (1 ≤ N ≤ 90 000).
В каждой следующей строке содержится описание команды:
число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно),
число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя.
Солдаты в строе нумеруются с нуля.
Требуемая скорость выполнения команды - O(log N).
*/

#include <iostream>
using namespace std;

template<typename T, class Comparer>
class AVL {
public:
	AVL(Comparer compare) : compare(compare) {
		root = nullptr;
	}

	~AVL() {
		AVLNode::flush(root);
	}

	bool empty() const {
		return root == nullptr;
	}

	int size() const {
		return empty() ? 0 : root->children_amount + 1;
	}

	int height() const {
		return empty() ? 0 : root->height + 1;
	}

	int insert(T const& given_key) {
		int result = 0;
		if (empty()) root = new AVLNode(given_key);
		else root = root->insert(given_key, result, compare);
		return result;
	}

	void remove(int number) {
		if (!empty()) root = root->remove(number);
	}

private:
	struct AVLNode {
		AVLNode *left, *right;
		T key;
		int height, children_amount;

		AVLNode(T const& given_key) : key(given_key) {
			left = right = nullptr;
			height = 1, children_amount = 0;
		}

		int getBalanceFactor() const {
			int r = right == nullptr ? 0 : right->height;
			int l = left == nullptr ? 0 : left->height;
			return r - l;
		}

		void fix_children() {
			children_amount = 0;
			if (left != nullptr) ++children_amount;
			if (right != nullptr) ++children_amount;
			children_amount += count_children(left) + count_children(right);
		}

		void fix_height() {
			int rh = right == nullptr ? 0 : right->height;
			int lh = left == nullptr ? 0 : left->height;
			height = (rh > lh ? rh : lh) + 1;
		}

		int count_children(AVLNode* node) const {
			if (node == nullptr) return 0;
			else return node->children_amount;
		}

		int position() {
			int current_number = count_children(right);
			if (right != nullptr) ++current_number;
			return current_number;
		}

		AVLNode* insert(T const& given_key, int& result, Comparer compare) {
			++children_amount;
			if (compare(given_key, key)) {
				result += count_children(right) + 1;
				if (right != nullptr) ++result;
				left = left == nullptr ? new AVLNode(given_key) : left->insert(given_key, result, compare);
			}
			else
				right = right == nullptr ? new AVLNode(given_key) : right->insert(given_key, result, compare);
			return balance();
		}

		AVLNode* findMinimum() {
			--children_amount;
			return left != nullptr ? left->findMinimum() : this;
		}

		AVLNode* removeMinimum() {
			if (left == nullptr) return right;
			left = left->removeMinimum();
			return balance();
		}

		AVLNode* remove(int number) {
			if (this == nullptr) return nullptr; // if root is empty

			int current_number = position();

			if (number > current_number) { // going left
				--children_amount;
				left = left->remove(number - current_number - 1);
				return balance();
			}
			else if (number < current_number) { // going right
				--children_amount;
				right = right->remove(number);
				return balance();
			}
			else { // gotcha
				AVLNode* l = left;
				AVLNode* r = right;
				int node_children = children_amount;
				delete this;
				if (r == nullptr) return l;
				AVLNode* min = r->findMinimum();
				min->right = r->removeMinimum();
				min->left = l;
				min->children_amount = node_children - 1;
				return min->balance();
			}
		}

		AVLNode* balance() {
			fix_height();
			switch (getBalanceFactor()) { // difference in levels
			case -2:
				if (left->getBalanceFactor() > 0) left = left->rotateLeft();
				return rotateRight();
			case 2:
				if (right->getBalanceFactor() < 0) right = right->rotateRight();
				return rotateLeft();
			default:
				return this;
			}
		}

		AVLNode* rotateRight() {
			auto result = left;
			left = result->right;
			result->right = this;
			fix_height();
			result->fix_height();
			fix_children();
			result->fix_children();
			return result;
		}

		AVLNode* rotateLeft() {
			auto result = right;
			right = result->left;
			result->left = this;
			fix_height();
			result->fix_height();
			fix_children();
			result->fix_children();
			return result;
		}

		void static flush(AVLNode* node) {
			if (node->left != nullptr) flush(node->left);
			if (node->right != nullptr) flush(node->right);
			if (node != nullptr) delete node;
		}
	};
	AVLNode* root;
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
	AVL<int, int_comparer> tree(icomp);
	int N;
	cin >> N;
	for (int i = 0; i < N; ++i) {
		int command, value;
		cin >> command >> value;
		if (command == 1) cout << tree.insert(value) << endl;
		else tree.remove(value);
	}
	return 0;
}