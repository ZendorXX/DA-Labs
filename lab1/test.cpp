#include <random>
#include <iostream>
#include <vector>   
#include <cstdint>
#include <algorithm>
#include <chrono>   
#include <random>   

using namespace std;

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

const int Mask = 0xFF;

void RadixSort(MyMap &Map) {
    for (int Position = 0; Position < sizeof(uint64_t); ++Position) {
        item *Result = new item[Map.size];
        int *Count = new int[Mask + 1];

        for (int i = 0; i < Mask + 1; ++i) {
            Count[i] = 0;
        }

        for (int i = 0; i < Map.size; ++i) {
            ++Count[((Map.data[i].key >> (8 * Position)) & Mask)];
        }

        for (int i = 1; i < Mask + 1; ++i) {
            Count[i] += Count[i - 1];
        }

        for (int i = Map.size; i > 0; --i) {
            int InsertPosition = --Count[((Map.data[i - 1].key >> (8 * Position)) & Mask)];
            Result[InsertPosition] = Map.data[i - 1];
        }

        delete[] Map.data;

        Map.data = Result;

        delete[] Count;
    }
}


bool IsSorted(const vector<item>& objects) {
    for (size_t i = 0; i < objects.size() - 1; ++i) {
        if (objects[i].key > objects[i + 1].key) {
            std::cout << i << " " << i + 1 << "\n";
            return false;
        }
    }
    return true;
}

bool IsSorted(const MyMap &objects) {
    for (size_t i = 0; i < objects.size - 1; ++i) {
        if (objects.data[i].key > objects.data[i + 1].key) {
            std::cout << i << " " << i + 1 << "\n";
            return false;
        }
    }
    return true;
}


bool cmp(const item &a, const item &b)
{
    return a.key < b.key;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    vector<item> objects;

    MyMap objects_copy;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,1e9);

    int cnt;
    std::cin >> cnt; 
    for (uint64_t i = 0; i < cnt; ++i) {
        uint64_t key = dist(rng);
        objects.push_back(item{key, "abaca"});

        if (objects_copy.data == nullptr) {
            objects_copy.capacity = 1;
            objects_copy.data = new item[objects_copy.capacity];
        }
        else if (objects_copy.size == objects_copy.capacity) {
            objects_copy.capacity *= 2;
            item *Newdata = new item[objects_copy.capacity];
            for (size_t i = 0; i < objects_copy.size; ++i) {
                Newdata[i] = objects_copy.data[i];
            }
            delete[] objects_copy.data;
            objects_copy.data = Newdata;
        }
        objects_copy.data[objects_copy.size++] = item{key, "abaca"};
    }

    auto begin_std_sort = std::chrono::steady_clock::now();
    stable_sort(objects.begin(), objects.end(), cmp);
    auto end_std_sort = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_std_sort - begin_std_sort);

    std::cout << "The time of std::stable_sort: " << elapsed_ms.count() << " ms\n";
    if (IsSorted(objects)) std::cout << "CORRECT\n";
    else std::cout << "NOT CORRECT\n";

    auto begin = std::chrono::steady_clock::now();
    RadixSort(objects_copy);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms_radix = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    
    std::cout << "The time of radix: " << elapsed_ms_radix.count() << " ms\n";
    if (IsSorted(objects_copy)) std::cout << "CORRECT\n";
    else std::cout << "NOT CORRECT\n";

    delete[] objects_copy.data;

    return 0;
}