/*
Фрэнку опять прилетел новый заказ. Однако в этот раз город играет по очень странным правилам:
все дороги в городе односторонние и связывают только офисы нанимателей перевозчика.
Множество офисов, между любыми двумя из которых существует путь, образуют квартал, если нельзя добавить никакие другие, чтобы условие выполнялось.
Фрэнку интересно, каково минимальное количество односторонних дорог нужно ещё построить, чтобы весь город стал квраталом.
Первая строка содержит пару целых чисел n и m (1 ≤ n ≤ 105, 0 ≤ m ≤ 105) — количество вершин и рёбер заданного ориентированного графа.
Следующие m строк содержат описания рёбер, по одному в строке.
Каждая из строк содержит по два целых числа a и b (1 ≤ a, b ≤ n) — начало и конец ребра.
Нумерация ведётся с 1. Петли допустимы.
*/

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class graph {
public:
    graph(size_t V) {
        adj.resize(V);
        rev.resize(V);
    }

    void add(size_t from, size_t to) {
        adj[from].push_back(to);
        rev[to].push_back(from);
    }

    pair<vector<size_t>, size_t> getStrongConnections() {
        vector<char> colors(adj.size(), 'W');
        vector<size_t> queue;
        for (size_t i = 0; i < adj.size(); ++i) dfs(i, colors, queue);
        reverse(queue.begin(), queue.end());
        vector<size_t> component(adj.size());
        colors.assign(adj.size(), 'W');
        size_t numberOfIteration = 0;
        for (auto vertex : queue)
            if (colors[vertex] == 'W') { // if we haven't matched the SCC for this vertex
                special_dfs(vertex, numberOfIteration, colors, component);
                ++numberOfIteration;
            }
        return make_pair(component, numberOfIteration);
    }

    size_t numberEdgesToStrongConnection() {
        auto components = getStrongConnections();
        if (components.second == 1) return 0; // if there is only one SCC all is ok already
        vector<bool> no_going_from(components.second, true);
        vector<bool> no_going_to(components.second, true);
        size_t sources = components.second, sinks = components.second;
        // now we need to check all the edges (so it is O(E))
        for (size_t from = 0; from < adj.size(); ++from)
            for (auto to : adj[from])
                if (components.first[from] != components.first[to]) { // they're from different SCC
                    if (no_going_from[components.first[from]]) --sinks;
                    if (no_going_to[components.first[to]]) --sources;
                    no_going_from[components.first[from]] = false;
                    no_going_to[components.first[to]] = false;
                }
        return (sources > sinks ? sources : sinks); // maximum of sources and sinks amounts
    }

private:
    vector<vector<size_t>> adj; // direct edges
    vector<vector<size_t>> rev; // reverse edges

    void dfs(size_t point, vector<char>& colors, vector<size_t>& queue) {
        if (colors[point] == 'W') { // use dfs only if point is White
            colors[point] = 'G'; // now we got into the point so it Grey now
            for (auto vertex : adj[point]) dfs(vertex, colors, queue); // dfsing all the kids
            colors[point] = 'B'; // we got out of the point so it is Black now
            queue.push_back(point); // keep point as visited
        }
    }

    void special_dfs(size_t point, size_t component_num, vector<char>& colors, vector<size_t>& components) {
        if (colors[point] == 'W') { // use dfs only if point is White
            colors[point] = 'G'; // now we got into the point so it Grey now
            for (auto vertex : rev[point]) special_dfs(vertex, component_num, colors, components); // dfsing all the kids
            colors[point] = 'B'; // we got out of the point so it is Black now
            components[point] = component_num; // keep point as visited
        }
    }
};

int main() {
    size_t V, E;
    cin >> V >> E;
    graph G(V);
    for (size_t i = 0; i < E; ++i) {
        size_t from, to;
        cin >> from >> to;
        G.add(from - 1, to - 1);
    }
    cout << G.numberEdgesToStrongConnection();
    return 0;
}