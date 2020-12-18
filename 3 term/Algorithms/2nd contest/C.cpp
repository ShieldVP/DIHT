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

#include <cstdio>
#include <vector>
#include <stack>
#include <map>
#include <limits>
#include <algorithm>
#include <cmath>
using namespace std;

struct Point {
    long long x = 0, y = 0, z = 0;

    Point() = default;
};

bool operator==(const Point& A, const Point& B) {
    return A.x == B.x && A.y == B.y && A.z == B.z;
}

bool operator<(const Point& A, const Point& B) {
    return (A.z < B.z) || (A.z == B.z && A.x < B.x) || (A.z == B.z && A.x == B.x && A.y < B.y);
}

struct Vector {
    long long x = 0, y = 0, z = 0;
    double norma = -1;

    Vector() = default;
    Vector(long long _x, long long _y, long long _z) : x(_x), y(_y), z(_z) {}
    Vector(const Point& A, const Point& B) : x(B.x - A.x), y(B.y - A.y), z(B.z - A.z) {}

    double len() {
        if (norma >= 0) return norma;
        return norma = std::sqrt(x * x + y * y + z * z);
    }
};

Vector crossProduct(const Vector& A, const Vector& B) {
    return Vector(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}

long long dotProduct(const Vector& A, const Vector& B) {
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

bool relativeSignFlat(const Vector& normal, const Point& in, const Point& A) {
    return dotProduct(normal, Vector(A, in)) > 0;
}

struct Edge {
    Point A, B, C;

    Edge(const Point& _A, const Point& _B, const Point& _C) : A(_A), B(_B), C(_C) {}

    bool flatContains(const Point& D) const {
        return A == D || B == D || C == D;
    }
};

bool operator<(const Edge& A, const Edge& B) {
    if (A.A < A.B) {
        if (B.A < B.B) return A.A == B.A ? A.B < B.B : A.A < B.A;
        else return A.A == B.B ? A.B < B.A : A.A < B.B;
    }
    else {
        if (B.A < B.B) return A.B == B.A ? A.A < B.B : A.B < B.A;
        else return A.B == B.B ? A.A < B.A : A.B < B.B;
    }
}

bool operator==(const Edge& A, const Edge& B) {
    return (A.A == B.A && A.B == B.B) || (A.B == B.A && A.A == B.B);
}

struct Face {
    Point A, B, C;
    Vector normal;
    long long D;

    Face(const Point& _A, const Point& _B, const Point& _C) : A(_A), B(_B), C(_C) {}
};

double flatDistance(Face& face, const Point& A) {
    return abs(dotProduct(face.normal, Vector(A, face.A))) / face.normal.len();
}

class PointsSet {
public:
    PointsSet(std::vector<Point>  arr) : array(std::move(arr)) {}

    void computeConvexHull() {
        // searching for the first edge in convex hull
        auto firstPoint = array.front(), secondPoint = firstPoint;
        for (auto& point : array)
            if (point < firstPoint)
                firstPoint = point;
        auto minTG = std::numeric_limits<double>::infinity();
        for (auto& point : array) {
            if (point.x == firstPoint.x && point.y == firstPoint.y) continue;
            auto h = point.z - firstPoint.z;
            auto l = std::sqrt(pow(point.x - firstPoint.x, 2) + pow(point.y - firstPoint.y, 2));
            auto newTG = h / l;
            if (newTG < minTG) {
                secondPoint = point;
                minTG = newTG;
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

        std::stack<Edge> edges;
        std::map<Edge, size_t> used;
        edges.emplace(firstPoint, secondPoint, thirdPoint);
        //++used[edges.top()];
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

            hull.emplace_back(edge.A, edge.B, array[newPoint]);
            hull.back().normal = newNormal;
            hull.back().D = -dotProduct(newNormal, Vector(edge.A.x, edge.A.y, edge.A.z));
            edges.emplace(edge.A, array[newPoint], edge.B);
            ++used[edges.top()];
            edges.emplace(edge.B, array[newPoint], edge.A);
            ++used[edges.top()];
        }
    }

    double distanceToHull(const Point& A) {
        auto minDist = std::numeric_limits<double>::infinity();
        for (auto& face : hull)
            minDist = std::min(flatDistance(face, A), minDist);
        return minDist;
    }

    void print() const {
        for (auto& face : hull)
            printf("%lf %lf %lf %lf\n", face.normal.x / (double)face.D, face.normal.y / (double)face.D, face.normal.z / (double)face.D, 1.0);
    }

private:
    std::vector<Point> array;
    std::vector<Face> hull;
};

int main() {
    size_t N, M;
    scanf("%lu", &N);
    std::vector<Point> arr(N);
    for (auto& point : arr)
        scanf("%lld %lld %lld", &point.x, &point.y, &point.z);

    PointsSet set = std::move(arr);
    set.computeConvexHull();

    scanf("%lu", &M);
    Point point;
    for (size_t i = 0; i < M; ++i) {
        scanf("%lld %lld %lld", &point.x, &point.y, &point.z);
        printf("%0.4lf\n", set.distanceToHull(point));
    }
    return 0;
}