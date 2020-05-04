/*
Вам дан неориентированный граф, состоящий из n вершин.
На каждой вершине записано число; число, записанное на вершине i, равно ai.
Изначально в графе нет ни одного ребра.
Вы можете добавлять ребра в граф за определенную стоимость.
За добавление ребра между вершинами x и y надо заплатить ax + ay монет.
Также существует m специальных предложений, каждое из которых характеризуется тремя числами x, y и w,
и означает, что можно добавить ребро между вершинами x и y за w монет.
Эти специальные предложения не обязательно использовать: если существует такая пара вершин x и y,
такая, что для нее существует специальное предложение, можно все равно добавить ребро между ними за ax + ay монет.
Сколько монет минимально вам потребуется, чтобы сделать граф связным?
Граф является связным, если от каждой вершины можно добраться до любой другой вершины, используя только ребра этого графа.
*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class DSU {
public:
    explicit DSU(size_t n) {
        for (size_t i = 0; i < n; i++) id.push_back(i);
        size.resize(n, 1);
    }

    size_t find(size_t x) {
        if (id[x] == x) return x;
        return id[x] = find(id[x]);
    }

    void unite(size_t _x, size_t _y) {
        auto x = find(_x), y = find(_y);
        if (x != y) {
            if (size[x] < size[y]) {
                id[x] = y;
                size[y] += size[x];
            }
            else {
                id[y] = x;
                size[x] += size[y];
            }
        }
    }

    bool equivalent(size_t x, size_t y) {
        return find(x) == find(y);
    }

private:
    vector<size_t> id, size;
};

class graph {
public:
    graph(size_t _V) {
        V = _V;
    }

    void set_numbers(const vector<size_t> &numbers) {
        size_t min = 0;
        for (size_t i = 0; i < V; ++i)
            if (numbers[min] > numbers[i]) min = i;
        for (size_t i = 0; i < V; ++i)
            add(min, i, numbers[i] + numbers[min]);
    }

    void add(size_t from, size_t to, size_t weight) {
        edges.emplace_back(from, to, weight);
        edges.emplace_back(to, from, weight);
    }

    size_t min_ostov() {
        sort(edges.begin(), edges.end(), [](path edge1, path edge2){return edge1.weight < edge2.weight;});
        DSU components(V);
        size_t length = 0;
        for (auto &edge: edges)
            if (!components.equivalent(edge.from, edge.to)) { // merge
                length += edge.weight;
                components.unite(edge.from, edge.to);
            }
        return length;
    }

private:
    struct path {
        path (size_t f, size_t t, size_t w) {
            from = f, to = t, weight = w;
        }
        size_t from, to, weight;
    };

    vector<path> edges;
    size_t V;
};

int main() {
    size_t n, m;
    cin >> n >> m;
    graph G(n);

    vector<size_t> numbers(n);
    for (size_t i = 0; i < n; ++i) cin >> numbers[i];
    G.set_numbers(numbers);

    for (size_t i = 0; i < m; ++i) {
        size_t f, t, w;
        cin >> f >> t >> w;
        G.add(f - 1, t - 1, w);
    }
    cout << G.min_ostov();
    return 0;
}