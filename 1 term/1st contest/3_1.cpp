/*
Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i].
Если такого элемента нет, выведите n.
Время работы поиска k для каждого элемента B[i]: O(log(k)). n, m ≤ 10000.
*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int bin_search(vector<int> A, int x) {
	if (A[A.size() - 1] < x) return A.size(); // if there is no such element
	int lc = 0; // left corner
	int rc = A.size() - 1; // right corner
	int mid = (lc + rc) / 2; // middle position
	while (lc < rc) {
		if (A[mid] == x) return mid;
		else if (A[mid] > x) rc = mid;
		else lc = mid + 1;
		mid = (lc + rc) / 2;
	}
	return (abs(x - A[lc]) <= abs(A[rc] - x)) ? lc : rc; // nearest element
}

vector<int> for_each_search(vector<int> A, vector<int> B) {
	vector<int> res;
	for (int i = 0; i < B.size(); i++) res.push_back(bin_search(A, B[i]));
	return res;
}

int main(){
	int n, m;
	cin >> n >> m;
	
	vector<int> A;
	for (int i = 0; i < n; i++) {
		int tmp;
		cin >> tmp;
		A.push_back(tmp);
	}
	
	vector<int> B;
	for (int i = 0; i < m; i++) {
		int tmp;
		cin >> tmp;
		B.push_back(tmp);
	}
	
	auto result = for_each_search(A, B);
	for (int i = 0; i < m; i++) cout << result[i] << ' ';
	return 0;
}