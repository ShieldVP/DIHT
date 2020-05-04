/*
Шрек и Фиона пригласили всех своих друзей на свою свадьбу.
На церемонии они хотят рассадить их всех на две непустые части так, 
чтобы количество знакомств между двумя частями было минимальным. 
Всего приглашенных на свадьбу n, а каждое знакомство обоюдно.
Вам дан граф, в котором ребро означает знакомство между людьми. 
Помогите Шреку и Фионе поделить гостей на две непустые части.
*/

#include <iostream>
#include <vector>
#include <limits>
using namespace std;

class DSU {
public:
    explicit DSU(size_t n) {
        for (size_t i = 0; i < n; i++) id.push_back(i);
        size.resize(n, 1);
        existence.resize(n, true);
    }

    size_t find(size_t x) {
        if (id[x] == x) return x;
        return id[x] = find(id[x]);
    }

    void unite(size_t _x, size_t _y) {
        existence[_y] = false;
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

    bool self(size_t x) {
        return existence[x];
    }

    vector<size_t> get_elements(size_t _x) {
        auto x = find(_x);
        vector<size_t> elements;
        for (size_t i = 0; i < id.size(); ++i)
            if (find(i) == x) elements.emplace_back(i);
        return elements;
    }

private:
    vector<size_t> id, size;
    vector<bool> existence;
};

class graph {
public:
    explicit graph(size_t _V) {
        V = _V, E = 0;
        edges.resize(V, vector<size_t> (V, 0));
    }

    void add(size_t from, size_t to) {
        edges[from][to] = 1;
        ++E;
    }

    void read_matrix() {
        for (size_t i = 0; i < V; ++i)
            for (size_t j = 0; j < V; ++j) {
                char connected;
                cin >> connected;
                if (connected == '1') add(i, j);
            }
    }

    pair<vector<size_t>, vector<size_t>> minimal_cut() {
        DSU vertices(V);
        vector<size_t> first_pole;
        auto current_penalty = INF;

        for (size_t phase = 0; phase < V - 1; ++phase) {
            vector<bool> set(V, false);
            vector<size_t> weights(V, 0);
            for (size_t it = 0, previous = 0; it < V - phase; ++it) {
                size_t chosen_vertex = INF;
                for (size_t i = 0; i < V; ++i)
                    if (!set[i] && vertices.self(i) && (chosen_vertex == INF || weights[i] > weights[chosen_vertex]))
                        chosen_vertex = i;
                if (it != V - phase - 1) { // counting weights
                    set[chosen_vertex] = true;
                    for (size_t i = 0; i < V; ++i)
                        weights[i] += edges[chosen_vertex][i];
                    previous = chosen_vertex;
                }
                else { // finally combine
                    if (weights[chosen_vertex] < current_penalty) {
                        current_penalty = weights[chosen_vertex];
                        first_pole = vertices.get_elements(chosen_vertex);
                    }
                    vertices.unite(previous, chosen_vertex);
                    for (size_t i = 0; i < V; ++i) {
                        edges[i][previous] += edges[chosen_vertex][i];
                        edges[previous][i] = edges[i][previous]; // symmetric
                    }
                }
            }
        }

        vector<bool> used_vertices(V, false);
        vector<size_t> difference;
        for (auto &vertex : first_pole) used_vertices[vertex] = true;
        for (size_t i = 0; i < V; ++i)
            if (!used_vertices[i]) difference.emplace_back(i);
        return make_pair(first_pole, difference);
    }

private:
    size_t V, E;
    vector<vector<size_t>> edges;
    const size_t INF = numeric_limits<size_t>::max();
};

int main() {
    size_t n;
    cin >> n;
    graph G(n);
    G.read_matrix();
    auto parts = G.minimal_cut();
    for (auto &vertex : parts.first) cout << vertex + 1 << ' ';
    cout << endl;
    for (auto &vertex : parts.second) cout << vertex + 1 << ' ';
    return 0;
}