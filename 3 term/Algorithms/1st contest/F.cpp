/*
 Заданы две строки s, t и целое число k. Рассмотрим множество всех таких непустых строк, которые встречаются как подстроки в s и t одновременно.
 Найдите k-ую в лексикографическом порядке строку из этого множества. В первой строке выведите искомую строку или -1, если такой не существует.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>

class SuffixArray {
public:
    explicit SuffixArray(const std::string& str_);

    size_t size() const;
    const std::vector<size_t>& get() const;
    std::string getString() const;
    std::vector<size_t> getLCP();
    std::string getSubstr(size_t position, size_t length) const;

    size_t differentSubstringsNumber();

protected:
    std::vector<size_t> array;
    std::vector<size_t> LCP;
    size_t len;

private:
    std::string base;
    static const unsigned char ALPHABET = 128;  // there will be only ASCII symbols

    void construct();
    void sortChars(std::vector<size_t>& counter, std::vector<size_t>& classify, size_t& classesAmount);
    size_t getLCPFairly(size_t firstSuffix, size_t secondSuffix, size_t knownPrefix = 0);
};

SuffixArray::SuffixArray(const std::string &str) :
        base(str + char(0)),
        len(base.size())
{
    array.resize(len, 0);
    construct();
    array.erase(array.begin());
    base.pop_back();
    --len;
}

size_t SuffixArray::size() const {
    return len;
}

const std::vector<size_t>& SuffixArray::get() const {
    return array;
}

std::string SuffixArray::getString() const {
    return base;
}

std::vector<size_t> SuffixArray::getLCP() {
    if (!LCP.empty()) return LCP;
    std::vector<size_t> inverse(len);
    for (size_t i = 0; i < len; ++i)
        inverse[array[i]] = i;  // says order statistics in array by suffix
    LCP.resize(len - 1, 0);
    // descending from the longest suffix to the shortest
    size_t maxPrefix = 0;
    for (size_t i = 0; i < len; ++i) {
        if (inverse[i] == len - 1) continue;
        auto j = array[inverse[i] + 1];  // next substring in array
        maxPrefix = getLCPFairly(i, j, maxPrefix);
        if (maxPrefix != 0) LCP[inverse[i]] = maxPrefix--;
    }
    return LCP;
}

std::string SuffixArray::getSubstr(size_t position, size_t length) const {
    return base.substr(position, length);
}

size_t SuffixArray::differentSubstringsNumber() {
    size_t sum = (len * (len + 1)) >> 1u;
    for (auto lcp : getLCP())
        sum -= lcp;
    return sum;
}

void SuffixArray::construct() {
    std::vector<size_t> counter(ALPHABET, 0), classify(len, 0);
    size_t classesAmount = 1;
    // counter:  for each symbol counts its' occurrences in the text
    // array:    array of suffixes in lexicographic order
    // classify: relation from suffixes to the classes of equality

    // first iteration for substrings length of 1
    sortChars(counter, classify, classesAmount);

    std::vector<size_t> newArray(len), newClassify(len);
    for (size_t seg = 1; seg < len; seg <<= 1u) {
        for (size_t i = 0; i < len; ++i)
            newArray[i] = array[i] < seg ? array[i] + len - seg : array[i] - seg;
        counter.assign(classesAmount, 0);
        for (size_t i = 0; i < len; ++i)
            ++counter[classify[newArray[i]]];
        for (size_t i = 1; i < classesAmount; ++i)
            counter[i] += counter[i - 1];
        for (size_t i = 0; i < len; ++i)
            array[--counter[classify[newArray[len - 1 - i]]]] = newArray[len - 1 - i];
        newClassify[array[0]] = 0, classesAmount = 1;
        for (size_t i = 1; i < len; ++i) {
            size_t secondPartCurr = (array[i] + seg) % len, secondPartPrev = (array[i - 1] + seg) % len;
            if (classify[array[i]] != classify[array[i - 1]] ||
                classify[secondPartCurr] != classify[secondPartPrev])
                ++classesAmount;
            newClassify[array[i]] = classesAmount - 1;
        }
        classify = newClassify;
    }
}

void SuffixArray::sortChars(std::vector<size_t>& counter, std::vector<size_t>& classify, size_t& classesAmount)  {
    for (auto symbol : base)
        ++counter[symbol];
    for (size_t i = 1; i < ALPHABET; ++i)
        counter[i] += counter[i - 1];  // to reserve place in array for previous elements
    for (size_t i = 1; i < len; ++i)
        array[--counter[base[i]]] = i;  // filling array in classes backwards
    for (size_t i = 1; i < len; ++i) {
        if (base[array[i]] != base[array[i - 1]]) ++classesAmount;
        classify[array[i]] = classesAmount - 1;
    }
}

size_t SuffixArray::getLCPFairly(size_t firstSuffix, size_t secondSuffix, size_t knownPrefix) {
    while (firstSuffix + knownPrefix < len && secondSuffix + knownPrefix < len
           && base[firstSuffix + knownPrefix] == base[secondSuffix + knownPrefix])
        ++knownPrefix;
    return knownPrefix;
}


class ConnectedSuffixArray : SuffixArray {
public:
    ConnectedSuffixArray(const std::string& str1, const std::string& str2) :
    SuffixArray(str1 + char(1) + str2),
    len1(str1.size()),
    len2(str2.size())
    {}

    std::optional<std::string> commonSubstringInOrder(size_t number);

private:
    size_t len1, len2;
};

std::optional<std::string> ConnectedSuffixArray::commonSubstringInOrder(size_t number) {
    size_t currentNumber = 0;
    getLCP();
    auto localMin = LCP.front();
    for (size_t i = 1; i < LCP.size(); ++i) {
        localMin = std::min(localMin, LCP[i]);
        if (LCP[i] != 0 && (array[i] < len1 != array[i + 1] < len1)) {
            currentNumber += LCP[i] - localMin;
            if (currentNumber >= number) {
                currentNumber -= LCP[i];
                size_t resultLen = localMin;
                while (resultLen < number - currentNumber) ++resultLen;
                return getSubstr(array[i], resultLen);
            }
            localMin = LCP[i];
        }
    }
    return std::nullopt;
}

int main() {
    std::string str1, str2;
    size_t k;
    std::cin >> str1 >> str2 >> k;
    ConnectedSuffixArray arr(str1, str2);
    auto result = arr.commonSubstringInOrder(k);
    if (result) std::cout << *result;
    else std::cout << -1;
    return 0;
}