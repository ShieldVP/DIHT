/*
Леон и Матильда собрались пойти в магазин за молоком.
Каково минимальное количество улиц, по которым пройдёт хотя бы один из ребят (либо Матильда, либо Леон, либо оба вместе)?
Первая строка содержит количество вершин n (1 ≤ n ≤ 105), количество ребер m (0 ≤ m ≤ 105)
и номера вершин графа leon, matilda, milk, в которых находятся соответственно Леон, Матильда и магазин с молоком.
Следующие m строк содержат ребра графа. В каждой строке два числа, разделенные пробелом, если между i и j существует ребро.
Гарантируется, что в графе нет петель и мультиребер. Граф связный и неориентированный. Вершины нумеруются с 1.
*/

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class graph {
public:
    graph(size_t V) {
        adj.resize(V);
    }

    ~graph() {
        adj.clear();
    }

    void add(size_t from, size_t to) {
        adj[to].push_back(from);
        adj[from].push_back(to);
    }

    size_t number_of_vertex() {
        return adj.size();
    }

    size_t get_min_dist(size_t LEON, size_t MATILDA, size_t MILK) {
        vector<size_t> distances;
        distances.resize(number_of_vertex(), 0);
        bfs(distances, LEON);
        bfs(distances, MATILDA);
        bfs(distances, MILK);
        size_t min_distance = distances[0];
        for (auto distance : distances)
            if (distance < min_distance) min_distance = distance;
        return min_distance;
    }

private:
    vector<vector<size_t>> adj;

    void bfs(vector<size_t>& distances, size_t from) {
        queue<size_t> q = {};
        q.push(from);
        vector<bool> walked;
        walked.resize(number_of_vertex(), false);
        walked[from] = true;
        vector<size_t> local_distances;
        local_distances.resize(number_of_vertex(), 0);
        while (!q.empty()) {
            size_t vertex = q.front();
            q.pop();
            for (auto to : adj[vertex])
                if (!walked[to]) {
                    walked[to] = true;
                    q.push(to);
                    local_distances[to] = local_distances[vertex] + 1;
                }
        }
        for (size_t i = 0; i < distances.size(); ++i) distances[i] += local_distances[i];
    }
};

size_t distance(vector<pair<size_t, size_t>>& input, size_t V, size_t LEON, size_t MATILDA, size_t MILK) {
    graph quiver(V);
    for (auto edge : input) quiver.add(edge.first - 1, edge.second - 1);
    return quiver.get_min_dist(LEON - 1, MATILDA - 1, MILK - 1);
}

int main() {
    size_t V, E, LEON, MATILDA, MILK;
    cin >> V >> E >> LEON >> MATILDA >> MILK;
    vector<pair<size_t, size_t>> input;
    for (size_t i = 0; i < E; ++i) {
        size_t from, to;
        cin >> from >> to;
        input.push_back(make_pair(from, to));
    }
    cout << distance(input, V, LEON, MATILDA, MILK);
    return 0;
}