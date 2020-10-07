#include <stdio.h>

int solve(int A, int B, int C, int D);

int main() {
	int A, B, C, D;
	scanf("%d%d%d%d", &a, &B, &C, &D);
	int x = 0;
	while (((A * x + B) * x + C) * x + D != 0) ++x;
	printf(x == solve(A, B, C, D) ? "Good\n" : "Bad\n");
	return 0;
}