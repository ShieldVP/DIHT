#include <stdio.h>

void printSet(unsigned long long set) {
	short shift = 0;
	const int numbersAmount = 10, lettersAmount = 26;
	for (; shift < numbersAmount; ++shift) {
		if (set & 1)
		       	putchar((char)(shift + (short)'0'));
		set >>= 1;
	}
	for (; shift < numbersAmount + lettersAmount; ++shift) {
		if (set & 1)
			putchar((char)(shift - numbersAmount + (short)'A'));
		set >>= 1;
	}
	for (; shift < numbersAmount + lettersAmount + lettersAmount; ++shift) {
		if (set & 1)
			putchar((char)(shift - (numbersAmount + lettersAmount) + (short)'a'));
		set >>= 1;
	}
}

int main(int argc, char* argv[]) {
	unsigned long long resultSet = 0, currentSet = 0;
	const int numbersAmount = 10, lettersAmount = 26;
	short symbol = getchar();
	while (symbol != EOF) {
		if (symbol == (short)'^') {
			resultSet ^= currentSet;
			currentSet = 0;
		}
		else if (symbol == (short)'&') {
			resultSet &= currentSet;
			currentSet = 0;
		}
		else if (symbol == (short)'|') {
			resultSet |= currentSet;
			currentSet = 0;
		}
		else if (symbol == (short)'~') {
			resultSet ^= ~0ull;
		}
		else if (symbol <= (short)'9') {  // numbers
			currentSet |= 1ull << (symbol - (short)'0');
		}
		else if (symbol < (short)'Z') {  // capital letters
			currentSet |= 1ull << (numbersAmount + (symbol - (short)'A'));
		}
		else {  // lower case letters
			currentSet |= 1ull << (numbersAmount + lettersAmount + (symbol - (short)'a'));
		}
		symbol = getchar();
	}
	printSet(resultSet);
	return 0;
}
