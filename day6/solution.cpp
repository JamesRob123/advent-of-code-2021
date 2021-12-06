#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
#include <numeric>

std::vector<int> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open file"};
    }

    std::vector<int> input {};
    int n;
    while (fs >> n) {
        input.push_back(n);
        fs.get();
    }

    return input;
}

int solve1(std::vector<int> input) {
    for (int i = 0; i < 80; ++i) {
        auto size = input.size();
        // Update day
        for (int i = 0; i < size; ++i) {
            //std::cout << vec[i] << ',';
            if (input[i] == 0) {
                input[i] = 6;
                input.push_back(8);
            } else {
                --input[i];
            }
        }
    }
    return input.size();
}

long long solve2(std::vector<int>& input) {
    std::array<long long, 9> fishes {};

    // Every fish can only have a value 0 - 8 so we can compress them
    for (int n : input) {
        ++fishes[n];
    }

    for (int i = 0; i < 256; ++i) {
        // Simulating day i
        std::rotate(fishes.begin(), fishes.begin() + 1, fishes.begin() + 7);
        long long temp = fishes[6];
        fishes[6] += fishes[7];
        fishes[7] = fishes[8];
        fishes[8] = temp;
    }

    return std::accumulate(fishes.begin(), fishes.end(), 0LL);
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}