/*
Шрек и Осёл уже были на пути домой. Им оставалось только преодолеть лес, который отделял их от болота. 
Но они поссорились, поэтому не хотят идти вместе.
Лес представляет собой опушки, пронумерованы числами от 1 до n и соединенные m дорожками
(может быть несколько дорожек соединяющих две опушки, могут быть дорожки, соединяющие опушку с собой же).
Из-за ссоры, если по дорожке прошел один из друзей, то второй по той же дорожке уже идти не может. 
Сейчас друзья находятся на опушке с номером s, а болото Шрека — на опушке с номером t. 
Помогите Шреку и Ослу добраться до болота.
*/

#include <iostream>
#include <vector>
#include <map>
using namespace std;

class graph {
public:
    explicit graph(size_t _V, size_t _E) {
        V = _V;
        E = _E;
        edges.resize(V);
        read_edges();
    }

    void read_edges() {
        for (size_t i = 0; i < E; ++i) {
            size_t from, to;
            cin >> from >> to;
            add(from - 1, to - 1);
        }
    }

    void add(size_t from, size_t to) {
        if (back.find(tf(from, to)) == back.end()) {
            edges[from].emplace_back(to, 1);
            edges[to].emplace_back(from, 0);
            back[tf(from, to)] = edges[to].size() - 1;
            back[tf(to, from)] = edges[from].size() - 1;
        }
        else ++edges[from][back[tf(to, from)]].capacity;
    }

    pair<vector<size_t>, vector<size_t>> get_two_pathes(size_t from, size_t to) {
        auto empty = make_pair(vector<size_t>(), vector<size_t>());
        vector<size_t> ways[2];
        for (auto &way : ways) {
            vector<bool> visited(V, false);
            auto flow = dfs(from, to, visited);
            if (flow == 0) return empty;
            else if (flow >= 2) break;
        }
        for (auto &way : ways) repair(way, from, to);
        return make_pair(ways[0], ways[1]);
    }

private:
    struct path {
        path(size_t _to, int cap) {
            to = _to, capacity = cap, flow = 0;
        }
        size_t to;
        int capacity, flow;
    };

    size_t V, E;
    vector<vector<path>> edges;
    map<size_t, size_t> back;
    static const int INF = numeric_limits<int>::max();

    size_t tf(size_t x, size_t y) {
        return V * x + y; // (x, y) e R2 <--> f e R
    }

    size_t dfs(size_t from, size_t final_goal, vector<bool> &visited, int flow = INF) {
        if (from == final_goal) return flow;
        visited[from] = true;
        for (auto &edge : edges[from])
            if (!visited[edge.to] && edge.capacity > edge.flow) {
                auto delta = dfs(edge.to, final_goal, visited, min(flow, edge.capacity - edge.flow));
                if (delta > 0) {
                    auto &back_edge = edges[edge.to][back[tf(from, edge.to)]];
                    edge.flow += delta, back_edge.flow -= delta;
                    return delta;
                }
            }
        return 0;
    }

    void repair(vector<size_t> &way, size_t from, size_t to) {
        way.emplace_back(from);
        auto current = from;
        while (current != to)
            for (auto &edge : edges[current])
                if (edge.flow > 0 && edge.capacity > 0) {
                    way.emplace_back(edge.to);
                    current = edge.to;
                    --edge.flow;
                    break;
                }
    }
};

int main() {
    size_t n, m, s, t;
    cin >> n >> m >> s >> t;
    graph G(n, m);
    auto pathes = G.get_two_pathes(s - 1, t - 1);
    if (pathes.first.empty()) cout << "NO";
    else {
        cout << "YES" << endl;
        for (auto &vertex : pathes.first) cout << vertex + 1 << ' ';
        cout << endl;
        for (auto &vertex : pathes.second) cout << vertex + 1 << ' ';
    }
    return 0;
}