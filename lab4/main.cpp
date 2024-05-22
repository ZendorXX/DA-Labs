#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> patterns;
    std::string word = "";

    for (size_t inputIndex = 0; inputIndex < input.size(); ++inputIndex) {
        if (input[inputIndex] != ' ' && inputIndex != input.size() - 1) {
            word += input[inputIndex];
        }
        else {
            if (inputIndex == input.size() - 1) {
                word += input[inputIndex];
            }
            patterns.push_back(word);
            word = "";
        }
    }

    std::string text = "";

    while(std::getline(std::cin, input)) {
        text += input + ' '; 
    }

    for (size_t patternsIndex = 0; patternsIndex < patterns.size(); ++patternsIndex) {
        std::string tmp = patterns[patternsIndex] + '#' + text;
        std::cout << "====================================\n";
        std::cout << tmp << '\n';
    }

    return 0;
}