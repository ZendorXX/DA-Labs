#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>   
#include <random>

struct wordData {
    std::string word;
    size_t lineNumber;
    size_t wordNumber;
};

bool isEqual(wordData a, wordData b) {
    if (a.word.size() != b.word.size()) {
        return false;
    }
    for (size_t i = 0; i < a.word.size(); ++i) {
        if (tolower(a.word[i]) != tolower(b.word[i])) {
            return false;
        }
    }
    return true;
}

std::vector<size_t> slow_z_function(const std::vector<wordData>& input)
{
	size_t len = input.size();
	std::vector<size_t> z(len, 0);

	for (size_t i = 1; i < len; i++)
	{
		while ((i + z[i] < len))
		{
			if (!isEqual(input[z[i]], input[i+z[i]]))
				break;
			z[i]++;
		}
	}

	return z;
}

std::vector<size_t> z_function(const std::vector<wordData>& input) {
    std::vector<size_t> result(input.size());
    for (size_t index = 1, left = 0, right = 0; index < input.size(); ++index) {
        if (index <= right) {
            result[index] = std::min(right - index + 1, result[index - left]);
        }

        while (index + result[index] < input.size() && isEqual(input[result[index]], input[index + result[index]])) {
            ++result[index];
        }

        if (index + result[index] - 1 > right) {
            left = index;
            right = index + result[index] - 1;
        }
    }
    return result;
}

std::vector<std::string> split(const std::string& input, const std::string& delimeter, const bool skipEmpty)
{
	std::vector<std::string> result;
	std::string temp = input;

	while (true)
	{
		size_t pos = temp.find(delimeter);

		if (pos == std::string::npos)
		{
			if (!(skipEmpty && temp.length() == 0)) {
				result.push_back(temp);
            }
			break;
		}

		std::string part1 = temp.substr(0, pos);
		std::string part2 = temp.substr(pos + 1);

		if (!(skipEmpty && part1.length() == 0)) {
			result.push_back(part1);
        }

		temp = part2;
	}

	return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,1e9);

    std::string input;
    std::getline(std::cin, input);

    size_t N = 5;

    std::vector<wordData> pattern;
    //std::vector<std::string> words = split(input, " ", true);
    for (size_t wordNumber = 0; wordNumber < N; ++wordNumber) {
        pattern.push_back({"aba", 0, wordNumber + 1});
    }

    std::vector<wordData> text;

    size_t M = 10;
    for (size_t lineNumber = 0; std::getline(std::cin, input); ++lineNumber) {
        //std::vector<std::string> words = split(input, " ", true);
        for (size_t wordNumber = 0; wordNumber < M; ++wordNumber) {
            text.push_back({"aba", lineNumber + 1, wordNumber + 1});
        }
    }

    std::vector<wordData> allData;
    for (size_t i = 0; i < pattern.size(); ++i) {
        allData.push_back(pattern[i]);
    }
    allData.push_back({"$", 0, 0});
    for (size_t i = 0; i < text.size(); ++i) {
        allData.push_back(text[i]);
    }

    auto begin_1 = std::chrono::steady_clock::now();
    std::vector<size_t> z = z_function(allData);
    auto end_1 = std::chrono::steady_clock::now();

    /*for (size_t i = 0; i < z.size(); ++i) {
        size_t j = i + pattern.size() + 1;
        if (z[j] == pattern.size()) {
            std::cout << text[i].lineNumber << ", " << text[i].wordNumber << '\n';
        } 
    }*/

    auto begin_2 = std::chrono::steady_clock::now();
    std::vector<size_t> slow_z = slow_z_function(allData);
    auto end_2 = std::chrono::steady_clock::now();

    return 0;
}