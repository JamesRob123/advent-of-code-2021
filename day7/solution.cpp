#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

std::vector<int> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open file"};
    }

    std::vector<int> input {};
    int current;
    while (fs >> current) {
        input.push_back(current);
        fs.get();
    }
    return input;
}

int solve1(const std::vector<int>& input) {
    auto calc = [&input](int val) {
        int count = 0;
        for (int n : input) {
            count += std::abs(n - val);
        }
        return count;
    };

    int low = -1;
    int high = *std::max_element(input.begin(), input.end());
    while (low < high - 1) {
        int mid = (high + low) / 2;
        if (calc(mid) < calc(mid + 1)) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return calc(high);
}

int solve2(const std::vector<int>& input) {
    auto calc = [&input](int val) {
        int count = 0;
        for (int n : input) {
            count += (std::abs(n - val) + 1) * std::abs(n - val) / 2;
        }
        return count;
    };

    int low = -1;
    int high = *std::max_element(input.begin(), input.end());
    while (low < high - 1) {
        int mid = (high + low) / 2;
        if (calc(mid) < calc(mid + 1)) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return calc(high);
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}