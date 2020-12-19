/*
Арсений опять уснул. И снова на планете Ка-Пэкс без него никак — два фермера поссорились из-за того, что их территории могут перекрываться. 
Естественно, как самого рассудительного, Арсения позвали урегулировать конфликт. Для начала он решил узнать, насколько серьезен конфликт. 
Для этого он решил узнать, пересекаются ли фермы вообще. 
Помогите Арсению это понять, если известно, что каждая ферма представляет собой выпуклый многоугольник. 
Первая строка содержит число N точек первого многоугольника. Затем идут N строк с координатами точек первого многоугольника по часовой стрелке (координаты – действительные числа).
Второй прямоугольник задается аналогично. N, M ≤ 80000.
Выведите “YES”, если фермы пересекаются, и “NO”, если нет (без кавычек).
*/

#include <iostream>
#include <vector>
#include <tuple>

struct Point {
    long double x, y;
	
	Point() : 
	x(0), 
	y(0) 
	{};
	
	Point(long double x, long double y) : 
	x(x), 
	y(y) 
	{};
	
	Point& operator+=(const Point& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	
	Point& operator-=(const Point& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	
	Point operator-() const {
		return Point(-x, -y);
    }
};

Point operator+(const Point& A, const Point& B) {
	auto C = A;
	return C += B;
}

Point operator-(const Point& A, const Point& B) {
	auto C = A;
	return C -= B;
}

std::istream& operator>>(std::istream& stream, Point& A) {
    stream >> A.x >> A.y;
    return stream;
}

static double crossProductZ(const Point& A, const Point& B) {
	return A.x * B.y - A.y * B.x;
}

struct Polygon {
public:	
	Polygon() = default;

    Polygon(size_t vertices) {
        read(vertices);
    }
	
	void add(const Point& newPoint) {
		Vertices.push_back(newPoint);
	}
	
	size_t vertices() const {
		return Vertices.size();
	}

	size_t front() const {
		return rightBottomPoint;
	}

	Point& radiusVector(size_t i) const {
		return Vertices[i % Vertices.size()];
	}
	
	Point edgeVector(size_t i) const {
		return radiusVector(i + 1) - radiusVector(i);
	}
	
	Polygon zeroReflex() const {
		Polygon result;
		for (size_t i = 0; i < Vertices.size(); ++i)
			result.add(-radiusVector(i + leftTopPoint));
		return result;
	}
	
	bool contains(const Point& A) const {
		bool result = ((Vertices.front().y < A.y && Vertices.back().y >= A.y) || (Vertices.back().y < A.y && Vertices.front().y >= A.y)) &&
                      (Vertices.front().x + (A.y - Vertices.front().y) / (Vertices.back().y - Vertices.front().y) * (Vertices.back().x - Vertices.front().x) <= A.x);
        for (size_t i = 1; i < Vertices.size(); ++i)
            if (((Vertices[i].y < A.y && Vertices[i - 1].y >= A.y) || (Vertices[i - 1].y < A.y && Vertices[i].y >= A.y)) &&
                (Vertices[i].x + (A.y - Vertices[i].y) / (Vertices[i - 1].y - Vertices[i].y) * (Vertices[i - 1].x - Vertices[i].x) <= A.x))
                result = !result;
        return result;
	}
	
private:
	std::vector<Point> Vertices;

	size_t leftTopPoint = 0, rightBottomPoint = 0;
	
    void read(size_t vertices) {
        Vertices.resize(vertices);
        for (size_t i = 0; i < vertices; ++i) {
            std::cin >> Vertices[i];
			if (std::tie(Vertices[i].y, -Vertices[i].x) > std::tie(Vertices[leftTopPoint].y, -Vertices[leftTopPoint].x)) 
				leftTopPoint = i;
            if (std::tie(Vertices[i].y, -Vertices[i].x) < std::tie(Vertices[rightBottomPoint].y, -Vertices[rightBottomPoint].x))
            	rightBottomPoint = i;
		}
    }
};

Polygon MinkoskiySum(const Polygon& P, const Polygon& Q) {
	Polygon result;
	for (size_t i = 0, j = 0; i < P.vertices() || j < Q.vertices();) {
		result.add(P.radiusVector(i + P.front()) + Q.radiusVector(j + Q.front()));
		auto rotation = crossProductZ(P.edgeVector(i + P.front()), Q.edgeVector(j + Q.front()));
		if (rotation <= 0) ++i;
		if (rotation >= 0) ++j;
	}
	return result;
}

bool isIntersect(const Polygon& P, const Polygon& Q) {
	return MinkoskiySum(P, Q.zeroReflex()).contains(Point(0, 0));
}

int main() {
    size_t N, M;
    std::cin >> N;
    Polygon P(N);
    std::cin >> M;
    Polygon Q(M);
    std::cout << (isIntersect(P, Q) ? "YES" : "NO");
    return 0;
}