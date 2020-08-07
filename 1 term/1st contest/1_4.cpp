/*
Дано натуральное число N. Представить N в виде A + B, так, что НОД(A, B) максимален, 1 ≤ A ≤ B. Вывести A и B.
Если возможно несколько ответов - вывести ответ с минимальным A. 1 < n ≤ 10 000 000.
*/

#include <iostream>
using namespace std;

pair<int, int> maxNOD(int x) {
	pair<int, int> res;
	for (int i = 2; i * i <= x; i++) {
		if (x % i == 0) {
			res.first = x / i;
			res.second = x - res.first;
			return res;
		}
	}
	res.first = 1;
	res.second = x - 1;
	return res;
}

int main() {
	int N;
	cin >> N;
	auto out = maxNOD(N);
	cout << out.first << ' ' << out.second;
	return 0;
}