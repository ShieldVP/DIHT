/*
В круг выстроено N человек, пронумерованных числами от 1 до N.
Будем исключать каждого k-ого до тех пор, пока не уцелеет только один человек.
Например, если N=10, k=3, то сначала умрет 3-й, потом 6-й, затем 9-й, затем 2-й, затем 7-й, потом 1-й, потом 8-й, за ним - 5-й, и потом 10-й.
Таким образом, уцелеет 4-й.
Необходимо определить номер уцелевшего.
N, k ≤ 10000.
*/

#include <iostream>
#include <vector>
using namespace std;

int schitalochka(int n, int k) {
	vector<int> a;
	for (int i = 0; i < n; i++) a.push_back(i + 1);
	int index = k % n - 1;
	while (a.size() > 1) {
		a.erase(a.begin() + index % a.size());
		index = (index + k - 1) % a.size();
	}
	return a[0];
}

int main() {
	int N, k;
	cin >> N >> k;
	cout << schitalochka(N, k);
	return 0;
}