/*
ООО "Новые Железные Дороги" поручило вам разработатьсистему бронирования билетов
на новый маршрут поезда дальнего следования. Маршрут проходит через N станций,
занумерованных от 0 до N-1. Вместимость поезда ограничена.
В систему бронирования последовательно приходят запросы от пассажиров
с указанием номера начальной и конечной станции, а также количество билетов,
которые пассажир хочет приобрести.Требуемая скорость обработки каждого запроса - O(log(N)).
*/

#include <iostream>
#include <vector>
using namespace std;

class SegmentTree {
public:
    SegmentTree(const vector<size_t> &array) {
        len = array.size();
        tree.resize(4 * len), adds.resize(4 * len, 0);
        build(array, 1, 0, len - 1);
    }

    void add(size_t num, size_t start, size_t end) {
        stuff_add(1, 0, len - 1, start, end, num);
    }

    size_t maximum(size_t start, size_t end) {
        return stuff_max(1, 0, len - 1, start, end);
    }

private:
    vector<size_t> tree, adds;
    size_t len;

    void push(size_t vertex, size_t vl, size_t vr) {
        if (vl != vr) {
            adds[2 * vertex] += adds[vertex];
            adds[2 * vertex + 1] += adds[vertex];
        }
        tree[vertex] += adds[vertex];
        adds[vertex] = 0;
    }

    void build(const vector<size_t> &array, size_t vertex, size_t diap_left, size_t diap_right) {
        if (diap_left == diap_right) tree[vertex] = array[diap_left];
        else {
            auto middle = (diap_left + diap_right) >> 1;
            build(array, 2 * vertex, diap_left, middle);
            build(array, 2 * vertex + 1, middle + 1, diap_right);
            tree[vertex] = max(tree[2 * vertex], tree[2 * vertex + 1]);
        }
    }

    size_t stuff_max(size_t vertex, size_t diap_left, size_t diap_right, size_t lb, size_t rb) {
        push(vertex, diap_left, diap_right);
        if (rb < diap_left || diap_right < lb) return 0;
        if (lb <= diap_left && diap_right <= rb) return tree[vertex];
        auto middle = (diap_left + diap_right) >> 1;
        int left_child = stuff_max(2 * vertex, diap_left, middle, lb, rb);
        int right_child = stuff_max(2 * vertex + 1, middle + 1, diap_right, lb, rb);
        return max(left_child, right_child);
    }

    void stuff_add(size_t vertex, size_t diap_left, size_t diap_right, size_t lb, size_t rb, size_t num) {
        push(vertex, diap_left, diap_right);
        if (diap_right >= lb && rb >= diap_left) {
            if (lb <= diap_left && diap_right <= rb) {
                adds[vertex] += num;
                push(vertex, diap_left, diap_right);
            }
            else {
                auto middle = (diap_left + diap_right) >> 1;
                stuff_add(2 * vertex, diap_left, middle, lb, rb, num);
                stuff_add(2 * vertex + 1, middle + 1, diap_right, lb, rb, num);
                tree[vertex] = max(tree[2 * vertex], tree[2 * vertex + 1]);
            }
        }
    }
};

int main() {
    size_t N, limit, tasks;
    cin >> N;
    vector<size_t> passengers(N - 1);
    for (auto &passenger : passengers) cin >> passenger;
    SegmentTree controller = passengers;
    cin >> limit >> tasks;
    for (size_t task = 0; task < tasks; ++task) {
        size_t from, to, amount;
        cin >> from >> to >> amount;
        if (controller.maximum(from, to - 1) + amount <= limit)
            controller.add(amount, from, to - 1);
        else cout << task << " ";
    }
    return 0;
}