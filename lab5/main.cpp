#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std; 
 
vector<int> buildSuffixArray(const string &s) { 
    int n = s.size(); 
    vector<int> suffix_array(n);

    int max_symbol = -1;
    for (int i = 0; i < n; ++i) {
        if (s[i] > max_symbol) {
            max_symbol = s[i];
        }
    }

    vector<int> cnt(max(max_symbol + 1, n), 0); 
    for (int i = 0; i < n; ++i) {
        ++cnt[s[i]]; 
    }
    for (int i = 1; i < cnt.size(); ++i) {
        cnt[i] += cnt[i - 1]; 
    }

    for (int i = n - 1; i >= 0; --i) {
        suffix_array[--cnt[s[i]]] = i; 
    }
    
    vector<int> eq_class(n);
    eq_class[suffix_array[0]] = 0; 
    int class_value = 1; 
    for (int i = 1; i < n; ++i) { 
        if (s[suffix_array[i]] != s[suffix_array[i - 1]]) {
            ++class_value; 
        }
        eq_class[suffix_array[i]] = class_value - 1; 
    }
     
    vector<int> tmp_suffix_array(n), tmp_eq_class(n); 
    for (int len = 1; len < n; len *= 2) { 
        for (int i = 0; i < n; ++i) { 
            tmp_suffix_array[i] = suffix_array[i] - len; 
            if (tmp_suffix_array[i] < 0) {
                tmp_suffix_array[i] += n; 
            }
        }
         
        cnt.assign(class_value, 0);

        for (int i = 0; i < n; ++i) {
            ++cnt[eq_class[tmp_suffix_array[i]]];
        } 
        for (int i = 1; i < class_value; ++i) {
            cnt[i] += cnt[i - 1]; 
        }
        for (int i = n - 1; i >= 0; --i) {
            suffix_array[--cnt[eq_class[tmp_suffix_array[i]]]] = tmp_suffix_array[i]; 
        }
         
        tmp_eq_class[suffix_array[0]] = 0; 
        class_value = 1; 
        for (int i = 1; i < n; ++i) { 
            pair<int, int> curr = {eq_class[suffix_array[i]], eq_class[(suffix_array[i] + len) % n]}; 
            pair<int, int> prev = {eq_class[suffix_array[i - 1]], eq_class[(suffix_array[i - 1] + len) % n]}; 
            if (curr != prev) {
                ++class_value; 
            }
            tmp_eq_class[suffix_array[i]] = class_value - 1; 
        } 
        eq_class = tmp_eq_class; 
    } 

    vector<int> result(n - 1);
    for (int i = 1; i < n; ++i) {
        result[i - 1] = suffix_array[i];
    }
     
    return result;
} 

vector<int> searchPattern(const string &text, const string &pattern, const vector<int> &suffixArray) {
    vector<int> result;

    if (pattern == "") {
        return result;
    }

    int n = text.size(), m = pattern.size();
    int L = 0, R = n;

    while (L < R) {
        int M = (L + R) / 2;
        if (text.compare(suffixArray[M], m, pattern) >= 0) {
            R = M;
        }
        else {
            L = M + 1;
        }
    }
    int first_entry = L;

    R = n;
    while (L < R) {
        int M = (L + R) / 2;
        if (text.compare(suffixArray[M], m, pattern) > 0) {
            R = M;
        }
        else {
            L = M + 1;
        }
    }
    int last_entry = L;

    if (first_entry == last_entry) {
        return result;
    }

    result.resize(last_entry - first_entry);
    for (int i = first_entry, j = 0; i < last_entry; ++i) {
        result[j++] = suffixArray[i];
    }
    sort(result.begin(), result.end());
    
    return result;
}

int main() { 
    ios::sync_with_stdio(false); 
    cin.tie(nullptr); 
     
    string text; 
    getline(cin, text); 
     
    vector<int> suffix_array = buildSuffixArray(text + "$"); 
    
    string pattern;
    int pattern_number = 0;

    while (getline(cin, pattern)) {
        ++pattern_number;
        vector<int> entries = searchPattern(text, pattern, suffix_array);

        if (!entries.empty()) {
            cout << pattern_number << ": ";

            for (size_t i = 0; i < entries.size(); ++i) {
                cout << entries[i] + 1;
                if (i < entries.size() - 1) {
                    cout << ", ";
                }
            }

            cout << '\n'; 
        }
    }
    
    return 0; 
}