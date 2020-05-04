/*
Требуется найти в связном графе остовное дерево минимального веса.
Воспользуйтесь алгоритмом Крускала.
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
    graph(size_t _V, size_t _E) {
        V = _V, E = _E;
    }

    void add(size_t from, size_t to, size_t weight) {
        edges.emplace_back(from, to, weight);
        edges.emplace_back(to, from, weight);
    }

    size_t min_ostov() {
        sort(edges.begin(), edges.end(), [](path edge1, path edge2){return edge1.weight < edge2.weight;});
        DSU components(V);
        size_t length = 0;
        for (auto &edge : edges)
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
    size_t V, E;
};

int main() {
    size_t n, m;
    cin >> n >> m;
    graph G(n, m);
    for (size_t i = 0; i < m; ++i) {
        size_t f, t, w;
        cin >> f >> t >> w;
        G.add(f - 1, t - 1, w);
    }
    cout << G.min_ostov();
    return 0;
}