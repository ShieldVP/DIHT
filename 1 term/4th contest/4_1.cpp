/*
Дано невзвешенное дерево.
Расстоянием между двумя вершинами будем называть количество ребер в пути, соединяющем эти две вершины.
Для каждой вершины определите расстояние до самой удаленной от нее вершины.
В первой строке записано количество вершин n ≤ 10000. Затем следует n - 1 строка, описывающая ребра дерева.
Каждое ребро - это два различных целых числа - индексы вершин в диапазоне [0, n-1].
Индекс корня – 0. В каждом ребре родительской вершиной является та, чей номер меньше.
*/

#include <iostream>
#include <vector>
using namespace std;

class Tree {
public:
	Tree(size_t const& N) {
		tree.resize(N);
	}

	~Tree() {
		tree.clear();
	}

	bool empty() const {
		return tree.empty();
	}

	void insert(size_t const& parent, size_t const& child) {
		tree[parent].children.push_back(child);
		tree[child].parent = parent;
	}

	void correct_parametres() {
		for (int i = tree.size() - 1; i >= 0; --i)
			if (!tree[i].children.empty()) {
				size_t max_height = 0, pre_max_height = 0;
				for (size_t j = 0; j < tree[i].children.size(); ++j) {
					if (tree[tree[i].children[j]].height >= max_height) {
						pre_max_height = max_height;
						max_height = tree[tree[i].children[j]].height;
					}
					else if (tree[tree[i].children[j]].height > pre_max_height) pre_max_height = tree[tree[i].children[j]].height;
				}
				tree[i].height = max_height + 1;
				tree[i].second_height = pre_max_height + 1;
			}
		for (size_t i = 1; i < tree.size(); ++i) {
			size_t parent = tree[i].parent;
			if (tree[parent].children.size() == 1) tree[i].higher_length = tree[parent].higher_length + 1;
			else if (tree[parent].height == tree[i].height + 1) {
				if (tree[parent].higher_length < tree[parent].second_height) tree[i].higher_length = tree[parent].second_height + 1;
				else tree[i].higher_length = tree[parent].higher_length + 1;
			}
			else if (tree[parent].higher_length < tree[parent].height) tree[i].higher_length = tree[parent].height + 1;
			else tree[i].higher_length = tree[parent].higher_length + 1;
		}
	}

	size_t further_node_distance(size_t const& number) const {
		return tree[number].height > tree[number].higher_length ? tree[number].height : tree[number].higher_length; // max of height and higher_length
	}

private:
	struct node {
		vector<size_t> children;
		int parent = -1;
		size_t height = 0, second_height = 0, higher_length = 0;
	};
	vector<node> tree;
};

int main() {
	size_t N;
	cin >> N;
	Tree tree(N);
	for (size_t i = 0; i < N - 1; ++i) {
		size_t parent, child;
		cin >> parent >> child;
		if (child < parent) parent, child = child, parent;
		tree.insert(parent, child);
	}
	tree.correct_parametres();
	for (size_t i = 0; i < N; ++i) cout << tree.further_node_distance(i) << endl;
	return 0;
}