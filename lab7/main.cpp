#include <iostream>
#include <vector>
#include <algorithm>

using ul = unsigned long;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    size_t n, m;

    std::cin >> n >> m;

    std::vector<ul> w(n);
    std::vector<ul> c(n);

    for (size_t i = 0; i < n; ++i) {
        std::cin >> w[i] >> c[i];
    }

    std::vector<std::vector<std::vector<ul>>> dp(n + 1, std::vector<std::vector<ul>>(n + 1, std::vector<ul>(m + 1, 0)));

    for (size_t i = 1; i < n + 1; ++i) {
        for (size_t j = 1; j < n + 1; ++j) {
            for (size_t k = 1; k < m + 1; ++k) {
                dp[i][j][k] = dp[i - 1][j][k];

                if (k >= w[i - 1]) {
                    if (j == 1) {
                        dp[i][j][k] = std::max(dp[i][j][k], dp[i - 1][j][k - w[i - 1]] + c[i - 1]);
                    }
                    else if (dp[i - 1][j - 1][k - w[i - 1]] > 0) {
                        dp[i][j][k] = std::max(dp[i][j][k], (dp[i - 1][j - 1][k - w[i - 1]] / (j - 1) + c[i - 1]) * j);
                    }
                }
            }
        }
    }

    ul max_value = 0;
    size_t j_max = 0;
    for (size_t j = 0; j < n + 1; ++j) {
        if (dp[n][j][m] > max_value) {
            max_value = dp[n][j][m];
            j_max = j;
        }
    }

    std::vector<size_t> path;
    while (dp[n][j_max][m] > 0) {
        if (dp[n][j_max][m] == dp[n - 1][j_max][m]) {
            --n;
        }
        else {
            m -= w[n - 1];
            --j_max;
            path.push_back(n--);
        }
    }

    std::reverse(path.begin(), path.end());

    std::cout << max_value << '\n';
    for (size_t elem : path) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';

    return 0;
}