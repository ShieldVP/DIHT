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
		Vertices_.push_back(newPoint);
	}
	
	size_t verticesSize() const {
		return Vertices_.size();
	}

	size_t front() const {
		return rightBottomPoint_;
	}

	const Point& radiusVector(size_t i) const {
		return Vertices_[i % Vertices_.size()];
	}
	
	Point edgeVector(size_t i) const {
		return radiusVector(i + 1) - radiusVector(i);
	}
	
	Polygon zeroReflex() const {
		Polygon result;
		for (size_t i = 0; i < Vertices_.size(); ++i)
			result.add(-radiusVector(i + leftTopPoint_));
		return result;
	}
	
	bool contains(const Point& A) const {
        for (size_t i = 0; i < Vertices_.size(); ++i)
            if (betweenHeight(radiusVector(i), radiusVector(i + 1), A) && isLeftToSegment(radiusVector(i), radiusVector(i + 1), A))
                result = !result;
        return result;
	}
	
private:
	std::vector<Point> Vertices_;

	size_t leftTopPoint_ = 0, rightBottomPoint_ = 0;
	
    void read(size_t vertices) {
        Vertices_.resize(vertices);
        for (size_t i = 0; i < vertices; ++i) {
            std::cin >> Vertices_[i];
			if (std::tie(Vertices_[i].y, -Vertices_[i].x) > std::tie(Vertices_[leftTopPoint_].y, -Vertices_[leftTopPoint_].x)) 
				leftTopPoint_ = i;
            if (std::tie(Vertices_[i].y, -Vertices_[i].x) < std::tie(Vertices_[rightBottomPoint_].y, -Vertices_[rightBottomPoint_].x))
            	rightBottomPoint_ = i;
		}
    }

    bool betweenHeight(const Point& segmentStart, const Point& segmentEnd, const Point& A) {
    	return (segmentStart.y < A.y && segmentEnd.y >= A.y) || (segmentEnd.y < A.y && segmentStart.y >= A.y);
    }

    bool isLeftToSegment(const Point& segmentStart, const Point& segmentEnd, const Point& A) {
    	auto seg_dy = segmentEnd.y - segmentStart.y, seg_dx = segmentEnd.x - segmentStart.x;
    	return (A.y - segmentStart.y) * seg_dx / seg_dy <= A.x - segmentStart.x;
    }
};

Polygon MinkoskiySum(const Polygon& P, const Polygon& Q) {
	Polygon result;
	for (size_t i = 0, j = 0; i < P.verticesSize() || j < Q.verticesSize();) {
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