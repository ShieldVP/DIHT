/*
Требуется найти в связном графе остовное дерево минимального веса.
Воспользуйтесь алгоритмом Прима.
*/

#include <iostream>
#include <vector>
#include <limits>
using namespace std;

class graph {
public:
    graph(size_t _V, size_t _E) {
        V = _V, E = _E;
        edges.resize(V);
    }

    void add(size_t from, size_t to, size_t weight) {
        edges[from].emplace_back(to, weight);
        edges[to].emplace_back(from, weight);
    }

    size_t min_ostov() const {
        vector<size_t> min_weight_to(V, INF);
        vector<bool> used(V, false);
        min_weight_to[0] = 0;
        size_t length = 0;
        for (size_t it = 0; it < V; ++it) {
            size_t temp_min_weight = INF, from = 0;
            for (size_t vertex = 0; vertex < V; ++vertex)
                if (!used[vertex] && min_weight_to[vertex] < temp_min_weight) {
                    from = vertex;
                    temp_min_weight = min_weight_to[vertex];
                }
            length += min_weight_to[from];
            used[from] = true;
            for (auto &edge : edges[from]) {
                auto to = edge.to, weight = edge.weight;
                if (weight < min_weight_to[to]) min_weight_to[to] = weight;
            }
        }
        return length;
    }

private:
	struct path {
    	path (size_t t, size_t w) {
        	to = t, weight = w;
    	}
    	size_t to, weight;
	};

    size_t V, E;
	vector<vector<path>> edges;
	const size_t INF = std::numeric_limits<size_t>::max();
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
