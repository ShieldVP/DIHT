/*
Задано дерево с корнем, содержащее (1 ≤ n ≤ 100 000) вершин, пронумерованных от 0 до n-1.
Требуется ответить на m (1 ≤ m ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин.
Запросы генерируются следующим образом. Заданы числа a1, a2 и числа x, y и z.
Числа a3, ..., a2m генерируются следующим образом: ai = (x ⋅ ai-2 + y ⋅ ai-1 + z) mod n.
Первый запрос имеет вид (a1, a2). Если ответ на i-1-й запрос равен v,
то i-й запрос имеет вид ((a2i-1 + v) mod n, a2i).
Первая строка содержит два числа: n и m.
Корень дерева имеет номер 0.
Вторая строка содержит n-1 целых чисел, i-е из этих чисел равно номеру родителя вершины i.
Третья строка содержит два целых числа в диапазоне от 0 до n-1: a1 и a2.
Четвертая строка содержит три целых числа: x, y и z, эти числа неотрицательны и не превосходят 109.
*/

#include <iostream>
#include <vector>
using namespace std;

class LCA_Tree {
public:
    LCA_Tree(const vector<size_t> &ancestors) {
        V = ancestors.size() + 1, log = 1, timer = 0;
        while ((1ull << log) <= V) ++log;
        Adj.resize(V), tin.resize(V, 0), tout.resize(V, 0);
        up.resize(V, vector<size_t>(log + 1));

        for (size_t i = 0; i < V - 1; ++i)
            Adj[ancestors[i]].emplace_back(i + 1);

        dfs(0);
    }

    size_t lca(size_t first, size_t second) {
        if (ancestor(first, second)) return first;
        if (ancestor(second, first)) return second;
        for (long vertex = log; vertex >= 0; --vertex)
            if (!ancestor(up[first][vertex], second))
                first = up[first][vertex];
        return up[first][0];
    }

private:
    vector<vector<size_t>> Adj, up;
    vector<size_t> tin, tout;
    size_t V, log, timer;

    void dfs(size_t vertex, size_t parent = 0) {
        tin[vertex] = ++timer;
        up[vertex][0] = parent;
        for (size_t i = 0; i < log; ++i)
            up[vertex][i + 1] = up[up[vertex][i]][i];
        for (auto &to : Adj[vertex])
            if (to != parent) dfs(to, vertex);
        tout[vertex] = ++timer;
    }

    bool ancestor(size_t a, size_t b) const {
        return tin[a] <= tin[b] && tout[a] >= tout[b];
    }
};

int main() {
    size_t n, m, a1, a2, x, y, z, v = 0, sum = 0;
    cin >> n >> m;

    vector<size_t> ancestors(n - 1);
    for (auto &ancestor : ancestors) cin >> ancestor;
    LCA_Tree tree = ancestors;

    cin >> a1 >> a2 >> x >> y >> z;
    for (size_t i = 0; i < m; ++i) {
        v = tree.lca((a1 + v) % n, a2);
        sum += v;
        a1 = (x * a1 + y * a2 + z) % n;
        a2 = (x * a2 + y * a1 + z) % n;
    }
    cout << sum;
    return 0;
}