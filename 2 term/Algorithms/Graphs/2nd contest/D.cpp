/*
 Рик отправляет Морти в путешествие по N вселенным. У него есть список всех существующих однонаправленных телепортов.
 Чтобы Морти не потерялся, Рику необходимо узнать, между какими вселенными существуют пути, а между какими нет.
 Дана матрица смежности, построить матрицу смежности её транзитивного замыкания.
*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define ul uint64_t

class graph {
public:
    explicit graph(size_t V) {
        edges.resize(V);
        maskLength.resize(V);
    }

    void add(size_t from, const string& sequence) {
        edges[from].push_back(transform(sequence));
        maskLength[from].push_back(sequence.size());
    }

    void makeTransitive() {
        for (auto viaWho = 0; viaWho < edges.size(); ++viaWho)
            for (auto from = 0; from < edges.size(); ++from)
                // if exists path from 'from' to 'viaWho'
                if (edges[from][viaWho / 64] >> (maskLength[from][viaWho / 64] - 1 - viaWho % 64) & 1)
                    for (auto to = 0; to < edges[from].size(); ++to)
                        edges[from][to] |= edges[viaWho][to];
    }

    void printEdges() {
        makeTransitive();
        for (auto i = 0; i < edges.size(); ++i) {
            for (auto j = 0; j < edges[i].size(); ++j)
                cout << detransform(edges[i][j], maskLength[i][j]);
            cout << endl;
        }
    }

private:
    vector<vector<ul>> edges;
    vector<vector<size_t>> maskLength;

    ul transform(const string& sequence) {
        ul result = 0;
        for (char i : sequence) {
            result <<= 1;
            if (i == '1') ++result;
        }
        return result;
    }

    string detransform(ul sequence, size_t len) {
        string result;
        while (sequence != 0) {
            result += (sequence & 1) == 1 ? '1' : '0';
            sequence >>= 1;
        }
        for (auto i = 0; i < result.size() / 2; ++i)
            swap(result[i], result[result.size() - 1 - i]);
        result = string(len - result.size(), '0') + result;
        return result;
    }
};

int main() {
    size_t N;
    cin >> N;
    graph G(N);
    for (auto i = 0; i < N; ++i) {
        string input;
        cin >> input;
        for (auto j = 0; j < input.size(); j += 64)
            if (j + 64 < input.size()) G.add(i, input.substr(j, 64));
            else G.add(i, input.substr(j));
    }
    G.printEdges();
    return 0;
}