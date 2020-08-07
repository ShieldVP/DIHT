#define ui unsigned int

#include <iostream>
#include <vector>
using namespace std;

ui power(int x, int n) {
	if (n == 0) return 1;

	if (n % 2 == 0) {
		int tmp = power(x, n / 2);
		return tmp * tmp;
	}
	else {
		int tmp = power(x, (n - 1) / 2);
		return x * tmp * tmp;
	}
}

int digit(int x, int i, int system) {
	return (x / power(system, i)) % system;
}

void radix(ui*& arr, ui n) {
	// Will change 10 system to "system" system
	int system = 256;

	// Finding max element from array
	ui max = arr[0];
	for (int i = 1; i < n; ++i)
		if (arr[i] > max) max = arr[i];

	// Counting number of digits of "max" in "system" system
	ui max_amount_digits = 0;
	for (; max > 0; max /= system) ++max_amount_digits;

	// Sorting
	for (ui i = 0; i < max_amount_digits; ++i) {
		vector<ui>* table = new vector<ui>[system];
		/* for example
		0     1 2 3 ... system-1
		-     - - - --- --------
		arr[0]
		*/

		// Sort by count
		for (int j = 0; j < n; ++j)
			table[digit(arr[j], i, system)].push_back(arr[j]);

		ui k = 0;
		for (int j = 0; j < system; ++j)
			for (ui l = 0; l < table[j].size(); ++l)
				arr[k++] = table[j][l];

		delete[] table;
	}
}

unsigned int sum(ui*& arr, ui n, ui L) {
	unsigned int S = 0;
	for (int i = 0; i < n; i += 2) S = (S + arr[i]) % L;
	return S;
}

int main() {
	ui N, K, L;
	unsigned long long M;
	cin >> N >> K >> M >> L;
	ui *arr = new ui[N];
	arr[0] = K;
	for (ui i = 0; i < N - 1; ++i)
		arr[i + 1] = (ui)((arr[i] * (unsigned long long) M) & 0xFFFFFFFFU) % L;

	radix(arr, N);
	cout << sum(arr, N, L);

	return 0;
}