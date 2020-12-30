#include <cstdio>
#include <vector>
#include <cmath>
#include <cinttypes>
#include <algorithm>

struct Point {
    int64_t x = 0, y = 0;

    Point() = default;

    Point(int64_t x, int64_t y) :
    x(x),
    y(y)
    {}
};

bool operator==(const Point& A, const Point& B) {
    return A.x == B.x && A.y == B.y;
}

bool operator!=(const Point& A, const Point& B) {
    return !(A == B);
}

std::istream& operator>>(std::istream& stream, Point& A) {
    stream >> A.x >> A.y >> A.z;
    return stream;
}

class PointsSet {
public:
    explicit PointsSet(size_t size);

    void buildConvexHull();

    double convexHullPerimeter() const;

private:
    double dist(const Point& A, const Point& B) const;

    int64_t crossProductZ(const Point& A, const Point& B, const Point& C) const;

    std::vector<Point> points;
};

explicit PointsSet::PointsSet(size_t size) {
    points.resize(size);
    for (size_t i = 0; i < size; ++i)
        std::cin >> points[i];
}

void PointsSet::buildConvexHull() {
    size_t rightBottom = 0;
    for (size_t i = 1; i < points.size(); ++i)
        if (points[i].y < points[rightBottom].y || (points[i].y == points[rightBottom].y && points[i].x > points[rightBottom].x))
            rightBottom = i;
    std::swap(points[0], points[rightBottom]);
    std::sort(points.begin() + 1, points.end(), [&](const Point& A, const Point& B) {
        if (B.x == A.x && B.y == A.y) return true;
        auto cross = crossProductZ(A, points.front(), B);
        if (cross == 0) return dist(points.front(), A) > dist(points.front(), B);
        return cross > 0;
    });
    auto last = std::unique(points.begin(), points.end());
    points.erase(last, points.end());
    size_t size = 1;
    for (size_t i = 2; i < points.size(); ++i) {
        while (size > 0 && crossProductZ(points[size - 1], points[size], points[i]) > 0)
            --size;
        std::swap(points[i], points[++size]);
    }
}

double PointsSet::convexHullPerimeter() const {
    double perimeter = dist(points[0], points[size]);
    for (size_t i = 0; i < size; ++i)
        perimeter += dist(points[i], points[i + 1]);
    return perimeter;
}

double PointsSet::dist(const Point& A, const Point& B) const {
    return std::sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

int64_t PointsSet::crossProductZ(const Point& A, const Point& B, const Point& C) const {
    return (A.x - B.x) * (C.y - B.y) - (C.x - B.x) * (A.y - B.y);  // BA x BC
}

int main() {
    size_t N;
    std::cin >> N
    PointsSet points(N);
    points.buildConvexHull();
    std::cout << points.convexHullPerimeter();
    return 0;
}