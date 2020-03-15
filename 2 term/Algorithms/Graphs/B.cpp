/*
Первая строка содержит пару целых чисел n и m (1 ≤ n, m ≤ 105) — количество вершин и рёбер заданного ориентированного графа.
Следующие m строк содержат описания рёбер, по одному в строке.
Каждая из строк содержит по два целых числа a и b (0 ≤ a, b < n) — начало и конец ребра.
Если такого порядка не существует, то выведите «NO» (без кавычек).
В противном случае, выведите «YES» в первую из строк,
во вторую строку выведите перестановку индексов вершин (целые числа от 0 до n - 1),
такую, что для каждого ребра его стартовая вершина находится строго левее его конечной вершины.
*/

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class graph {
public:
    graph(size_t V) {
        adj.resize(V);
    }

    void add(size_t from, size_t to) {
        adj[from].push_back(to);
    }

    vector<size_t> toposort() {
        vector<char> colors(adj.size(), 'W');
        vector<size_t> queue;
        bool cycleExist = false;
        for (size_t i = 0; i < adj.size() && !cycleExist; ++i) dfs(i, colors, queue, cycleExist);
        cycleExist ? queue.clear() : reverse(queue.begin(), queue.end());
        return queue;
    };
private:
    vector<vector<size_t>> adj;

    void dfs(size_t point, vector<char>& colors, vector<size_t>& queue, bool& cycleExist) {
        if (colors[point] == 'G') cycleExist = true;
        else if (colors[point] == 'W' && !cycleExist) { // only for white vertices dfs can be used
            colors[point] = 'G'; // now it is grey point
            for (auto vertex : adj[point]) dfs(vertex, colors, queue, cycleExist); // dfsing all the neighbours
            colors[point] = 'B'; // we dfsed all the neighbours
            queue.push_back(point); // keep the point visited
        }
    }
};

vector<size_t> topologySort(vector<pair<size_t, size_t>> &input, size_t V) {
    graph quiver(V);
    for (auto edge : input) quiver.add(edge.first, edge.second);
    return quiver.toposort();
}

int main() {
    size_t V, E;
    cin >> V >> E;
    vector<pair<size_t, size_t>> input;
    for (size_t i = 0; i < E; ++i) {
        size_t from, to;
        cin >> from >> to;
        input.push_back(make_pair(from, to));
    }
    vector<size_t> output = topologySort(input, V);
    if (output.empty()) cout << "NO";
    else {
        cout << "YES" << endl;
        for (size_t i : output) cout << i << ' ';
    }
    return 0;
}