#include <iostream>
#include <vector>
#include <fstream>

std::vector<int> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open file"};
    }

    std::vector<int> input {};
    int current = 0;
    while (fs >> current) {
        input.push_back(current);
    }
    return input;
}

int solve1(const std::vector<int>& input) {
    int count = 0;
    for (int i = 1; i < input.size(); ++i) {
        if (input[i] > input[i - 1]) {
            ++count;
        }
    }
    return count;
}

int solve2(const std::vector<int>& input) {
    int count = 0;
    for (int i = 3; i < input.size(); ++i) {
        if (input[i - 3] < input[i]) {
            ++count;
        }
    }
    return count;
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';
    

    return 0;
}