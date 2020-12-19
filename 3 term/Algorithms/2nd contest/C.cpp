/*
 Арсений и Илья затеяли странную игру. Сначала Илья придумывает, какие бы точки он хотел видеть внутри воображаемого яблока.
 А затем он спрашивает Арсения, что если бы червячок находился в некоторой точке внутри такого минимально возможного яблока,
 то сколько бы ему пришлось бы скушать мякоти, чтобы выбраться на поверхность.
 На первой строке число N (4 ≤ N ≤ 1,000). Далее N строк содержат координаты точек для яблока. 
 Никакие четыре из них не лежат в одной плоскости. Далее число запросов q (1 ≤ Q ≤ 105) и Q строк, содержащих точки-запросы. 
 Все точки запросы гарантированно внутри минимально возможного воображаемого яблока. 
 Все координаты целые, по модулю не более 10 000. 
 Для каждого запроса выведите число с четырьмя знаками после запятой – расстояния до поверхности минимально возможного воображаемого яблока.
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <stack>
#include <map>
#include <limits>
#include <algorithm>
#include <cmath>
#include <tuple>
using namespace std;

struct Point {
    long long x = 0, y = 0, z = 0;

    Point() = default;
};

bool operator==(const Point& A, const Point& B) {
    return std::tie(A.x, A.y, A.z) == std::tie(B.x, B.y, B.z);
}

bool operator<(const Point& A, const Point& B) {
    return std::tie(A.z, A.x, A.y) < std::tie(B.z, B.x, B.y);
}

std::istream& operator>>(std::istream& stream, Point& A) {
    stream >> A.x >> A.y >> A.z;
    return stream;
}

struct Vector {
    long long x = 0, y = 0, z = 0;
    double norma = -1;

    Vector() = default;
    Vector(long long x, long long y, long long z) : 
    x(x), 
    y(y), 
    z(z) 
    {}
    Vector(const Point& A, const Point& B) : 
    x(B.x - A.x), 
    y(B.y - A.y), 
    z(B.z - A.z) 
    {}

    double len() {
        if (norma >= 0) return norma;
        return norma = std::sqrt(x * x + y * y + z * z);
    }
};

static Vector crossProduct(const Vector& A, const Vector& B) {
    return Vector(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}

static long long dotProduct(const Vector& A, const Vector& B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

static bool relativeSignFlat(const Vector& normal, const Point& in, const Point& A) {
    return dotProduct(normal, Vector(A, in)) > 0;
}

struct Edge {
    Point A, B, C;

    Edge(const Point& A, const Point& B, const Point& C) : 
    A(A), 
    B(B), 
    C(C) 
    {}

    bool flatContains(const Point& D) const {
        return A == D || B == D || C == D;
    }
};

bool operator<(const Edge& first, const Edge& second) {
    if (first.A < first.B) return second.A < second.B ? 
               std::tie(first.A, first.B) < std::tie(second.A, second.B) : 
               std::tie(first.A, first.B) < std::tie(second.B, second.A);
    else return second.A < second.B ? 
               std::tie(first.B, first.A) < std::tie(second.A, second.B) : 
               std::tie(first.B, first.A) < std::tie(second.B, second.A);
}

bool operator==(const Edge& first, const Edge& second) {
    return (first.A == second.A && first.B == second.B) ||
           (first.B == second.A && first.A == second.B);
}

struct Face {
    Point A, B, C;
    Vector normal;

    Face(const Point& A, const Point& B, const Point& C, const Vector& normal) : 
    A(A), 
    B(B), 
    C(C),
    normal(normal)
    {}
};

static double flatDistance(Face& face, const Point& point) {
    return abs(dotProduct(face.normal, Vector(point, face.A))) / face.normal.len();
}

class PointsSet {
public:
    PointsSet(std::vector<Point>  arr) : array(std::move(arr)) {}

    void computeConvexHull() {
        std::stack<Edge> edges;
        std::map<Edge, size_t> used;
        edges.emplace(getFirstEdge());
        while (!edges.empty()) {
            auto edge = edges.top();
            edges.pop();
            if (++used[edge] > 2) continue;

            size_t newPoint = 0;
            while (edge.flatContains(array[newPoint])) ++newPoint;

            auto newNormal = crossProduct(Vector(array[newPoint], edge.A), Vector(array[newPoint], edge.B));
            auto rightSign = relativeSignFlat(newNormal, array[newPoint], edge.C);

            for (size_t point = newPoint + 1; point < array.size(); ++point) {
                if (edge.flatContains(array[point])) continue;
                if (relativeSignFlat(newNormal, edge.A, array[point]) != rightSign) {
                    newPoint = point;
                    newNormal = crossProduct(Vector(array[newPoint], edge.A), Vector(array[newPoint], edge.B));
                    rightSign = relativeSignFlat(newNormal, array[newPoint], edge.C);
                }
            }

            hull.emplace_back(edge.A, edge.B, array[newPoint], newNormal);
            edges.emplace(edge.A, array[newPoint], edge.B);
            ++used[edges.top()];
            edges.emplace(edge.B, array[newPoint], edge.A);
            ++used[edges.top()];
        }
    }

    double distanceToHull(const Point& point) {
        auto minDist = std::numeric_limits<double>::infinity();
        for (auto& face : hull)
            minDist = std::min(flatDistance(face, point), minDist);
        return minDist;
    }

private:
    std::vector<Point> array;
    std::vector<Face> hull;

    Edge getFirstEdge() const {
        auto firstPoint = array.front(), secondPoint = firstPoint;
        for (auto& point : array)
            if (point < firstPoint)
                firstPoint = point;
        auto minTan = std::numeric_limits<double>::infinity();
        for (auto& point : array) {
            if (point.x == firstPoint.x && point.y == firstPoint.y) continue;
            auto h = point.z - firstPoint.z;
            auto l = std::sqrt(pow(point.x - firstPoint.x, 2) + pow(point.y - firstPoint.y, 2));
            auto newTan = h / l;
            if (newTan < minTan) {
                secondPoint = point;
                minTan = newTan;
            }
        }
        size_t j = 0;
        while (array[j] == firstPoint || array[j] == secondPoint) ++j;
        auto thirdPoint = array[j++];
        while (array[j] == firstPoint || array[j] == secondPoint) ++j;
        auto basePoint = array[j];
        auto firstNormal = crossProduct(Vector(thirdPoint, firstPoint), Vector(thirdPoint, secondPoint));
        auto firstSign = relativeSignFlat(firstNormal, firstPoint, basePoint);
        for (size_t point = j + 1; point < array.size(); ++point) {
            if (array[point] == firstPoint || array[point] == secondPoint) continue;
            if (relativeSignFlat(firstNormal, firstPoint, array[point]) != firstSign) {
                thirdPoint = array[point];
                firstNormal = crossProduct(Vector(thirdPoint, firstPoint), Vector(thirdPoint, secondPoint));
                firstSign = relativeSignFlat(firstNormal, firstPoint, basePoint);
            }
        }
        return Edge(firstPoint, secondPoint, thirdPoint);
    }
};

int main() {
    size_t N, M;
    std::cin >> N;
    std::vector<Point> arr(N);
    for (auto& point : arr)
        std::cin >> point;

    PointsSet set = std::move(arr);
    set.computeConvexHull();

    std::cin >> M;
    Point point;
    for (size_t i = 0; i < M; ++i) {
        std::cin >> point;
        std::cout << std::setprecision(4) << set.distanceToHull(point);
    }
    return 0;
}