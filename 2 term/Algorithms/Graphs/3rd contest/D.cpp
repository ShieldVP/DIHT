/*
Шреку необходимо пробраться в башню, чтобы спасти прекрасную принцессу Фиону. 
И сейчас его путь лежит через старый полуразвалившийся мост. 
Осёл очень боится упасть с этого моста в лавовую реку под ним и отказывается идти дальше, пока мост не будет полностью починен. 
Шрек не может идти без друга и решил отремонтировать его.
Мост представляет собой поле из n × m клеток, причем некоторые клетки ещё остались целыми. 
У Шрека есть только дощечки размера 1 × 2, установка которых занимает у него a секунд, и 1 × 1, установка которых занимает b секунд. 
Ему необходимо закрыть все пустые клетки, причем только их, не накладывая дощечки друг на друга.
Определите, через какое минимальное количество секунд Шрек и Осёл смогут продолжить свой путь дальше.
1 ≤ n, m ≤ 100, |a| ≤ 1 000, |b| ≤ 1 000
*/

#include <iostream>
#include <vector>
#include <map>
using namespace std;

class graph {
public:
    explicit graph(size_t _V1, size_t _V2) {
        V1 = _V1, V2 = _V2, E = 0;
        edges.resize(V1);
    }

    void add(size_t from, size_t to, bool normal_oriented = true) {
        if (normal_oriented) edges[from].emplace_back(to);
        else edges[to].emplace_back(from);
        ++E;
    }

    size_t getPairs() const {
        if (V1 == 0 || V2 == 0) return 0;
        if (V1 == 1 || V2 == 1) return E != 0;

        vector<size_t> mt(V2, INF);
        for (size_t v = 0; v < V1; ++v) {
            vector<bool> used(V1, false);
            dfs(v, used, mt);
        }

        size_t count = 0;
        for (size_t i = 0; i < V2; ++i)
            if (mt[i] < INF) ++count;
        return count;
    }

private:
    vector<vector<size_t>> edges;
    size_t V1, V2, E;
    const size_t INF = numeric_limits<size_t>::max();

    bool dfs(size_t v, vector<bool> &used, vector<size_t> &mt) const {
        if (used[v]) return false;
        used[v] = true;
        for (auto &to : edges[v])
            if (mt[to] == INF || dfs(mt[to], used, mt)) {
                mt[to] = v;
                return true;
            }
        return false;
    }
};

class bridge {
public:
    explicit bridge(size_t _N, size_t _M) {
        N = _N, M = _M, white_broken_tiles = 0, black_broken_tiles = 0;
        tiles.resize(N, vector<bool> (M, true));
    }

    void break_tile(size_t x, size_t y) {
        tiles[x][y] = false;
        bool black = (x + y) % 2 == 0;
        tf[order_number(x, y)] = black ? black_broken_tiles++ : white_broken_tiles++;
    }

    int repair_cost(int single_tile, int double_tile) {
        auto broken_tiles = white_broken_tiles + black_broken_tiles;
        if (2 * single_tile <= double_tile) return single_tile * broken_tiles;

        graph G(black_broken_tiles, white_broken_tiles);
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j)
                if (!tiles[i][j]) {
                    bool black = (i + j) % 2 == 0;
                    size_t cur_num = tf[order_number(i, j)];
                    if (i != 0 && !tiles[i - 1][j])
                        G.add(cur_num, tf[order_number(i - 1, j)], black);
                    if (j != 0 && !tiles[i][j - 1])
                        G.add(cur_num, tf[order_number(i, j - 1)], black);
                }
        auto pairs = G.getPairs();
        return double_tile * pairs + single_tile * (broken_tiles - 2 * pairs);
    }

private:
    size_t N, M, white_broken_tiles, black_broken_tiles;
    vector<vector<bool>> tiles;
    map<size_t, size_t> tf;

    size_t order_number(size_t x, size_t y) const {
        return y * N + x;
    }
};

int main() {
    size_t n, m;
    int a, b;
    cin >> n >> m >> a >> b;
    bridge Br(n, m);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j) {
            char tmp;
            cin >> tmp;
            if (tmp == '*') Br.break_tile(i, j);
        }
    cout << Br.repair_cost(b, a);
    return 0;
}