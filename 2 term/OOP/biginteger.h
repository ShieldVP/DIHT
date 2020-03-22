#include <string>
#include <vector>

typedef long long digit;

class BigInteger {
public:
    // Casts
    BigInteger();
    BigInteger(long long number);
    BigInteger(size_t number);
    BigInteger(int number);
    BigInteger(const std::string& number);
    BigInteger(const char* number);
    std::string toString() const;
    operator std::string() const;
    operator int() const;
    operator bool() const;
    // Functions
    BigInteger abs() const;
    // Comparing
    bool operator ==(const BigInteger& other) const;
    bool operator !=(const BigInteger& other) const;
    bool operator <(const BigInteger& other) const;
    bool operator >(const BigInteger& other) const;
    bool operator <=(const BigInteger& other) const;
    bool operator >=(const BigInteger& other) const;
    // Operations
    BigInteger& operator =(const std::string& number);
    BigInteger& operator =(const char* number);
    BigInteger& operator =(long long number);
    BigInteger& operator =(size_t number);
    BigInteger& operator =(int number);
    BigInteger& operator =(const BigInteger& number);
    BigInteger& operator ++();
    const BigInteger operator ++(int);
    BigInteger& operator --();
    const BigInteger operator --(int);
    const BigInteger operator +(const BigInteger& other) const;
    const BigInteger operator -(const BigInteger& other) const;
    const BigInteger operator *(const BigInteger& other) const;
    const BigInteger operator /(const BigInteger& other) const;
    const BigInteger operator %(const BigInteger& other) const;
    const std::pair<BigInteger, BigInteger> smartDiv(const BigInteger& other);
    BigInteger& operator +=(const BigInteger& other);
    BigInteger& operator -=(const BigInteger& other);
    BigInteger& operator *=(const BigInteger& other);
    BigInteger& operator /=(const BigInteger& other);
    BigInteger& operator %=(const BigInteger& other);
    BigInteger operator +() const;
    const BigInteger operator -() const;
private:
    const long long BASE = 1e9;
    const unsigned short int STEP = 9;
    bool negative = false;
    bool zero = true;
    std::vector<long long> digits;
};

BigInteger operator "" _bi(const char* number) {
    return BigInteger(number);
}

BigInteger BigInteger::abs() const {
    if (negative) return -(*this);
    else return *this;
}

BigInteger::BigInteger() {
    digits.push_back(0);
    negative = false;
    zero = true;
}

BigInteger::BigInteger(long long number) {
    *this = number;
}

BigInteger::BigInteger(int number) {
    *this = number;
}

BigInteger::BigInteger(size_t number) {
    *this = number;
}

BigInteger::BigInteger(const std::string& number) {
    *this = number;
}

BigInteger::BigInteger(const char* number) {
    *this = number;
}

BigInteger::operator std::string() const {
    return toString();
}

BigInteger::operator int() const {
    if (abs() > 2147483647_bi && *this != -2147483648_bi) return 1;
    else return std::stoi(toString());
}

BigInteger::operator bool() const {
    return !zero;
}

std::string BigInteger::toString() const {
    if (zero) return "0";
    std::string result = negative ? "-" : "";
    for (size_t i = 0; i < digits.size(); ++i) {
        std::string tmp = std::to_string(digits[digits.size() - 1 - i]);
        if (i != 0) result += std::string(STEP - tmp.size(), '0');
        result += tmp;
    }
    return result;
}

bool BigInteger::operator ==(const BigInteger& other) const {
    return (zero && other.zero) || (negative == other.negative && digits == other.digits);
}

bool BigInteger::operator !=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator <(const BigInteger& other) const {
    return *this <= other && *this != other;
}

bool BigInteger::operator >(const BigInteger& other) const {
    return !(*this == other || *this < other);
}

bool BigInteger::operator <=(const BigInteger& other) const {
    bool result = true;
    if (negative && !other.negative) result = true;
    else if (!negative && other.negative) result = false;
    else if (digits.size() < other.digits.size()) result = !negative;
    else if (digits.size() > other.digits.size()) result = negative;
    else {
        if (negative) result = false;
        for (size_t i = 0; i < digits.size(); ++i)
            if (digits[digits.size() - 1 - i] > other.digits[digits.size() - 1 - i]) {
                result = !result;
                break;
            }
            else if (digits[digits.size() - 1 - i] < other.digits[digits.size() - 1 - i]) break;
    }
    return result;
}

bool BigInteger::operator >=(const BigInteger& other) const {
    return *this == other || *this > other;
}

// Operations

std::ostream& operator <<(std::ostream& stream, const BigInteger& number) {
    stream << number.toString();
    return stream;
}

std::istream& operator >>(std::istream& stream, BigInteger& number) {
    std::string tmp;
    stream >> tmp;
    number = tmp;
    return stream;
}

BigInteger& BigInteger::operator =(const std::string& number) {
    digits.clear();
    negative = number[0] == '-';
    for (digit i = number.size(); i > 0; i -= STEP)
        if (i < STEP) digits.push_back(stoull(number.substr(negative, i - negative)));
        else digits.push_back(stoull(number.substr(i - STEP, STEP)));
    while (!digits.empty() && digits.back() == 0) digits.pop_back();
    zero = digits.empty();
    if (zero) digits.push_back(0);
    return *this;
}

BigInteger& BigInteger::operator =(const char* number) {
    return *this = std::string(number);
}

BigInteger& BigInteger::operator =(long long number) {
    return *this = std::to_string(number);
}

BigInteger& BigInteger::operator =(int number) {
    return *this = std::to_string(number);
}

BigInteger& BigInteger::operator =(size_t number) {
    return *this = std::to_string(number);
}

BigInteger& BigInteger::operator =(const BigInteger& number) {
    digits = number.digits;
    negative = number.negative;
    zero = number.zero;
    return *this;
}

BigInteger& BigInteger::operator ++() {
    if (negative) {
        negative = false;
        *this -= 1;
        if (!zero) negative = true;
    }
    else *this += 1;
    return *this;
}

const BigInteger BigInteger::operator ++(int) {
    auto tmp = *this;
    *this += 1;
    return tmp;
}

BigInteger& BigInteger::operator --() {
    if (negative) {
        negative = false;
        *this += 1;
        negative = true;
    }
    else *this -= 1;
    return *this;
}

const BigInteger BigInteger::operator --(int) {
    auto tmp = *this;
    *this -= 1;
    return tmp;
}

const BigInteger BigInteger::operator +(const BigInteger& other) const {
    BigInteger tmp = *this;
    return tmp += other;
}

const BigInteger BigInteger::operator -(const BigInteger& other) const {
    BigInteger tmp = *this;
    return tmp -= other;
}

const BigInteger BigInteger::operator *(const BigInteger& other) const {
    BigInteger tmp = *this;
    return tmp *= other;
}

const BigInteger BigInteger::operator /(const BigInteger& other) const {
    BigInteger tmp = *this;
    return tmp /= other;
}

const BigInteger BigInteger::operator %(const BigInteger& other) const {
    BigInteger tmp = *this;
    return tmp %= other;
}

const std::pair<BigInteger, BigInteger> BigInteger::smartDiv(const BigInteger& other) {
    if (other.zero) return std::make_pair(other, other); // atata
    else if (abs() == other.abs()) return std::make_pair(negative == other.negative ? 1 : -1, 0);
    else if (abs() < other.abs()) return std::make_pair(0, *this);
    else if (other == "1") return std::make_pair(*this, 0);
    else if (other == "-1") return std::make_pair(-(*this), 0);
    else { // this > other > 0
        BigInteger minued, origin = *this, oth = other.abs();
        origin.negative = negative != other.negative;
        for (long long i = (long long)digits.size() - 1; i >= 0; --i) {
            minued.digits.insert(minued.digits.begin(), origin.digits[i]);
            while (!minued.digits.empty() && minued.digits.back() == 0) minued.digits.pop_back();
            if (minued.digits.empty()) minued = 0;
            else minued.zero = false;
            if (minued >= oth) {
                long long lb = 0, rb = BASE;
                while (lb <= rb) {
                    auto middle = (lb + rb) / 2;
                    if (oth * BigInteger(middle) <= minued) {
                        lb = middle + 1;
                        origin.digits[i] = middle;
                    }
                    else rb = middle - 1;
                }
                minued -= oth * BigInteger(origin.digits[i]);
            }
            else origin.digits[i] = 0;
        }
        while (!origin.digits.empty() && origin.digits.back() == 0) origin.digits.pop_back();
        return std::make_pair(origin, minued);
    }
}

BigInteger& BigInteger::operator +=(const BigInteger& other) {
    if (other.zero) return *this;
    else if (zero) return *this = other;
    else if (negative && other.negative) return *this = -(-(*this) + (-other)); // negative + negative
    else if (!negative && other.negative) return *this = *this - (-other); // not_negative + negative
    else if (negative && !other.negative) return *this = other - (-(*this)); // negative + not_negative
    else { // not_negative + not_negative
        bool dischargeTransferring = false;
        size_t maxLen = digits.size() > other.digits.size() ? digits.size() : other.digits.size();
        for (size_t i = 0; i < maxLen || dischargeTransferring; ++i) {
            if (i == digits.size()) digits.push_back(0);
            if (dischargeTransferring) ++digits[i];
            if (i < other.digits.size()) digits[i] += other.digits[i];
            dischargeTransferring = digits[i] >= BASE;
            if (dischargeTransferring) digits[i] -= BASE;
        }
        return *this;
    }
}

BigInteger& BigInteger::operator -=(const BigInteger& other) {
    if (other.zero) return *this;
    else if (zero) return *this = -other;
    else if (other.negative) return *this = *this + (-other);
    else if (negative) return *this = -(-(*this) + other);
    else if (*this == other) return *this = 0; // a - a = 0 change my mind
    else if (*this < other) return *this = -(other - *this);
    else { // both are positive and this > other
        bool dischargeTransferring = false;
        for (size_t i = 0; i < other.digits.size() || dischargeTransferring; ++i) {
            if (dischargeTransferring) --digits[i];
            if (i < other.digits.size()) digits[i] -= other.digits[i];
            dischargeTransferring = digits[i] < 0;
            if (dischargeTransferring) digits[i] += BASE;
        }
        while (!digits.empty() && digits.back() == 0) digits.pop_back();
        return *this;
    }
}

BigInteger& BigInteger::operator *=(const BigInteger& other) {
        if (zero || other.zero) return *this = 0;
        else if (other == "1") return *this;
        else if (*this == "1") return *this = other;
        else if (*this == "-1") return *this = -other;
        else if (other == "-1") return *this = -(*this);
        else if (negative && other.negative) return *this = -(*this) * -other;
        else if (negative && !other.negative) return *this = -(-(*this) * other);
        else if (!negative && other.negative) return *this = -(*this * -other);
        else {
            if (other.digits.size() == 1) {
                if (digits.size() == 1) return *this = digits[0] * other.digits[0];
                else {
                    long long tmp = 0;
                    for (unsigned long long i = 0; i < digits.size(); ++i) {
                        tmp += digits[i] * other.digits[0];
                        digits[i] = tmp % BASE;
                        tmp /= BASE;
                    }
                    while(tmp > 0){
                        digits.push_back(tmp % BASE);
                        tmp /= BASE;
                    }
                    return *this;
                }
            }
            else if (digits.size() == 1) {
                auto tmp = other;
                return *this = tmp *= *this;
            }
            size_t len = digits.size() > other.digits.size() ? digits.size() : other.digits.size();
            if (len % 2 != 0) ++len; // making len even
            BigInteger a1, a0, b1, b0;
            a0.digits.pop_back(); a1.digits.pop_back(); b0.digits.pop_back(); b1.digits.pop_back();
            a0.zero = false; a1.zero = false; b0.zero = false; b1.zero = false;
            for (unsigned long long i = 0; i < len; ++i) {
                if (i < len / 2) { // a0, b0
                    if (i < digits.size()) a0.digits.push_back(digits[i]);
                    else a0.digits.push_back(0);
                    if (i < other.digits.size()) b0.digits.push_back(other.digits[i]);
                    else b0.digits.push_back(0);
                }
                else { // a1, b1
                    if (i < digits.size()) a1.digits.push_back(digits[i]);
                    else a1.digits.push_back(0);
                    if (i < other.digits.size()) b1.digits.push_back(other.digits[i]);
                    else b1.digits.push_back(0);
                }
            }
            while (!a0.digits.empty() && a0.digits.back() == 0) a0.digits.pop_back();
            if (a0.digits.empty()) a0 = 0;
            while (!a1.digits.empty() && a1.digits.back() == 0) a1.digits.pop_back();
            if (a1.digits.empty()) a1 = 0;
            while (!b0.digits.empty() && b0.digits.back() == 0) b0.digits.pop_back();
            if (b0.digits.empty()) b0 = 0;
            while (!b1.digits.empty() && b1.digits.back() == 0) b1.digits.pop_back();
            if (b1.digits.empty()) b1 = 0;
            auto lefts = a1 * b1, rights = a0 * b0, middle = (a1 + a0) * (b1 + b0) - lefts - rights; // Karatsuba's method
            lefts.digits.insert(lefts.digits.begin(), len, 0); // * 10^(len)
            middle.digits.insert(middle.digits.begin(), len / 2, 0); // * 10^(len / 2)
            return *this = lefts + middle + rights;
        }
    }

BigInteger& BigInteger::operator /=(const BigInteger& other) {
    return *this = smartDiv(other).first;
}

BigInteger& BigInteger::operator %=(const BigInteger& other) {
    return *this = smartDiv(other).second;
}

BigInteger BigInteger::operator +() const {
    return *this;
}

const BigInteger BigInteger::operator -() const {
    BigInteger tmp = *this;
    tmp.negative = !negative;
    return tmp;
}

// All types of left ==

bool operator ==(const BigInteger& left, int right) {
    return left == BigInteger(right);
}

bool operator ==(const BigInteger& left, long long right) {
    return left == BigInteger(right);
}

bool operator ==(const BigInteger& left, size_t right) {
    return left == BigInteger(right);
}

bool operator ==(const BigInteger& left, char* right) {
    return left == BigInteger(right);
}

bool operator ==(const BigInteger& left, const std::string& right) {
    return left == BigInteger(right);
}

// All types of left !=

bool operator !=(const BigInteger& left, int right) {
    return left != BigInteger(right);
}

bool operator !=(const BigInteger& left, long long right) {
    return left != BigInteger(right);
}

bool operator !=(const BigInteger& left, size_t right) {
    return left != BigInteger(right);
}

bool operator !=(const BigInteger& left, char* right) {
    return left != BigInteger(right);
}

bool operator !=(const BigInteger& left, const std::string& right) {
    return left != BigInteger(right);
}

// All types of left <

bool operator <(const BigInteger& left, int right) {
    return left < BigInteger(right);
}

bool operator <(const BigInteger& left, long long right) {
    return left < BigInteger(right);
}

bool operator <(const BigInteger& left, size_t right) {
    return left < BigInteger(right);
}

bool operator <(const BigInteger& left, char* right) {
    return left < BigInteger(right);
}

bool operator <(const BigInteger& left, const std::string& right) {
    return left < BigInteger(right);
}

// All types of left <=

bool operator <=(const BigInteger& left, int right) {
    return left <= BigInteger(right);
}

bool operator <=(const BigInteger& left, long long right) {
    return left <= BigInteger(right);
}

bool operator <=(const BigInteger& left, size_t right) {
    return left <= BigInteger(right);
}

bool operator <=(const BigInteger& left, char* right) {
    return left <= BigInteger(right);
}

bool operator <=(const BigInteger& left, const std::string& right) {
    return left <= BigInteger(right);
}

// All types of left >

bool operator >(const BigInteger& left, int right) {
    return left > BigInteger(right);
}

bool operator >(const BigInteger& left, long long right) {
    return left > BigInteger(right);
}

bool operator >(const BigInteger& left, size_t right) {
    return left > BigInteger(right);
}

bool operator >(const BigInteger& left, char* right) {
    return left > BigInteger(right);
}

bool operator >(const BigInteger& left, const std::string& right) {
    return left > BigInteger(right);
}

// All types of left >=

bool operator >=(const BigInteger& left, int right) {
    return left >= BigInteger(right);
}

bool operator >=(const BigInteger& left, long long right) {
    return left >= BigInteger(right);
}

bool operator >=(const BigInteger& left, size_t right) {
    return left >= BigInteger(right);
}

bool operator >=(const BigInteger& left, char* right) {
    return left >= BigInteger(right);
}

bool operator >=(const BigInteger& left, const std::string& right) {
    return left >= BigInteger(right);
}

// All types of right ==

bool operator ==(int left, const BigInteger& right) {
    return BigInteger(left) == right;
}

bool operator ==(long long left, const BigInteger& right) {
    return BigInteger(left) == right;
}

bool operator ==(size_t left, const BigInteger& right) {
    return BigInteger(left) == right;
}

bool operator ==(char* left, const BigInteger& right) {
    return BigInteger(left) == right;
}

bool operator ==(const std::string& left, const BigInteger& right) {
    return BigInteger(left) == right;
}

// All types of right !=

bool operator !=(int left, const BigInteger& right) {
    return BigInteger(left) != right;
}

bool operator !=(long long left, const BigInteger& right) {
    return BigInteger(left) != right;
}

bool operator !=(size_t left, const BigInteger& right) {
    return BigInteger(left) != right;
}

bool operator !=(char* left, const BigInteger& right) {
    return BigInteger(left) != right;
}

bool operator !=(const std::string& left, const BigInteger& right) {
    return BigInteger(left) != right;
}

// All types of right <

bool operator <(int left, const BigInteger& right) {
    return BigInteger(left) < right;
}

bool operator <(long long left, const BigInteger& right) {
    return BigInteger(left) < right;
}

bool operator <(size_t left, const BigInteger& right) {
    return BigInteger(left) < right;
}

bool operator <(char* left, const BigInteger& right) {
    return BigInteger(left) < right;
}

bool operator <(const std::string& left, const BigInteger& right) {
    return BigInteger(left) < right;
}

// All types of right <=

bool operator <=(int left, const BigInteger& right) {
    return BigInteger(left) <= right;
}

bool operator <=(long long left, const BigInteger& right) {
    return BigInteger(left) <= right;
}

bool operator <=(size_t left, const BigInteger& right) {
    return BigInteger(left) <= right;
}

bool operator <=(char* left, const BigInteger& right) {
    return BigInteger(left) <= right;
}

bool operator <=(const std::string& left, const BigInteger& right) {
    return BigInteger(left) <= right;
}

// All types of right >

bool operator >(int left, const BigInteger& right) {
    return BigInteger(left) > right;
}

bool operator >(long long left, const BigInteger& right) {
    return BigInteger(left) > right;
}

bool operator >(size_t left, const BigInteger& right) {
    return BigInteger(left) > right;
}

bool operator >(char* left, const BigInteger& right) {
    return BigInteger(left) > right;
}

bool operator >(const std::string& left, const BigInteger& right) {
    return BigInteger(left) > right;
}

// All types of right >=

bool operator >=(int left, const BigInteger& right) {
    return BigInteger(left) >= right;
}

bool operator >=(long long left, const BigInteger& right) {
    return BigInteger(left) >= right;
}

bool operator >=(size_t left, const BigInteger& right) {
    return BigInteger(left) >= right;
}

bool operator >=(char* left, const BigInteger& right) {
    return BigInteger(left) >= right;
}

bool operator >=(const std::string& left, const BigInteger& right) {
    return BigInteger(left) >= right;
}

// All types of left + for BigInteger

const BigInteger operator +(const BigInteger& left, int right) {
    return left + BigInteger(right);
}

const BigInteger operator +(const BigInteger& left, size_t right) {
    return left + BigInteger(right);
}

const BigInteger operator +(const BigInteger& left, long long right) {
    return left + BigInteger(right);
}

const BigInteger operator +(const BigInteger& left, char* right) {
    return left + BigInteger(right);
}

const BigInteger operator +(const BigInteger& left, const std::string& right) {
    return left + BigInteger(right);
}

// All types of left - for BigInteger

const BigInteger operator -(const BigInteger& left, int right) {
    return left - BigInteger(right);
}

const BigInteger operator -(const BigInteger& left, size_t right) {
    return left - BigInteger(right);
}

const BigInteger operator -(const BigInteger& left, long long right) {
    return left - BigInteger(right);
}

const BigInteger operator -(const BigInteger& left, char* right) {
    return left - BigInteger(right);
}

const BigInteger operator -(const BigInteger& left, const std::string& right) {
    return left - BigInteger(right);
}

// All types of left * for BigInteger

const BigInteger operator *(const BigInteger& left, int right) {
    return left * BigInteger(right);
}

const BigInteger operator *(const BigInteger& left, size_t right) {
    return left * BigInteger(right);
}

const BigInteger operator *(const BigInteger& left, long long right) {
    return left * BigInteger(right);
}

const BigInteger operator *(const BigInteger& left, char* right) {
    return left * BigInteger(right);
}

const BigInteger operator *(const BigInteger& left, const std::string& right) {
    return left * BigInteger(right);
}

// All types of left / for BigInteger

const BigInteger operator /(const BigInteger& left, int right) {
    return left / BigInteger(right);
}

const BigInteger operator /(const BigInteger& left, size_t right) {
    return left / BigInteger(right);
}

const BigInteger operator /(const BigInteger& left, long long right) {
    return left / BigInteger(right);
}

const BigInteger operator /(const BigInteger& left, char* right) {
    return left / BigInteger(right);
}

const BigInteger operator /(const BigInteger& left, const std::string& right) {
    return left / BigInteger(right);
}

// All types of left % for BigInteger

const BigInteger operator %(const BigInteger& left, int right) {
    return left % BigInteger(right);
}

const BigInteger operator %(const BigInteger& left, size_t right) {
    return left % BigInteger(right);
}

const BigInteger operator %(const BigInteger& left, long long right) {
    return left % BigInteger(right);
}

const BigInteger operator %(const BigInteger& left, char* right) {
    return left % BigInteger(right);
}

const BigInteger operator %(const BigInteger& left, const std::string& right) {
    return left % BigInteger(right);
}

// All types of right + for BigInteger

const BigInteger operator +(int left, const BigInteger& right) {
    return BigInteger(left) + right;
}

const BigInteger operator +(size_t left, const BigInteger& right) {
    return BigInteger(left) + right;
}

const BigInteger operator +(long long left, const BigInteger& right) {
    return BigInteger(left) + right;
}

const BigInteger operator +(char* left, const BigInteger& right) {
    return BigInteger(left) + right;
}

const BigInteger operator +(const std::string& left, const BigInteger& right) {
    return BigInteger(left) + right;
}

// All types of right - for BigInteger

const BigInteger operator -(int left, const BigInteger& right) {
    return BigInteger(left) - right;
}

const BigInteger operator -(size_t left, const BigInteger& right) {
    return BigInteger(left) - right;
}

const BigInteger operator -(long long left, const BigInteger& right) {
    return BigInteger(left) - right;
}

const BigInteger operator -(char* left, const BigInteger& right) {
    return BigInteger(left) - right;
}

const BigInteger operator -(const std::string& left, const BigInteger& right) {
    return BigInteger(left) - right;
}

// All types of right * for BigInteger

const BigInteger operator *(int left, const BigInteger& right) {
    return BigInteger(left) * right;
}

const BigInteger operator *(size_t left, const BigInteger& right) {
    return BigInteger(left) * right;
}

const BigInteger operator *(long long left, const BigInteger& right) {
    return BigInteger(left) * right;
}

const BigInteger operator *(char* left, const BigInteger& right) {
    return BigInteger(left) * right;
}

const BigInteger operator *(const std::string& left, const BigInteger& right) {
    return BigInteger(left) * right;
}

// All types of right / for BigInteger

const BigInteger operator /(int left, const BigInteger& right) {
    return BigInteger(left) / right;
}

const BigInteger operator /(size_t left, const BigInteger& right) {
    return BigInteger(left) / right;
}

const BigInteger operator /(long long left, const BigInteger& right) {
    return BigInteger(left) / right;
}

const BigInteger operator /(char* left, const BigInteger& right) {
    return BigInteger(left) / right;
}

const BigInteger operator /(const std::string& left, const BigInteger& right) {
    return BigInteger(left) / right;
}

// All types of right % for BigInteger

const BigInteger operator %(int left, const BigInteger& right) {
    return BigInteger(left) % right;
}

const BigInteger operator %(size_t left, const BigInteger& right) {
    return BigInteger(left) % right;
}

const BigInteger operator %(long long left, const BigInteger& right) {
    return BigInteger(left) % right;
}

const BigInteger operator %(char* left, const BigInteger& right) {
    return BigInteger(left) % right;
}

const BigInteger operator %(const std::string& left, const BigInteger& right) {
    return BigInteger(left) % right;
}

/// Rational

class Rational {
public:
    // Casts
    Rational();
    Rational(int number);
    Rational(const BigInteger& number);
    Rational(const Rational& number);
    std::string toString() const;
    std::string asDecimal(size_t precision) const;
    operator double() const;
    // Comparing
    bool operator ==(const Rational& other) const;
    bool operator !=(const Rational& other) const;
    bool operator <(const Rational& other) const;
    bool operator >(const Rational& other) const;
    bool operator <=(const Rational& other) const;
    bool operator >=(const Rational& other) const;
    // Operations
    Rational& operator =(const BigInteger& number);
    Rational& operator =(int number);
    Rational& operator =(const Rational& number);
    const Rational operator +(const Rational& other) const;
    const Rational operator -(const Rational& other) const;
    const Rational operator *(const Rational& other) const;
    const Rational operator /(const Rational& other) const;
    Rational& operator +=(const Rational& other);
    Rational& operator -=(const Rational& other);
    Rational& operator *=(const Rational& other);
    Rational& operator /=(const Rational& other);
    Rational operator +() const;
    const Rational operator -() const;
private:
    BigInteger numerator, denominator;
    BigInteger nod() const;
};

BigInteger Rational::nod() const {
    BigInteger m = numerator.abs(), n = denominator;
    while (n != 0) {
        m %= n;
        std::swap(m, n);
    }
    return m;
}

std::string Rational::asDecimal(size_t precision = 0) const {
    std::string result = numerator > 0 ? "" : "-";
    auto numeratorWork = numerator;
    auto firstDiv = numeratorWork.smartDiv(denominator);
    auto div = firstDiv.first, mod = firstDiv.second.abs();
    if (mod != 0) {
        BigInteger tmp = mod.toString() + std::string(precision, '0');
        auto secondDiv = tmp.smartDiv(denominator);
        std::string divOst = (secondDiv.first + (2 * secondDiv.second >= denominator ? 1 : 0)).toString();
        if (divOst.size() > precision) result += (++div).toString();
        else {
            divOst = std::string(precision - divOst.size(), '0') + divOst;
            while (divOst.back() == '0') divOst.pop_back();
            result += div.toString();
            if (!divOst.empty()) result += "." + divOst;
        }
    }
    return result;
}

Rational::Rational() {
    numerator = 0;
    denominator = 1;
}

Rational::Rational(int number) {
    numerator = number;
    denominator = 1;
}

Rational::Rational(const BigInteger& number) {
    numerator = number;
    denominator = 1;
}

Rational::Rational(const Rational& number) {
    numerator = number.numerator;
    denominator = number.denominator;
}

Rational::operator double() const {
    return stod(asDecimal(10));
}

std::string Rational::toString() const {
    return numerator.toString() + (denominator == 1 ? "" : "/" + denominator.toString());
}

bool Rational::operator ==(const Rational& other) const {
    return numerator == other.numerator && denominator == other.denominator;
}

bool Rational::operator !=(const Rational& other) const {
    return !(*this == other);
}

bool Rational::operator <(const Rational& other) const {
    if (numerator < 0 && other.numerator >= 0) return true;
    else if (numerator == 0 && other.numerator > 0) return true;
    else if (numerator == 0 && other.numerator < 0) return false;
    else if (numerator > 0 && other.numerator <= 0) return false;
    else return *this - other < Rational(0);
}

bool Rational::operator >(const Rational& other) const {
    return !(*this == other || *this < other);
}

bool Rational::operator <=(const Rational& other) const {
    return *this < other || *this == other;
}

bool Rational::operator >=(const Rational& other) const {
    return *this == other || *this > other;
}

// Operations

Rational& Rational::operator =(int number) {
    numerator = number;
    denominator = 1;
    return *this;
}

Rational& Rational::operator =(const BigInteger& number) {
    numerator = number;
    denominator = 1;
    return *this;
}

Rational& Rational::operator =(const Rational& number) {
    numerator = number.numerator;
    denominator = number.denominator;
    return *this;
}

const Rational Rational::operator +(const Rational& other) const {
    Rational tmp = *this;
    return tmp += other;
}

const Rational Rational::operator -(const Rational& other) const {
    Rational tmp = *this;
    return tmp -= other;
}

const Rational Rational::operator *(const Rational& other) const {
    Rational tmp = *this;
    return tmp *= other;
}

const Rational Rational::operator /(const Rational& other) const {
    Rational tmp = *this;
    return tmp /= other;
}

Rational& Rational::operator +=(const Rational& other) {
    if (other.numerator == 0) return *this;
    else if (numerator == 0) return *this = other;
    else { // not_negative + not_negative
        if (denominator == other.denominator) numerator += other.numerator;
        else {
            numerator = numerator * other.denominator + denominator * other.numerator;
            denominator *= other.denominator;
        }
        auto NOD = nod();
        numerator /= NOD;
        denominator /= NOD;
        return *this;
    }
}

Rational& Rational::operator -=(const Rational& other) {
    if (other.numerator == 0) return *this;
    else if (numerator == 0) return *this = -other;
    else if (*this == other) return *this = 0; // a - a = 0 change my mind
    else { // both are positive and this > other
        if (denominator == other.denominator) numerator -= other.numerator;
        else {
            numerator = numerator * other.denominator - denominator * other.numerator;
            denominator *= other.denominator;
        }
        auto NOD = nod();
        numerator /= NOD;
        denominator /= NOD;
        return *this;
    }
}

Rational& Rational::operator *=(const Rational& other) {
    if (numerator == 0 || other.numerator == 0) return *this = 0;
    else if (other == Rational(1)) return *this;
    else if (*this == Rational(1)) return *this = other;
    else if (*this == Rational(1)) return *this = -other;
    else if (other == Rational(1)) return *this = -(*this);
    else {
        numerator *= other.numerator;
        denominator *= other.denominator;
        auto NOD = nod();
        numerator /= NOD;
        denominator /= NOD;
        return *this;
    }
}

Rational& Rational::operator /=(const Rational& other) {
    if (other.numerator == 0) return *this = other; // atata
    else { // this > other != 0
        numerator *= other.denominator;
        denominator *= other.numerator;
        if (denominator < 0) {
            numerator *= -1;
            denominator *= -1;
        }
        auto NOD = nod();
        numerator /= NOD;
        denominator /= NOD;
        return *this;
    }
}

Rational Rational::operator +() const {
    return *this;
}

const Rational Rational::operator -() const {
    Rational tmp = *this;
    tmp.numerator = -tmp.numerator;
    return tmp;
}

// All types of left ==

bool operator ==(const Rational& left, int right) {
    return left == Rational(right);
}

bool operator ==(const Rational& left, const BigInteger& right) {
    return left == Rational(right);
}

// All types of left !=

bool operator !=(const Rational& left, int right) {
    return left != Rational(right);
}

bool operator !=(const Rational& left, const BigInteger& right) {
    return left != Rational(right);
}

// All types of left <

bool operator <(const Rational& left, int right) {
    return left < Rational(right);
}

bool operator <(const Rational& left, const BigInteger& right) {
    return left < Rational(right);
}

// All types of left <=

bool operator <=(const Rational& left, int right) {
    return left <= Rational(right);
}

bool operator <=(const Rational& left, const BigInteger& right) {
    return left <= Rational(right);
}

// All types of left >

bool operator >(const Rational& left, int right) {
    return left > Rational(right);
}

bool operator >(const Rational& left, const BigInteger& right) {
    return left > Rational(right);
}

// All types of left >=

bool operator >=(const Rational& left, int right) {
    return left >= Rational(right);
}

bool operator >=(const Rational& left, const BigInteger& right) {
    return left >= Rational(right);
}

// All types of right ==

bool operator ==(int left, const Rational& right) {
    return Rational(left) == right;
}

bool operator ==(const BigInteger& left, const Rational& right) {
    return Rational(left) == right;
}

// All types of right !=

bool operator !=(int left, const Rational& right) {
    return Rational(left) != right;
}

bool operator !=(const BigInteger& left, const Rational& right) {
    return Rational(left) != right;
}

// All types of right <

bool operator <(int left, const Rational& right) {
    return Rational(left) < right;
}

bool operator <(const BigInteger& left, const Rational& right) {
    return Rational(left) < right;
}

// All types of right <=

bool operator <=(int left, const Rational& right) {
    return Rational(left) <= right;
}

bool operator <=(const BigInteger& left, const Rational& right) {
    return Rational(left) <= right;
}

// All types of right >

bool operator >(int left, const Rational& right) {
    return Rational(left) > right;
}

bool operator >(const BigInteger& left, const Rational& right) {
    return Rational(left) > right;
}

// All types of right >=

bool operator >=(int left, const Rational& right) {
    return Rational(left) >= right;
}

bool operator >=(const BigInteger& left, const Rational& right) {
    return Rational(left) >= right;
}

// All types of left + for Rational

const Rational operator +(const Rational& left, int right) {
    return left + Rational(right);
}

const Rational operator +(const Rational& left, const BigInteger& right) {
    return left + Rational(right);
}

// All types of left - for Rational

const Rational operator -(const Rational& left, int right) {
    return left - Rational(right);
}

const Rational operator -(const Rational& left, const BigInteger& right) {
    return left - Rational(right);
}

// All types of left * for Rational

const Rational operator *(const Rational& left, int right) {
    return left * Rational(right);
}

const Rational operator *(const Rational& left, const BigInteger& right) {
    return left * Rational(right);
}

// All types of left / for Rational

const Rational operator /(const Rational& left, int right) {
    return left / Rational(right);
}

const Rational operator /(const Rational& left, const BigInteger& right) {
    return left / Rational(right);
}

// All types of right + for Rational

const Rational operator +(int left, const Rational& right) {
    return Rational(left) + right;
}

const Rational operator +(const BigInteger& left, const Rational& right) {
    return Rational(left) + right;
}

// All types of right - for Rational

const Rational operator -(int left, const Rational& right) {
    return Rational(left) - right;
}

const Rational operator -(const BigInteger& left, const Rational& right) {
    return Rational(left) - right;
}

// All types of right * for Rational

const Rational operator *(int left, const Rational& right) {
    return Rational(left) * right;
}

const Rational operator *(const BigInteger& left, const Rational& right) {
    return Rational(left) * right;
}

// All types of right / for Rational

const Rational operator /(int left, const Rational& right) {
    return Rational(left) / right;
}

const Rational operator /(const BigInteger& left, const Rational& right) {
    return Rational(left) / right;
}