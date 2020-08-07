/*
Рик и Морти снова бороздят просторы вселенных, но решили ограничиться только теми, номера которых меньше M.
Они могут телепортироваться из вселенной с номером z во вселенную (z+1) mod M за a бутылок лимонада
или во вселенную (z^2+1) mod M за b бутылок лимонада.
Рик и Морти хотят добраться из вселенной с номером x во вселенную с номером y.
Сколько бутылок лимонада отдаст Рик за такое путешествие, если он хочет потратить их как можно меньше?
*/

#include <iostream>
#include <vector>
#include <deque>
using namespace std;

class graph {
public:
    graph(size_t V, size_t cost1, size_t cost2) {
        edges.resize(V);
        if (cost1 < cost2){
            for (size_t i = 0; i < V - 1; ++i) {
                add(i, i + 1, cost1);
                add(i, (i * i + 1) % V, cost2);
            }
            add(V - 1, 0, cost1);
            add(V - 1, (V * V + 2) % V, cost2);
        }
        else {
            for (size_t i = 0; i < V - 1; ++i) {
                add(i, (i * i + 1) % V, cost2);
                add(i, i + 1, cost1);
            }
            add(V - 1, (V * V + 2) % V, cost2);
            add(V - 1, 0, cost1);
        }
    }

    void add(size_t from, size_t to, size_t weight) {
        edges[from].push_back(make_pair(to, weight));
    }

    size_t getMinDist(size_t from, size_t to) {
        vector<int> distances;
        distances.resize(edges.size(), -1);
        distances[from] = 0;

        deque<size_t> queue;
        queue.push_front(from);

        while (!queue.empty()) {
            size_t point = queue.front();
            queue.pop_front();
            for (auto edge : edges[point]) {
                auto vertex = edge.first, cost = edge.second;
                if (distances[vertex] == -1 || distances[vertex] > distances[point] + cost) {
                    distances[vertex] = distances[point] + cost;
                    if (edge == edges[point][0]) queue.push_front(vertex);
                    else queue.push_back(vertex);
                }
            }
        }

        return distances[to];
    }

private:
    vector<vector<pair<size_t, size_t>>> edges;
};

int main() {
    size_t a, b, M, x, y;
    cin >> a >> b >> M >> x >> y;
    graph G(M, a, b);
    cout << G.getMinDist(x, y);
    return 0;
}