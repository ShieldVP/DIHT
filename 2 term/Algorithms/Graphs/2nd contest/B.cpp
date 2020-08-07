/*
 Рику необходимо попасть на межвселенную конференцию.
 За каждую телепортацию он платит бутылками лимонада, поэтому хочет потратить их на дорогу как можно меньше (он же всё-таки на конференцию едет!).
 Однако после K перелетов подряд Рика начинает сильно тошнить, и он ложится спать на день.
 Ему известны все существующие телепортации. Теперь Рик хочет найти путь (наименьший по стоимости в бутылках лимонада),
 учитывая, что телепортация не занимает времени, а до конференции осталось 10 минут (то есть он может совершить не более K перелетов)!
 В первой строке находятся количество вселенных N, количество существующих телепортаций M, количество перелетов подряд K, номер вселенной Рика S, номер вселенной конференции F (2 ≤ N ≤ 300, 1 ≤ M ≤ 10^5, 1 ≤ K ≤ 300, 1 ≤ S ≤ N, 1 ≤ F ≤ N).
 Далее идет M строк, задающих телепортации. i-я строка содержит три натуральных числа: S_i, F_i и P_i, где S_i - номер вселенной, из которой ведет i-я телепортация, F_i - номер вселенной, в которую ведет i-я телепортация, P_i - стоимость i-й телепортации. 1 ≤ S_i ≤ N, 1 ≤ F_i ≤ N, 1 ≤ P_i ≤ 10^6.
*/

#include <iostream>
#include <vector>
using namespace std;

class graph {
public:
    explicit graph(size_t V) {
        edges.resize(V);
    }

    void add(size_t from, size_t to, size_t weight) {
        edges[from].push_back(make_pair(to, weight));
    }

    int getMinDist(size_t from, size_t to, size_t limit) const {
        vector<int> distances;
        distances.resize(edges.size(), -1); // will consider infinity = -1
        distances[from] = 0;

        for (auto counter = 0; counter < limit; ++counter) {
            auto tmpDistances = distances;
            for (auto i = 0; i < edges.size(); ++i)
                for (const auto& edge : edges[i]) // i = from, edge.first = to, edge.second = cost
                    if (tmpDistances[edge.first] > distances[i] + edge.second && distances[i] != -1)
                        tmpDistances[edge.first] = distances[i] + edge.second;
            distances = tmpDistances;
        }

        return distances[to];
    }

private:
    vector<vector<pair<size_t, size_t>>> edges;
};

int main() {
    size_t N, M, K, S, F;
    cin >> N >> M >> K >> S >> F;
    graph G(N);
    for (auto i = 0; i < M; ++i) {
        size_t from, to, cost;
        cin >> from >> to >> cost;
        G.add(from - 1, to - 1, cost);
    }
    cout << G.getMinDist(S - 1, F - 1, K);
    return 0;
}