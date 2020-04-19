/*
Структура Point - точка на плоскости.
 Точку можно задать двумя числами типа double.
 Должны быть открыты поля x и y.
 Точки можно сравнивать операторами == и !=.
Класс Line - прямая.
 Прямую можно задать двумя точками, можно двумя числами (угловой коэффициент и сдвиг), можно точкой и числом (угловой коэффициент).
 Линии можно сравнивать операторами == и !=.
Абстрактный класс Shape - фигура.
Класс Polygon - многоугольник. Многоугольник - частный случай фигуры.
 У многоугольника можно спросить verticesCount() - количество вершин - и std::vector<Point> getVertices - сами вершины без возможности изменения.
 Можно спросить isConvex() - выпуклый ли. Можно сконструировать многоугольник из вектора точек-вершин в порядке обхода.
 Можно сконструировать многоугольник из точек, передаваемых в качестве параметров через запятую (т.е. неуказанное число аргументов).
 Для простоты будем считать, что многоугольники с самопересечениями никогда не возникают (гарантируется, что в тестах таковые будут отсутствовать).
Класс Ellipse - эллипс. Эллипс - частный случай фигуры.
 У эллипса можно спросить std::pair<Point,Point> focuses() - его фокусы; std::pair<Line, Line> directrices() - пару его директрис;
 double eccentricity() - его эксцентриситет, Point center() - его центр.
 Эллипс можно сконструировать из двух точек и double (два фокуса и сумма расстояний от эллипса до них);
Класс Circle - круг. Круг - частный случай эллипса.
 У круга можно спросить double radius() - радиус.
 Круг можно задать точкой и числом (центр и радиус).
Класс Rectangle - прямоугольник. Прямоугольник - частный случай многоугольника.
 У прямоугольника можно спросить Point center() - его центр; std::pair<Line, Line> diagonals() - пару его диагоналей.
 Прямоугольник можно сконструировать по двум точкам (его противоположным вершинам) и числу (отношению смежных сторон),
 причем из двух таких прямоугольников выбирается тот, у которого более короткая сторона расположена по левую сторону от диагонали,
 если смотреть от первой заданной точки в направлении второй.
Класс Square - квадрат. Квадрат - частный случай прямоугольника.
 У квадрата можно спросить Circle circumscribedCircle(), Circle inscribedCircle().
 Квадрат можно задать двумя точками - противоположными вершинами.
Класс Triangle - треугольник. Треугольник - частный случай многоугольника.
 У треугольника можно спросить Circle circumscribedCircle(), Circle inscribedCircle(), Point centroid() - его центр масс,
 Point !!!orthocenter() - его ортоцентр, Line EulerLine() - его прямую Эйлера, Circle ninePointsCircle() - его окружность Эйлера.

У любой фигуры можно спросить:
 double perimeter() - периметр;
 double area() - площадь;
 operator==(const Shape& another) - совпадает ли эта фигура с другой;
 isCongruentTo(const Shape& another) - равна ли эта фигура другой в геометрическом смысле;
 isSimilarTo(const Shape& another) - подобна ли эта фигура другой;
 containsPoint(Point point) - находится ли точка внутри фигуры.
С любой фигурой можно сделать:
 rotate(Point center, double angle) - поворот на угол (в градусах, против часовой стрелки) относительно точки;
 reflex(Point center) - симметрию относительно точки;
 reflex(Line axis) - симметрию относительно прямой;
 scale(Point center, double coefficient) - гомотетию с коэффициентом coefficient и центром center.
*/

#include <cassert>
#include <vector>
#include <cmath>
#include <algorithm>

// Some functions

double dabs(double x) {
    return (x >= 0 ? x : -x);
}
double sqr(double num) {
    return num * num;
}

template <typename T>
bool areEqual(const T &A, const T &B) {
    return A == B;
}
bool areEqual(double A, double B) {
    double epsilon = 1e-14;
    return dabs(A - B) < epsilon;
}

template <typename T>
bool cyclicShifted(const std::vector<T> &first, const std::vector<T> &second) {
    if (first.size() != second.size()) return false;
    for (std::size_t i = 0; i < first.size(); ++i) {
        bool equal = true;
        for (std::size_t j = 0; j < first.size(); ++j)
            if (!areEqual(first[(i + j) % first.size()], second[j])) {
                equal = false;
                break;
            }
        if (equal) return true;
    }
    return false;
}

/// Declarations

struct Point;
struct Vector;
class Line;
class Shape;
class Polygon;
class Triangle;
class Rectangle;
class Square;
class Ellipse;
class Circle;

struct Point {
    Point(double nx = 0, double ny = 0);
    Point(const Vector &v);

    bool operator==(const Point &other) const;
    bool operator!=(const Point &other) const;

    double distance(const Point &other) const;
    double distance(const Line &L) const;
    void transform(const Vector &v);
    Point apply(const Vector &v) const;
    Vector radiusVector() const;
    bool between(const Point &A, const Point &B) const;
    bool layOn(const Line &L) const;

    double x = 0, y = 0;
};

struct Vector {
    Vector(double nx = 0, double ny = 0);
    Vector(const Point &A, const Point &B);

    bool operator==(const Vector &other) const;
    bool operator!=(const Vector &other) const;

    double operator*(const Vector &other) const;
    Vector &operator+=(const Vector &other);
    Vector operator+(const Vector &other) const;
    Vector &operator-=(const Vector &other);
    Vector operator-(const Vector &other) const;
    Vector &operator*=(double num);
    Vector operator*(double num) const;
    Vector &operator/=(double num);
    Vector operator/(double num) const;
    Vector operator+() const;
    Vector operator-() const;

    double abs() const;
    double cosine(const Vector &other) const;
    Vector getNormal() const;
    bool isNormalTo(const Vector &other) const;
    bool isCollinearTo(const Vector &other) const;

    double x = 0, y = 0;
};

class Line {
public:
    Line(const Point &A, const Point &B);
    Line(double k, double b);
    Line(const Point &A, double k);
    Line(const Point &A, const Vector &v);

    bool operator==(const Line &other) const;
    bool operator!=(const Line &other) const;

    bool isCollinearTo(const Line &other) const;
    bool isParallelTo(const Line &other) const;
    bool contains(const Point &A) const;
    Point projection(const Point &A) const;
    Point intersection(const Line &other) const;

private:
    Point Main;
    Vector Direction;
};

class Shape {
public:
    virtual ~Shape() = default;

    virtual bool operator==(const Shape &another) const = 0;
    bool operator!=(const Shape &another) const;

    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool isCongruentTo(const Shape &another) const = 0;
    virtual bool isSimilarTo(const Shape &another) const = 0;
    virtual bool containsPoint(const Point &point) const = 0;

    virtual void rotate(const Point &center, double angle) = 0;
    virtual void reflex(const Point &center) = 0;
    virtual void reflex(const Line &axis) = 0;
    virtual void scale(const Point &center, double coefficient) = 0;

protected:
    constexpr static double PI = 3.14159265358979323846;
};

class Polygon : public Shape {
public:
    template <typename... Args>
    Polygon(const Args&... args);
    Polygon(const std::vector<Point> &vertices);

    std::size_t verticesCount() const;
    std::vector<Point> getVertices() const;
    bool isConvex() const;

    bool operator==(const Shape &another) const override {
        try {
            auto other = dynamic_cast<const Polygon &>(another);
            bool result = cyclicShifted(Vertices, other.Vertices);
            std::reverse(other.Vertices.begin(), other.Vertices.end());
            return result || cyclicShifted(Vertices, other.Vertices);
        }
        catch (...) {
            return false;
        }
    }

    double perimeter() const override {
        double P = Vertices[0].distance(Vertices[Vertices.size() - 1]);
        for (std::size_t i = 0; i < Vertices.size() - 1; ++i)
            P += Vertices[i].distance(Vertices[i + 1]);
        P = static_cast<long long>(P * 1e9) / 1e9;
        return P;
    }
    double area() const override {
        double S = Vertices[0].x * (Vertices[1].y - Vertices[Vertices.size() - 1].y);
        for (std::size_t i = 1; i < Vertices.size(); ++i)
            S += Vertices[i].x * (Vertices[(i + 1) % Vertices.size()].y - Vertices[i - 1].y);
        S = static_cast<long long>(0.5 * dabs(S) * 1e9) / 1e9;
        return S;
    }
    bool isCongruentTo(const Shape &another) const override {
        if (!isSimilarTo(another)) return false;

        try {
            auto other = dynamic_cast<const Polygon &>(another);
            std::vector<double> selfSides(Vertices.size()), otherSides(Vertices.size());
            for (std::size_t i = 0; i < Vertices.size(); ++i) {
                selfSides[i] = Vertices[i].distance(Vertices[(i + 1) % Vertices.size()]);
                otherSides[i] = other.Vertices[i].distance(other.Vertices[(i + 1) % Vertices.size()]);
            }

            bool result = cyclicShifted(selfSides, otherSides);
            std::reverse(otherSides.begin(), otherSides.end());
            return result || cyclicShifted(selfSides, otherSides);
        }
        catch (...) {
            return false;
        }
    }
    bool isSimilarTo(const Shape &another) const override {
        try {
            auto other = dynamic_cast<const Polygon &>(another);
            if (Vertices.size() != other.Vertices.size()) return false;
            std::vector<double> selfCosines(Vertices.size()), otherCosines(Vertices.size());

            selfCosines[0] = Vector(Vertices[0], Vertices[Vertices.size() - 1]).cosine(Vector(Vertices[0], Vertices[1]));
            otherCosines[0] = Vector(other.Vertices[0], other.Vertices[Vertices.size() - 1]).cosine(Vector(other.Vertices[0], other.Vertices[1]));
            for (std::size_t i = 1; i < Vertices.size(); ++i) {
                selfCosines[i] = Vector(Vertices[i], Vertices[i - 1]).cosine(Vector(Vertices[i], Vertices[(i + 1) % Vertices.size()]));
                otherCosines[i] = Vector(other.Vertices[i], other.Vertices[i - 1]).cosine(Vector(other.Vertices[i], other.Vertices[(i + 1) % Vertices.size()]));
            }

            bool result = cyclicShifted(selfCosines, otherCosines);
            std::reverse(otherCosines.begin(), otherCosines.end());
            return result || cyclicShifted(selfCosines, otherCosines);
        }
        catch (...) {
            return false;
        }
    }
    bool containsPoint(const Point &point) const override {
        bool result = ((Vertices.front().y < point.y && Vertices.back().y >= point.y) ||
                       (Vertices.back().y < point.y && Vertices.front().y >= point.y)) &&
                      (Vertices.front().x + (point.y - Vertices.front().y) / (Vertices.back().y - Vertices.front().y) *
                                            (Vertices.back().x - Vertices.front().x) <= point.x);
        for (std::size_t i = 1; i < Vertices.size(); ++i)
            if (((Vertices[i].y < point.y && Vertices[i - 1].y >= point.y) ||
                 (Vertices[i - 1].y < point.y && Vertices[i].y >= point.y)) &&
                (Vertices[i].x + (point.y - Vertices[i].y) / (Vertices[i - 1].y - Vertices[i].y) *
                                 (Vertices[i - 1].x - Vertices[i].x) <= point.x))
                result = !result;
        return result;
    }

    void rotate(const Point &center, double angle) override {
        angle = Shape::PI * (angle / 180.0);
        for (auto &vertex : Vertices) {
            double X = vertex.x, Y = vertex.y;
            vertex.x = (X - center.x) * cos(angle) - (Y - center.y) * sin(angle) + center.x;
            vertex.y = (X - center.x) * sin(angle) + (Y - center.y) * cos(angle) + center.y;
        }
    }
    void reflex(const Point &center) override {
        for (auto &vertex : Vertices)
            vertex.transform(Vector(vertex, center) * 2);
    }
    void reflex(const Line &axis) override {
        for (auto &vertex : Vertices)
            vertex.transform(Vector(vertex, axis.projection(vertex)) * 2);
    }
    void scale(const Point &center, double coefficient) override {
        for (auto &vertex : Vertices)
            vertex.transform(Vector(center, vertex) * (coefficient - 1));
    }

protected:
    std::vector<Point> Vertices;

private:
    void pushVertices() {}
    template <typename Head, typename... Tail>
    void pushVertices(const Head& head, const Tail&... tail);
};

class Triangle : public Polygon {
public:
    template <typename... Args>
    Triangle(const Args&... args) : Polygon(args...) {}
    Triangle(const std::vector<Point> &vertices) : Polygon(vertices) {}

    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
    Point centroid() const;
    Point orthocenter() const;
    Line EulerLine() const;
    Circle ninePointsCircle() const;
};

class Rectangle : public Polygon {
public:
    template <typename... Args>
    Rectangle(const Args&... args) : Polygon(args...) {}
    Rectangle(const std::vector<Point> &vertices) : Polygon(vertices) {}
    Rectangle(const Point &A, const Point &B, double relative);

    Point center() const;
    std::pair<Line, Line> diagonals() const;

private:
    void rotateCosine(const Point &center, double cosine);
};

class Square : public Rectangle {
public:
    template <typename... Args>
    Square(const Args&... args) : Rectangle(args...) {}
    Square(const std::vector<Point> &vertices) : Rectangle(vertices) {}
    Square(const Point &A, const Point &C, double relative) : Rectangle(A, C, relative) {
        assert(areEqual(relative, 1));
    }
    Square(const Point &A, const Point &C) : Rectangle(A, C, 1) {}

    Circle circumscribedCircle() const;
    Circle inscribedCircle() const;
};

class Ellipse : public Shape {
public:
    Ellipse(const Point &Focus1, const Point &Focus2, double distance);

    Point center() const;
    double eccentricity() const;
    std::pair<Point, Point> focuses() const;
    std::pair<Line, Line> directrices() const;

    bool operator==(const Shape &another) const override {
        try {
            auto other = dynamic_cast<const Ellipse &>(another);
            return areEqual(RA, other.RA) && (Focuses == other.Focuses ||
                                              (Focuses.first == other.Focuses.second &&
                                               other.Focuses.first == Focuses.second));
        }
        catch (...) {
            return false;
        }
    }

    double perimeter() const override {
        return PI * (3 * (RA + RB) - sqrt((3 * RA + RB) * (RA + 3 * RB)));
    }
    double area() const override {
        return Shape::PI * RA * RB;
    }
    bool isCongruentTo(const Shape &another) const override {
        try {
            auto other = dynamic_cast<const Ellipse &>(another);
            return areEqual(RA, other.RA) && areEqual(RC, other.RC);
        }
        catch (...) {
            return false;
        }
    }
    bool isSimilarTo(const Shape &another) const override {
    try {
        auto other = dynamic_cast<const Ellipse &>(another);
        return areEqual(RA * other.RB, RB * other.RA);
    }
    catch (...) {
        return false;
    }
    }
    bool containsPoint(const Point &point) const override {
        return point.distance(Focuses.first) + point.distance(Focuses.second) <= 2 * RA;
    }

    void rotate(const Point &center, double angle) override {
        angle = PI * (angle / 180.0);
        {
            auto &vertex = Focuses.first;
            double X = vertex.x, Y = vertex.y;
            vertex.x = (X - center.x) * cos(angle) - (Y - center.y) * sin(angle) + center.x;
            vertex.y = (X - center.x) * sin(angle) + (Y - center.y) * cos(angle) + center.y;
        }
        {
            auto &vertex = Focuses.second;
            double X = vertex.x, Y = vertex.y;
            vertex.x = (X - center.x) * cos(angle) - (Y - center.y) * sin(angle) + center.x;
            vertex.y = (X - center.x) * sin(angle) + (Y - center.y) * cos(angle) + center.y;
        }
    }
    void reflex(const Point &center) override {
        Focuses.first.transform(Vector(Focuses.first, center) * 2);
        Focuses.second.transform(Vector(Focuses.second, center) * 2);
    }
    void reflex(const Line &axis) override {
        Focuses.first.transform(Vector(Focuses.first, axis.projection(Focuses.first)) * 2);
        Focuses.second.transform(Vector(Focuses.second, axis.projection(Focuses.second)) * 2);
    }
    void scale(const Point &center, double coefficient) override {
        Focuses.first.transform(Vector(center, Focuses.first) * (coefficient - 1));
        Focuses.second.transform(Vector(center, Focuses.second) * (coefficient - 1));
        RA *= coefficient;
        RB *= coefficient;
        RC *= coefficient;
    }

protected:
    double RA, RB, RC;

private:
    std::pair<Point, Point> Focuses;
};

class Circle : public Ellipse {
public:
    Circle(const Point &O, double R) : Ellipse(O, O, 2 * R) {}

    double radius() const;
};

/// Realisations

// Point

Point::Point(double nx, double ny) {
    x = nx;
    y = ny;
}

Point::Point(const Vector &v) {
    x = v.x;
    y = v.y;
}

bool Point::operator==(const Point &other) const {
    return areEqual(x, other.x) && areEqual(y, other.y);
}

bool Point::operator!=(const Point &other) const {
    return !(*this == other);
}

double Point::distance(const Point &other) const {
    return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

double Point::distance(const Line &L) const {
    return distance(L.projection(*this));
}

void Point::transform(const Vector &v) {
    x += v.x;
    y += v.y;
}

Point Point::apply(const Vector &v) const {
    auto tmp = *this;
    tmp.transform(v);
    return tmp;
}

Vector Point::radiusVector() const {
    return Vector(x, y);
}

bool Point::between(const Point &A, const Point &B) const {
    if (!Vector(*this, A).isCollinearTo(Vector(A, B))) return false;
    auto min_x = std::min(A.x, B.x), max_x = std::max(A.x, B.x), min_y = std::min(A.y, B.y), max_y = std::max(A.y, B.y);
    return min_x <= x && x <= max_x && min_y <= y && y <= max_y;
}

bool Point::layOn(const Line &L) const {
    return L.contains(*this);
}

Point middlePoint(const Point &A, const Point &B) {
    return Point((A.x + B.x) * 0.5, (A.y + B.y) * 0.5);
}

// Vector

Vector::Vector(double nx, double ny) {
    x = nx;
    y = ny;
}

Vector::Vector(const Point &A, const Point &B) {
    x = B.x - A.x;
    y = B.y - A.y;
}

bool Vector::operator==(const Vector &other) const {
    return areEqual(x, other.x) && areEqual(y, other.y);
}

bool Vector::operator!=(const Vector &other) const {
    return !(*this == other);
}

double Vector::operator*(const Vector &other) const {
    return x * other.x + y * other.y;
}

Vector &Vector::operator+=(const Vector &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector Vector::operator+(const Vector &other) const {
    auto tmp = *this;
    return tmp += other;
}

Vector &Vector::operator-=(const Vector &other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector Vector::operator-(const Vector &other) const {
    auto tmp = *this;
    return tmp -= other;
}

Vector &Vector::operator*=(double num) {
    x *= num;
    y *= num;
    return *this;
}

Vector Vector::operator*(double num) const {
    auto tmp = *this;
    return tmp *= num;
}

Vector operator*(double num, const Vector &v) {
    return v * num;
}

Vector &Vector::operator/=(double num) {
    assert(num != 0);
    x /= num;
    y /= num;
    return *this;
}

Vector Vector::operator/(double num) const {
    auto tmp = *this;
    return tmp /= num;
}

Vector Vector::operator+() const {
    return *this;
}

Vector Vector::operator-() const {
    return *this * (-1);
}

double Vector::abs() const {
    return sqrt(sqr(x) + sqr(y));
}

double Vector::cosine(const Vector &other) const {
    assert((x != 0 || y != 0) && (other.x != 0 || other.y != 0));
    return (*this * other) / sqrt((sqr(x) + sqr(y)) * (sqr(other.x) + sqr(other.y)));
}

Vector Vector::getNormal() const {
    assert(x != 0 || y != 0);
    return Vector(-y, x);
}

bool Vector::isNormalTo(const Vector &other) const {
    return areEqual(*this * other, 0);
}

bool Vector::isCollinearTo(const Vector &other) const {
    return areEqual(x * other.y, y * other.x);
}

// Line

Line::Line(const Point &A, const Point &B) {
    Main = A;
    Direction = Vector(A, B);
}

Line::Line(double k, double b) {
    Main = Point(0, b);
    Direction = Vector(1, k);
}

Line::Line(const Point &A, double k) {
    Main = A;
    Direction = Vector(1, k);
}

Line::Line(const Point &A, const Vector &v) {
    Main = A;
    Direction = v;
}

bool Line::isCollinearTo(const Line &other) const {
    return Direction.isCollinearTo(other.Direction);
}

bool Line::isParallelTo(const Line &other) const {
    return isCollinearTo(other) && !contains(other.Main);
}

bool Line::contains(const Point &A) const {
    return Vector(Main, A).isCollinearTo(Direction);
}

bool Line::operator==(const Line &other) const {
    return isCollinearTo(other) && contains(other.Main);
}

bool Line::operator!=(const Line &other) const {
    return !(*this == other);
}

Point Line::projection(const Point &A) const {
    if (contains(A)) return A;
    return intersection(Line(A, Direction.getNormal()));
}

Point Line::intersection(const Line &other) const {
    assert(!isCollinearTo(other));
    double c1 = Direction.y * Main.x - Direction.x * Main.y;
    double c2 = other.Direction.y * other.Main.x - other.Direction.x * other.Main.y;
    double D = other.Direction.y * Direction.x - Direction.y * other.Direction.x;
    double D_x = Direction.x * c2 - other.Direction.x * c1;
    double D_y = Direction.y * c2 - other.Direction.y * c1;
    return Point(D_x / D, D_y / D);
}

// Shape

bool Shape::operator!=(const Shape &another) const {
    return !(*this == another);
}

// Polygon

template <typename... Args>
Polygon::Polygon(const Args&... args) {
    pushVertices(args...);
}

Polygon::Polygon(const std::vector<Point> &vertices) {
    Vertices = vertices;
}

std::size_t Polygon::verticesCount() const {
    return Vertices.size();
}

std::vector<Point> Polygon::getVertices() const{
    return Vertices;
}

bool Polygon::isConvex() const {
    Vector v1(Vertices[0], Vertices[Vertices.size() - 1]);
    Vector v2(Vertices[0], Vertices[1]);
    bool Rotate = v1.x * v2.y > v1.y * v2.x;
    for (std::size_t i = 1; i < Vertices.size(); ++i) {
        v1 = Vector(Vertices[i], Vertices[i - 1]);
        v2 = Vector(Vertices[i], Vertices[(i + 1) % Vertices.size()]);
        if (Rotate != (v1.x * v2.y > v1.y * v2.x)) return false;
    }
    return true;
}

template <typename Head, typename... Tail>
void Polygon::pushVertices(const Head& head, const Tail&... tail) {
    Vertices.push_back(head);
    pushVertices(tail...);
}

// Triangle

Circle Triangle::circumscribedCircle() const {
    Vector AB(Polygon::Vertices[0], Polygon::Vertices[1]), AC(Polygon::Vertices[0], Polygon::Vertices[2]);
    auto M = middlePoint(Polygon::Vertices[0], Polygon::Vertices[1]);
    auto N = middlePoint(Polygon::Vertices[0], Polygon::Vertices[2]);
    Line LM(M, AB.getNormal()), LN(N, AC.getNormal());
    auto O = LM.intersection(LN);
    return Circle(O, O.distance(Polygon::Vertices[0]));
}

Circle Triangle::inscribedCircle() const {
    auto CA = Vector(Vertices[0], Vertices[1]), CB = Vector(Vertices[0], Vertices[2]);
    auto a = CA * CB.abs(), b = CB * CA.abs();
    auto O = Vertices[0].apply((a + b) / perimeter());
    return Circle(O, 2.0 * area() / perimeter());
}

Point Triangle::centroid() const {
    double X = 0, Y = 0;
    for (auto &vertex : Polygon::Vertices) {
        X += vertex.x;
        Y += vertex.y;
    }
    return Point(X / 3.0, Y / 3.0);
}

Point Triangle::orthocenter() const {
    Vector AB(Polygon::Vertices[0], Polygon::Vertices[1]), AC(Polygon::Vertices[0], Polygon::Vertices[2]);
    Line H1(Polygon::Vertices[1], AC.getNormal()), H2(Polygon::Vertices[2], AB.getNormal());
    return H1.intersection(H2);
}

Line Triangle::EulerLine() const {
    return Line(orthocenter(), centroid());
}

Circle Triangle::ninePointsCircle() const {
    auto outCircle = circumscribedCircle();
    outCircle.scale(orthocenter(), 0.5);
    return outCircle;
}

// Rectangle

Rectangle::Rectangle(const Point &A, const Point &C, double relative) {
    auto shift = A.radiusVector();
    double a = sqrt((sqr(A.x - C.x) + sqr(A.y - C.y)) / (1.0 + sqr(relative))), b = a * relative;
    if (relative < 1) std::swap(a, b);
    Vertices = {Point(0, 0), Point(0, a), Point(b, a), Point(b, 0)};
    for (auto &vertex : Vertices) vertex.transform(shift);
    rotateCosine(A, Vector(A, Vertices[2]).cosine(Vector(A, C)));
    Vertices[2] = C;
}

Point Rectangle::center() const {
    return middlePoint(Polygon::Vertices[0], Polygon::Vertices[2]);
}

std::pair<Line, Line> Rectangle::diagonals() const {
    Line D1(Polygon::Vertices[0], Polygon::Vertices[2]);
    Line D2(Polygon::Vertices[1], Polygon::Vertices[3]);
    return std::make_pair(D1, D2);
}

void Rectangle::rotateCosine(const Point &center, double cosine) {
    double sine = sqrt(1 - sqr(cosine));
    for (auto &vertex : Vertices) {
        double X = vertex.x, Y = vertex.y;
        vertex.x = (X - center.x) * cosine - (Y - center.y) * sine + center.x;
        vertex.y = (X - center.x) * sine + (Y - center.y) * cosine + center.y;
    }
}

// Square

Circle Square::circumscribedCircle() const {
    return Circle(center(), Polygon::Vertices[0].distance(Polygon::Vertices[2]) / 2);
}

Circle Square::inscribedCircle() const {
    return Circle(center(), Polygon::Vertices[0].distance(Polygon::Vertices[1]) / 2);
}

// Ellipse

Ellipse::Ellipse(const Point &Focus1, const Point &Focus2, double distance) {
    RA = distance / 2.0;
    RB = sqrt(sqr(distance) - sqr(Focus1.x - Focus2.x) - sqr(Focus1.y - Focus2.y)) / 2.0;
    RC = Focus1.distance(Focus2) / 2.0;
    Focuses = std::make_pair(Focus1, Focus2);
}

Point Ellipse::center() const {
    return middlePoint(Focuses.first, Focuses.second);
}

double Ellipse::eccentricity() const {
    return RC / RA;
}

std::pair<Point, Point> Ellipse::focuses() const {
    return Focuses;
}

std::pair<Line, Line> Ellipse::directrices() const {
    Vector FF(Focuses.first, Focuses.second);
    auto dir = FF.getNormal(), axis = FF * (RA / (FF.abs() * eccentricity()));
    auto C = center(), D1 = C.apply(axis), D2 = C.apply(-1 * axis);
    return std::make_pair(Line(D1, dir), Line(D2, dir));
}

// Circle

double Circle::radius() const {
    return Ellipse::RA;
}