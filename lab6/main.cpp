#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <numeric>

class BigInt {
private:
    const static int DIGIT_LENGH = 4;
    const static int MAX_DIGIT = 1e4;

    std::vector<int> digits;

    void RemoveLeadingZeros();

public:
    BigInt();
    BigInt(const size_t& size);
    BigInt(const std::string& str);

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator^(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator==(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    friend std::ostream& operator<< (std::ostream& os, const BigInt& number);
    friend std::istream& operator>> (std::istream& is, BigInt& number);
};

BigInt::BigInt() : digits(1, 0) {};

BigInt::BigInt(const size_t& size) : digits(size, 0) {}

BigInt::BigInt(const std::string& str) {
    for (size_t i = str.size(); i > 0; (i < DIGIT_LENGH) ? i = 0 : i -= DIGIT_LENGH) {
        size_t length = (i < DIGIT_LENGH) ? i : DIGIT_LENGH;
        digits.push_back(std::stoi(str.substr(i - length, length)));
    }
}

void BigInt::RemoveLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

BigInt BigInt::operator+(const BigInt& other) const {
    BigInt result(std::max(digits.size(), other.digits.size()));
    int carry = 0;

    for (size_t i = 0; i < result.digits.size(); ++i) {
        int this_digit = i < digits.size() ? digits[i] : 0;
        int other_digit = i < other.digits.size() ? other.digits[i] : 0;

        int sum = this_digit + other_digit + carry;
        carry = sum / MAX_DIGIT;
        result.digits[i] = sum % MAX_DIGIT;
    }

    if (carry) {
        result.digits.push_back(carry);
    }

    result.RemoveLeadingZeros();
    return result;
}

BigInt BigInt::operator-(const BigInt& other) const {
    BigInt result(digits.size());
    int borrow = 0;

    for (size_t i = 0; i < result.digits.size(); ++i) {
        int this_digit = digits[i];
        int other_digit = i < other.digits.size() ? other.digits[i] : 0;

        int diff = this_digit - other_digit - borrow;

        if (diff < 0) {
            diff += MAX_DIGIT;
            borrow = 1;
        }
        else {
            borrow = 0;
        }
        result.digits[i] = diff;
    }

    result.RemoveLeadingZeros();
    return result;
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt result(digits.size() + other.digits.size());

    for (size_t i = 0; i < digits.size(); ++i) {
        long long carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            long long product = result.digits[i + j] +
                                carry +
                                static_cast<long long>(digits[i]) * (j < other.digits.size() ? other.digits[j] : 0);

            result.digits[i + j] = product % MAX_DIGIT;
            carry = product / MAX_DIGIT;
        }
    }

    result.RemoveLeadingZeros();
    return result;
}

BigInt BigInt::operator^(const BigInt& other) const {
    BigInt result("1");
    BigInt base = *this;
    BigInt exp = other;

    while (exp > BigInt("0")) {
        if ((exp.digits[0] % 2) == 1) {
            result = result * base;
        }
        base = base * base;
        exp = exp / BigInt("2");
    }

    result.RemoveLeadingZeros();
    return result;
}

BigInt BigInt::operator/(const BigInt &other) const {
    BigInt dividend = *this;
    BigInt divisor = other;
    BigInt result(dividend.digits.size());
    BigInt current;

    for (size_t i = dividend.digits.size(); i > 0; --i) {
        current.digits.insert(current.digits.begin(), dividend.digits[i - 1]);
        current.RemoveLeadingZeros();

        int low = 0, high = BigInt::MAX_DIGIT - 1;
        int quotient = 0;
        while (low <= high) {
            int mid = std::midpoint(low, high);
            BigInt product = divisor * BigInt(std::to_string(mid));
            if (product <= current) {
                quotient = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        current = current - (divisor * BigInt(std::to_string(quotient)));

        result.digits[i - 1] = quotient;
    }

    result.RemoveLeadingZeros();
    return result;
}


bool BigInt::operator<(const BigInt& other) const{
    if (digits.size() != other.digits.size()) {
        return digits.size() < other.digits.size();
    }

    for (size_t i = digits.size(); i > 0; --i) {
        if (digits[i - 1] != other.digits[i - 1]) {
            return digits[i - 1] < other.digits[i - 1];
        }
    }

    return false;
}

bool BigInt::operator>(const BigInt& other) const{
    return other < *this;
}

bool BigInt::operator==(const BigInt& other) const{
    return digits == other.digits;
}

bool BigInt::operator<=(const BigInt& other) const{
    return !(*this > other);
}

bool BigInt::operator>=(const BigInt& other) const{
    return !(*this < other);
}

std::ostream& operator<< (std::ostream& os, const BigInt& number) {
     if (number.digits.empty()) {
        os << "0";
        return os;
    }

    os << number.digits.back();

    for (size_t i = number.digits.size() - 1; i > 0; --i) {
        os << std::setw(BigInt::DIGIT_LENGH) << std::setfill('0') << number.digits[i - 1];
    }

    return os;
}

std::istream& operator>> (std::istream& is, BigInt& number) {
    std::string input;
    is >> input;
    
    number = BigInt(input);
    number.RemoveLeadingZeros();

    return is;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    BigInt number1, number2;
    char operation;

    while (std::cin >> number1 >> number2 >> operation) {
    
        if      (operation == '+') {
                BigInt res = number1 + number2;
                std::cout << res << '\n';
        }
        else if (operation == '-') {
            if (number1 < number2) {std::cout << "Error\n";}
            else {
                BigInt res = number1 - number2;
                std::cout << res << '\n';
            }
        }
        else if (operation == '*') {
                BigInt res = number1 * number2;
                std::cout << res << '\n';
        }
        else if (operation == '^') {   
            if (number1 == BigInt("0") and number2 == BigInt("0")) {std::cout << "Error\n";}
            else{std::cout << (number1 ^ number2) << '\n';}
        }
        else if (operation == '/') {
            if (number2 == BigInt("0")) {std::cout << "Error\n";}
            else {
                BigInt res = number1 / number2;
                std::cout << res << '\n';
            }
        }
        else if (operation == '<') {
            if (number1 < number2)  {std::cout << "true\n";} else {std::cout << "false\n";}
        }
        else if (operation == '>') {
            if (number1 > number2)  {std::cout << "true\n";} else {std::cout << "false\n";}
        }
        else if (operation == '=') {
            if (number1 == number2) {std::cout << "true\n";} else {std::cout << "false\n";}
        }
    }

    return 0;
}