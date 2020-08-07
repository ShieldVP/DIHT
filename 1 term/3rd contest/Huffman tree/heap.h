#include <vector>

template <typename ValueType>
class Heap {
public:
    ValueType top() const;
    void insert(const ValueType& element);
    void removeTop();
    bool empty() const;
    size_t size() const;

private:
    std::vector<ValueType> body;

    bool compare(const ValueType& a, const ValueType& b) const {
        return *a < *b;
    }
    void sieve(size_t n);
    bool haveChild(size_t n) const;
    size_t getMinChild(size_t n) const;
};

template<typename ValueType>
ValueType Heap<ValueType>::top() const {
    return body.front();
}

template<typename ValueType>
void Heap<ValueType>::insert(const ValueType &element) {
    body.push_back(element);
    sieve(body.size() - 1);
}

template<typename ValueType>
void Heap<ValueType>::removeTop() {
    std::swap(body[0], body[body.size() - 1]);
    body.pop_back();
    size_t current = 0;
    while (haveChild(current) && compare(body[getMinChild(current)], body[current])) {
        auto tmp = getMinChild(current);
        std::swap(body[tmp], body[current]);
        current = tmp;
    }
}

template<typename ValueType>
bool Heap<ValueType>::empty() const {
    return body.empty();
}

template<typename ValueType>
size_t Heap<ValueType>::size() const {
    return body.size();
}

template<typename ValueType>
void Heap<ValueType>::sieve(size_t n) {
    while (n > 0) {
        if (compare(body[n], body[n / 2])) std::swap(body[n], body[n / 2]);
        else break;
        n /= 2;
    }
}

template<typename ValueType>
bool Heap<ValueType>::haveChild(size_t n) const {
    return 2 * n + 1 < body.size();
}

template<typename ValueType>
size_t Heap<ValueType>::getMinChild(size_t n) const {
    return 2 * n + (2 * n + 2 >= body.size() || compare(body[2 * n + 1], body[2 * n + 2]) ? 1 : 2);
}

