#include <iostream>
#include <vector>

int main() {
    size_t n;
    std::cin >> n;

    std::vector<int> sequence(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> sequence[i];
    }

    std::vector<size_t> count(3, 0); // size_t -- ?
    for (size_t i = 0; i < n; ++i) {
        ++count[sequence[i] - 1];
    }

    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1]; // теперь в соответстующих count[i] лежат границы для зон 1, 2, 3
    }

    std::vector<std::vector<size_t>> countInZones(3, std::vector<size_t>(3, 0));

    for (size_t i = 0; i < n; ++i) {
        size_t zone = (i < count[0]) ? 0 : (count[0] <= i && i < count[1]) ? 1 : 2;
        ++countInZones[zone][sequence[i] - 1];
    }

    size_t swapTwoElements = 
        std::min(countInZones[1][0], countInZones[0][1]) +
        std::min(countInZones[2][1], countInZones[1][2]) +
        std::min(countInZones[0][2], countInZones[2][0]);

    size_t remainingWrongElements = 
        (std::max(countInZones[1][0], countInZones[0][1]) - std::min(countInZones[1][0], countInZones[0][1]) + 
        std::max(countInZones[2][1], countInZones[1][2]) - std::min(countInZones[2][1], countInZones[1][2]) +
        std::max(countInZones[0][2], countInZones[2][0]) - std::min(countInZones[0][2], countInZones[2][0])) / 3;

    size_t swapThreeElements = 2 * remainingWrongElements;

    size_t minSwap = swapTwoElements + swapThreeElements;

    std::cout << minSwap << '\n';

    return 0;
}