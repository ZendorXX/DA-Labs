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

    void Normalize();
    size_t Size();

    BigInt operator+(BigInt& other);
    BigInt operator-(BigInt& other);
    BigInt operator*(BigInt& other);
    BigInt operator^(BigInt& other);
    BigInt operator/(BigInt& other);

    bool operator<(BigInt& other);
    bool operator>(BigInt& other);
    bool operator==(BigInt& other);

    friend std::ostream& operator<< (std::ostream& os, const BigInt& number);
    friend std::istream& operator>> (std::istream& is, BigInt& number);
};

BigInt::BigInt() : digits(1, 0) {};

BigInt::BigInt(const size_t& size) : digits(size, 0) {};

BigInt::BigInt(const std::string& str) {
    for (size_t i = str.size(); i > 0; (i < DIGIT_LENGH) ? i = 0 : i -= DIGIT_LENGH) {
        size_t length = (i < DIGIT_LENGH) ? i : DIGIT_LENGH;
        digits.push_back(std::stoi(str.substr(i - length, length)));
    }
};

void BigInt::Normalize() {
    while(digits.back() == 0 and digits.size() > 1) {
        digits.pop_back();
    }

    std::reverse(digits.begin(), digits.end());
}

size_t BigInt::Size() {
    return digits.size();
}

BigInt BigInt::operator+(BigInt& other) {
    size_t max_size = this->Size() > other.Size() ? this->Size() : other.Size();
    BigInt result(max_size);
    int this_digit = 0, other_digit = 0;
    bool flag = false;

    for (size_t i = 0; i < max_size; ++i) {
        this_digit = i < this->Size() ? this->digits[i] : 0;
        other_digit = i < other.Size() ? other.digits[i] : 0;

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

BigInt BigInt::operator-(BigInt& other) {
    size_t max_size = this->Size() > other.Size() ? this->Size() : other.Size();
    BigInt result(max_size);
    int this_digit = 0, other_digit = 0;
    bool flag = false;

    for (size_t i = 0; i < max_size; ++i) {
        this_digit = i < this->Size() ? this->digits[i] : 0;
        other_digit = i < other.Size() ? other.digits[i] : 0;

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

BigInt BigInt::operator*(BigInt& other) {
    BigInt result(this->Size() + other.Size());

    for (size_t i = 0; i < this->Size(); ++i) {
        for (size_t j = 0; j < other.Size(); ++j) {
            result.digits[i + j] += this->digits[i] * other.digits[j];
        }
    }

    for (size_t i = 0; i < result.Size() - 1; ++i) {
        result.digits[i + 1] += result.digits[i] / BigInt::MAX_DIGIT;
        result.digits[i] %= BigInt::MAX_DIGIT;
    }

    return result;
}

BigInt BigInt::operator^(BigInt& other) {
}

bool BigInt::operator<(BigInt& other) {
    if (this->Size() != other.Size()) {
        return this->Size() < other.Size();
    }

    for (size_t i = 0; i < this->Size(); ++i) {
        if (this->digits[i] != other.digits[i]) {
            return this->digits[i] < other.digits[i];
        }
    }

    return false;
}

bool BigInt::operator>(BigInt& other) {
    if (this->Size() != other.Size()) {
        return this->Size() > other.Size();
    }

    for (size_t i = 0; i < this->Size(); ++i) {
        if (this->digits[i] != other.digits[i]) {
            return this->digits[i] > other.digits[i];
        }
    }

    return false;
}

bool BigInt::operator==(BigInt& other) {
    if (this->Size() != other.Size()) {
        return false;
    }

    for (size_t i = 0; i < this->Size(); ++i) {
        if (this->digits[i] != other.digits[i]) {
            return false;
        }
    }

    return true;
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
                res.Normalize();
                std::cout << res << '\n';
        }
        else if (operation == '-') {
            if (number1 < number2) {std::cout << "Error\n";}
            else {
                BigInt res = number1 - number2;
                res.Normalize();
                std::cout << res << '\n';
            }
        }
        else if (operation == '*') {
                BigInt res = number1 * number2;
                res.Normalize();
                std::cout << res << '\n';
        }
        /*else if (operation == '^') {   
            if (number1 == BigInt("0") and number2 == BigInt("0")) {std::cout << "Error\n";}
            else{std::cout << (number1 ^ number2) << '\n';}
        }
        else if (operation == '/') {
            if (number2 == BigInt("0")) {std::cout << "Error\n";}
            else {std::cout << number1 / number2 << '\n';}
        }*/
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