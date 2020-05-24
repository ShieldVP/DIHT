/*
Дано число N и последовательность из N целых чисел.
Найти вторую порядковую статистику на заданных диапазонах.
Для решения задачи используйте структуру данных Sparse Table.
Требуемое время обработки каждого диапазона O(1). 
Время подготовки структуры данных O(n*log(n)).
*/

#include <iostream>
#include <vector>
using namespace std;

class RMQ {
public:
    RMQ(const vector<long long> &origin) {
        orig = origin;

        table.resize(1);
        log.push_back(0); // log2(1)
        for (size_t i = 0; i < orig.size(); ++i) {
            table[0].push_back(i); // First line
            log.push_back(log[i >> 1] + 1); // Logs
        }

        // Some dynamics
        for (size_t line = 1; (1 << line) <= orig.size(); ++line) {
            table.resize(table.size() + 1);
            for (size_t column = 0; column + (1 << line) <= orig.size(); ++column) {
                auto current = table[line - 1][column];
                auto shifted = table[line - 1][column + (1 << (line - 1))];
                table[line].push_back(compare(current, shifted));
            }
        }
    }

    long long rs2q(size_t start, size_t end) const {
        auto _min = rmq(start, end);
        if (start == _min) return orig[rmq(start + 1, end)];
        else if (end == _min) return orig[rmq(start, end - 1)];
        auto lmin = rmq(start, _min - 1);
        auto rmin = rmq(_min + 1, end);
        return min(orig[lmin], orig[rmin]);
    }

private:
    vector<vector<long long>> table;
    vector<long long> orig, log;

    size_t compare(size_t a, size_t b) const {
        return orig[a] < orig[b] ? a : b;
    }

    size_t rmq(size_t start, size_t end) const {
        size_t line = log[end - start], gap = (1 << line) - 1;
        return compare(table[line][start], table[line][end - gap]);
    }
};

int main() {
    size_t N, M;
    cin >> N >> M;
    vector<long long> origin(N);
    for (auto &num : origin) cin >> num;
    RMQ magic(origin);
    for (size_t i = 0; i < M; ++i) {
        size_t start, end;
        cin >> start >> end;
        cout << magic.rs2q(start - 1, end - 1) << endl;
    }
    return 0;
}