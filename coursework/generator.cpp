#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

// Функция для генерации случайного слова
std::string generate_random_word(int min_length, int max_length) {
    static const std::string chars = "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<> length_dist(min_length, max_length);
    std::uniform_int_distribution<> char_dist(0, chars.size() - 1);

    int word_length = length_dist(generator);
    std::string word;
    for (int i = 0; i < word_length; ++i) {
        word += chars[char_dist(generator)];
    }
    return word;
}

// Функция для генерации строки из случайных слов
std::string generate_random_string(int num_words, int min_word_length, int max_word_length) {
    std::string result;
    for (int i = 0; i < num_words; ++i) {
        if (!result.empty()) {
            result += " ";
        }
        result += generate_random_word(min_word_length, max_word_length);
    }
    return result;
}

// Функция для записи строк в файл
void write_to_file(const std::string& filename, const std::string& s1, const std::string& s2) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << "\n";
        return;
    }
    file << s1 << "\n";
    file << s2 << "\n";
    file.close();
}

int main() {
    // Параметры генерации тестов
    int num_words_s1 = 10000; // Количество слов в первой строке
    int num_words_s2 = 10000; // Количество слов во второй строке
    int min_word_length = 1;  // Минимальная длина слова
    int max_word_length = 6; // Максимальная длина слова

    // Генерация случайных строк
    std::string s1 = generate_random_string(num_words_s1, min_word_length, max_word_length);
    std::string s2 = generate_random_string(num_words_s2, min_word_length, max_word_length);

    // Запись строк в файл
    write_to_file("in.txt", s1, s2);

    std::cout << "Test data has been generated and written to 'in' file.\n";

    return 0;
}