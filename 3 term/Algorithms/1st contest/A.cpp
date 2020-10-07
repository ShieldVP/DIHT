/*
 Найдите все вхождения шаблона в строку с помощью префикс-функции.
 Длина шаблона – p, длина строки – n. Время O(n + p), доп. память – O(p).
 p ≤ 30000, n ≤ 300000. 
*/

#include <iostream>
#include <vector>
using namespace std;

vector<size_t> prefixFunction(const string& str) {
    vector<size_t> pf(str.size(), 0);
    for (size_t i = 1; i < str.size(); ++i) {
        auto lastPref = pf[i - 1];
        while (lastPref > 0 && str[i] != str[lastPref])
            lastPref = pf[lastPref - 1];
        pf[i] = lastPref + (str[i] == str[lastPref] ? 1 : 0);
    }
    return pf;
}

vector<size_t> KMP(const string& pattern) {
    auto patternPF = prefixFunction(pattern);
    auto last = patternPF.back();
    auto newSymbol = (char)getchar();
    vector<size_t> result;
    for (size_t i = 0; newSymbol != EOF; ++i) {
        auto lastPref = last;
        while (lastPref > 0 && newSymbol != pattern[lastPref])
            lastPref = patternPF[lastPref - 1];
        last = lastPref + (newSymbol == pattern[lastPref] ? 1 : 0);
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