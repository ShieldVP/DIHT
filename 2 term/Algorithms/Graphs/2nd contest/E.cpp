/*
Рик решил на день почувствовать себя бизнесменом!
В городе есть несколько обменников валюты. В рамках данной задачи считаем, что каждый обменник специализируется только на двух валютах и может производить операции только с ними. Возможно, существуют обменники, специализирующиеся на одинаковых парах валют. В каждом обменнике — свой обменный курс: курс обмена A на B — это количество единиц валюты B, выдаваемое за 1 единицу валюты A. Также в каждом обменнике есть комиссия — сумма, которую вы должны заплатить, чтобы производить операцию. Комиссия взимается в той валюте, которую меняет клиент.
Например, если вы хотите поменять 100 долларов США на русские рубли в обменнике, где курс обмена равен 29.75, а комиссия равна 0.39, вы получите (100 - 0.39) ⋅ 29.75 = 2963.3975 рублей (эх, были времена).
Вы точно знаете, что в городе используется всего N валют. Пронумеруем их числами 1, 2, …, N. Тогда каждый обменник представляют 6 чисел: целые A и B — номера обмениваемых валют, а также вещественные RAB, CAB, RBA и CBA — обменные курсы и комиссии при переводе из A в B и из B в A соответственно.
Рик обладает некоторой суммой в валюте S. Он задаётся вопросом, можно ли, после нескольких операций обмена увеличить свой капитал. Конечно, он хочет, чтобы в конце его деньги вновь были в валюте S. Помогите ему ответить на его вопрос. Рик должен всегда должен иметь неотрицательную сумму денег.
*/

#include <iostream>
#include <vector>
#include <limits>
using namespace std;

struct path {
    path(size_t f, size_t t, double w, double b) {
        from = f;
        to = t;
        weight = w;
        bias = b;
    }

    size_t from, to;
    double weight, bias;
};

class graph {
public:
    explicit graph(size_t V) {
        vertices = V;
    }

    void add(size_t from, size_t to, double weight, double bias) {
        edges.emplace_back(from, to, weight, bias);
    }

    bool search(size_t start, double init_cash) const {
        double inf = numeric_limits<double>::infinity();
        vector<double> cash(vertices, -inf);
        cash[start] = init_cash;
        bool got_cycle = false;
        for (size_t it = 0; it < vertices; ++it) {
            got_cycle = false;
            for (auto &edge : edges)
                if (cash[edge.from] > -inf) {
                    auto new_value = (cash[edge.from] - edge.bias) * edge.weight;
                    if (cash[edge.to] < new_value && new_value > 0) {
                        cash[edge.to] = new_value;
                        got_cycle = true;
                    }
                }
        }
        return got_cycle;
    }

private:
    size_t vertices;
    vector<path> edges;
};

int main() {
    size_t N, M, S;
    double V;
    cin >> N >> M >> S >> V;
    graph G(N);
    for (auto i = 0; i < M; ++i) {
        size_t A, B;
        double RAB, CAB, RBA, CBA;
        cin >> A >> B >> RAB >> CAB >> RBA >> CBA;
        G.add(A - 1, B - 1, RAB, CAB);
        G.add(B - 1, A - 1, RBA, CBA);
    }
    cout << (G.search(S - 1, V) ? "YES" : "NO");
    return 0;
}