#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

vector<string> split(const string& str) {
    vector<string> words;
    stringstream ss(str);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }
    return words;
}

pair<int, vector<int>> computeLCSLength(const vector<string>& seq1, const vector<string>& seq2) {
    int n = seq1.size();
    int m = seq2.size();

    vector<int> prev(m + 1, 0), curr(m + 1, 0);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (seq1[i - 1] == seq2[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        prev.swap(curr);
    }
    return {prev[m], prev};
}


vector<string> restoreLCS(const vector<string>& seq1, const vector<string>& seq2) {
    int n = seq1.size();
    int m = seq2.size();

    vector<int> prev(m + 1, 0), curr(m + 1, 0);
    vector<string> lcs;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (seq1[i - 1] == seq2[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        prev.swap(curr);
    }

    int i = n, j = m;
    while (i > 0 && j > 0) {
        if (seq1[i - 1] == seq2[j - 1]) {
            lcs.push_back(seq1[i - 1]);
            --i;
            --j;
        } else if (prev[j] == prev[j - 1]) {
            --j;
        } else {
            --i;
        }
    }

    reverse(lcs.begin(), lcs.end());
    return lcs;
}

int main() {
    string line1, line2;
    getline(cin, line1);
    getline(cin, line2);

    vector<string> seq1 = split(line1);
    vector<string> seq2 = split(line2);

    auto lcsLengthAndLastRow = computeLCSLength(seq1, seq2);

    vector<string> lcs = restoreLCS(seq1, seq2);

    cout << lcsLengthAndLastRow.first << endl;
    for (const auto& word : lcs) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}
