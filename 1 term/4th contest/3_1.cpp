/*
Дано N кубиков. Требуется определить каким количеством способов можно выстроить из этих кубиков пирамиду.
*/

#include <iostream>
using namespace std;

long long count(size_t n) {
	if (n == 0) return 1;
	long long** table = new long long*[n + 1];
	for (size_t i = 0; i < n + 1; ++i) table[i] = new long long[n + 1];
	for (size_t i = 0; i < n + 1; ++i) {
		table[0][i] = table[1][i] = table[i][1] = 1;
		table[i][0] = 0;
	}
	for (size_t i = 2; i < n + 1; ++i) {
		for (size_t j = 2; j < n + 1; ++j) {
			table[i][j] = table[i][j - 1];
			if (i >= j) table[i][j] += table[i - j][j];
		}
	}
	long long res = table[n][n];
	for (size_t i = 0; i < n + 1; ++i) delete [] table[i];
	return res;
}

int main() {
	size_t n;
	cin >> n;
	cout << count(n);
	return 0;
}