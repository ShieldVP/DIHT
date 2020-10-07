/*
 Для заданной строки s длины n (1 ≤ n ≤ 105) требуется подсчитать число пар (i, j),
 1 ≤ i < j ≤ n, таких что подстрока s[i..j] является палиндромом. 
*/

#include <iostream>
#include <string>
#include <vector>

static int min(int a, int b) {
    return a < b ? a : b;
}

size_t countPalindromes(const std::string& str) {
    int len = str.size(), len2 = len * 2;
    std::vector<int> d(len2 - 1);
    int l = 0, r = -1, overall = 0;
    for (int i = 0; i < len2 - 1; ++i) {
        d[i] = i > r ? 1 : min(d[l + r - i], r - i + 1);
        while ((i + d[i] < len2 - 1) && (i >= d[i]) && ((i - d[i]) % 2 == 1 || str[(i + d[i]) / 2] == str[(i - d[i]) / 2]))
            ++d[i];
        if (i + d[i] - 1 > r) // refresh (l, r) if new right border > r
            l = i - d[i] + 1, r = i + d[i] - 1;
        overall += i % 2 == 0 ? (d[i] - 1) / 2 : d[i] / 2; // refresh overall palindromes counter
    }
    return overall; // trivial and fake palindromes aren't count
}

int main() {
    std::string str;
    std::cin >> str;
    std::cout << countPalindromes(str) << std::endl;
    return 0;
}