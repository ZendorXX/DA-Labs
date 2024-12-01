#include <iostream>
#include <vector>
#include <algorithm>

class BigInt {
private:
    const static int DIGIT_LENGH = 4;
    const static int MAX_DIGIT = 1e4;

    std::vector<int> digits;

public:
    BigInt();
    BigInt(const size_t& size);
    BigInt(const std::string& str);

    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;

    void RemoveLeadingZeros();
    void ReverseDigits();
    size_t Size();

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator^(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

    bool operator<(BigInt& other);
    bool operator>(BigInt& other);
    bool operator==(BigInt& other);
    bool operator<=(BigInt& other);
    bool operator>=(BigInt& other);

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

BigInt::BigInt(const BigInt& other) : digits(other.digits) {}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this == &other) {
        return *this;
    }
    digits = other.digits;
    return *this;
}

BigInt::BigInt(BigInt&& other) noexcept : digits(std::move(other.digits)) {}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    digits = std::move(other.digits);
    return *this;
}

void BigInt::RemoveLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

void BigInt::ReverseDigits() {
    std::reverse(digits.begin(), digits.end());
}


size_t BigInt::Size() {
    return digits.size();
}

BigInt BigInt::operator+(const BigInt& other) const {
    size_t max_size = digits.size() > other.digits.size() ? digits.size() : other.digits.size();
    BigInt result(max_size);
    int this_digit = 0, other_digit = 0;
    bool flag = false;

    for (size_t i = 0; i < max_size; ++i) {
        this_digit = i < digits.size() ? digits[i] : 0;
        other_digit = i < other.digits.size() ? other.digits[i] : 0;

        int sum = this_digit + other_digit;
        
        if (flag) {
            ++sum;
        }

        if (sum > BigInt::MAX_DIGIT) {
            sum -= BigInt::MAX_DIGIT;
            flag = true;
        }
        else {
            flag = false;
        }
        result.digits[i] = sum;
    }


    if (flag) {
        result.digits.push_back(1);
    }

    return result;
}

BigInt BigInt::operator-(const BigInt& other) const {
    size_t max_size = digits.size() > other.digits.size() ? digits.size() : other.digits.size();
    BigInt result(max_size);
    int this_digit = 0, other_digit = 0;
    bool flag = false;

    for (size_t i = 0; i < max_size; ++i) {
        this_digit = i < digits.size() ? digits[i] : 0;
        other_digit = i < other.digits.size() ? other.digits[i] : 0;

        int diff = this_digit - other_digit;
        
        if (flag) {
            --diff;
        }

        if (diff < 0) {
            diff += BigInt::MAX_DIGIT;
            flag = true;
        }
        else {
            flag = false;
        }
        result.digits[i] = diff;
    }


    if (flag) {
        --result.digits.back();
    }

    return result;
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt result(digits.size() + other.digits.size());

    for (size_t i = 0; i < digits.size(); ++i) {
        for (size_t j = 0; j < other.digits.size(); ++j) {
            result.digits[i + j] += digits[i] * other.digits[j];
        }
    }

    for (size_t i = 0; i < result.Size() - 1; ++i) {
        result.digits[i + 1] += result.digits[i] / BigInt::MAX_DIGIT;
        result.digits[i] %= BigInt::MAX_DIGIT;
    }

    return result;
}

BigInt BigInt::operator^(const BigInt& other) const {
    BigInt result("1");
    BigInt exp = other;

    /*while (exp > BigInt("0")) {

    }*/
}

BigInt BigInt::operator/(const BigInt &other) const {
    BigInt dividend = *this;
    BigInt divisor = other;   
    BigInt result(dividend.digits.size());  
    BigInt current;           


    for (int i = dividend.Size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), dividend.digits[i]);
        current.RemoveLeadingZeros();

        int k = 0, left = 0, right = MAX_DIGIT - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            BigInt product = divisor * BigInt(std::to_string(mid));
            if (product <= current) {
                k = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        result.digits[i] = k;

        current = current - (divisor * BigInt(std::to_string(k)));
    }

    result.RemoveLeadingZeros();  
    return result;
}


bool BigInt::operator<(BigInt& other) {
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

bool BigInt::operator>(BigInt& other) {
    if (digits.size() != other.digits.size()) {
        return digits.size() > other.digits.size();
    }

    for (size_t i = digits.size(); i > 0; --i) {
        if (digits[i - 1] != other.digits[i - 1]) {
            return digits[i - 1] > other.digits[i - 1];
        }
    }


    return false;
}

bool BigInt::operator==(BigInt& other) {
    if (digits.size() != other.digits.size()) {
        return false;
    }

   for (size_t i = digits.size(); i > 0; --i) {
        if (digits[i - 1] != other.digits[i - 1]) {
            return false;
        }
    }

    return true;
}

bool BigInt::operator<=(BigInt& other) {
    return !(*this > other);
}

bool BigInt::operator>=(BigInt& other) {
    return !(*this < other);
}

std::ostream& operator<< (std::ostream& os, const BigInt& number) {
    for (size_t i = 0; i < number.digits.size(); ++i) {
        os << number.digits[i];
    }

    return os;
}

std::istream& operator>> (std::istream& is, BigInt& number) {
    std::string input;
    is >> input;
    
    number = BigInt(input);

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
                res.ReverseDigits();
                std::cout << res << '\n';
        }
        else if (operation == '-') {
            if (number1 < number2) {std::cout << "Error\n";}
            else {
                BigInt res = number1 - number2;
                res.ReverseDigits();
                std::cout << res << '\n';
            }
        }
        else if (operation == '*') {
                BigInt res = number1 * number2;
                res.ReverseDigits();
                std::cout << res << '\n';
        }
        /*else if (operation == '^') {   
            if (number1 == BigInt("0") and number2 == BigInt("0")) {std::cout << "Error\n";}
            else{std::cout << (number1 ^ number2) << '\n';}
        }*/
        else if (operation == '/') {
            if (number2 == BigInt("0")) {std::cout << "Error\n";}
            else {
                BigInt res = number1 / number2;
                res.ReverseDigits();
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