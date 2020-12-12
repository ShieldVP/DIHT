#include <cstdio>
#include <vector>
#include <cmath>
#include <cinttypes>
#include <algorithm>

struct Point {
    int64_t x = 0, y = 0;

    Point() {
        read();
    }

    Point(int64_t x, int64_t y) : x(x), y(y) {}

    void read() {
        int64_t _x, _y;
        scanf("%ld%ld", &x, &y);
        x = _x, y = _y;
    }
};

bool operator==(const Point& A, const Point& B) {
    return A.x == B.x && A.y == B.y;
}

bool operator!=(const Point& A, const Point& B) {
    return A.x != B.x || A.y != B.y;
}

class PointsSet {
public:
    explicit PointsSet(size_t size) {
        set.resize(size, Point(0, 0));
        for (size_t i = 0; i < size; ++i)
            set[i] = Point();
    }

    double convexHullPerimeter() {
        size_t rightBottom = 0;
        for (size_t i = 1; i < set.size(); ++i)
            if (set[i].y < set[rightBottom].y || (set[i].y == set[rightBottom].y && set[i].x > set[rightBottom].x))
                rightBottom = i;
        std::swap(set[0], set[rightBottom]);
        std::sort(set.begin() + 1, set.end(), [&](const Point& A, const Point& B) {
            if (B.x == A.x && B.y == A.y) return true;
            auto cross = crossProductZ(A, set.front(), B);
            if (cross == 0) return dist(set.front(), A) > dist(set.front(), B);
            return cross > 0;
        });
        auto last = std::unique(set.begin(), set.end());
        set.erase(last, set.end());
        size_t size = 1;
        for (size_t i = 2; i < set.size(); ++i) {
            while (size > 0 && crossProductZ(set[size - 1], set[size], set[i]) > 0)
                --size;
            std::swap(set[i], set[++size]);
        }
        double perimeter = dist(set[0], set[size]);
        for (size_t i = 0; i < size; ++i)
            perimeter += dist(set[i], set[i + 1]);
        return perimeter;
    }

private:
    double dist(const Point& A, const Point& B) const {
        return std::sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
    }

    int64_t crossProductZ(const Point& A, const Point& B, const Point& C) const {
        return (A.x - B.x) * (C.y - B.y) - (C.x - B.x) * (A.y - B.y);  // BA x BC
    }

    std::vector<Point> set;
};

int main() {
    size_t N;
    scanf("%zd", &N);
    PointsSet set(N);
    printf("%f", set.convexHullPerimeter());
    return 0;
}