#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> result;
    std::string word;

    for (char ch : s) {
        if (ch == ' ') {
            if (!word.empty()) {
                result.push_back(word);
                word.clear();
            }
        }
        else {
            word += ch;
        }
    }

    if (!word.empty()) {
        result.push_back(word);
    }

    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    std::string s1, s2;

    std::getline(std::cin, s1);
    std::getline(std::cin, s2);

    std::vector<std::string> words1 = split(s1);
    std::vector<std::string> words2 = split(s2);

    

    return 0;
}