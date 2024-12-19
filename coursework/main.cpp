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

std::vector<int> compute_lcs(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    size_t n = a.size(), m = b.size();
    std::vector<int> previous(m + 1, 0), current(m + 1, 0);

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                current[j] = previous[j - 1] + 1;
            }
            else {
                current[j] = std::max(previous[j], current[j - 1]);
            }
        }
        std::swap(previous, current);
    }

    return previous;
}

std::vector<std::string> hirschberg(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    size_t n = a.size(), m = b.size();

    if (n == 0 || m == 0) {
        return {};
    }
    if (n == 1) {
        for (const std::string& word : b) {
            if (word == a[0]) {
                return {a[0]};
            }
        }
        return {};
    }
    if (m == 1) {
        for (const std::string& word : a) {
            if (word == b[0]) {
                return {b[0]};
            }
        }
        return {};
    }

    size_t mid = n / 2;

    std::vector<int> lcs_lengths_left = compute_lcs({a.begin(), a.begin() + mid}, b);
    std::vector<int> lcs_lengths_right = compute_lcs({a.rbegin(), a.rbegin() + (n - mid)}, {b.rbegin(), b.rend()});

    size_t optimal_split = 0;
    size_t max_length = 0;

    for (size_t i = 0; i <= m; ++i) {
        size_t current_length = lcs_lengths_left[i] + lcs_lengths_right[m - i];
        if (current_length > max_length) {
            max_length = current_length;
            optimal_split = i;
        }
    }

    std::vector<std::string> result_lcs_left = hirschberg({a.begin(), a.begin() + mid}, {b.begin(), b.begin() + optimal_split});
    std::vector<std::string> result_lcs_right = hirschberg({a.begin() + mid, a.end()},{b.begin() + optimal_split, b.end()});

    result_lcs_left.insert(result_lcs_left.end(), result_lcs_right.begin(), result_lcs_right.end());
    return result_lcs_left;
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

    std::vector<std::string> lcs = hirschberg(words1, words2);

    std::cout << lcs.size() << "\n";
    for (const std::string& word : lcs) {
        std::cout << word << " ";
    }
    std::cout << "\n";

    return 0;
}