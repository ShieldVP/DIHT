#include <cstdio>
#include <cmath>
#include <algorithm>

struct Point {
    int x = 0, y = 0, z = 0;

    Point() = default;
};

std::istream& operator>>(std::istream& stream, Point& A) {
    stream >> A.x >> A.y >> A.z;
    return stream;
}

struct Vector {
    int a, b, c;

    Vector() = default;
    Vector(int a, int b, int c) :
    a(a),
    b(b),
    c(c)
    {};
    Vector(const Point& start, const Point& end) {
        a = end.x - start.x;
        b = end.y - start.y;
        c = end.z - start.z;
    }

    Vector& operator -=(const Vector& other) {
        a -= other.a; b -= other.b; c -= other.c;
        return *this;
    }

    bool isCollinear(const Vector& other) const {
        return (a * other.b == other.a * b) && (a * other.c == other.a * c);
    }
};

Vector operator -(const Vector& A, const Vector& B) {
    auto C = A;
    return C -= B;
}

int dot(const Vector& a, const Vector& b) {
    return a.a * b.a + a.b * b.b + a.c * b.c;
}

Vector cross(const Vector& a, const Vector& b) {
    Vector res{};
    res.a = a.b * b.c - a.c * b.b;
    res.b = a.c * b.a - a.a * b.c;
    res.c = a.a * b.b - a.b * b.a;
    return res;
}

double distanceBetweenLines(const Point& P1, const Point& P2, const Point& P3, const Point& P4) {
    Vector dir1(P1, P2), dir2(P3, P4), shift(P1, P3);

    int64_t A = dot(dir1, dir1), C = dot(dir2, dir2), D(dot(shift, shift));
    int64_t B = 2 * dot(dir1, dir2), E = 2 * dot(dir1, shift), F = 2 * dot(dir2, shift);

    int64_t deltaGeneral = 4 * A * C - B * B, delta1 = B * F - 2 * E * C, delta2 = E * B - 2 * A * F;

    if (deltaGeneral == 0) {  // => dir1 || dir2
        Vector V13 = shift, V14(P1, P4), V23(P2, P3), V24(P2, P4);

        double minDist = std::min(dot(V13, V13), dot(V14, V14));
        minDist = std::min(minDist, (double)dot(V23, V23));
        minDist = std::min(minDist, (double)dot(V24, V24));

        if (!shift.isCollinear(dir1) && dot(V13, dir1) > 0 != dot(V23, dir1) > 0) {
            auto crossV1 = cross(dir1, V13);
            minDist = std::min(minDist, dot(crossV1, crossV1) / (double)A);
        }

        return std::sqrt(minDist);
    }

    delta1 = std::max(delta1, -deltaGeneral), delta1 = std::min(delta1, 0l);
    delta2 = std::max(delta2, 0l), delta2 = std::min(delta2, deltaGeneral);
    double t1 = (double)delta1 / deltaGeneral, t2 = (double)delta2 / deltaGeneral;
    double undersqrt = A * t1 * t1 + B * t1 * t2 + C * t2 * t2 + D + E * t1 + F * t2;

    if (delta1 == -deltaGeneral) {  // t1 == -1
        double t02 = (B - F) / (2.0 * C);
        t02 = std::max(t02, 0.0); t02 = std::min(t02, 1.0);
        undersqrt = std::min(undersqrt, A * t1 * t1 + B * t1 * t02 + C * t02 * t02 + D + E * t1 + F * t02);
    }
    else if (delta1 == 0) {  // t1 == 0
        double t02 = -F / (2.0 * C);
        t02 = std::max(t02, 0.0); t02 = std::min(t02, 1.0);
        undersqrt = std::min(undersqrt, A * t1 * t1 + B * t1 * t02 + C * t02 * t02 + D + E * t1 + F * t02);
    }

    if (delta2 == deltaGeneral) {  // t2 == 1
        double t01 = -(B + E) / (2.0 * A);
        t01 = std::max(t01, -1.0); t01 = std::min(t01, 0.0);
        undersqrt = std::min(undersqrt, A * t01 * t01 + B * t01 * t2 + C * t2 * t2 + D + E * t01 + F * t2);
    }
    else if (delta2 == 0) {  // t2 == 0
        double t01 = -E / (2.0 * A);
        t01 = std::max(t01, -1.0); t01 = std::min(t01, 0.0);
        undersqrt = std::min(undersqrt, A * t01 * t01 + B * t01 * t2 + C * t2 * t2 + D + E * t01 + F * t2);
    }

    return std::sqrt(undersqrt);
}

int main() {
    Point A, B, C, D;
    std::cin >> A >> B >> C >> D;
    std::cout << std::setprecision(6) << distanceBetweenLines(A, B, C, D);
    return 0;
}