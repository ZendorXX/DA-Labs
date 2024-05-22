#include <iostream>
#include <cstdint>

struct item {
    uint64_t key;
    char value[65];
};

struct MyMap{
    item *data;
    uint64_t size;
    uint64_t capacity;

    MyMap() {
        data = nullptr;
        size = 0;
        capacity = 0;
    }
};

const uint64_t Mask = 0xFFFF;

void RadixSort(MyMap &map) {
    for (uint64_t position = 0; position < sizeof(uint64_t) / 2; ++position) {
        item *result = new item[map.size];
        uint64_t *count = new uint64_t[Mask + 1];

        for (size_t i = 0; i < Mask + 1; ++i) {
            count[i] = 0;
        }

        for (size_t i = 0; i < map.size; ++i) {
            ++count[((map.data[i].key >> (16 * position)) & Mask)];
        }

        for (size_t i = 1; i < Mask + 1; ++i) {
            count[i] += count[i - 1];
        }

        for (size_t i = map.size; i > 0; --i) {
            size_t InsertPosition = --count[((map.data[i - 1].key >> (16 * position)) & Mask)];
            result[InsertPosition] = map.data[i - 1];
        }

        delete[] map.data;

        map.data = result;

        delete[] count;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    MyMap map;
    item input;

    while (std::cin >> input.key >> input.value) {
        if (map.data == nullptr) {
            map.capacity = 1;
            map.data = new item[map.capacity];
        }
        else if (map.size == map.capacity) {
            map.capacity *= 2;
            item *NewData = new item[map.capacity];
            for (size_t i = 0; i < map.size; ++i) {
                NewData[i] = map.data[i];
            }
            delete[] map.data;
            map.data = NewData;
        }
        map.data[map.size++] = input;
    }

    RadixSort(map);

    for (size_t i = 0; i < map.size; ++i) {
        std::cout << map.data[i].key << '\t' << map.data[i].value << '\n';
    }

    delete[] map.data;

    return 0;
} 
