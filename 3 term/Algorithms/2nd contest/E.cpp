/*
Арсений открыл эзотерический салон в свои снах на планете Ка-Пэкс. У Арсения всё будет хорошо. А вот у его клиентов — не факт. 
Если хотя бы какие-нибудь их палочки в гадании "Мокусо Дзэй" пересеклись, то день точно сложится удачно. 
А если нет — то стоит ждать беды. Чтобы точнее сказать, что конкретно пойдет хорошо в этот день, нужно знать, какие именно палочки пересекаются.
Помогите Арсению узнать, как пройдет день у его клиентов. 
Палочка представляется как отрезок прямой. В первой строке входного файла записано число N (1 ≤ N ≤ 125   000) — количество палочек в гадании.
Следующие N строк содержат описания палочек: (i + 1)-я строка содержит координаты концов i-й палочки — целые числа x1, y1, x2, y2
(-10   000 ≤ x1, y1, x2, y2 ≤ 10   000).
В первой строке выходного файла выведите слово "YES", если день сложится удачно.
Вторая строка должна содержать числа i и j — номера палочек, которые пересекаются. Если день пройдет плохо, выведите в единственной строке выходного файла "NO". 
*/

#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

struct Point {
    int x, y;

    Point() {
        std::cin >> x >> y;
    }
};

struct LineSegment {
    Point start, end;
    size_t id;

    LineSegment(const Point& s, const Point& e) : start(s), end(e) {}

    double computeY(int x) const {
        if (x == start.x) return start.y;
        if (x == end.x) return end.y;
        if (start.x == end.x) return start.y;
        return start.y + (end.y - start.y) * (x - start.x) / (double)(end.x - start.x);
    }
};

struct Event {
    size_t id;
    int x;
    bool isLeft;

    Event (int x, int isLeft, size_t id) : x(x), isLeft(isLeft), id(id) {}
};

bool isBasicallyIntersect(const LineSegment& A, const LineSegment& B) {
    auto up_A = std::max(A.start.y, A.end.y), up_B = std::max(B.start.y, B.end.y),
            down_A = std::min(A.start.y, A.end.y), down_B = std::min(B.start.y, B.end.y);
    return std::max(A.start.x, B.start.x) <= std::min(A.end.x, B.end.x) &&
           std::max(down_A, down_B) <= std::min(up_A, up_B);
}

int64_t crossProductZ(const Point& A, const Point& B, const Point& C) {
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);  // AB x AC
}

bool isIntersect(const LineSegment& A, const LineSegment& B) {
    return isBasicallyIntersect(A, B) &&
           crossProductZ(A.start, A.end, B.start) * crossProductZ(A.start, A.end, B.end) <= 0 &&
           crossProductZ(B.start, B.end, A.start) * crossProductZ(B.start, B.end, A.end) <= 0;
}

bool operator==(const Point& A, const Point& B) {
    return A.x == B.x && A.y == B.y;
}

bool operator<(const LineSegment& A, const LineSegment& B) {
    auto testX = std::max(A.start.x, B.start.x);
    if (A.start == B.start) testX = std::min(A.end.x, B.end.x);
    return A.computeY(testX) < B.computeY(testX);
}

class SweepingLine {
public:
    explicit SweepingLine(size_t setSize) {
        pointers.resize(setSize);
    }

    void insert(const LineSegment lineSeg, size_t lineSegmentID) {
        pointers[lineSegmentID] = currentLines.insert(lineSeg).first;
    }

    void erase(size_t lineSegmentID) {
        currentLines.erase(pointers[lineSegmentID]);
    }

    bool isReal(std::set<LineSegment>::iterator iter) const {
        return iter != currentLines.end();
    }

    std::set<LineSegment>::iterator findNext(const LineSegment& lineSeg) const {
        return currentLines.lower_bound(lineSeg);
    }

    std::set<LineSegment>::iterator next(std::set<LineSegment>::iterator it) const {
        return it == currentLines.end() ? currentLines.end() : ++it;
    }

    std::set<LineSegment>::iterator prev(std::set<LineSegment>::iterator it) const {
        return it == currentLines.begin() ? currentLines.end() : --it;
    }

    std::set<LineSegment>::iterator next(size_t lineSegmentID) const {
        return next(pointers[lineSegmentID]);
    }

    std::set<LineSegment>::iterator prev(size_t lineSegmentID) const {
        return prev(pointers[lineSegmentID]);
    }

private:
    std::set<LineSegment> currentLines;
    std::vector<std::set<LineSegment>::iterator> pointers;
};

std::pair<int, int> findIntersection(const std::vector<LineSegment>& givenSet) {
    std::vector<Event> events;
    for (size_t i = 0; i < givenSet.size(); ++i) {
        events.emplace_back(givenSet[i].start.x, true, i);
        events.emplace_back(givenSet[i].end.x, false, i);
    }
    sort(events.begin(), events.end(), [](const Event& A, const Event& B) {
        return A.x == B.x ? A.isLeft > B.isLeft : A.x < B.x;
    });

    SweepingLine line(givenSet.size());
    for (const auto& event : events) {
        if (event.isLeft) {
            auto next = line.findNext(givenSet[event.id]), prev = line.prev(next);
            if (line.isReal(next) && isIntersect(*next, givenSet[event.id]))
                return std::make_pair(next->id, event.id);
            if (line.isReal(prev) && isIntersect(*prev, givenSet[event.id]))
                return std::make_pair(prev->id, event.id);
            line.insert(givenSet[event.id], event.id);
        }
        else {
            auto next = line.next(event.id), prev = line.prev(event.id);
            if (line.isReal(next) && line.isReal(prev) && isIntersect(*next, *prev))
                return std::make_pair(prev->id, next->id);
            line.erase(event.id);
        }
    }

    return std::make_pair(-1, -1);
}

int main() {
    size_t N;
    std::cin >> N;
    std::vector<LineSegment> set;
    for (size_t i = 0; i < N; ++i) {
        Point A, B;
        if (A.x < B.x) set.emplace_back(A, B);
        else set.emplace_back(B, A);
        set[i].id = i;
    }
    auto result = findIntersection(set);
    if (result.first < 0) std::cout << "NO";
    else std::cout << "YES" << std::endl << std::min(result.first, result.second) + 1 << ' ' << std::max(result.first, result.second) + 1;
    return 0;
}