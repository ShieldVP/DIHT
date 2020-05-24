/*
У художника-авангардиста есть полоска разноцветного холста. 
За один раз он перекрашивает некоторый отрезок полоски в некоторый цвет. 
После каждого перекрашивания специально обученный фотограф делает снимок
части получившегося творения для музея современного искусства.
Для правильного экспонирования требуется знать яркость самого темного цвета на выбранном фотографом отрезке. 
Напишите программу для определения яркости самого темного цвета на отрезке.
Требуемая скорость определения яркости — O(log N).
Цвет задается тремя числами R, G и B (0 ≤ R, G, B ≤ 255), его яркость = R + G + B.
Цвет (R1, G1, B1) темнее цвета (R2, G2, B2), если R1 + G1 + B1 < R2 + G2 + B2.
Первая строка содержит число N –– длина отрезка (координаты [0...N-1]).
Затем следует N строк по 3 числа –– цвета отрезка.
Следующая строка содержит число K –– количество перекрашиваний и фотографирований.
Затем задано K строк, описывающих перекрашивание и фотографирование.
В каждой такой строке 7 натуральных чисел: C, D, R, G, B, E, F,
где [C, D] –– координаты перекрашиваемого отрезка, 
(R, G, B) –– цвет покраски, [E, F] –– фотографируемый отрезок.
*/

#include <iostream>
#include <vector>
using namespace std;

class SegmentTree {
public:
    SegmentTree(const vector<size_t> &array) {
        len = array.size();
        tree.resize(4 * len), adds.resize(4 * len, 0), pushed.resize(4 * len, false);
        build(array, 1, 0, len - 1);
    }

    void modify(size_t num, size_t start, size_t end) {
        stuff_modify(1, 0, len - 1, start, end, num);
    }

    size_t minimum(size_t start, size_t end) {
        return stuff_min(1, 0, len - 1, start, end);
    }

private:
    vector<size_t> tree, adds;
    vector<bool> pushed;
    size_t len, INF = 3 * 255 + 1;

    void push(size_t vertex, size_t vl, size_t vr) {
        if (pushed[vertex]) {
            if (vl != vr) {
                adds[2 * vertex] = adds[vertex];
                pushed[2 * vertex] = true;
                adds[2 * vertex + 1] = adds[vertex];
                pushed[2 * vertex + 1] = true;
            }
            tree[vertex] = adds[vertex];
            pushed[vertex] = false;
        }
    }

    void build(const vector<size_t> &array, size_t vertex, size_t diap_left, size_t diap_right) {
        if (diap_left == diap_right) tree[vertex] = array[diap_left];
        else {
            auto middle = (diap_left + diap_right) >> 1;
            build(array, 2 * vertex, diap_left, middle);
            build(array, 2 * vertex + 1, middle + 1, diap_right);
            tree[vertex] = min(tree[2 * vertex], tree[2 * vertex + 1]);
        }
    }

    size_t stuff_min(size_t vertex, size_t diap_left, size_t diap_right, size_t lb, size_t rb) {
        push(vertex, diap_left, diap_right);
        if (rb < diap_left || diap_right < lb) return INF;
        if (lb <= diap_left && diap_right <= rb) return tree[vertex];
        auto middle = (diap_left + diap_right) >> 1;
        int left_child = stuff_min(2 * vertex, diap_left, middle, lb, rb);
        int right_child = stuff_min(2 * vertex + 1, middle + 1, diap_right, lb, rb);
        return min(left_child, right_child);
    }

    void stuff_modify(size_t vertex, size_t diap_left, size_t diap_right, size_t lb, size_t rb, size_t num) {
        push(vertex, diap_left, diap_right);
        if (diap_right >= lb && rb >= diap_left) {
            if (lb <= diap_left && diap_right <= rb) {
                adds[vertex] = num;
                pushed[vertex] = true;
                push(vertex, diap_left, diap_right);
            }
            else {
                auto middle = (diap_left + diap_right) >> 1;
                stuff_modify(2 * vertex, diap_left, middle, lb, rb, num);
                stuff_modify(2 * vertex + 1, middle + 1, diap_right, lb, rb, num);
                tree[vertex] = min(tree[2 * vertex], tree[2 * vertex + 1]);
            }
        }
    }
};

int main() {
    size_t N, redraws;
    cin >> N;
    vector<size_t> segments(N);
    for (auto &segment : segments) {
        size_t R, G, B;
        cin >> R >> G >> B;
        segment = R + G + B;
    }
    SegmentTree line = segments;
    cin >> redraws;
    for (size_t redraw = 0; redraw < redraws; ++redraw) {
        size_t C, D, R, G, B, E, F;
        cin >> C >> D >> R >> G >> B >> E >> F;
        line.modify(R + G + B, C, D);
        cout << line.minimum(E, F) << " ";
    }
    return 0;
}