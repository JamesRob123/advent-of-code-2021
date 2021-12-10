// OBS: this uses c++20 features 
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

std::vector<std::bitset<12>> get_input(const std::string& file_name) {
    std::fstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open file"};
    }

    std::vector<std::bitset<12>> input {};
    std::bitset<12> n {};
    while (fs >> n) {
        input.push_back(n);
    }
    
    return input;
};

int solve1(const std::vector<std::bitset<12>>& input) {
    int arr[12] {};
    int count = 0;
    for (auto& b : input) {
        for (int i = 0; i < 12; ++i) {
            arr[i] += b[i];
        }
        ++count;
    }

    std::bitset<12> gamma {};
    std::bitset<12> epilson {};
    for (int i = 0; i < 12; ++i) {
        gamma[i] = (count - arr[i] < arr[i]);
        epilson[i] = (count - arr[i] > arr[i]);
    }

    return gamma.to_ulong() * epilson.to_ulong();
}

int solve2(std::vector<std::bitset<12>>& input) {
    auto get_ones = [](const auto& input, int pos) {
        int count = 0;
        for (int i = 0; i < input.size(); ++i) {
            count += input[i][pos];
        }
        return count;
    };

    auto input_copy = input;
    for (int i = 11; i >= 0; --i) {
        int ones = get_ones(input_copy, i);
        bool val = (input_copy.size() - ones <= ones);
        std::erase_if(input_copy, [&val, &i](const auto& bw) {
            return bw[i] != val;
        });
        if (input_copy.size() == 1) {
            break;
        }
    }

    for (int i = 11; i >= 0; --i) {
        int ones = get_ones(input, i);
        bool val = !(input.size() - ones <= ones);
        std::erase_if(input, [&val, &i](const auto& bw) {
            return bw[i] != val;
        });
        if (input.size() == 1) {
            break;
        }
    }

    return input[0].to_ulong() * input_copy[0].to_ulong();
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}