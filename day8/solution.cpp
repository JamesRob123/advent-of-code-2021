#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file\n"};
    }

    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> input {};
    std::string line {};
    int index = 0;
    while (std::getline(fs, line)) {
        std::istringstream is {line};
        std::string current {};
        input.push_back({std::vector<std::string>(), std::vector<std::string>()});
        while (is >> current) {
            if (current[0] == '|') {
                break;
            }
            input[index].first.push_back(current);
        }

        while (is >> current) {
            input[index].second.push_back(current);
        }

        ++index;
    }

    return input;
}

template <typename Input>
int solve1(const Input& input) {
    int count = 0;
    for (const auto& current_line : input) {
        for (const auto& current : current_line.second) {
            auto size = current.size();
            if (size == 2 || size == 3 || size == 4 || size == 7) {
                ++count;
            }
        }
    }
    return count;
}



template <typename Input>
int solve2(const Input& input) {
    uint8_t valid_numbers[10] = {
        0b01110111,
        0b00100100,
        0b01011101,
        0b01101101,
        0b00101110,
        0b01101011,
        0b01111011,
        0b00100101,
        0b01111111,
        0b01101111
    };

    auto check = [&valid_numbers](const std::vector<std::string>& nums, uint8_t* converter) -> bool {
        for (const auto& num : nums) {
            uint8_t n {};
            for (char c : num) {
                n |= converter[c - 'a'];
            }
            for (int i = 0; i < 10; ++i) {
                if (n == valid_numbers[i]) {
                    goto found;
                }
            }
            return false;
found:;
        }
        return true;
    };

    unsigned ans {};
    for (const auto& current_line : input) {
        uint8_t converter[7] {1, 2, 4, 8, 16, 32, 64};
        do {
            if (check(current_line.first, converter)) {
                break;
            }
        } while(std::next_permutation(std::begin(converter), std::end(converter)));

        unsigned result {};
        for (const auto& num : current_line.second) {
            uint8_t n {};
            for (char c : num) {
                n |= converter[c - 'a'];
            }

            for (int i = 0; i < 10; ++i) {
                if (n == valid_numbers[i]) {
                    result *= 10;
                    result += i;
                    break;
                }
            }
        }
        ans += result;
    }

    return ans;
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';
}