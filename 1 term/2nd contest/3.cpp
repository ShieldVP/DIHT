/*
Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition
реализуйте методом прохода двумя итераторами от начала массива к концу.
*/

#include <iostream>
using namespace std;

template <typename T>
int median(T *&arr, int lb, int rb) {
	if ((arr[lb] <= arr[(lb + rb) / 2] <= arr[rb]) || (arr[rb] <= arr[(lb + rb) / 2] <= arr[lb])) return (lb + rb) / 2;
	else if ((arr[(lb + rb) / 2] <= arr[lb] <= arr[rb]) || (arr[rb] <= arr[lb] <= arr[(lb + rb) / 2])) return lb;
	return rb;
}

template <typename T>
int partition(T*& arr, int lb, int rb) {
	int i = lb; // last element which <= pivot
	T pivot = arr[rb];
	for (int j = lb; j < rb; ++j) {
		if (arr[j] <= pivot) {
			swap(arr[i], arr[j]);
			++i;
		}
	}
	swap(arr[rb], arr[i]);
	return i;
}

template <typename T>
int order_statistics(T*& arr, int n, int k) {
	int lb = 0, rb = n - 1; // lb - left border, rb - right border
	while (lb < rb - 1) {
		int pivot = median(arr, lb, rb);
		swap(arr[pivot], arr[rb]);
		pivot = partition(arr, lb, rb);
		if (pivot == k) return arr[pivot];
		else if (pivot > k) rb = pivot;
		else lb = pivot;
	}
	return arr[k];
}

int main() {
	int n, k;
	cin >> n >> k;
	int* arr = new int[n];
	for (int i = 0; i < n; ++i) cin >> arr[i];
	cout << order_statistics(arr, n, k);
	return 0;
}