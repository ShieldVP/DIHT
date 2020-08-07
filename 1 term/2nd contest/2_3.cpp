/*
На числовой прямой окрасили N отрезков.
Известны координаты левого и правого концов каждого отрезка [Li, Ri]. Найти длину окрашенной части числовой прямой.
*/

#include <iostream>
using namespace std;

struct point {
	int coordinate, type;
};

bool operator>(point a, point b) {
	if (a.coordinate != b.coordinate) return a.coordinate > b.coordinate;
	return a.type > b.type;
}

template <typename T>
bool compare(T a, T b) {
	return a > b;
}

template <typename T>
int max_child(T*& arr, unsigned int N, unsigned int n) {
	if (2 * n + 2 >= N || compare(arr[2 * n + 1], arr[2 * n + 2])) return 2 * n + 1;
	else return 2 * n + 2;
}

template <typename T>
void sort(T*& arr, int n) {
	for (int i = 1; i < n; ++i) {
		int N = i - 1;
		while (N > 0) {
			if (compare(arr[N], arr[(N - 1) / 2])) swap(arr[N], arr[(N - 1) / 2]);
			else break;
			N = (N - 1) / 2;
		}
	}

	for (int i = 0; i < n; ++i) {
		int N = n - i - 1;
		swap(arr[0], arr[N]);
		int j = 0;
		while (2 * j + 1 < N && compare(arr[max_child(arr, N, j)], arr[j])) {
			int tmp = max_child(arr, N, j);
			swap(arr[tmp], arr[j]);
			j = tmp;
		}
	}
}

int colored_length(point*& arr, int n) {
	sort(arr, n);
	int line_colored = 0, total_length = 0;
	for (int i = 0; i < n; ++i) {
		if (line_colored) total_length += arr[i].coordinate - arr[i - 1].coordinate;
		line_colored += arr[i].type;
	}
	return total_length;
}

int main() {
	int N;
	cin >> N;
	point* points = new point[2 * N];
	for (int i = 0; i < N; ++i) {
		cin >> points[2 * i].coordinate >> points[2 * i + 1].coordinate;
		points[2 * i].type = 1; // L
		points[2 * i + 1].type = -1; // R
	}
	cout << colored_length(points, 2 * N);
	return 0;
}