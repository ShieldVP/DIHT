/*
 Найдите все вхождения шаблона в строку с помощью префикс-функции.
 Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
 p ≤ 30000, n ≤ 300000. 
*/

#include <iostream>
#include <vector>
using namespace std;

vector<int> prefixFunction(const string& str) {
    /*
    vector<int> pf(str.size() + 1, 0);
    pf[0] = -1;  // let prefix function of empty string be -1
    for (int i = 1; i < str.size() + 1; ++i) {
        for (int k = pf[i - 1]; k >= 0; k = pf[k])
            if (str[k] == str[i - 1]) {
                pf[i] = k + 1;
                break;
            }
    }
    return pf;
    */

    vector<int> pf(str.size(), 0);
    for (size_t i = 1; i < str.size(); ++i) {
        int j = pf[i-1];
        while (j > 0 && str[i] != str[j])
            j = pf[j-1];
        if (str[i] == str[j]) ++j;
        pf[i] = j;
    }
    return pf;
}

vector<int> KMP(const string& pattern) {
    auto p = prefixFunction(pattern);

    int last = p.back();
    char newSymbol = char(getchar());
    vector<int> result;
    for (int i = 0; newSymbol != EOF; ++i) {
        int j = last;
        while (j > 0 && newSymbol != pattern[j])
            j = p[j-1];
        if (newSymbol == pattern[j]) ++j;
        last = j;
        if (last == pattern.size()) result.push_back(i - last);
        newSymbol = char(getchar());
    }
    return result;
}

int main() {
    string pattern;
    cin >> pattern;
    for (auto position : KMP(pattern))
    	cout << position << ' ';
    return 0;
}