/*
 Рику необходимо попасть на межвселенную конференцию.
 За каждую телепортацию он платит бутылками лимонада, поэтому хочет потратить их на дорогу как можно меньше (он же всё-таки на конференцию едет!).
 Однако после K перелетов подряд Рика начинает сильно тошнить, и он ложится спать на день.
 Ему известны все существующие телепортации. Теперь Рик хочет найти путь (наименьший по стоимости в бутылках лимонада),
 учитывая, что телепортация не занимает времени, а до конференции осталось 10 минут (то есть он может совершить не более K перелетов)!
 В первой строке даны количество вселенных N (2 ≤ N ≤ 100), количество существующих телепортаций M (1 ≤ M ≤ N ⋅ (N - 1) / 2), стартовая вселенная S (1 ≤ S ≤ N) и конечная вселенная F (1 ≤ F ≤ N).
 Далее в M строках дается описание телепортаций в формате Si, Fi, Pi, где Si и Fi — номера вселенных, которые связывает i-я телепортация (телепорты двунаправленные, 1 ≤ Si, Fi ≤ N), Pi — вероятность быть избитым на i-й телепортации в процентах (0 ≤ Pi ≤ 100).
*/

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

class graph {
public:
    explicit graph(size_t V) {
        edges.resize(V);
    }

    void add(size_t from, size_t to, double weight) {
        edges[from].push_back(make_pair(to, weight));
        edges[to].push_back(make_pair(from, weight));
    }

    double getMinDist(size_t from, size_t to) const {
        vector<double> distances;
        distances.resize(edges.size(), 0);
        distances[from] = 1;

        for (auto counter = 0; counter < edges.size(); ++counter)
            for (auto i = 0; i < edges.size(); ++i)
                for (const auto& edge : edges[i]) // i = from, edge.first = to, edge.second = probability
                    if (distances[edge.first] < distances[i] * edge.second)
                        distances[edge.first] = distances[i] * edge.second;

        return 1 - distances[to];
    }

private:
    vector<vector<pair<size_t, double>>> edges;
};

int main() {
    size_t N, M, S, F;
    cin >> N >> M >> S >> F;
    graph G(N);
    for (auto i = 0; i < M; ++i) {
        size_t from, to, cost;
        cin >> from >> to >> cost;
        G.add(from - 1, to - 1, 1 - cost / 100.0);
    }
    cout << G.getMinDist(S - 1, F - 1) << setprecision(6);
    return 0;
}