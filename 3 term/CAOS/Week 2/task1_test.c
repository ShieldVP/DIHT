#include <stdio.h>
#include <stdlib.h>

int summ(int x0, int N, int *X);

int main() {
	int x0, N;
	int *X;
	scanf("%d%d", &x0, &N);
	X = malloc(sizeof(int) * N);
	int sum = x0;
	for (int i = 0; i < N; ++i) {
		scanf("%d", &X[i]);
		sum += X[i];
	}
	printf(sum == summ(x0, N, X) ? "Good\n" : "Bad\n");
	free(X);
	return 0;
}