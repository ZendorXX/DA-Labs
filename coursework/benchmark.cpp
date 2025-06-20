#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <sys/resource.h>

// Разделение строки на слова
std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> result;
    std::string word;

    for (char ch : s) {
        if (ch == ' ') {
            if (!word.empty()) {
                result.push_back(word);
                word.clear();
            }
        } else {
            word += ch;
        }
    }

    if (!word.empty()) {
        result.push_back(word);
    }

    return result;
}

// Классическое решение НОП
std::vector<std::string> classical_lcs(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    size_t n = a.size(), m = b.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    // Построение таблицы НОП
    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Восстановление ответа
    std::vector<std::string> lcs;
    size_t i = n, j = m;
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            lcs.push_back(a[i - 1]);
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }

    std::reverse(lcs.begin(), lcs.end());
    return lcs;
}

// Алгоритм Хиршберга
std::vector<int> compute_lcs(const std::vector<std::string>& a, const std::vector<std::string>& b) {
    size_t n = a.size(), m = b.size();
    std::vector<int> previous(m + 1, 0), current(m + 1, 0);

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                current[j] = previous[j - 1] + 1;
            } else {
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
    std::vector<std::string> result_lcs_right = hirschberg({a.begin() + mid, a.end()}, {b.begin() + optimal_split, b.end()});

    result_lcs_left.insert(result_lcs_left.end(), result_lcs_right.begin(), result_lcs_right.end());
    return result_lcs_left;
}

size_t get_memory_usage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return static_cast<size_t>(usage.ru_maxrss);
    }
    return 0;
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

    // Замер для алгоритма Хиршберга
    /*auto start_time = std::chrono::high_resolution_clock::now();
    size_t memory_before = get_memory_usage();
    std::vector<std::string> lcs_hirschberg = hirschberg(words1, words2);
    size_t memory_after = get_memory_usage();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_hirschberg = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    size_t memory_hirschberg = memory_after - memory_before;*/

    // Замер для классического алгоритма
    auto start_time = std::chrono::high_resolution_clock::now();
    size_t memory_before = get_memory_usage();
    std::vector<std::string> lcs_classical = classical_lcs(words1, words2);
    size_t memory_after = get_memory_usage();
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_classical = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    size_t memory_classical = memory_after - memory_before;

    

    // Вывод результатов
    std::cout << "Classical LCS:\n";
    std::cout << "Length: " << lcs_classical.size() << "\n";
    std::cout << "Time taken: " << duration_classical << " ms\n";
    std::cout << "Memory used: " << memory_classical << " KB\n";

    /*std::cout << "\nHirschberg LCS:\n";
    std::cout << "Length: " << lcs_hirschberg.size() << "\n";
    std::cout << "Time taken: " << duration_hirschberg << " ms\n";
    std::cout << "Memory used: " << memory_hirschberg << " KB\n";
*/
    return 0;
}